;-------------------------------------------------------------------------------
;-- File name: startup_PE0.asm
;-------------------------------------------------------------------------------
;-- Environment:
;--              Device:         U2A8
;--              IDE:            CCRH V2.06.00 or later
;-------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;  asm macros
;-----------------------------------------------------------------------------
  PE0_USED     .set 1
  CUx_EBV      .set 0x00078000
;-------------------------------------------------------------------------------
;-------------  Public functions
;-------------------------------------------------------------------------------
  .public _stacktop

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
  mov   0x0000003, r11 ; DV, RINT bit
  or    r11, r10
  ldsr  r10, 3, 1 ; EBASE
  synci

  ;-- Then set 1 to PSW.EBV -> RBASE!=EBASE
  ;-- Then set 1 to PSW.CU0 and CU1
  stsr  5, r10, 0 ; PSW
  mov   CUx_EBV, r11 ; CUx_EBV
  or    r11, r10
  ldsr  r10, 5, 0 ; PSW
  synci

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
;----EBASE.RINT=1, interrupts are handled at this entry
    .offset 0x0100
    .extern _vIrq_Handler
    jr _vIrq_Handler


;-----------------------------------------------------------------------------
;   for table reference method
;-----------------------------------------------------------------------------

;-------------  End of exception table
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
;   dummy section
;-----------------------------------------------------------------------------

    .section ".data", data
.L.dummy.data:
    .section ".bss", bss
.L.dummy.bss:
    .section ".const", const
.L.dummy.const:
    .section ".text", text
.L.dummy.text:
