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

# PX4 arch-support layer for the TI AM67 / J722S Cortex-R5F.
#
# The flat firmware link (platforms/nuttx/CMakeLists.txt) requires at minimum
# the `arch_hrt` and `arch_board_reset` targets. Implemented so far:
#
#   [x] hrt          - high-resolution timer on DMTimer1  (arch_hrt)
#   [x] board_reset  - reset/halt hooks                   (arch_board_reset)
#   [x] version      - MCU/board version strings          (arch_version)
#   [x] board_critmon- crit/irq monitor time source       (arch_board_critmon)
#   [x] io_pins      - px4_arch_configgpio/gpiowrite/read (in px4_arch/micro_hal.h,
#                      over am67_gpio; no separate lib needed - header-only)
#   [x] spi          - PX4 SPI bus/CS glue (over am67_mcspi)  (arch_spi)
#
# See the board directory (boards/t3gemstone/o1) for the top board layer.

add_subdirectory(hrt)
add_subdirectory(board_reset)
add_subdirectory(board_critmon)
add_subdirectory(version)
add_subdirectory(spi)
add_subdirectory(io_pins)
