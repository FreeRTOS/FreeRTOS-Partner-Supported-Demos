;-------------------------------------------------------------------------------
;-- File name: coldreset.asm
;-------------------------------------------------------------------------------
;-- Environment:
;--              IDE:            CCRH V2.06.00 or later
;-------------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Macro
;-----------------------------------------------------------------------------
  RESET_ENABLE .set 1
  PE0_USED     .set 1
  ;PE1_USED     .set 1
  ;PE2_USED     .set 0
  ;PE3_USED     .set 0

  ; local ram address
  LOCAL_RAM_CPU0_ADDR         .set 0xFDC00000     ; LOCAL RAM(CPU0) start address
  LOCAL_RAM_CPU0_END          .set 0xFDC0FFFF     ; LOCAL RAM(CPU0) end address

  LOCAL_RAM_CPU1_ADDR         .set 0xFDA00000     ; LOCAL RAM(CPU1) start address
  LOCAL_RAM_CPU1_END          .set 0xFDA0FFFF     ; LOCAL RAM(CPU1) end address

  LOCAL_RAM_CPU2_ADDR         .set 0xFD800000     ; LOCAL RAM(CPU2) start address
  LOCAL_RAM_CPU2_END          .set 0xFD80FFFF     ; LOCAL RAM(CPU2) end address

  ; cluster0 ram address
  CLUSTER_RAM0_ADDR0           .set 0xFE000000     ; CLUSTER0 RAM start address
  CLUSTER_RAM0_END0            .set 0xFE057FFF     ; CLUSTER0 RAM end address

  CLUSTER_RRAM0_ADDR0           .set 0xFE058000     ; CLUSTER0 RRAM start address
  CLUSTER_RRAM0_END0            .set 0xFE057FFF     ; CLUSTER0 RRAM end address

  CLUSTER_RAM0_ADDR1           .set 0xFE060000     ; CLUSTER0 RAM start address
  CLUSTER_RAM0_END1            .set 0xFE07FFFF     ; CLUSTER0 RAM end address

  ; cluster1 ram address
  CLUSTER_RAM1_ADDR0           .set 0xFE080000     ; CLUSTER0 RAM start address
  CLUSTER_RAM1_END0            .set 0xFE0DFFFF     ; CLUSTER0 RAM end address

  CLUSTER_RRAM1_ADDR0           .set 0xFE0E0000     ; CLUSTER0 RRAM start address
  CLUSTER_RRAM1_END0            .set 0xFE0FFFFF     ; CLUSTER0 RRAM end address

  ; local ram self address
  SELF_RAM_ADDR               .set 0xFDE00000     ; SELF RAM start address
  SELF_RAM_END                .set 0xFDE0FFFF     ; SELF RAM end address

  ; target dependence informations (specify values suitable to your system)
  MEV_ADDR                    .set CLUSTER_RAM0_ADDR0  ; cluster RAM data Address

  EIPC  .set 0
  EIPSW .set 1
  EIIC  .set 13

  CUx_EBV      .set 0x00078000

;-------------------------------------------------------------------------------
;-------------  Public functions
;-------------------------------------------------------------------------------
  .extern _RESET_PE0
  .extern _RESET_PEn
  .extern Irq_Handler

;-------------------------------------------------------------------------------
;-------------  Common Startup Routine
;-------------------------------------------------------------------------------
  .section "STARTUP_CODE", text
  .public    _RESET
  .align 4

_RESET:
  ; PE0 set Boot Control Register for all PE
  stsr  0,    r10, 2
  andi  0x7,  r10, r10
  cmp   0,  r10 ; PE0
  bne   aa
  synci
  bb: jr   _BOOTCTRL_SETTING
  aa: jr   _PE_RESET

;-----------------------------------------------------------------------------
;   PEn activation
;-----------------------------------------------------------------------------
_BOOTCTRL_SETTING:
     ld.w    0xfffb2000[r0], r10     ; get BOOTCTR
     mov   0x00, r3 ; enabled PE0 by default, disable all other PEn
     st.w    r11, 0xfffb2000[r0]     ; set BOOTCTRL
