;-------------------------------------------------------------------------------
;-- Module      = startup_PE0.asm
;-- Version     = 1.0.0
;-------------------------------------------------------------------------------
;--                                  COPYRIGHT
;-------------------------------------------------------------------------------
;-- Copyright (c) 2025 by Renesas Electronics Europe GmbH,
;--               a company of the Renesas Electronics Corporation
;-------------------------------------------------------------------------------
;-- Purpose:      Startup Code
;--
;-------------------------------------------------------------------------------
;--
;-- Warranty Disclaimer
;--
;-- Because the Product(s) is licensed free of charge, there is no warranty
;-- of any kind whatsoever and expressly disclaimed and excluded by Renesas,
;-- either expressed or implied, including but not limited to those for
;-- non-infringement of intellectual property, merchantability and/or
;-- fitness for the particular purpose.
;-- Renesas shall not have any obligation to maintain, service or provide bug
;-- fixes for the supplied Product(s) and/or the Application.
;--
;-- Each User is solely responsible for determining the appropriateness of
;-- using the Product(s) and assumes all risks associated with its exercise
;-- of rights under this Agreement, including, but not limited to the risks
;-- and costs of program errors, compliance with applicable laws, damage to
;-- or loss of data, programs or equipment, and unavailability or
;-- interruption of operations.
;--
;-- Limitation of Liability
;--
;-- In no event shall Renesas be liable to the User for any incidental,
;-- consequential, indirect, or punitive damage (including but not limited
;-- to lost profits) regardless of whether such liability is based on breach
;-- of contract, tort, strict liability, breach of warranties, failure of
;-- essential purpose or otherwise and even if advised of the possibility of
;-- such damages. Renesas shall not be liable for any services or products
;-- provided by third party vendors, developers or consultants identified or
;-- referred to the User by Renesas in connection with the Product(s) and/or
;-- the Application.
;--
;-------------------------------------------------------------------------------
;-- Environment:
;--              Device:         U2C8
;--              IDE:            CCRH V2.07.00 or later
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
;-- Revision Control History:
;-- 1.0.0 :   19th-Aug-2025    : Initial Version
;-------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;  asm macros
;-----------------------------------------------------------------------------
  PE0_USED     .set 1
  CUx_EBV      .set 0x00078000
;-------------------------------------------------------------------------------
;-------------  Public functions
;-------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;  system stack
;-----------------------------------------------------------------------------
MEVSIZE  .set  0x4
  .section ".mev_address.bss", bss
  .align 4
  .ds  (MEVSIZE)
  .align 4
_mevtop:



STACKSIZE  .set  0x1000
  .section ".stack.bss", bss
  .align 4
  .ds  (STACKSIZE)
  .align 4
_stacktop:

;-----------------------------------------------------------------------------
;  section initialize table
;-----------------------------------------------------------------------------
  .section ".INIT_DSEC.const", const
  .align 4
  .dw      #__s.data,  #__e.data,  #__s.data.R

  .section ".INIT_BSEC.const", const
  .align 4
  .dw  #__s.bss,  #__e.bss

;-----------------------------------------------------------------------------
;  startup
;-----------------------------------------------------------------------------
  .section "STARTUP_CODE_PE0", text
  .public  _RESET_PE0
  .align 4
_RESET_PE0:
  ;-- Initialization of the stack pointer
  mov  #_stacktop, sp    ;  set sp register

  ;-- Initialization of the global pointer
  mov  #__gp_data, gp    ;  set gp register

  ;-- Initialization of the ep register
  mov  #__ep_data, ep    ;  set ep register

  ;-- Initialization of the RAM areas
  mov  #__s.INIT_DSEC.const, r6
  mov  #__e.INIT_DSEC.const, r7
  mov  #__s.INIT_BSEC.const, r8
  mov  #__e.INIT_BSEC.const, r9
  jarl32  __INITSCT_RH, lp  ;  call in library function


;-------------  PE0 Initialization
  ;-- Initialization of the interrupt base pointer
  .extern _g_vector_table_PE0
  mov   #_g_vector_table_PE0, r10
  ldsr  r10, 4, 1 ; INTBP

  ;-- First set EBASE register address
  mov   #__sex_entry_PE0, r10
  ;-- Set DV bit for direct vector interupt and BV bit for reducing interrupt processing
  mov   0x0000003, r11 ; DV bit
  or    r11, r10
  ldsr  r10, 3, 1 ; EBASE

  ;-- then set 1 to PSW.EBV -> RBASE!=EBASE
  ;-- then set 1 to PSW.CU0 and CU1
  stsr  5, r10, 0 ; PSW
  mov   CUx_EBV, r11 ; CUx_EBV
  or    r11, r10
  ldsr  r10, 5, 0 ; PSW


  ;-- Jump to the initialisation functions of the library
  ;-- and from there to main()
  ; set various flags to PSW via FEPSW
  stsr  5, r10, 0    ; r10 <- PSW
  ldsr  r10, 3, 0    ; FEPSW <- r10
  mov  #_exit, lp    ; lp <- #_exit
  mov  #_main, r10
  ldsr  r10, 2, 0    ; FEPC <- #_main
  ; apply PSW and PC to start user mode
  feret

