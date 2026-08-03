############################################################################
#   Copyright (c) 2024 PX4 Development Team. All rights reserved.
#   (License: BSD-3-Clause - see other PX4 CMakeLists for full text.)
############################################################################

px4_add_library(arch_version
	board_identity.c
	board_mcu_version.c
)
