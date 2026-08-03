############################################################################
#
#   Copyright (c) 2024 PX4 Development Team. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name PX4 nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

# Minimal board-support library for the first bring-up. As the AM67 arch layer
# grows (io_pins/gpio, spi), add the corresponding sources here (led.c,
# spi.cpp, ...) and link the arch_* targets, mirroring the STM32 boards.

# Board-scoped parameters. IMU_GYRO_RATEMAX normally comes from the
# vehicle_angular_velocity module, which this sensor-only board does not build;
# provide it here so it is a real, runtime-tunable parameter (see the ICM20948
# FIFO drain rate). Scoped to this board so it never collides with the module's
# definition on full builds.
set_property(GLOBAL APPEND PROPERTY PX4_MODULE_CONFIG_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/board_params.yaml)

add_library(drivers_board
	init.c
	i2c.cpp
	spi.cpp
	timer_config.cpp
)

target_link_libraries(drivers_board
	PRIVATE
		arch_spi
		nuttx_arch
		nuttx_drivers
		px4_layer
)
