/*===========================================================================*/
/* Module      = io_macros_v2.h                                              */
/* Version     = V1.04                                                       */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2014 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose:      Definition of Macros to define I/O Register                 */
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
/* History:                                                                  */
/*              V1.01, define new macro _GHS_PRAGMA_IO_TYPEDEF_              */
/*                     define new macro __IOREGARRAY                         */
/*              V1.02, added mode for usage with strict ANSI                 */
/*              V1.03, added typedef for 64-bit integer                      */
/*              V1.04, added define for GHS far function calls               */
/* Environment:                                                              */
/*              Device:         all RH850 core devices                       */
/*              IDE:            GHS Multi for V800  V6.xx or later           */
/*===========================================================================*/

#ifndef __IO_MACROS_H
  #define __IO_MACROS_H

  #if !defined(__STRICT_ANSI__ ) & defined(__ghs__)
    #ifdef __LANGUAGE_ASM__
    #else
     typedef unsigned char      u08_T;
     typedef unsigned short     u16_T;
     typedef unsigned long      u32_T;
     typedef union {
       struct TAG_LONG {
         unsigned long    l;
         unsigned long    h;
       };
       unsigned long long hl;
     } u64_T;
    
     typedef struct bitf08 { 
        u08_T bit00:1;
        u08_T bit01:1;
        u08_T bit02:1;
        u08_T bit03:1;
        u08_T bit04:1;
        u08_T bit05:1;
        u08_T bit06:1;
        u08_T bit07:1;
     } bitf08_T;
    
     typedef struct bitf16 { 
        u16_T  bit00:1;
        u16_T  bit01:1;
        u16_T  bit02:1;
        u16_T  bit03:1;
        u16_T  bit04:1;
        u16_T  bit05:1;
        u16_T  bit06:1;
        u16_T  bit07:1;
        u16_T  bit08:1;
        u16_T  bit09:1;
        u16_T  bit10:1;
        u16_T  bit11:1;
        u16_T  bit12:1;
        u16_T  bit13:1;
        u16_T  bit14:1;
        u16_T  bit15:1;
     } bitf16_T;
    
     typedef struct bitf32 { 
        u32_T bit00:1;
        u32_T bit01:1;
        u32_T bit02:1;
        u32_T bit03:1;
        u32_T bit04:1;
        u32_T bit05:1;
        u32_T bit06:1;
        u32_T bit07:1;
        u32_T bit08:1;
        u32_T bit09:1;
        u32_T bit10:1;
        u32_T bit11:1;
        u32_T bit12:1;
        u32_T bit13:1;
        u32_T bit14:1;
        u32_T bit15:1;
        u32_T bit16:1;
        u32_T bit17:1;
        u32_T bit18:1;
        u32_T bit19:1;
        u32_T bit20:1;
        u32_T bit21:1;
        u32_T bit22:1;
        u32_T bit23:1;
        u32_T bit24:1;
        u32_T bit25:1;
        u32_T bit26:1;
        u32_T bit27:1;
        u32_T bit28:1;
        u32_T bit29:1;
        u32_T bit30:1;
        u32_T bit31:1;
     } bitf32_T;
    
     #define PRAGMA(x)         _Pragma(#x)
    
     #define __ZNEAR(type, x)  PRAGMA( ghs startzda)                \
         type    x;                                                 \
         PRAGMA( ghs endzda) 
    
     #define __SNEAR(type, x)  PRAGMA( ghs startsda)                \
         type    x;                                                 \
         PRAGMA( ghs endsda) 
    
     #define __FAR(type, x)    PRAGMA( ghs startdata)               \
         type    x;                                                 \
         PRAGMA( ghs enddata)
    
     #define __FARFUNC         __farcall

     #define __READ            volatile const 
     #define __WRITE           volatile
     #define __READ_WRITE      volatile
    
     #ifndef __IOREG8
       #define __IOREG8(reg,addr,attrib)     PRAGMA( ghs io reg addr) \
       extern attrib u08_T reg
     #endif
    
     #ifndef __IOREG16
       #define __IOREG16(reg,addr,attrib)    PRAGMA( ghs io reg addr) \
       extern attrib u16_T reg
     #endif
    
     #ifndef __IOREG32
       #define __IOREG32(reg,addr,attrib)    PRAGMA( ghs io reg addr) \
       extern attrib u32_T reg
     #endif
    
     #ifndef __IOREG
       #define __IOREG(reg,addr,attrib,type) PRAGMA( ghs io reg addr) \
       extern attrib type reg
     #endif
    
     #ifndef __IOREGARRAY
       #define __IOREGARRAY(reg, array, addr, attrib, type) PRAGMA(ghs io reg addr) \
       extern attrib type reg[array]
     #endif
    
     #define _GHS_PRAGMA_IO_TYPEDEF_
    
    #endif /* __LANGUAGE_ASM__ */
  #else    /* __STRICT_ANSI__ */
  typedef unsigned char     u08_T;
  typedef unsigned short    u16_T;
  typedef unsigned long     u32_T;
 
  typedef struct bitf08 { 
     int bit00:1;
     int bit01:1;
     int bit02:1;
     int bit03:1;
     int bit04:1;
     int bit05:1;
     int bit06:1;
     int bit07:1;
  } bitf08_T;
 
  #endif   /* __STRICT_ANSI__ */
#endif /* __IO_MACROS_H */
