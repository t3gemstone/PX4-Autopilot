/****************************************************************************
 * boards/t3gemstone/o1/nuttx-config/include/board.h
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_T3GEMSTONE_O1_NUTTX_CONFIG_INCLUDE_BOARD_H
#define __BOARDS_T3GEMSTONE_O1_NUTTX_CONFIG_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/* The AM67 / J722S Cortex-R5F clocks, muxes and power are configured by the
 * Linux-side Device Manager / bootloader before remoteproc starts this core,
 * so there is nothing for NuttX to program here. The system tick and the PX4
 * HRT drive DMTimer0/DMTimer1 directly (see arch/arm/src/am67/am67_timer.c and
 * platforms/nuttx/src/px4/ti/am67/hrt/hrt.c).
 */

/****************************************************************************
 * Assembly Language Macros
 ****************************************************************************/

#ifdef __ASSEMBLY__
  .macro  config_sdram
  .endm
#endif /* __ASSEMBLY__ */

#endif /* __BOARDS_T3GEMSTONE_O1_NUTTX_CONFIG_INCLUDE_BOARD_H */