$ifdef PE1_USED
    ld.w    0xfffb2000[r0], r10     ; get BOOTCTRL
    ori     3, r10, r11             ; set 1 in BOOTCTRL.BC1 for enabled PE1
    st.w    r11, 0xfffb2000[r0]     ; set BOOTCTRL
$endif

$ifdef PE2_USED
    ld.w    0xfffb2000[r0], r10     ; get BOOTCTRL
    ori     7, r10, r11             ; set 1 in BOOTCTRL.BC2 for enabled PE2
    st.w    r11, 0xfffb2000[r0]     ; set BOOTCTRL
$endif

_PE_RESET:

    ;--------------------------------
    ; Register Initialization
    ;--------------------------------

    ;- General purpose register
    $nowarning
    mov     r0, r1          ; r1  Assembler reserved register
    $warning
    mov     r0, r2          ; r2  Register for address and data variables
    mov     r0, r3          ; r3  Stack pointer(SP)
    mov     r0, r4          ; r4  Global pointer(GP)
    mov     r0, r5          ; r5  Text pointer(TP)
    mov     r0, r6          ; r6  Register for addresses and data variables
    mov     r0, r7          ; r7  Register for addresses and data variables
    mov     r0, r8          ; r8  Register for addresses and data variables
    mov     r0, r9          ; r9  Register for addresses and data variables
    mov     r0, r10         ; r10 Register for addresses and data variables
    mov     r0, r11         ; r11 Register for addresses and data variables
    mov     r0, r12         ; r12 Register for addresses and data variables
    mov     r0, r13         ; r13 Register for addresses and data variables
    mov     r0, r14         ; r14 Register for addresses and data variables
    mov     r0, r15         ; r15 Register for addresses and data variables
    mov     r0, r16         ; r16 Register for addresses and data variables
    mov     r0, r17         ; r17 Register for addresses and data variables
    mov     r0, r18         ; r18 Register for addresses and data variables
    mov     r0, r19         ; r19 Register for addresses and data variables
    mov     r0, r20         ; r20 Register for addresses and data variables
    mov     r0, r21         ; r21 Register for addresses and data variables
    mov     r0, r22         ; r22 Register for addresses and data variables
    mov     r0, r23         ; r23 Register for addresses and data variables
    mov     r0, r24         ; r24 Register for addresses and data variables
    mov     r0, r25         ; r25 Register for addresses and data variables
    mov     r0, r26         ; r26 Register for addresses and data variables
    mov     r0, r27         ; r27 Register for addresses and data variables
    mov     r0, r28         ; r28 Register for addresses and data variables
    mov     r0, r29         ; r29 Register for addresses and data variables
    mov     r0, r30         ; r30 Element pointer(EP)
    mov     r0, r31         ; r31 Link pointer(LP)

    ;- Basic system register
    ldsr    r0,  0, 0       ; SR0,0  EIPC
    ldsr    r0,  2, 0       ; SR2,0  FEPC
    ldsr    r0, 16, 0       ; SR16,0 CTPC
    ldsr    r0, 28, 0       ; SR28,0 EIWR
    ldsr    r0, 29, 0       ; SR29,0 FEWR
    ldsr    r0,  3, 1       ; SR3,1  EBASE
    ldsr    r0,  4, 1       ; SR4,1  INTBP
    ldsr    r0,  6, 2       ; SR6,2  MEA
    ldsr    r0,  8, 2       ; SR8,2  MEI
    ldsr    r0, 18, 2       ; SR18,2 RBIP

    ;- FPU system register
    mov     0x00010020, r10 ; set 1 in PSW.CU0 for writing FPU Function Register
                            ; set 1 in PSW.ID for prohibiting EI-level interrupt
    ldsr    r10, 5, 0       ; SR5,0  PSW

    mov     0x00220000, r10 ; set 1 in FPSR.FS
    ldsr    r10, 6, 0       ; SR6,0  FPSR
    ldsr    r0,  7, 0       ; SR7,0  FPEPC
    ldsr    r0,  8, 0       ; SR8,0  FPST
    ldsr    r0,  9, 0       ; SR9,0  FPCC

    ;- MPU function register
    ldsr    r0,  8, 5       ; SR8,5  MCA
    ldsr    r0,  9, 5       ; SR9,5  MCS
    ldsr    r0, 11, 5       ; SR11,5 MCR

    mov     r0,r2
    _loop:          ; Initialize MPU 32 Areas
    ldsr    r2, 16, 5       ; SR16,5 MPIDX
    ldsr    r0, 20, 5       ; SR20,5 MPLA
    ldsr    r0, 21, 5       ; SR21,5 MPUA
    ldsr    r0, 22, 5       ; SR22,5 MPAT
    add     0x1, r2         ;
    cmp     0x20, r2        ;
    bne     _loop

    ldsr    r0, 24, 5       ; SR24,5 MPID0
    ldsr    r0, 25, 5       ; SR25,5 MPID1
    ldsr    r0, 26, 5       ; SR26,5 MPID2
    ldsr    r0, 27, 5       ; SR27,5 MPID3
    ldsr    r0, 28, 5       ; SR28,5 MPID4
    ldsr    r0, 29, 5       ; SR29,5 MPID5
    ldsr    r0, 30, 5       ; SR30,5 MPID6
    ldsr    r0, 31, 5       ; SR31,5 MPID7
    ldsr    r0, 12, 5       ; SR12,5 MCI

    ;- Cache Operation Function Registers
    ldsr    r0, 16, 4       ; SR16,4 ICTAGL
    ldsr    r0, 17, 4       ; SR17,4 ICTAGH
    ldsr    r0, 18, 4       ; SR18,4 ICDATL
    ldsr    r0, 19, 4       ; SR19,4 ICDATH
    ldsr    r0, 28, 4       ; SR28,4 ICERR

    ;- Virtualization support function system register
    ldsr    r0, 20, 1       ; SR20,1 HVSB

    ;- Guest Context Register
    ldsr    r0,  0, 9       ; SR0,9  GMEIPC
    ldsr    r0,  2, 9       ; SR2,9  GMFEPC
    ldsr    r0, 19, 9       ; SR19,9 GMEBASE
    ldsr    r0, 20, 9       ; SR20,9 GMINTBP
    ldsr    r0, 28, 9       ; SR28,9 GMEIWR
    ldsr    r0, 29, 9       ; SR29,9 GMFEWR
    ldsr    r0,  6, 9       ; SR6,9  GMMEA
    ldsr    r0, 8, 9        ; SR8,9 GMMEI

    ;--------------------------------
    ; jump to entry point of each PE
    ;--------------------------------

    stsr    0, r10, 2               ; get PEID.PEID
    cmp     0, r10
    bz      __start_PE0

