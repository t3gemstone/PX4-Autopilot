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
 * @file io_timer_hw_description.h
 *
 * Minimal constexpr helpers used by the board's timer_config.cpp to build the
 * io_timers[]/timer_io_channels[] tables. The Timer:: tokens are also what the
 * output_groups_from_timer_config.py generator matches to derive the PWM
 * output groups, so this DSL form is required (not raw struct init).
 *
 * A "timer" here is one am67 EPWM/eCAP module (a rate group); a channel's
 * timer_channel 1/2 = output A/B (eCAP APWM = 1). Actual output is done by the
 * arch pwm_servo layer over the NuttX lower-halves, so no GPIO/base machinery
 * beyond the module base is needed.
 */

#pragma once

#include <px4_arch/io_timer.h>
#include <stdint.h>

namespace Timer
{
enum Timer {
	Timer0 = 0,   /* EPWM0 */
	Timer1 = 1,   /* EPWM1 */
	Timer2 = 2,   /* eCAP0 */
};

enum Channel {
	Channel1 = 1, /* output A (or eCAP APWM) */
	Channel2 = 2, /* output B */
};

struct TimerChannel {
	Timer   timer;
	Channel channel;
};
}

static inline constexpr io_timers_t initIOTimer(Timer::Timer timer)
{
	io_timers_t ret{};

	switch (timer) {
	case Timer::Timer0: ret.base = 0x23000000; break; /* EPWM0 */
	case Timer::Timer1: ret.base = 0x23010000; break; /* EPWM1 */
	case Timer::Timer2: ret.base = 0x23110000; break; /* eCAP1 (GPIO-16) */
	}

	return ret;
}

static inline constexpr timer_io_channels_t initIOTimerChannel(const io_timers_t io_timers_conf[MAX_IO_TIMERS],
		Timer::TimerChannel tc)
{
	(void)io_timers_conf;
	timer_io_channels_t ret{};
	ret.timer_index   = (uint8_t)tc.timer;
	ret.timer_channel = (uint8_t)tc.channel;
	return ret;
}
