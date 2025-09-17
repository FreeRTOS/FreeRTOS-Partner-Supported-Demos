/*============================================================================*/
/* Project      = AUTOSAR Renesas X2x MCAL Components                         */
/* File name    = Compiler.h                                                  */
/* SW-VERSION   = 2.1.1                                                       */
/*============================================================================*/
/*                                  COPYRIGHT                                 */
/*============================================================================*/
/* (c) 2017,2018,2020 Renesas Electronics Corporation. All rights reserved.   */
/*============================================================================*/
/* Purpose:                                                                   */
/* Provision for compiler macros                                              */
/*============================================================================*/
/*                                                                            */
/* Unless otherwise agreed upon in writing between your company and           */
/* Renesas Electronics Corporation the following shall apply!                 */
/*                                                                            */
/* Warranty Disclaimer                                                        */
/*                                                                            */
/* There is no warranty of any kind whatsoever granted by Renesas. Any        */
/* warranty is expressly disclaimed and excluded by Renesas, either expressed */
/* or implied, including but not limited to those for non-infringement of     */
/* intellectual property, merchantability and/or fitness for the particular   */
/* purpose.                                                                   */
/*                                                                            */
/* Renesas shall not have any obligation to maintain, service or provide bug  */
/* fixes for the supplied Product(s) and/or the Application.                  */
/*                                                                            */
/* Each User is solely responsible for determining the appropriateness of     */
/* using the Product(s) and assumes all risks associated with its exercise    */
/* of rights under this Agreement, including, but not limited to the risks    */
/* and costs of program errors, compliance with applicable laws, damage to    */
/* or loss of data, programs or equipment, and unavailability or              */
/* interruption of operations.                                                */
/*                                                                            */
/* Limitation of Liability                                                    */
/*                                                                            */
/* In no event shall Renesas be liable to the User for any incidental,        */
/* consequential, indirect, or punitive damage (including but not limited     */
/* to lost profits) regardless of whether such liability is based on breach   */
/* of contract, tort, strict liability, breach of warranties, failure of      */
/* essential purpose or otherwise and even if advised of the possibility of   */
/* such damages. Renesas shall not be liable for any services or products     */
/* provided by third party vendors, developers or consultants identified or   */
/* referred to the User by Renesas in connection with the Product(s) and/or   */
/* the Application.                                                           */
/*                                                                            */
/*============================================================================*/
/* Environment:                                                               */
/*              Devices:        X2x                                           */
/*============================================================================*/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 *  1.2.0:  26/08/2020   : Release
 *  1.1.0:  19/06/2020   : Release
 *  1.0.5:  16/01/2020   : Remove AR Release version.
 *
 *  1.0.4:  21/12/2018  : Delete FE_INTERRUPT_ENTRY and FE_INTERRUPT_LEAVE.
 *
 *  1.0.3:  11/01/2018  : Added new macro FE_INT and _INTERRUPT_FE_ for 
 *                         for multicore support.
 *                       : Added SYNCM
 *
 *  1.0.2:  24/04/2017  : Correct code for backup and restore FPSR, FPEPC.
 *
 *  1.0.1:  04/04/2017  : Added MISRA C Rule Violations comments
 *
 *  1.0.0:  05/01/2017  : Initial Version
 * 
 */
/******************************************************************************/

#ifndef COMPILER_H
#define COMPILER_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#include "Compiler_Cfg.h"    /* Module specific memory and pointer classes */


/*******************************************************************************
**                      Version Information                                  **
*******************************************************************************/

/*******************************************************************************
**                      MISRA C Rule Violations                               **
*******************************************************************************/

/* 1. MISRA C RULE VIOLATION:                                                 */
/* Message       : (2:3453)A function could probably be used instead of this  */
/*                 function-like macro.                                       */
/* Rule          : MISRA-C:2004 Rule 19.7                                     */
/*                 REFERENCE - None                                           */
/* Justification : The Macro is preferred here rather than a function for     */
/*                 better execution efficiency.                               */
/* Verification  : However, part of the code is verified manually and it is   */
/*                 not having any impact.                                     */
/* Reference     : Look for START Msg(2:3453)-1 and                           */
/*                 END Msg(2:3453)-1 tags in the code.                        */
/******************************************************************************/

