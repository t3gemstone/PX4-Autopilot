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
 * @file io_timer.c
 *
 * AM67 io_timer stubs. The portable pwm_out module only needs
 * io_timer_get_group here; all output work is done in pwm_servo.c over the
 * NuttX EPWM/eCAP lower-halves.
 */

#include <px4_platform_common/px4_config.h>
#include <nuttx/arch.h>

#include <drivers/drv_pwm_output.h>
#include <px4_arch/io_timer.h>

/* Bitmask of the channels driven by a given timer (group). Mirrors the
 * channel layout in pwm_servo.c / timer_config.cpp. */
uint32_t io_timer_get_group(unsigned timer)
{
	switch (timer) {
	case 0: return 0x3;   /* EPWM0: ch0, ch1 */
	case 1: return 0xC;   /* EPWM1: ch2, ch3 */
	case 2: return 0x10;  /* eCAP0: ch4 */
	default: return 0;
	}
}

uint32_t io_timer_channel_get_gpio_output(unsigned channel)
{
	/* Pad muxing for EPWM/eCAP outputs is done by the NuttX lower-half setup,
	 * not by PX4, so no GPIO configuration is exposed here. */
	(void)channel;
	return 0;
}
