;-------------------------------------------------------------------------------
;-- Module      = startup_PEn.asm
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

;-------------------------------------------------------------------------------
;-------------  Selection of external interrupt service handler
;-------------  User modifiable section
;-------------  Please uncomment the required interrupt service handler
;-------------------------------------------------------------------------------


;-------------------------------------------------------------------------------
;-------------  ASM macros
;-------------------------------------------------------------------------------
  CUx_EBV .set 0x00078000
  PE1_USED     .set 1

;-------------------------------------------------------------------------------
;------------- Public function delaration
;-------------------------------------------------------------------------------
  .extern __unused_isr
  .extern _SYNC_PE0
  .extern _exit

  .section "STARTUP_CODE_PE0", text
  .align 4
;-----------------------------------------------------------------------------
;  system stack
;-----------------------------------------------------------------------------
$ifdef PE1_USED
STACKSIZE  .set  0x1000
  .section ".stack_pe1.bss", bss
  .align 4
  .ds  (STACKSIZE)
  .align 4
_PE1_stacktop:
$endif

$ifdef PE2_USED
STACKSIZE  .set  0x1000
  .section ".stack_pe2.bss", bss
  .align 4
  .ds  (STACKSIZE)
  .align 4
_PE2_stacktop:
$endif

;_stacktop:
;-----------------------------------------------------------------------------
;  section initialize table
;-----------------------------------------------------------------------------
  .section ".INIT_DSEC.const", const
  .align 4
  .dw  #__s.data,  #__e.data,  #__s.data.R

  .section ".INIT_BSEC.const", const
  .align 4
  .dw  #__s.bss,  #__e.bss

;-----------------------------------------------------------------------------
;  startup
;-----------------------------------------------------------------------------
  .section "STARTUP_CODE_PEn", text
  .align 4
  .public  _RESET_PEn

_RESET_PEn:
  ;-- Initialization of the stack pointer
  ;mov  #_stacktop, sp    ;  set sp register

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

;-------------  PEn Initialization
  stsr    0, r10, 2               ; get PEID.PEID
$ifdef PE1_USED
  cmp     1, r10
  bz      __PE1
$endif

$ifdef PE2_USED
  cmp     2, r10
  bz      __PE2
$endif

$ifdef PE2_USED
__PE2:
    jarl __initsp_PE2, lp
    jr32 __DONE
$endif

$ifdef PE1_USED
__PE1:
    jarl __initsp_PE1, lp
    jr32 __DONE
$endif

$ifdef PE2_USED
__initsp_PE2:
    mov  #_PE2_stacktop, sp    ;  set sp register
    mov  #__sex_entry_PE2, r10  ;-- First set EBASE register address
    jr32 __DONE
$endif
    
$ifdef PE1_USED
__initsp_PE1:
    mov  #_PE1_stacktop, sp    ;  set sp register
    mov  #__sex_entry_PE1, r10  ;-- First set EBASE register address
$endif


__DONE:
  ;-- Set RINT bit for direct vector interupt and
  ;-- And Set RINT bit for reducing interrupt processing
  mov   0x0000003, r11        ; DV bit
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
  
;-----------------------------------------------
; jump to entry point of each PE main function
;-----------------------------------------------

stsr    0, r10, 2               ; get PEID
$ifdef PE1_USED
    cmp     1, r10
    bz      _INIT_PE1           ; if PEID is 1
$endif

$ifdef PE2_USED
    cmp     2, r10
    bz      _INIT_PE2           ; if PEID is 2
$endif

_exit:
  br  _exit      ;  end of program

_loop:
  br    _loop

$ifdef PE1_USED
_INIT_PE1:
  mov  #_main_PE1, r10
  ldsr  r10, 2, 0    ; FEPC <- #_main
  ; apply PSW and PC to start user mode
  feret
  br  _exit
$endif

$ifdef PE2_USED
_INIT_PE2:
  mov  #_main_PE2, r10
  ldsr  r10, 2, 0    ; FEPC <- #_main
  ; apply PSW and PC to start user mode
  feret
  br  _exit
$endif

  ldsr  r10, 2, 0    ; FEPC <- #_main
  ; apply PSW and PC to start user mode
  feret


;-------------------------------------------------------------------------------
;-------------  Start Section for exception table
;PE1
$ifdef PE1_USED
.section "ex_entry_PE1", const
.public __ex_entry_PE1
.align 4

;-------------------------------------------------------------------------------
__ex_entry_PE1:
;----RESET
    .offset 0x0000
    jr _RESET

;----SYSERR
    .offset 0x0010
    .extern _SYSERR_PE1
    jr __unused_isr

;----FETRAP
    .offset 0x0030
    .extern _FETRAP_PE1
    jr __unused_isr

;----TRAP0
    .offset 0x0040
    .extern _vTRAP0_Handler
    jr _vTRAP0_Handler

;----TRAP1
    .offset 0x0050
    .extern _EITRAP1_PE1
    jr __unused_isr

;----RIEX
    .offset 0x0060
    .extern _RIEX_PE1
    jr __unused_isr

;----UCPOP
    .offset 0x0080
    .extern _UCPOP_PE1
    jr __unused_isr

;----MIP_MDP
    .offset 0x0090
    .extern _MIP_MDP_PE1
    jr __unused_isr

;----PIE
    .offset 0x00a0
    .extern _PIE_PE1
    jr __unused_isr

;----MAE_MAEX
    .offset 0x00c0
    ;.extern _MAE_MAEX_PE1
    jr __unused_isr

;----FENMI
    .offset 0x00e0
    ;.extern _FENMI_PE1
    jr __unused_isr

;----FEINT
    .offset 0x00f0
    ;.extern _FEINT_PE1
    jr __unused_isr

;----EIINTn priority_0
    .offset 0x0100
    .extern _vIrq_Handler
    jr _vIrq_Handler

$endif ;end PE1


; PE2
$ifdef PE2_USED
.section "ex_entry_PE2", const
.public __ex_entry_PE2
.align 4

;-------------------------------------------------------------------------------
__ex_entry_PE2:
;----RESET
    .offset 0x0000
    jr _RESET

;----SYSERR
    .offset 0x0010
    .extern _SYSERR_PE2
    jr __unused_isr

;----FETRAP
    .offset 0x0030
    .extern _FETRAP_PE2
    jr __unused_isr

;----TRAP0
    .offset 0x0040
    .extern _vTRAP0_Handler
    jr _vTRAP0_Handler

;----TRAP1
    .offset 0x0050
    .extern _EITRAP1_PE2
    jr __unused_isr

;----RIEX
    .offset 0x0060
    .extern _RIEX_PE2
    jr __unused_isr

;----UCPOP
    .offset 0x0080
    .extern _UCPOP_PE2
    jr __unused_isr

;----MIP_MDP
    .offset 0x0090
    .extern _MIP_MDP_PE2
    jr __unused_isr

;----PIE
    .offset 0x00a0
    .extern _PIE_PE2
    jr __unused_isr

;----MAE_MAEX
    .offset 0x00c0
    .extern _MAE_MAEX_PE2
    jr __unused_isr

;----FENMI
    .offset 0x00e0
    ;.extern _FENMI_PE2
    jr __unused_isr

;----FEINT
    .offset 0x00f0
    ;.extern _FEINT_PE2
    jr __unused_isr

;----EIINTn priority_0
    .offset 0x0100
    .extern _vIrq_Handler
    jr _vIrq_Handler

$endif ;end PE2


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
