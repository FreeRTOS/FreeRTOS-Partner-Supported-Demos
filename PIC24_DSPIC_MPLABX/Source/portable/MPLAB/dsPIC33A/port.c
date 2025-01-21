/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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


#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"

#define portTIMER_PRESCALE 8
#define INLINE inline __attribute__((always_inline))
/* Defined for backward compatability with project created prior to
FreeRTOS.org V4.3.0. */
#ifndef configKERNEL_INTERRUPT_PRIORITY
    #define configKERNEL_INTERRUPT_PRIORITY 1
#endif

/* Use _T1Interrupt as the interrupt handler name if the application writer has
not provided their own. */
#ifndef configTICK_INTERRUPT_HANDLER
	#define configTICK_INTERRUPT_HANDLER _T1Interrupt
#endif /* configTICK_INTERRUPT_HANDLER */


/* Records the nesting depth of calls to portENTER_CRITICAL(). */
UBaseType_t uxCriticalNesting = 0;

#if configKERNEL_INTERRUPT_PRIORITY != 1
    #error If configKERNEL_INTERRUPT_PRIORITY must be = 1 - almost lowest priority 
#endif

volatile StackType_t TopOfStack_addr = 0;

/* Hardware specifics. */
#define portINITIAL_SR  0x00000000  // SR Initial value for Task; CTX=0, IPL=0 

#define MAX_RAM_LIM_ADDRESS 0x00007FFC


INLINE void set_SPLIM(uint32_t new_splim);
__attribute__(( weak )) void vApplicationSetupTickTimerInterrupt( void );
/*-----------------------------------------------------------*/

/*
 * Initialise the stack of a task to look exactly as if a call to
 * portSAVE_CONTEXT had been called.
 *
 * See header file for description.
 */

#if ( portHAS_STACK_OVERFLOW_CHECKING == 1 )
 StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, StackType_t * pxEndOfStack, TaskFunction_t pxCode, void *pvParameters )
 {
    /* Setup the stack as if a yield had occurred. */
    *pxTopOfStack++ = 0xBAAAAAAD;  
    *pxTopOfStack++ = 0xCAFEB0BA;  
    *pxTopOfStack++ = 0xBEEFBABE;  
    *pxTopOfStack++ = 0xDEADBEEF;  

    *pxTopOfStack++ = portINITIAL_SR;                /*  Used when context-switch on portYIELD() - ISR-RETFIE  */
    *pxTopOfStack++ = ( StackType_t ) pxCode;        /*  Save the program counter - RETFIE will restore it */
    *pxTopOfStack++ = portINITIAL_SR;                /*  this may not be needed, since we unified the context switching always from ISR */
    *pxTopOfStack++ = ( StackType_t ) pvParameters;  /*  Parameters are passed in W0 */

    // Registers default value, unique for debugging   
    *pxTopOfStack++ = 0x000000A1;    // W1
    *pxTopOfStack++ = 0x000000A2;    // W2
    *pxTopOfStack++ = 0x000000A3;    // W3
    *pxTopOfStack++ = 0x000000A4;    // W4
    *pxTopOfStack++ = 0x000000A5;    // W5
    *pxTopOfStack++ = 0x000000A6;    // W6
    *pxTopOfStack++ = 0x000000A7;    // W7
    *pxTopOfStack++ = 0x000000A8;    // W8
    *pxTopOfStack++ = 0x000000A9;    // W9
    *pxTopOfStack++ = 0x00000A10;    // W10
    *pxTopOfStack++ = 0x00000A11;    // W11
    *pxTopOfStack++ = 0x00000A12;    // W12
    *pxTopOfStack++ = 0x00000A13;    // W13
    *pxTopOfStack++ = 0x00000A14;    // W14

    *pxTopOfStack++ = 0x000000F0;    // F0
    *pxTopOfStack++ = 0x000000F1;    // F1
    *pxTopOfStack++ = 0x000000F2;    // F2
    *pxTopOfStack++ = 0x000000F3;    // F3
    *pxTopOfStack++ = 0x000000F4;    // F4
    *pxTopOfStack++ = 0x000000F5;    // F5
    *pxTopOfStack++ = 0x000000F6;    // F6
    *pxTopOfStack++ = 0x000000F7;    // F7
    *pxTopOfStack++ = 0x000000F8;    // F8
    *pxTopOfStack++ = 0x000000F9;    // F9
    *pxTopOfStack++ = 0x00000F10;    // F10
    *pxTopOfStack++ = 0x00000F11;    // F11
    *pxTopOfStack++ = 0x00000F12;    // F12
    *pxTopOfStack++ = 0x00000F13;    // F13
    *pxTopOfStack++ = 0x00000F14;    // F14
    *pxTopOfStack++ = 0x00000F15;    // F15

    *pxTopOfStack++ = 0x00000F16;    // F16
    *pxTopOfStack++ = 0x00000F17;    // F17
    *pxTopOfStack++ = 0x00000F18;    // F18
    *pxTopOfStack++ = 0x00000F19;    // F19
    *pxTopOfStack++ = 0x00000F20;    // F20
    *pxTopOfStack++ = 0x00000F21;    // F21
    *pxTopOfStack++ = 0x00000F22;    // F22
    *pxTopOfStack++ = 0x00000F23;    // F23
    *pxTopOfStack++ = 0x00000F24;    // F24
    *pxTopOfStack++ = 0x00000F25;    // F24
    *pxTopOfStack++ = 0x00000F26;    // F26
    *pxTopOfStack++ = 0x00000F27;    // F27
    *pxTopOfStack++ = 0x00000F28;    // F28
    *pxTopOfStack++ = 0x00000F29;    // F29
    *pxTopOfStack++ = 0x00000F30;    // F30
    *pxTopOfStack++ = 0x00000F31;    // F31
    *pxTopOfStack++ = 0x00000000;    // FCR
    *pxTopOfStack++ = 0x00000000;    // FSR
    *pxTopOfStack++ = 0x00000000;    // FEAR    
 
    *pxTopOfStack++ = 0x00000000;    // RCOUNT
    *pxTopOfStack++ = 0x00000000;    // CORCON
    *pxTopOfStack++ = 0x00000101;    // MODCON
    *pxTopOfStack++ = 0x00000000;    // XMODSRT
    *pxTopOfStack++ = 0x00000000;    // XMODEND
    *pxTopOfStack++ = 0x00000000;    // YMODSRT
    *pxTopOfStack++ = 0x00000000;    // YMODEND
    *pxTopOfStack++ = 0x00000000;    // XBREV

    *pxTopOfStack++ = 0x00000000;    // ACCAL
    *pxTopOfStack++ = 0x00000000;    // ACCAH
    *pxTopOfStack++ = 0x00000000;    // ACCAU
    *pxTopOfStack++ = 0x00000000;    // ACCBL
    *pxTopOfStack++ = 0x00000000;    // ACCBH
    *pxTopOfStack++ = 0x00000000;    // ACCBU    

    *pxTopOfStack++ = 0x00000000;    // critical nesting - W0 
    *pxTopOfStack++ = ( StackType_t ) pxEndOfStack - 4; // SPLIM 

    return pxTopOfStack;

}