_exit:
  br  _exit      ;  end of program

;-------------------------------------------------------------------------------
;-------------  Interrupt service routine of unused interrupts
;-------------------------------------------------------------------------------

  .extern  __unused_isr
__unused_isr:
  br    __unused_isr

;-------------------------------------------------------------------------------
;-------------  Start Section for exception table
;-------------------------------------------------------------------------------
  .section "ex_entry_PE0", const
  .public __ex_entry_PE0
  .align 4
__ex_entry_PE0:
;----RESET
    .offset 0x0000
    ;.extern __unused_isr
    jr _RESET

;----SYSERR
    .offset 0x0010
    .extern _SYSERR_PE0
    jr __unused_isr

;----FETRAP
    .offset 0x0030
    .extern _FETRAP_PE0
    jr __unused_isr

;----TRAP0
    .offset 0x0040
    .extern _vTRAP0_Handler
    jr _vTRAP0_Handler

;----TRAP1
    .offset 0x0050
    .extern _EITRAP1_PE0
    jr __unused_isr

;----RIEX
    .offset 0x0060
    .extern _RIEX_PE0
    jr __unused_isr

;----UCPOP
    .offset 0x0080
    .extern _UCPOP_PE0
    jr __unused_isr

;----MIP_MDP
    .offset 0x0090
    .extern _MIP_MDP_PE0
    jr __unused_isr

;----PIE
    .offset 0x00a0
    .extern _PIE_PE0
    jr __unused_isr

;----MAE_MAEX
    .offset 0x00c0
    jr __unused_isr

;----FENMI
    .offset 0x00e0
    ;.extern _FENMI_PE0
    jr __unused_isr

;----FEINT
    .offset 0x00f0
    ;.extern _FEINT_PE0
    jr __unused_isr

;----EIINTn priority_0
    .offset 0x0100
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_1
    .offset 0x0110
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_2
    .offset 0x0120
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_3
    .offset 0x0130
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_4
    .offset 0x0140
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_5
    .offset 0x0150
    .extern _vIrq_Handler
    jr _vIrq_Handler

;----EIINTn priority_6
    .offset 0x0160
    .extern _EINTPRIO_6_PE0
    jr __unused_isr

;----EIINTn priority_7
    .offset 0x0170
    .extern _EINTPRIO_7_PE0
    jr __unused_isr

;----EIINTn priority_8
    .offset 0x0180
    .extern _EINTPRIO_8_PE0
    jr __unused_isr

;----EIINTn priority_9
    .offset 0x0190
    .extern _EINTPRIO_9_PE0
    jr __unused_isr

;----EIINTn priority_10
    .offset 0x01a0
    .extern _EINTPRIO_10_PE0
    jr __unused_isr

;----EIINTn priority_11
    .offset 0x01b0
    .extern _EINTPRIO_11_PE0
    jr __unused_isr

;----EIINTn priority_12
    .offset 0x01c0
    .extern _EINTPRIO_12_PE0
    jr __unused_isr

;----EIINTn priority_13
    .offset 0x01d0
    .extern _EINTPRIO_13_PE0
    jr __unused_isr

;----EIINTn priority_14
    .offset 0x01e0
    .extern _EINTPRIO_14_PE0
    jr __unused_isr

;----EIINTn priority_15
    .offset 0x01f0
    .extern _EINTPRIO_15_PE0
    jr __unused_isr

;----EIINTn priority_16
    .offset 0x0200
    .extern _EINTPRIO_16_PE0
    jr __unused_isr

;----EIINTn priority_17
    .offset 0x0210
    .extern _EINTPRIO_17_PE0
    jr __unused_isr

;----EIINTn priority_18
    .offset 0x0220
    .extern _EINTPRIO_18_PE0
    jr __unused_isr

;----EIINTn priority_19
    .offset 0x0230
    .extern _EINTPRIO_19_PE0
    jr __unused_isr

;----EIINTn priority_20
    .offset 0x0240
    .extern _EINTPRIO_20_PE0
    jr __unused_isr

;----EIINTn priority_21
    .offset 0x0250
    .extern _EINTPRIO_21_PE0
    jr __unused_isr

;----EIINTn priority_22
    .offset 0x0260
    .extern _EINTPRIO_22_PE0
    jr __unused_isr

;----EIINTn priority_23
    .offset 0x0270
    .extern _EINTPRIO_23_PE0
    jr __unused_isr

;----EIINTn priority_24
    .offset 0x0280
    .extern _EINTPRIO_24_PE0
    jr __unused_isr

;----EIINTn priority_25
    .offset 0x0290
    .extern _EINTPRIO_25_PE0
    jr __unused_isr

;----EIINTn priority_26
    .offset 0x02a0
    .extern _EINTPRIO_26_PE0
    jr __unused_isr

;----EIINTn priority_27
    .offset 0x02b0
    .extern _EINTPRIO_27_PE0
    jr __unused_isr

