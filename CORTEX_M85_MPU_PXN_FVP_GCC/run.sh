#!/bin/bash

# Copyright 2024 Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

FVP_Corstone_SSE-310 -a cpu0*=./build/cortex_m85_mpu_pxn_fvp_example.axf -C mps3_board.visualisation.disable-visualisation=1 -C core_clk.mul=200000000 -C mps3_board.hostbridge.userNetworking=1 -C mps3_board.telnetterminal0.start_telnet=0 -C mps3_board.uart0.out_file="-" -C mps3_board.uart0.unbuffered_output=1 --stat -C mps3_board.DISABLE_GATING=1
