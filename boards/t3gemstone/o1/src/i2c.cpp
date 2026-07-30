/****************************************************************************
 *
 *   Copyright (C) 2024 PX4 Development Team. All rights reserved.
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
 * @file i2c.cpp
 *
 * Board-specific I2C bus configuration (the px4_i2c_buses table used by
 * platforms/common/i2c.cpp and PX4 I2C sensor drivers).
 *
 * The AM67 NuttX driver can register MCU_I2C0 (am67 port 0, MCU domain
 * 0x04900000) and WKUP_I2C0 (am67 port 2, WKUP domain 0x2b200000). The
 * PX4<->am67 mapping is px4_i2cbus_initialize(bus) =
 * am67_i2cbus_initialize(bus - PX4_BUS_OFFSET) in micro_hal.h, so PX4 bus 1 ->
 * MCU_I2C0 and PX4 bus 3 -> WKUP_I2C0 (/dev/i2c2).
 *
 * Only WKUP_I2C0 is enabled and wired on this board; every I2C sensor lives
 * there. MCU_I2C0 carries no peripheral and is not built (CONFIG_AM67_I2C0 is
 * off), so it is intentionally omitted - listing it would make `i2cdetect -b 1`
 * report an invalid/uninitialised bus. is_external is left false (internal)
 * pending the board's HAT vs. on-board split.
 */

#include <px4_platform_common/i2c.h>

constexpr px4_i2c_bus_t px4_i2c_buses[I2C_BUS_MAX_BUS_ITEMS] = {
	{ 3, false },   /* WKUP_I2C0 (am67 port 2, /dev/i2c2) */
};
