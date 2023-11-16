/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software. If you wish to use our Amazon
 * FreeRTOS name, please do so in a fair use way that does not cause confusion.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Platform includes. */
#include "Ifx_Types.h"
#include "Gpt12/Std/IfxGpt12.h"
#include "IfxPort_PinMap.h"
#include "Port/Io/IfxPort_Io.h"

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Demo includes. */
#include "IntQueueTimer.h"
#include "IntQueue.h"

/* TIMER and LEDs configuration values. */
#define ISR_PRIORITY_GPT12_TIMER    31                      /* Define the GPT12 Timer interrupt priority             */
#define ISR_PROVIDER_GPT12_TIMER    IfxSrc_Tos_cpu0         /* Interrupt provider                                    */

#define RELOAD_VALUE                2441u                   /* Reload value to have an interrupt each 500ms, recalculated */

#define LED_1                       IfxPort_P00_5           /* Port/Pin for LED 1 */

/*-----------------------------------------------------------*/
IFX_INTERRUPT(IntQueueTestTimerHandler, 0, ISR_PRIORITY_GPT12_TIMER);

void IntQueueTestTimerHandler( void )
{
    IfxPort_setPinState(LED_1.port, LED_1.pinIndex, IfxPort_State_toggled);
    portYIELD_FROM_ISR( xSecondTimerHandler() );
}

/*-----------------------------------------------------------*/
void vInitialiseTimerForIntQueueTest( void )
{
    /* Initialize the GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);                                          /* Enable the GPT12 module      */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_16); /* Set GPT2 block prescaler     */

    /* Initialize the Timer T3 */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);                       /* Set T3 to timer mode         */
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);    /* Set T3 count direction       */
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_64); /* Set T3 input prescaler       */
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, RELOAD_VALUE);                        /* Set T3 start value           */


    /* Initialize the Timer T2 */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);                      /* Set T2 to reload mode        */
    IfxGpt12_T2_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_bothEdgesTxOTL); /* Set reload trigger  */
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, RELOAD_VALUE);                        /* Set T2 reload value          */

    /* Initialize the interrupt */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T3_getSrc(&MODULE_GPT120);                /* Get the interrupt address    */
    IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT12_TIMER);           /* Initialize service request   */
    IfxSrc_enable(src);                                                             /* Enable GPT12 interrupt       */

    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);                       /* Start the timer              */

    IfxPort_setPinMode(LED_1.port, LED_1.pinIndex, IfxPort_Mode_outputPushPullGeneral);
}

/*-----------------------------------------------------------*/
