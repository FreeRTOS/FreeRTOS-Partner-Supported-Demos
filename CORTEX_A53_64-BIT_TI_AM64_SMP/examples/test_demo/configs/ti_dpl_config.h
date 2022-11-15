/*
 *  Copyright (C) 2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Auto generated file
 */

#ifndef TI_DPL_CONFIG_H
#define TI_DPL_CONFIG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/MmuP_armv8.h>
#include <kernel/dpl/ClockP.h>

/* ----------- TimerP ----------- */
#include <kernel/dpl/TimerP.h>

#define CONFIG_TIMER0                  (0u)
#define CONFIG_TIMER0_BASE_ADDR        (0x2410000u)
#define CONFIG_TIMER0_INT_NUM          (153u)
#define CONFIG_TIMER0_INPUT_CLK_HZ     (25000000u)
#define CONFIG_TIMER0_INPUT_PRE_SCALER (1u)
/* This represents the required time period 'floored' to nearest usec */
#define CONFIG_TIMER0_USEC_PER_TICK    (1000u)
/* This represents the required time period 'floored' to nearest nsec */
#define CONFIG_TIMER0_NSEC_PER_TICK    (1000000u)
/* This represents the actual time period 'floored' to nearest nsec */
#define CONFIG_TIMER0_NSEC_PER_TICK_ACTUAL  (1000000u)
/* NOTE: nsec per tick is used as input to the timer API */
/* NOTE: The actual nsec per tick that will be achieved depends on timer input clock hz precision */

#define TIMER_NUM_INSTANCES  (1u)

extern HwiP_Object gTimerHwiObj[TIMER_NUM_INSTANCES];
extern uint32_t gTimerBaseAddr[TIMER_NUM_INSTANCES];

void Dpl_init(void);
void Dpl_deinit(void);

extern uint64_t Armv8_getCoreId();

#ifdef __cplusplus
}
#endif

#endif /* TI_DPL_CONFIG_H */