$ifdef PE1_USED
    cmp     1, r10
    bz      __start_PE1             ; if PEID.PEID is 1
$endif  ; PE1_USED

$ifdef PE2_USED
    cmp     2, r10
    bz      __start_PE2             ; if PEID.PEID is 2
$endif  ; PE2_USED

$ifdef PE2_USED
__start_PE2:
    jarl    _hdwinit_PE2, lp        ; initialize hardware
    jr32    _RESET_PEn              ; jump to RESET PE2
$endif  ; PE2_USED

$ifdef PE1_USED
__start_PE1:
    jarl    _hdwinit_PE1, lp        ; initialize hardware
    jr32    _RESET_PEn              ; jump to RESET PE1
$endif  ; PE1_USED

__start_PE0:
    jarl    _hdwinit_PE0, lp        ; initialize hardware
    jr32    _RESET_PE0          ; jump to RESET PE0 to start PE0 app
__exit:
    br      __exit

;-----------------------------------------------------------------------------
;   _hdwinit_PE0
;-----------------------------------------------------------------------------
  .section "STARTUP_CODE", text
  .align 4

_hdwinit_PE0:
    mov     lp, r14                 ; save return address

    ; clear Cluster RAM0
    mov     CLUSTER_RAM0_ADDR0, r6
    mov     CLUSTER_RAM0_END0, r7
    jarl    _zeroclr4, lp

    ; clear Cluster RAM0
    mov     CLUSTER_RAM0_ADDR1, r6
    mov     CLUSTER_RAM0_END1, r7
    jarl    _zeroclr4, lp

    ; clear Cluster RRAM0
    mov     CLUSTER_RRAM0_ADDR0, r6
    mov     CLUSTER_RRAM0_END0, r7
    jarl    _zeroclr4, lp

    ; clear Cluster RAM1
    mov     CLUSTER_RAM1_ADDR0, r6
    mov     CLUSTER_RAM1_END0, r7
    jarl    _zeroclr4, lp

    ; clear Cluster RRAM1
    mov     CLUSTER_RRAM1_ADDR0, r6
    mov     CLUSTER_RRAM1_END0, r7
    jarl    _zeroclr4, lp

    ; clear Local RAM(CPU0)
    mov     LOCAL_RAM_CPU0_ADDR, r6
    mov     LOCAL_RAM_CPU0_END, r7
    jarl    _zeroclr4, lp

  ; .mev_address section in cRAM_0 is used for Sync flags
    mov     #__s.mev_address.bss, r20
    st.w    r0, 0[r20]

    mov     r14, lp
    jmp     [lp]

