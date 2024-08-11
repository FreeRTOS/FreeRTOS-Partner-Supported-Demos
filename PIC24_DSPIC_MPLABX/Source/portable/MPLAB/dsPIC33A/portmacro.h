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

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		int64_t
#define portLONG		long
#define portSTACK_TYPE	int
#define portSHORT       short
#define portBASE_TYPE	int

typedef portSTACK_TYPE StackType_t;
typedef int BaseType_t;
typedef unsigned int UBaseType_t;

typedef uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL

/* Hardware specifics. */
#define portBYTE_ALIGNMENT			4
#define portSTACK_GROWTH			1
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )


/* Note that exiting a critical sectino will set the IPL bits to 0, nomatter
what their value was prior to entering the critical section. */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()

/* Critical section management. */
void portDISABLE_INTERRUPTS(void);

/* Critical section management. */
void portENABLE_INTERRUPTS(void);

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )


/* Required by the kernel aware debugger. */
#ifdef __DEBUG
	#define portREMOVE_STATIC_QUALIFIER
#endif

#define portNOP()         asm volatile ( "NOP" )


void __attribute__ ((naked)) portYIELD(void);

// When yielding from API (outside an ISR), we need to manually save the SR
// required by the RETFIE instruction executed at the end of the portYIELD procedure
// The PC saved when calling the function portYIELD, is the PC
// used by RETFIE.  Note that a RETFIE is manually implanted at the end of portYIELD
// Procedure:
//    push SR -> stack
//    rcall _portYIELD
//        then inside retfie -> restores PC, and the manually saved SR 
//
// NOTE: It is critical to protect portYIELD from beign interrupted
//       by systick, otherwise the YIELD procedure could be corrupted by 
//       a second YIELD started from systick.
//
//    *  Temporay disable the SysTick timer interrupt IEC1bits.T1IE = 0, so the portYIELD_WITHIN_API
//       and SysTick portYIELD dont collide and step on each other, and higher IPL interrupts can 
//       still be processed 
//
//    *  After testing, there may be other interrupts with higher IPL 
//       that could cause a portYIELD from ISR, in this case, the 
//       portYIELD_WITHIN_API() may be corrupted, it is safer to elevate the IPL
//       and cover the whole group of interrupts, rather than just disable the SysTick ISR
//       using IEC1bits.T1IE = 0, see portDISABLE_INTERRUPTS()
//
 
#define portYIELD_WITHIN_API() ({                                             \
                                  portDISABLE_INTERRUPTS();                   \
                                  __asm__ volatile ("MOV.l   SR, [W15++]");   \
                                  __asm__ volatile ("RCALL _portYIELD");      \
                                  __asm__ volatile ("NOP");                   \
                                  __asm__ volatile ("NOP");                   \
                               });




#define portSAVE_CONTEXT() ({           \
                                        \
       __asm__ volatile ("NOP");        \
       __asm__ volatile ("NOP");        \
                                        \
    __asm__ volatile(   "MOV.l   SR, [W15++]                \n"      \
                        "PUSH.l  W0                         \n"      \
                        "PUSH.l  W1                         \n"      \
                        "PUSH.l  W2                         \n"      \
                        "PUSH.l  W3                         \n"      \
                        "PUSH.l  W4                         \n"      \
                        "PUSH.l  W5                         \n"      \
                        "PUSH.l  W6                         \n"      \
                        "PUSH.l  W7                         \n"      \
                        "PUSH.l  W8                         \n"      \
                        "PUSH.l  W9                         \n"      \
                        "PUSH.l  W10                        \n"      \
                        "PUSH.l  W11                        \n"      \
                        "PUSH.l  W12                        \n"      \
                        "PUSH.l  W13                        \n"      \
                        "PUSH.l  W14                        \n"      \
                        "                                   \n"      \
                        "PUSH.l  F0                         \n"      \
                        "PUSH.l  F1                         \n"      \
                        "PUSH.l  F2                         \n"      \
                        "PUSH.l  F3                         \n"      \
                        "PUSH.l  F4                         \n"      \
                        "PUSH.l  F5                         \n"      \
                        "PUSH.l  F6                         \n"      \
                        "PUSH.l  F7                         \n"      \
                        "PUSH.l  F8                         \n"      \
                        "PUSH.l  F9                         \n"      \
                        "PUSH.l  F10                        \n"      \
                        "PUSH.l  F11                        \n"      \
                        "PUSH.l  F12                        \n"      \
                        "PUSH.l  F13                        \n"      \
                        "PUSH.l  F14                        \n"      \
                        "PUSH.l  F15                        \n"      \
                        "                                   \n"      \
                        "PUSH.l  F16                        \n"      \
                        "PUSH.l  F17                        \n"      \
                        "PUSH.l  F18                        \n"      \
                        "PUSH.l  F19                        \n"      \
                        "PUSH.l  F20                        \n"      \
                        "PUSH.l  F21                        \n"      \
                        "PUSH.l  F22                        \n"      \
                        "PUSH.l  F23                        \n"      \
                        "PUSH.l  F24                        \n"      \
                        "PUSH.l  F25                        \n"      \
                        "PUSH.l  F26                        \n"      \
                        "PUSH.l  F27                        \n"      \
                        "PUSH.l  F28                        \n"      \
                        "PUSH.l  F29                        \n"      \
                        "PUSH.l  F30                        \n"      \
                        "PUSH.l  F31                        \n"      \
                        "PUSH.l  FCR                        \n"      \
                        "PUSH.l  FSR                        \n"      \
                        "PUSH.l  FEAR                       \n"      \
                        "                                   \n"      \
                        "PUSH.l  RCOUNT                     \n"      \
                        "PUSH.l  CORCON                     \n"      \
                        "PUSH.l  MODCON                     \n"      \
                        "PUSH.l  XMODSRT                    \n"      \
                        "PUSH.l  XMODEND                    \n"      \
                        "PUSH.l  YMODSRT                    \n"      \
                        "PUSH.l  YMODEND                    \n"      \
                        "PUSH.l  XBREV                      \n"      \
                        "                                   \n"      \
                        "SLAC.l  A, [W15++]                 \n"      \
                        "SAC.l   A, [W15++]                 \n"      \
                        "SUAC.l  A, [W15++]                 \n"      \
                        "SLAC.l  B, [W15++]                 \n"      \
                        "SAC.l   B, [W15++]                 \n"      \
                        "SUAC.l  B, [W15++]                 \n"      \
                        "                                   \n"      \
                        "MOV.l   _uxCriticalNesting, W0     \n"      \
                        "PUSH.l  W0                         \n"      \
                        "PUSH.l  SPLIM                      \n"      \
                        "MOV.l   _pxCurrentTCB, W0          \n"      \
                        "MOV.l   W15, [W0]                  \n"      \
                        ); \
});





