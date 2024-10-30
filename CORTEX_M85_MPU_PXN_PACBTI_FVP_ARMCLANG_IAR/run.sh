#!/bin/bash

# Copyright 2024 Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

FVP_Corstone_SSE-315 -a ./build/cortex_m85_mpu_pxn_pacbti_fvp_example_merged.elf -C mps4_board.visualisation.disable-visualisation=1 -C core_clk.mul=200000000 -C mps4_board.hostbridge.userNetworking=1 -C mps4_board.telnetterminal0.start_telnet=0 -C mps4_board.uart0.out_file="-" -C mps4_board.uart0.unbuffered_output=1 -C vis_hdlcd.disable_visualisation=1 --stat -C mps4_board.subsystem.cpu0.CFGPACBTI=1 -C mps4_board.subsystem.cpu0.ID_ISAR5.PACBTI=1 -C mps4_board.subsystem.cpu0.semihosting-enable=1
