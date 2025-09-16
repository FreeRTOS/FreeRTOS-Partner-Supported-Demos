/*====================================================================================================================*/
/* Module       = App_Common_Sample.ld                                                                                */
/* SW-VERSION   = 2.1.1                                                                                               */
/*====================================================================================================================*/
/*                                                     COPYRIGHT                                                      */
/*====================================================================================================================*/
/* (c) 2021-2024 Renesas Electronics Corporation. All rights reserved.                                                */
/*====================================================================================================================*/
/* Purpose:     Template of common linker directive file                                                              */
/*                                                                                                                    */
/*====================================================================================================================*/
/* Unless otherwise agreed upon in writing between your company and Renesas Electronics Corporation the following     */
/* shall apply!                                                                                                       */
/*                                                                                                                    */
/* Warranty Disclaimer                                                                                                */
/*                                                                                                                    */
/* There is no warranty of any kind whatsoever granted by Renesas. Any warranty is expressly disclaimed and excluded  */
/* by Renesas, either expressed or implied, including but not limited to those for non-infringement of intellectual   */
/* property, merchantability and/or fitness for the particular purpose.                                               */
/*                                                                                                                    */
/* Renesas shall not have any obligation to maintain, service or provide bug fixes for the supplied Product(s) and/or */
/* the Application.                                                                                                   */
/*                                                                                                                    */
/* Each User is solely responsible for determining the appropriateness of using the Product(s) and assumes all risks  */
/* associated with its exercise of rights under this Agreement, including, but not limited to the risks and costs of  */
/* program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and             */
/* unavailability or interruption of operations.                                                                      */
/*                                                                                                                    */
/* Limitation of Liability                                                                                            */
/*                                                                                                                    */
/* In no event shall Renesas be liable to the User for any incidental, consequential, indirect, or punitive damage    */
/* (including but not limited to lost profits) regardless of whether such liability is based on breach of contract,   */
/* tort, strict liability, breach of warranties, failure of essential purpose or otherwise and even if advised of the */
/* possibility of such damages. Renesas shall not be liable for any services or products provided by third party      */
/* vendors, developers or consultants identified or referred to the User by Renesas in connection with the Product(s) */
/* and/or the Application.                                                                                            */
/*                                                                                                                    */
/*====================================================================================================================*/
/* Environment:                                                                                                       */
/*              Device:         U2C                                                                                   */
/*              IDE:            GHS Multi V7.1.4 or later                                                             */
/*====================================================================================================================*/
/* Revision Control History:                                                                                          */


SECTIONS
{
  /* Start of internal ROM area (iROM_0) */

  .intvect_PE0 0x0000          align(16)        :> iROM_0   /* start of interrupt vector. This section contains RESET and exception table for PE0 */
  .intvect_PE1 0x2000          align(16)        :>.   /* start of interrupt vector. This section contains exception table for PE1 */

  .rozdata                                      :>.   /* constant datas in ZDA area */
  .robase                      align(4)         :>.   /* initialize textpointer TP for SDA addressing */
  .rosdata                     align(4)         :>.   /*constant datas in SDA area */
  .rodata                      align(4)         :>.

  /* program code area */
  .text     align(4)                            :>.

  .fixaddr              align(4)                :>.    /* ghs internal (compiler) */
  .fixtype              align(4)                :>.    /* ghs internal (compiler) */
  .secinfo              align(4)                :>.    /* ghs internal (runtime library) */
  .syscall              align(4)                :>.    /* ghs internal (linker) */

  .romdata              ROM(.data)              :>.    /* constant data to initialize variables (copied to RAM at startup)*/
  .romzdata             ROM(.zdata)             :>.    /* constant data to initialize variables in ZDA area (copied to RAM at startup)*/
  .romsdata             ROM(.sdata)             :>.    /* constant data to initialize variables in SDA area (copied to RAM at startup)*/
  .romtdata             ROM(.tdata)             :>.    /* constant data to initialize variables in TDA area (copied to RAM at startup)*/
  .romsldata            ROM(.sldata)            :>.
  
  /* Start of PE0 internal RAM area (iRAM_0) */
  .stack    align(4)    pad(0x8000)             :> iRAM_0 /* initialized data */

  /* Start of PE1 internal RAM area (iRAM_1) */
  /* Initialized data in SDA area */
  .stack1_PE1   align(4) pad(0x10000)     :>iRAM_1   /* this section is used for stack pointer of CPU1 */

  /* Start of internal retention RAM area (iRAM_R) */
  .rcode    align(4) pad(0x2000)  :>iRAM_R
  .rdata    align(4)              :>.
  .rbss     align(4)              :>.
  .rsbss    align(4)              :>.
  .rstack   align(4) pad(0x0800)  :>.
  .top_of_RAM_R align(4)          :> iRAM_R_TOP

  /* Start of cluster RAM area (cRAM_0) */
  .SpinlockStatus                    pad(0x00FF)      :> cRAM_0   /* initialized data */ 
  .data                              align(4)         :>.   /* initialized data */
  .sdabase                           align(4)         :>.      /* initialize global pointer GP for SDA addressing */
  .zdata                             align(4)         :>.      /* initialized data in ZDA area */
  .sldata                            align(4)         :>.      /* initialized data in LSDA area */
  .sdata                             align(4)         :>.      /* initialized data in SDA area */

  .bss                               align(4) :>.      /* zero initialized data */
  .zbss                              align(4) :>.      /* zero initialized data in ZDA area */
  .slbss                             align(4) :>.      /* zero initialized data in LSDA area */
  .sbss                              align(4) :>.      /* zero initialized data in SDA area */
  .ramtext  align(128)  MAX_SIZE(0x2000)        :>.      /* initialized and zero-initialized data in TDA area */
  .tdata    align(4)    MAX_SIZE(0x0100)        :>.      /* initialized and zero-initialized data in TDA area */
  .exec     align(128)                          :>.

  .mev_address                 align(32)        :>.      /* Mutual Exclusive Variable is used for Sync flags between multi core */
  .heapbase align(4)                            :>.
  .heap     align(4)
            pad(addr(cRAM_0_TOP)-addr(.heapbase))
            NOCLEAR                             :>.      /* definition of heap size */        
  .top_of_RAM                     align(4)      :> cRAM_0_TOP

  /* Start of Cluster RAM area (cRAM_1) */
  .ETH_PORT_RAM_1                 align(32)    :> cRAM_1


  /*  Symbols for compiler references */
  ___ghs_romstart       = MEMADDR(iROM_0);
  ___ghs_romend         = MEMENDADDR(iROM_0);

  ___ghs_ramsstart       = MEMADDR(iRAM);
  ___ghs_ramsend         = MEMENDADDR(iRAM);

  ___ghs_ramstart       = MEMADDR(iRAM_0);
  ___ghs_ramend         = MEMENDADDR(iRAM_0);

  ___ghs_rramstart      = MEMADDR(iRAM_R);
  ___ghs_rramend        = MEMENDADDR(iRAM_R);

  ___ghs_ram1start      = MEMADDR(iRAM_1);
  ___ghs_ram1end        = MEMENDADDR(iRAM_1);

  ___ghs_cram0start     = MEMADDR(cRAM_0);
  ___ghs_cram0end       = MEMENDADDR(cRAM_0);

  ___ghs_cram1start     = MEMADDR(cRAM_1);
  ___ghs_cram1end       = MEMENDADDR(cRAM_1);
}

/*===========================================================================*/
/*      End of File                                                          */
/*===========================================================================*/