#define portRESTORE_CONTEXT() ({        \
                                        \
       __asm__ volatile ("NOP");        \
       __asm__ volatile ("NOP");        \
                                        \
    __asm__ volatile(   "                                   \n"      \
                        "MOV.l   _pxCurrentTCB, W0          \n"      \
                        "MOV.l   [W0], W15                  \n"      \
                        "POP.l   SPLIM                      \n"      \
                        "POP.l   W0                         \n"      \
                        "MOV.l   W0, _uxCriticalNesting     \n"      \
                        "                                   \n"      \
                        "LUAC.l  [--W15], B                 \n"      \
                        "LAC.l   [--W15], B                 \n"      \
                        "LLAC.l  [--W15], B                 \n"      \
                        "LUAC.l  [--W15], A                 \n"      \
                        "LAC.l   [--W15], A                 \n"      \
                        "LLAC.l  [--W15], A                 \n"      \
                        "                                   \n"      \
                        "POP.l   XBREV                      \n"      \
                        "POP.l   YMODEND                    \n"      \
                        "POP.l   YMODSRT                    \n"      \
                        "POP.l   XMODEND                    \n"      \
                        "POP.l   XMODSRT                    \n"      \
                        "POP.l   MODCON                     \n"      \
                        "POP.l   CORCON                     \n"      \
                        "POP.l   RCOUNT                     \n"      \
                        "                                   \n"      \
                        "POP.l   FEAR                       \n"      \
                        "POP.l   FSR                        \n"      \
                        "POP.l   FCR                        \n"      \
                        "POP.l   F31                        \n"      \
                        "POP.l   F30                        \n"      \
                        "POP.l   F29                        \n"      \
                        "POP.l   F28                        \n"      \
                        "POP.l   F27                        \n"      \
                        "POP.l   F26                        \n"      \
                        "POP.l   F25                        \n"      \
                        "POP.l   F24                        \n"      \
                        "POP.l   F23                        \n"      \
                        "POP.l   F22                        \n"      \
                        "POP.l   F21                        \n"      \
                        "POP.l   F20                        \n"      \
                        "POP.l   F19                        \n"      \
                        "POP.l   F18                        \n"      \
                        "POP.l   F17                        \n"      \
                        "POP.l   F16                        \n"      \
                        "                                   \n"      \
                        "POP.l   F15                        \n"      \
                        "POP.l   F14                        \n"      \
                        "POP.l   F13                        \n"      \
                        "POP.l   F12                        \n"      \
                        "POP.l   F11                        \n"      \
                        "POP.l   F10                        \n"      \
                        "POP.l   F9                         \n"      \
                        "POP.l   F8                         \n"      \
                        "POP.l   F7                         \n"      \
                        "POP.l   F6                         \n"      \
                        "POP.l   F5                         \n"      \
                        "POP.l   F4                         \n"      \
                        "POP.l   F3                         \n"      \
                        "POP.l   F2                         \n"      \
                        "POP.l   F1                         \n"      \
                        "POP.l   F0                         \n"      \
                        "                                   \n"      \
                        "POP.l   W14                        \n"      \
                        "POP.l   W13                        \n"      \
                        "POP.l   W12                        \n"      \
                        "POP.l   W11                        \n"      \
                        "POP.l   W10                        \n"      \
                        "POP.l   W9                         \n"      \
                        "POP.l   W8                         \n"      \
                        "POP.l   W7                         \n"      \
                        "POP.l   W6                         \n"      \
                        "POP.l   W5                         \n"      \
                        "POP.l   W4                         \n"      \
                        "POP.l   W3                         \n"      \
                        "POP.l   W2                         \n"      \
                        "POP.l   W1                         \n"      \
                        "POP.l   W0                         \n"      \
                        "MOV.l   [--W15], SR                \n\t"    \
                     ); \
});



#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
