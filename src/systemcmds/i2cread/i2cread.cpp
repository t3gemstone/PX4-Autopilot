/****************************************************************************
 *
 *   Copyright (c) 2024 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file i2cread.cpp
 *
 * Minimal I2C register read utility. Uses the same path as the PX4 I2C sensor
 * drivers (px4_i2cbus_initialize + I2C_TRANSFER), so a successful read proves
 * the whole PX4 -> arch shim -> NuttX driver chain end to end.
 *
 * Register read = write the 1-byte register address, then repeated-start read
 * <count> bytes. Pass -R to skip the register write (raw read of <count> bytes).
 */

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/getopt.h>

#include <stdlib.h>

namespace i2cread
{

static constexpr int MAX_COUNT = 32;

int read_reg(int bus, uint8_t addr, int reg, unsigned count, unsigned freq)
{
	if (count < 1 || count > MAX_COUNT) {
		PX4_ERR("count must be 1..%d", MAX_COUNT);
		return PX4_ERROR;
	}

	struct i2c_master_s *i2c_dev = px4_i2cbus_initialize(bus);

	if (i2c_dev == nullptr) {
		PX4_ERR("invalid/unavailable bus %d", bus);
		return PX4_ERROR;
	}

	uint8_t data[MAX_COUNT] {};
	i2c_msg_s msgv[2] {};
	unsigned nmsg = 0;

	uint8_t reg_addr = (uint8_t)reg;

	if (reg >= 0) {
		msgv[nmsg].frequency = freq;
		msgv[nmsg].addr = addr;
		msgv[nmsg].flags = I2C_M_NOSTOP;   // write reg addr, repeated-start (no STOP)
		msgv[nmsg].buffer = &reg_addr;
		msgv[nmsg].length = 1;
		nmsg++;
	}

	msgv[nmsg].frequency = freq;
	msgv[nmsg].addr = addr;
	msgv[nmsg].flags = I2C_M_READ;         // repeated-start read
	msgv[nmsg].buffer = data;
	msgv[nmsg].length = count;
	nmsg++;

	int ret = I2C_TRANSFER(i2c_dev, &msgv[0], nmsg);

	px4_i2cbus_uninitialize(i2c_dev);

	if (ret != PX4_OK) {
		PX4_ERR("transfer failed (%d): bus %d addr 0x%02x reg 0x%02x", ret, bus, addr, reg);
		return PX4_ERROR;
	}

	char line[8 + MAX_COUNT * 3] {};
	int pos = snprintf(line, sizeof(line), "0x%02x:", addr);

	for (unsigned i = 0; i < count; i++) {
		pos += snprintf(line + pos, sizeof(line) - pos, " %02x", data[i]);
	}

	PX4_INFO("bus %d %s", bus, line);
	return PX4_OK;
}

int usage(const char *reason = nullptr)
{
	if (reason) {
		PX4_ERR("%s", reason);
	}

	PRINT_MODULE_DESCRIPTION("Read bytes from an I2C device register (uses the PX4 I2C driver path).");

	PRINT_MODULE_USAGE_NAME_SIMPLE("i2cread", "command");
	PRINT_MODULE_USAGE_PARAM_INT('b', 1, 1, 10, "I2C bus", false);
	PRINT_MODULE_USAGE_PARAM_INT('a', 0, 0, 127, "Device address (7-bit)", false);
	PRINT_MODULE_USAGE_PARAM_INT('r', 0, 0, 255, "Register to read (omit or -R for raw read)", true);
	PRINT_MODULE_USAGE_PARAM_INT('c', 1, 1, MAX_COUNT, "Number of bytes to read", true);
	PRINT_MODULE_USAGE_PARAM_INT('f', 100000, 1000, 4000000, "Bus frequency (Hz)", true);
	PRINT_MODULE_USAGE_PARAM_FLAG('R', "Raw read (no register write)", true);

	return PX4_OK;
}

} // namespace i2cread

extern "C" {
	__EXPORT int i2cread_main(int argc, char *argv[]);
}

int i2cread_main(int argc, char *argv[])
{
	int bus = -1;
	int addr = -1;
	int reg = 0;
	unsigned count = 1;
	unsigned freq = 100000;
	bool raw = false;

	int myoptind = 1;
	int ch = 0;
	const char *myoptarg = nullptr;

	while ((ch = px4_getopt(argc, argv, "b:a:r:c:f:R", &myoptind, &myoptarg)) != EOF) {
		switch (ch) {
		case 'b': bus = strtol(myoptarg, nullptr, 0); break;
		case 'a': addr = strtol(myoptarg, nullptr, 0); break;
		case 'r': reg = strtol(myoptarg, nullptr, 0); break;
		case 'c': count = strtol(myoptarg, nullptr, 0); break;
		case 'f': freq = strtol(myoptarg, nullptr, 0); break;
		case 'R': raw = true; break;
		default:
			i2cread::usage();
			return -1;
		}
	}

	if (bus < 0 || addr < 0) {
		i2cread::usage("need -b <bus> and -a <addr>");
		return -1;
	}

	return i2cread::read_reg(bus, (uint8_t)addr, raw ? -1 : reg, count, freq);
}