$ifdef PE1_USED
;-----------------------------------------------------------------------------
;   _hdwinit_PE1
;-----------------------------------------------------------------------------
  .section "STARTUP_CODE", text
  .align 4

_hdwinit_PE1:
    mov     lp, r14                 ; save return address

    ; clear Local RAM(CPU1)
    mov     LOCAL_RAM_CPU1_ADDR, r6
    mov     LOCAL_RAM_CPU1_END, r7
    jarl    _zeroclr4, lp

    ; wait for PE0
    mov     r14, lp
    jmp     [lp]

$endif ;PE1_USED

$ifdef PE2_USED
;-----------------------------------------------------------------------------
;   _hdwinit_PE2
;-----------------------------------------------------------------------------
  .section "STARTUP_CODE", text
  .align 4

_hdwinit_PE2:
    mov     lp, r14                 ; save return address

    ; clear Local RAM(CPU2)
    mov     LOCAL_RAM_CPU2_ADDR, r6
    mov     LOCAL_RAM_CPU2_END, r7
    jarl    _zeroclr4, lp

    ; wait for PE0
    mov     r14, lp
    jmp     [lp]

$endif ;PE2_USED

;-----------------------------------------------------------------------------
;   zeroclr4
;-----------------------------------------------------------------------------
    .section "STARTUP_CODE", text
    .align 4
_zeroclr4:
    br      .L.zeroclr4.2
.L.zeroclr4.1:
    st.w    r0, [r6]
    add     4, r6
.L.zeroclr4.2:
    cmp     r6, r7
    bh      .L.zeroclr4.1
    jmp [lp]

;-------------------------------------------------------------------------------
;-------------  Add section ".RESET_PE0" to your linker command file
;-------------  at address 0x0000
;-------------------------------------------------------------------------------
  .section "RESET", text
  .align 4

;-------------------------------------------------------------------------------
;-------------  Initializing of interrupt vector table
;-------------  Please do not modify this section
;-------------------------------------------------------------------------------
  .public _RESET_VECTOR
  .offset 0x0000
_RESET_VECTOR:
$ifdef RESET_ENABLE
  .extern _RESET
   jr _RESET
$else
  jr __unused_isr
$endif ;RESET_ENABLE
