/*===========================================================================*/
/* File name: App_U2B_Sample.ld                                              */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:         U2B                                          */
/*              IDE:            GHS Multi V7.1.4 or later                    */
/*===========================================================================*/

CONSTANTS
{
  zero_start = 0xffff8000
}

MEMORY
 {
  iROM_0     : ORIGIN = 0x00000000,  LENGTH = 320k  /* Code flash User area */
  iROM_1     : ORIGIN = 0x00340100,  LENGTH = 64k  /* Code flash of PE1 */
  iROM_1_TOP : ORIGIN = .         ,  LENGTH = 0
  iROM_2     : ORIGIN = 0x00600000,  LENGTH = 32k  /* Code flash of PE2 */
  iROM_2_TOP : ORIGIN = .         ,  LENGTH = 0

  iRAM_2     : ORIGIN = 0xFD800000,  LENGTH = 64k  /* Local RAM of PE2 */
  iRAM_2_TOP : ORIGIN = .         ,  LENGTH = 0
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