#else

 StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
 {
    /* Setup the stack as if a yield had occurred. */

    *pxTopOfStack++ = 0xBAAAAAAD;  
    *pxTopOfStack++ = 0xCAFEB0BA;     
    *pxTopOfStack++ = 0xBEEFBABE;  
    *pxTopOfStack++ = 0xDEADBEEF;  

    *pxTopOfStack++ = portINITIAL_SR;                /*  Used when context-switch on portYIELD() - ISR-RETFIE  */
    *pxTopOfStack++ = ( StackType_t ) pxCode;        /*  Save the program counter - RETFIE will restore it */
    *pxTopOfStack++ = portINITIAL_SR;                /*  this may not be needed, since we unified the context switching always from ISR */
    *pxTopOfStack++ = ( StackType_t ) pvParameters;  /*  Parameters are passed in W0 */

    // Registers default value, unique for debugging 
    *pxTopOfStack++ = 0x000000A1;    // W1
    *pxTopOfStack++ = 0x000000A2;    // W2
    *pxTopOfStack++ = 0x000000A3;    // W3
    *pxTopOfStack++ = 0x000000A4;    // W4
    *pxTopOfStack++ = 0x000000A5;    // W5
    *pxTopOfStack++ = 0x000000A6;    // W6
    *pxTopOfStack++ = 0x000000A7;    // W7
    *pxTopOfStack++ = 0x000000A8;    // W8
    *pxTopOfStack++ = 0x000000A9;    // W9
    *pxTopOfStack++ = 0x00000A10;    // W10
    *pxTopOfStack++ = 0x00000A11;    // W11
    *pxTopOfStack++ = 0x00000A12;    // W12
    *pxTopOfStack++ = 0x00000A13;    // W13
    *pxTopOfStack++ = 0x00000A14;    // W14

    *pxTopOfStack++ = 0x000000F0;    // F0
    *pxTopOfStack++ = 0x000000F1;    // F1
    *pxTopOfStack++ = 0x000000F2;    // F2
    *pxTopOfStack++ = 0x000000F3;    // F3
    *pxTopOfStack++ = 0x000000F4;    // F4
    *pxTopOfStack++ = 0x000000F5;    // F5
    *pxTopOfStack++ = 0x000000F6;    // F6
    *pxTopOfStack++ = 0x000000F7;    // F7
    *pxTopOfStack++ = 0x000000F8;    // F8
    *pxTopOfStack++ = 0x000000F9;    // F9
    *pxTopOfStack++ = 0x00000F10;    // F10
    *pxTopOfStack++ = 0x00000F11;    // F11
    *pxTopOfStack++ = 0x00000F12;    // F12
    *pxTopOfStack++ = 0x00000F13;    // F13
    *pxTopOfStack++ = 0x00000F14;    // F14
    *pxTopOfStack++ = 0x00000F15;    // F15

    *pxTopOfStack++ = 0x00000F16;    // F16
    *pxTopOfStack++ = 0x00000F17;    // F17
    *pxTopOfStack++ = 0x00000F18;    // F18
    *pxTopOfStack++ = 0x00000F19;    // F19
    *pxTopOfStack++ = 0x00000F20;    // F20
    *pxTopOfStack++ = 0x00000F21;    // F21
    *pxTopOfStack++ = 0x00000F22;    // F22
    *pxTopOfStack++ = 0x00000F23;    // F23
    *pxTopOfStack++ = 0x00000F24;    // F24
    *pxTopOfStack++ = 0x00000F25;    // F24
    *pxTopOfStack++ = 0x00000F26;    // F26
    *pxTopOfStack++ = 0x00000F27;    // F27
    *pxTopOfStack++ = 0x00000F28;    // F28
    *pxTopOfStack++ = 0x00000F29;    // F29
    *pxTopOfStack++ = 0x00000F30;    // F30
    *pxTopOfStack++ = 0x00000F31;    // F31
 
    *pxTopOfStack++ = 0x00000000;    // RCOUNT
    *pxTopOfStack++ = 0x00000000;    // CORCON
    *pxTopOfStack++ = 0x00000101;    // MODCON
    *pxTopOfStack++ = 0x00000000;    // XMODSRT
    *pxTopOfStack++ = 0x00000000;    // XMODEND
    *pxTopOfStack++ = 0x00000000;    // YMODSRT
    *pxTopOfStack++ = 0x00000000;    // YMODEND
    *pxTopOfStack++ = 0x00000000;    // XBREV

    *pxTopOfStack++ = 0x00000000;    // ACCAH
    *pxTopOfStack++ = 0x00000000;    // ACCAL
    *pxTopOfStack++ = 0x00000000;    // ACCAU
    *pxTopOfStack++ = 0x00000000;    // ACCBH
    *pxTopOfStack++ = 0x00000000;    // ACCBL
    *pxTopOfStack++ = 0x00000000;    // ACCBU    

    *pxTopOfStack++ = 0x00000000;    // critical nesting - W0 
    *pxTopOfStack++ = MAX_RAM_LIM_ADDRESS; // SPLIM 

    return pxTopOfStack;

}
#endif

