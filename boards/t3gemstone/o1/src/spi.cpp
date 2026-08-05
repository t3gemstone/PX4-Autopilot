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
 * @file spi.cpp
 *
 * Board SPI bus configuration for the T3 Gemstone O1 (AM67 / MCU_MCSPI0).
 *
 * The AM67 MCSPI drives its chip selects in HARDWARE, so there are no CS GPIOs.
 * We overload px4_spi_bus_device_t::cs_gpio to carry the MCSPI *channel number*
 * (must be non-zero; 0 means "unused"). The arch spi layer
 * (platforms/nuttx/src/px4/ti/am67/spi/spi.cpp) reads this back in
 * am67_spi0select() to select the hardware channel.
 *
 * Wiring (per the Linux DTS): ICM-20948 IMU on MCU_SPI0 CS3, LPS22DF
 * barometer on MCU_SPI0 CS1.
 */

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/spi.h>
#include <drivers/drv_sensor.h>

constexpr px4_spi_bus_t px4_spi_buses[SPI_BUS_MAX_BUS_ITEMS] = {
	{
		{	/* devices */
			{
				AM67_MCSPI_CH_ICM20948,                                  /* cs_gpio -> MCSPI channel */
				0,                                                       /* drdy_gpio (polled, no DRDY line) */
				PX4_SPIDEV_ID(PX4_SPI_DEVICE_ID, DRV_IMU_DEVTYPE_ICM20948),
				DRV_IMU_DEVTYPE_ICM20948,
			},
			{
				AM67_MCSPI_CH_LPS22DF,                                   /* cs_gpio -> MCSPI channel */
				0,                                                       /* drdy_gpio (polled, no DRDY line) */
				PX4_SPIDEV_ID(PX4_SPI_DEVICE_ID, DRV_BARO_DEVTYPE_LPS22DF),
				DRV_BARO_DEVTYPE_LPS22DF,
			},
		},
		0,                      /* power_enable_gpio: rail via am67_sensors_power_enable() */
		PX4_SPI_BUS_SENSORS,    /* bus */
		false,                  /* is_external */
		false,                  /* requires_locking (only PX4 accesses this bus) */
	},
};