/* 2. MISRA C RULE VIOLATION:                                                 */
/* Message       : (2:3409)The replacement list of function-like macro        */
/*                 'FUNC_P2VAR' is not enclosed in ().                        */
/* Rule          : MISRA-C:2004 Rule 19.4                                     */
/*                 REFERENCE - ISO:C90-6.8                                    */
/* Justification : The AUTOSAR requirement [SWS_COMPILER_00061] and           */
/*                 [SWS_COMPILER_00063] defined syntax.                       */
/* Verification  : However, part of the code is verified manually and it is   */
/*                 not having any impact.                                     */
/* Reference     : Look for START Msg(2:3409)-2 and                           */
/*                 END Msg(2:3409)-2 tags in the code.                        */
/******************************************************************************/

/* 3. MISRA C RULE VIOLATION:                                                 */
/* Message       : (2:3410)Macro parameter not enclosed in ().                */
/* Rule          : MISRA-C:2004 Rule 19.10                                    */
/*                 REFERENCE - None                                           */
/* Justification : The AUTOSAR requirement [SWS_COMPILER_00061] and           */
/*                 [SWS_COMPILER_00063] defined syntax.                       */
/* Verification  : However, part of the code is verified manually and it is   */
/*                 not having any impact.                                     */
/* Reference     : Look for START Msg(2:3410)-3 and                           */
/*                 END Msg(2:3410)-3 tags in the code.                        */
/******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*
 * Compiler abstraction symbols
 */
 
#define STATIC  static
 
/* SWS_COMPILER_00057 */
#define INLINE  inline

/* SWS_COMPILER_00060 */
#define LOCAL_INLINE    static inline

#define NO_PROLOGUE  "ghs noprologue"

#define PRAGMA(x)    _Pragma(x)

#define _INTERRUPT_  __interrupt

#define FE_INT       "ghs interrupt(FE)"

#define _INTERRUPT_FE_  _Pragma(FE_INT)

#ifndef _GREENHILLS_C_RH850_
#define _GREENHILLS_C_RH850_
#endif

#ifndef _GREENHILLS_C_V850_
#define _GREENHILLS_C_V850_
#endif

/* SWS_COMPILER_00051 */
#ifndef NULL_PTR
  #define NULL_PTR  ((void *)0)
#endif

/* AUTOMATIC used for the declaration of local pointers */
/* SWS_COMPILER_00046 */
#define AUTOMATIC

/* TYPEDEF used for defining pointer types within type definitions */
/* SWS_COMPILER_00059 */
#define TYPEDEF

/* The compiler abstraction shall define the P2FUNC macro for the type
   definition of pointers to functions.
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
   fctname     function name respectively name of the defined type
 */
/* SWS_COMPILER_00039 */
#define P2FUNC(rettype, ptrclass, fctname) rettype (*fctname)

/* The compiler abstraction shall define the FUNC macro for the declaration and
   definition of functions, that ensures correct syntax of function
   declarations as required by a specific compiler. - used for API functions
   rettype     return type of the function
   memclass    classification of the function itself
 */
/* SWS_COMPILER_00001 */
#define FUNC(rettype, memclass) memclass rettype

/* The compiler abstraction shall define the FUNC_P2CONST macro for the
   declaration and definition of functions returning a pointer to a constant.
   This shall ensure the correct syntax of function declarations as required by
   a specific compiler.
 */
/* SWS_COMPILER_00061 */
/* MISRA Violation: START Msg(2:3453)-1 */
/* MISRA Violation: START Msg(2:3409)-2 */
/* MISRA Violation: START Msg(2:3410)-3 */
#define FUNC_P2CONST(rettype, ptrclass, memclass)\
 const ptrclass rettype * memclass