/*-----------------------------------------------------------*/
BaseType_t xPortStartScheduler( void )
{
    /* Setup a timer for the tick ISR. */
    vApplicationSetupTickTimerInterrupt();

    __asm__ volatile ("NOP");
    __asm__ volatile ("NOP");

    /* Restore the context of the first task to run. */
    portRESTORE_CONTEXT();
    
    __asm__ volatile ( "retfie" ); 

    /* should not get here */
    __asm__ volatile ("NOP");
    __asm__ volatile ("NOP");

    /* Should not reach here. */
    return pdTRUE;
}


void vPortEndScheduler( void )
{
    /* Not implemented in ports where there is nothing to return to.
    Artificially force an assert. */
    configASSERT( uxCriticalNesting == 1000UL );
}


void vPortEnterCritical( void )
{
    portDISABLE_INTERRUPTS();
    uxCriticalNesting++;
}


void vPortExitCritical( void )
{
    configASSERT( uxCriticalNesting );
    uxCriticalNesting--;
    if( uxCriticalNesting == 0 )
    {
        portENABLE_INTERRUPTS();
    }
}


/* Critical section management. */
inline void portDISABLE_INTERRUPTS(void)
{                   
   /* TickTimer should operate at IPL = configKERNEL_INTERRUPT_PRIORITY, to block the tick ISR, 
    * elevate IPL to a level above than  configKERNEL_INTERRUPT_PRIORITY */

   uint8_t new_ipl_level = configKERNEL_INTERRUPT_PRIORITY+1;

   __asm__ volatile(   "DISICTL %0 \n\t" \
                        : /* no output */
                        : "r"(new_ipl_level)
                        : /* no clobbers */
                    );
}

