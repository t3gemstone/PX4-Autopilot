
# Compiler flags for the ARM Cortex-R5F (ARMv7-R), used by the TI AM67 / J722S.
#
# PX4 selects this file via CMAKE_SYSTEM_PROCESSOR = CONFIG_BOARD_ARCHITECTURE
# ("cortex-r5f"). Without it, CMAKE_C_FLAGS carries no -mcpu and the NuttX
# assembly (cpsid/ldrex/strex) fails with "selected processor does not support".
#
# The FPU choice mirrors arch/arm/src/armv7-r/Toolchain.defs so the whole image
# (NuttX + PX4 libraries) is built with one consistent float ABI.

if(CONFIG_ARCH_FPU)
	if(CONFIG_ARM_FPU_ABI_SOFT)
		set(float_abi "softfp")
	else()
		set(float_abi "hard")
	endif()
	set(cpu_flags "-mcpu=cortex-r5 -mfpu=vfpv3-d16 -mfloat-abi=${float_abi}")
else()
	set(cpu_flags "-mcpu=cortex-r5 -mfloat-abi=soft")
endif()

set(CMAKE_C_FLAGS   "${cpu_flags}" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${cpu_flags}" CACHE STRING "" FORCE)
set(CMAKE_ASM_FLAGS "${cpu_flags} -D__ASSEMBLY__" CACHE STRING "" FORCE)