/* END Msg(2:3410)-3 */
/* END Msg(2:3409)-2 */
/* END Msg(2:3453)-1 */

/* The compiler abstraction shall define the FUNC_P2VAR macro for the 
   declaration and definition of functions returning a pointer to a variable.
   This shall ensure the correct syntax of function declarations as required
   by a specific compiler. */
/* SWS_COMPILER_00063 */
/* MISRA Violation: START Msg(2:3453)-1 */
/* MISRA Violation: START Msg(2:3409)-2 */
/* MISRA Violation: START Msg(2:3410)-3 */
#define FUNC_P2VAR(rettype, ptrclass, memclass) ptrclass rettype * memclass
/* END Msg(2:3410)-3 */
/* END Msg(2:3409)-2 */
/* END Msg(2:3453)-1 */

/* The compiler abstraction shall define the P2CONST macro for the declaration
   and definition of pointers in RAM pointing to constants
   ptrtype     type of the referenced constant
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
/* SWS_CONPILER_00013 */
#define P2CONST(ptrtype, memclass, ptrclass)  const ptrtype *

/* The compiler abstraction shall define the P2VAR macro for the declaration 
   and definition of pointers in RAM, pointing to variables.
   ptrtype     type of the referenced variable
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
/* SWS_COMPILER_00006 */
#define P2VAR(ptrtype, memclass, ptrclass) ptrtype *

/* The compiler abstraction shall define the CONSTP2VAR macro for the
   declaration and definition of constant pointers accessing variables.
   ptrtype     type of the referenced variable
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
/* SWS_COMPILER_00031 */
#define CONSTP2VAR(ptrtype, memclass, ptrclass) ptrtype * const

/* The compiler abstraction shall define the CONSTP2CONST macro for the
   declaration and definition of constant pointers accessing constants.
   ptrtype     type of the referenced constant
   memclass    classification of the pointer's variable itself
   ptrclass    defines the classification of the pointer's distance
 */
/* SWS_COMPILER_00032 */
#define CONSTP2CONST(ptrtype, memclass, ptrclass) const ptrtype * const

/* The compiler abstraction shall define the CONSTP2FUNC macro for the
   type definition of constant pointers to functions.
   rettype     return type of the function
   ptrclass    defines the classification of the pointer's distance
   fctname     function name respectively name of the defined type
 */
/* SWS_COMPILER_00065 */
#define CONSTP2FUNC(rettype, ptrclass, fctname) rettype (* const fctname)

/* The compiler abstraction shall define the CONST macro for the
   declaration and definition of constants.
   consttype   type of the constant
   memclass    classification of the constant itself
 */
/* SWS_COMPILER_00023 */
#define CONST(consttype, memclass) const consttype

/* The compiler abstraction shall define the VAR macro for the declaration
   and definition of variables.
   vartype     type of the variable
   memclass    classification of the variable itself
 */
/* SWS_COMPILER_00026 */
#define VAR(vartype, memclass) vartype


/* Inline assembler support for HALT instruction */
#define ASM_HALT() __asm("halt")

/* Inline assembler support for NOP instruction */
#define ASM_NOP() __asm("nop")

#define ENABLE_INTERRUPT()        __asm("ei")
#define DISABLE_INTERRUPT()       __asm("di")

/* Inline assembler support for SYNCI instruction */
#define EXECUTE_SYNCI() __asm("synci")

/* Inline assembler support for syncp instruction */
#define EXECUTE_SYNCP()           __asm("syncp")

/* Inline assembler support for syncm instruction */
#define EXECUTE_SYNCM()           __asm("syncm")

/* Utilize the GHS specific intrinsic functions for accessing the cpu system
registers */
#define LDSR                           __LDSR

#define STSR                           __STSR

void __LDSR(int regID, int selID, unsigned int val);

unsigned int __STSR(int regID, int selID);




#define __HAS_FPU__


/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

#endif /* COMPILER_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