/* Critical section management. */
inline void portENABLE_INTERRUPTS(void)
{   
   /* FreeRtos Tasks runs at IPL=0 */
   __asm__ volatile(   "DISICTL #0x0 \n\t" \
                        : /* no output */
                        : /* no inputs */
                        : /* no clobbers */
                    );                     
}


void __attribute__ ((naked)) portYIELD(void) 
{               
   /* portYIELD enters on :
        1 : Systick_Callback CTX=1, IPL1 
            but we want to save/restore Ctx-0 regs (IPL0 Task regs)
        2: SW Interrupts:
            Handled as trap IPL > 8, but CTX=0
            changing CTX is redundant for a SW Trap, but takes longer to test 
            for CTX value than just always change CTX=0 */
    
   __asm__ volatile ("CTXTSWP #0x0");                          

   /* Now focused on CTX-0, save the context, Save the context of the current task */
   portSAVE_CONTEXT(); 

   /* Switch to a different thread if ready */
   vTaskSwitchContext(); 

   /* Temporary override SPLIM to the MAX RAM Address to avoid Stack-error-trap the appropriate SPLIM 
    * for the next task will be restored inside portRESTORE_CONTEXT */
   set_SPLIM(MAX_RAM_LIM_ADDRESS); 

   /* we should still be on ctx-0 after pointing to the new TCB, Restore the context of whichever task is going to run */
   portRESTORE_CONTEXT();  

                    
   /* A portYIELD_WITHIN_API elevates IPL using portDISABLE_INTERRUPTS to avoid collisions with other ISR that 
    * could yield it just in case portYIELD was called from API */
   portENABLE_INTERRUPTS();
   
   /* No return to entry-isr , force a retfie to restore PC and SR to next-task */
    __asm__ volatile ( "retfie" ); 
}

void __attribute__((__interrupt__, naked)) configTICK_INTERRUPT_HANDLER( void )
{   
    IFS1bits.T1IF = 0;
    if( xTaskIncrementTick() != pdFALSE )
    { 
        /* Elevate IPL to avoid collisions with other ISR that could yield */
        portDISABLE_INTERRUPTS(); 
        /* goto portYIELD but don't return */
        __asm__ volatile ( "GOTO _portYIELD" ); 
    }
}


void  __attribute__ ((interrupt, naked)) _GeneralTrap(void) 
{
    /* _GeneralTrap covers multiple Traps only portYIELD if the software trap was triggered */            
    if (INTCON5bits.SOFT == 1) {
        INTCON5bits.SOFT = 0;   
        /* goto portYIELD but don't return */
        __asm__ volatile ( "GOTO _portYIELD" ); 
    }
}


/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*-----------------------------------------------------------*/


void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    /* When stack overflow happens, trap instead of attempting to recover.
    Read input arguments to learn about the offending task. */
    for( ;; )
    {
        /* Doesn't do anything yet. */
       __asm__ volatile ("NOP");
       __asm__ volatile ("NOP");
    }
}



INLINE void set_SPLIM(uint32_t new_splim)
{
   __asm__ volatile(   "MOV.l  %0, SPLIM \n\t" \
                        : /* no outputs */
                        : "r"(new_splim)
                        : /* no clobbers */
                    );
}

__attribute__(( weak )) void vApplicationSetupTickTimerInterrupt( void )
{
    const uint32_t ulCompareMatch = ( ( configCPU_CLOCK_HZ / portTIMER_PRESCALE ) / configTICK_RATE_HZ ) - 1;

	/* Prescale of 8. */
	T1CON = 0;
	TMR1 = 0;

	PR1 = ( uint32_t ) ulCompareMatch;

	/* Setup timer 1 interrupt priority. */
	IPC6bits.T1IP = configKERNEL_INTERRUPT_PRIORITY;

	/* Clear the interrupt as a starting condition. */
	IFS1bits.T1IF = 0;

	/* Enable the interrupt. */
	IEC1bits.T1IE = 1;

#if defined (__dsPIC33C__)
	/* Setup the prescale value. */
	T1CONbits.TCKPS = 1;
#elif defined (__dsPIC33A__)
    T1CONbits.TCKPS = 1;
#else
    T1CONbits.TCKPS0 = 1;
	T1CONbits.TCKPS1 = 0;
#endif    
	/* Start the timer. */
	T1CONbits.ON = 1;
}
