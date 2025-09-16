/*===========================================================================*/
/* Module       = App_U2C_Sample.ld                                          */
/* SW-VERSION   = 2.1.1                                                      */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* (c) 2021,2024 Renesas Electronics Corporation. All rights reserved.       */
/*===========================================================================*/
/* Purpose:     Template of linker directive file                            */
/*                                                                           */
/*===========================================================================*/
/*                                                                           */
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:         BGA468                                       */
/*              IDE:            GHS Multi V6.x.x or later                    */
/*===========================================================================*/
/* Revision Control History:                                                 */
/*===========================================================================*/

CONSTANTS
{
  zero_start = 0xffff8000
}

MEMORY
 {
  iROM_0     : ORIGIN = 0x00000000,  LENGTH = 256k  /* Code flash User area */
  dBaseROM   : ORIGIN = 0x00040100,  LENGTH = 64k  /* MCAL Driver Database area */
  iROM_1     : ORIGIN = 0x00300000,  LENGTH = 64k  /* Code flash of PE1 */
  iROM_1_TOP : ORIGIN = .         ,  LENGTH = 0

  iRAM_1     : ORIGIN = 0xFDA00000,  LENGTH = 64k  /* Local RAM of PE1 */
  iRAM_1_TOP : ORIGIN = .         ,  LENGTH = 0
  iRAM_0     : ORIGIN = 0xFDC00000,  LENGTH = 64k  /* Local RAM of PE0 */
  iRAM_0_TOP : ORIGIN = .         ,  LENGTH = 0
  iRAM       : ORIGIN = 0xFDE00000,  LENGTH = 64k  /* Local RAM of Self */
  iRAM_TOP   : ORIGIN = .         ,  LENGTH = 0
             
  cRAM_0     : ORIGIN = 0xFE000000,  LENGTH = 512k /* Cluster RAM 0 (512k) */
  cRAM_0_TOP : ORIGIN = .         ,  LENGTH = 0
  cRAM_1     : ORIGIN = 0xFE080000,  LENGTH = 384k /* Cluster RAM 1 (384k) */
  cRAM_1_TOP : ORIGIN = .         ,  LENGTH = 0
  iRAM_R     : ORIGIN = 0xFE0E0000,  LENGTH = 128k
  iRAM_R_TOP : ORIGIN = .         ,  LENGTH = 0
 }

/*===========================================================================*/
/*      End of File                                                          */
/*===========================================================================*/