;----EIINTn priority_28
    .offset 0x02c0
    .extern _EINTPRIO_28_PE0
    jr __unused_isr

;----EIINTn priority_29
    .offset 0x02d0
    .extern _EINTPRIO_29_PE0
    jr __unused_isr

;----EIINTn priority_30
    .offset 0x02e0
    .extern _EINTPRIO_30_PE0
    jr __unused_isr

;----EIINTn priority_31
    .offset 0x02f0
    .extern _EINTPRIO_31_PE0
    jr __unused_isr

;----EIINTn priority_32
    .offset 0x0300
    .extern _EINTPRIO_32_PE0
    jr __unused_isr

;----EIINTn priority_33
    .offset 0x0310
    .extern _EINTPRIO_33_PE0
    jr __unused_isr

;----EIINTn priority_34
    .offset 0x0320
    .extern _EINTPRIO_34_PE0
    jr __unused_isr

;----EIINTn priority_35
    .offset 0x0330
    .extern _EINTPRIO_35_PE0
    jr __unused_isr

;----EIINTn priority_36
    .offset 0x0340
    .extern _EINTPRIO_36_PE0
    jr __unused_isr

;----EIINTn priority_37
    .offset 0x0350
    .extern _EINTPRIO_37_PE0
    jr __unused_isr

;----EIINTn priority_38
    .offset 0x0360
    .extern _EINTPRIO_38_PE0
    jr __unused_isr

;----EIINTn priority_39
    .offset 0x0370
    .extern _EINTPRIO_39_PE0
    jr __unused_isr

;----EIINTn priority_40
    .offset 0x0380
    .extern _EINTPRIO_40_PE0
    jr __unused_isr

;----EIINTn priority_41
    .offset 0x0390
    .extern _EINTPRIO_41_PE0
    jr __unused_isr

;----EIINTn priority_42
    .offset 0x03a0
    .extern _EINTPRIO_42_PE0
    jr __unused_isr

;----EIINTn priority_43
    .offset 0x03b0
    .extern _EINTPRIO_43_PE0
    jr __unused_isr

;----EIINTn priority_44
    .offset 0x03c0
    .extern _EINTPRIO_44_PE0
    jr __unused_isr

;----EIINTn priority_45
    .offset 0x03d0
    .extern _EINTPRIO_45_PE0
    jr __unused_isr

;----EIINTn priority_46
    .offset 0x03e0
    .extern _EINTPRIO_46_PE0
    jr __unused_isr

;----EIINTn priority_47
    .offset 0x03f0
    .extern _EINTPRIO_47_PE0
    jr __unused_isr

;----EIINTn priority_48
    .offset 0x0400
    .extern _EINTPRIO_48_PE0
    jr __unused_isr

;----EIINTn priority_49
    .offset 0x0410
    .extern _EINTPRIO_49_PE0
    jr __unused_isr

;----EIINTn priority_50
    .offset 0x0420
    .extern _EINTPRIO_50_PE0
    jr __unused_isr

;----EIINTn priority_51
    .offset 0x0430
    .extern _EINTPRIO_51_PE0
    jr __unused_isr

;----EIINTn priority_52
    .offset 0x0440
    .extern _EINTPRIO_52_PE0
    jr __unused_isr

;----EIINTn priority_53
    .offset 0x0450
    .extern _EINTPRIO_53_PE0
    jr __unused_isr

;----EIINTn priority_54
    .offset 0x0460
    .extern _EINTPRIO_54_PE0
    jr __unused_isr

;----EIINTn priority_55
    .offset 0x0470
    .extern _EINTPRIO_55_PE0
    jr __unused_isr

;----EIINTn priority_56
    .offset 0x0480
    .extern _EINTPRIO_56_PE0
    jr __unused_isr

;----EIINTn priority_57
    .offset 0x0490
    .extern _EINTPRIO_57_PE0
    jr __unused_isr

;----EIINTn priority_58
    .offset 0x04a0
    .extern _EINTPRIO_58_PE0
    jr __unused_isr

;----EIINTn priority_59
    .offset 0x04b0
    .extern _EINTPRIO_59_PE0
    jr __unused_isr

;----EIINTn priority_60
    .offset 0x04c0
    .extern _EINTPRIO_60_PE0
    jr __unused_isr

;----EIINTn priority_61
    .offset 0x04d0
    .extern _EINTPRIO_61_PE0
    jr __unused_isr

;----EIINTn priority_62
    .offset 0x04e0
    .extern _EINTPRIO_62_PE0
    jr __unused_isr

;----EIINTn priority_63
    .offset 0x04f0
    .extern _EINTPRIO_63_PE0
    jr __unused_isr

;-----------------------------------------------------------------------------
;   for table reference method
;-----------------------------------------------------------------------------
    .section "g_vector_table_PE0", sdata
    .align 4


;-------------  End of exception table
;-------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;    dummy section
;-----------------------------------------------------------------------------

    .section ".data", data
.L.dummy.data:
    .section ".bss", bss
.L.dummy.bss:
    .section ".const", const
.L.dummy.const:
    .section ".text", text
.L.dummy.text:
