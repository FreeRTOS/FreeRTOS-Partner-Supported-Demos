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
#include <stdio.h>
#include <drivers/soc.h>
#include <kernel/dpl/AddrTranslateP.h>
#include "ti_dpl_config.h"
#include "ti_drivers_config.h"


/* ----------- DebugP ----------- */
void putchar_(char character)
{
    /* Output to UART console */
    DebugP_uartLogWriterPutChar(character);
}


/* ----------- MmuP_armv8 ----------- */
#define CONFIG_MMU_NUM_REGIONS  (3u)

MmuP_Config gMmuConfig = {
    .numRegions = CONFIG_MMU_NUM_REGIONS,
    .enableMmu = 1,
};

MmuP_RegionConfig gMmuRegionConfig[CONFIG_MMU_NUM_REGIONS] =
{
    {
        .vaddr = 0x0u,
        .paddr = 0x0u,
        .size = 0x80000000u,
        .attr = {
            .accessPerm = MMUP_ACCESS_PERM_PRIV_RW_USER_NONE,
            .privExecute = 1,
            .userExecute = 0,
            .shareable = MMUP_SHARABLE_OUTER,
            .attrIndx = MMUP_ATTRINDX_MAIR0,
            .global = 1
        }
    },
    {
        .vaddr = 0x70000000u,
        .paddr = 0x70000000u,
        .size = 0x200000u,
        .attr = {
            .accessPerm = MMUP_ACCESS_PERM_PRIV_RW_USER_NONE,
            .privExecute = 1,
            .userExecute = 0,
            .shareable = MMUP_SHARABLE_OUTER,
            .attrIndx = MMUP_ATTRINDX_MAIR7,
            .global = 1
        }
    },
    {
        .vaddr = 0x80000000u,
        .paddr = 0x80000000u,
        .size = 0x80000000u,
        .attr = {
            .accessPerm = MMUP_ACCESS_PERM_PRIV_RW_USER_NONE,
            .privExecute = 1,
            .userExecute = 0,
            .shareable = MMUP_SHARABLE_OUTER,
            .attrIndx = MMUP_ATTRINDX_MAIR7,
            .global = 1
        }
    },
};

/* ----------- TimerP ----------- */
#define CONFIG_TIMER0_CLOCK_SRC_MUX_ADDR (0x430081B4u)
#define CONFIG_TIMER0_CLOCK_SRC_MCU_HFOSC0 (0x0u)


HwiP_Object gTimerHwiObj[TIMER_NUM_INSTANCES];
uint32_t gTimerBaseAddr[TIMER_NUM_INSTANCES];

void TimerP_isr0(void *args)
{
    void IntQueueTestTimerHandler( void );

    IntQueueTestTimerHandler ();

    TimerP_clearOverflowInt(gTimerBaseAddr[CONFIG_TIMER0]);
    HwiP_clearInt(CONFIG_TIMER0_INT_NUM);
}

void TimerP_init()
{
    TimerP_Params timerParams;
    HwiP_Params timerHwiParams;
    int32_t status;

    /* set timer clock source */
    SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, 2);
    *(volatile uint32_t*)AddrTranslateP_getLocalAddr(CONFIG_TIMER0_CLOCK_SRC_MUX_ADDR) = CONFIG_TIMER0_CLOCK_SRC_MCU_HFOSC0;
    SOC_controlModuleLockMMR(SOC_DOMAIN_ID_MAIN, 2);

    gTimerBaseAddr[CONFIG_TIMER0] = (uint32_t)AddrTranslateP_getLocalAddr(CONFIG_TIMER0_BASE_ADDR);

    TimerP_Params_init(&timerParams);
    timerParams.inputPreScaler = CONFIG_TIMER0_INPUT_PRE_SCALER;
    timerParams.inputClkHz     = CONFIG_TIMER0_INPUT_CLK_HZ;
    timerParams.periodInNsec   = CONFIG_TIMER0_NSEC_PER_TICK;
    timerParams.oneshotMode    = 0;
    timerParams.enableOverflowInt = 1;
    timerParams.enableDmaTrigger  = 0;
    TimerP_setup(gTimerBaseAddr[CONFIG_TIMER0], &timerParams);

    HwiP_Params_init(&timerHwiParams);
    timerHwiParams.intNum = CONFIG_TIMER0_INT_NUM;
    timerHwiParams.callback = TimerP_isr0;
    timerHwiParams.isPulse = 0;
    timerHwiParams.priority = 4;
    status = HwiP_construct(&gTimerHwiObj[CONFIG_TIMER0], &timerHwiParams);
    DebugP_assertNoLog(status==SystemP_SUCCESS);

}

void TimerP_deinit()
{
    TimerP_stop(gTimerBaseAddr[CONFIG_TIMER0]);
    HwiP_destruct(&gTimerHwiObj[CONFIG_TIMER0]);

}

/* ----------- ClockP ----------- */
#define TIMER6_BASE_ADDR     (0x2460000u)

ClockP_Config gClockConfig = {
    .timerBaseAddr = TIMER6_BASE_ADDR,
    .timerHwiIntNum = 158,
    .timerInputClkHz = 25000000,
    .timerInputPreScaler = 1,
    .usecPerTick = 1000,
};

/* This function is called by __system_start */
void __mmu_init()
{
    MmuP_init();
    CacheP_enable(CacheP_TYPE_ALL);
}

void Dpl_init(void)
{
    /* initialize Hwi but keep interrupts disabled */
    HwiP_init();

    /* init debug log zones early */

    /* Initialize Debug module from Core0 only */
    if(0 == Armv8_getCoreId())
    {

    /* Debug log init */
    DebugP_logZoneEnable(DebugP_LOG_ZONE_ERROR);
    DebugP_logZoneEnable(DebugP_LOG_ZONE_WARN);
    /* UART console to use for reading input */
    DebugP_uartSetDrvIndex(CONFIG_UART_CONSOLE);

    }

    /* Initialize Clock from Core0 only */
    if(0 == Armv8_getCoreId())
    {

    /* initialize Clock */
    ClockP_init();


    }

    TimerP_init();
    /* Enable interrupt handling */
    HwiP_enable();
}

void Dpl_deinit(void)
{
    TimerP_deinit();
}
