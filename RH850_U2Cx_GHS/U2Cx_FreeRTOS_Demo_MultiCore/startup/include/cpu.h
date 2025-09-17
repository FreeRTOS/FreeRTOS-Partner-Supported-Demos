/*===========================================================================*/
/* Module      = cpu.h                                                       */
/* Version     = V1.34                                                       */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2014 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose:      Definition of Macros to access CPU System Registers         */
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
/*         Device:         RH850G3M core devices                             */
/*                         RH850G3K core devices                             */
/*                         RH850G4MH core devices                            */
/*                         U2B (__U2B__ macro) diversification added         */
/*                         U2B  virtualization registers added               */
/*===========================================================================*/

#ifndef __CPU_H__
#define __CPU_H__

#ifdef __ghs
#pragma ghs startnomisra

#define _INLINE_ __inline


/*
 * Utilize the GHS specific intrinsic functions
 *       __LDSR( n,m,v)
 *    v= __STSR((n,m)
 */
#include <../include/v800/v800_ghs.h>
#else
/* Other Tool vendors might have different approaches ... */
#define _INLINE_ inline
#endif

#ifndef __IO_MACROS_H
#include "io_macros_v2.h"
#endif

/* Basic control registers type definitions */
typedef union {
    u32_T     psw;               /* PSW, 32-bit access */
    struct {
        u32_T   Z:1,             /* Bit 0, zero */
                S:1,             /* Bit 1, sign */
                OV:1,            /* Bit 2, overflow */
                CY:1,            /* Bit 3, carry */
                SAT:1,           /* Bit 4, saturated */
                ID:1,            /* Bit 5, interrupt enable (0:enabled) */
                EP:1,            /* Bit 6, exception in process (0:interupt is processed */
                NP:1,            /* Bit 7, FE exception enable (0:enabled) */
                rsvd1:1,         /* Bit 8, reserved */
                DBG:3,           /* Bit 9-11, debug */
                rsvd2:3,         /* Bit 12-14, reserved */
                EBV:1,           /* Bit 15, 0:RBASE or 1:EBASE is used as exception processing base */
                CU0:1,           /* Bit 16, FPU */
                CU1:1,           /* Bit 17, reserved */
                CU2:1,           /* Bit 18, reserved */
                rsvd3:11,        /* Bit 19-29, reserved */
                UM:1,            /* Bit 30, User mode (0:supervisor, 1:user mode) */
                rsvd4:1;         /* Bit 31, reserved */
    };
} PSW_T;

typedef struct {
    u32_T   ctpsw;               /* CTPSW, 32-bit access */
    struct {                    
        u32_T   Z:1,             /* Bit 0, zero */
                S:1,             /* Bit 1, sign */
                OV:1,            /* Bit 2, overflow */
                CY:1,            /* Bit 3, carry */
                SAT:1;           /* Bit 4, saturated */
    };
} CTPSW_T;

typedef struct {
    const u32_T htcfg0;          /* HTCFG0, 32-bit access */
    struct {
        const u32_T rsvd1:16,    /* Bit 0-15, zero */
                    PEID:3,      /* processor entity identifier */
                    rsvd2:13;    /* Bit19-31, reserved */
    };
} HTCFG0_T;

typedef struct {
    u32_T   asid;                /* ASID, 32-bit access */
    struct {
        u32_T  ASID:10,          /* Bit 0-15, Address space identifier */
               rsvd1:22;         /* Bit10-31, reserved */
    };
} ASID_T;

typedef struct {
    u32_T   mei;                 /* MEI, 32-bit access */
    struct {                     
        u32_T   RW:1,            /* Bit 0, Address space identifier */
                ITYPE:5,         /* Bit 1-5, Instruction type */
                rsvd1:2,         /* Bit 6-7, reserved */
                U:1,             /* Bit 8, sign extension method of ITYPE */
                DS:2,            /* Bit 9-10, Acces size 0:byte,1:short,2:long,3:long long */
                rsvd2:5,         /* Bit 11-15, reserved */
                REG:5,           /* Bit 16-20, source/destination register accessed */
                rsvd3:11;        /* Bit 21-31, reserved */
    };
} MEI_T;

typedef union {
    u32_T     pid;               /* PID, 32-bit access */
    struct {                     
        u32_T   VID:8,           /* Bit 0-7, Instruction type */
                MPU:1,           /* Bit 8, MPU available */
                SFPU:1,          /* Bit 9, single precision FPU available */
                DFPU:1;          /* Bit 10, double precision FPU available */
    };
} PID_T;

typedef union {
    u32_T     mctl;              /* MCTL, 32-bit access */
    struct {
       u32_T  UIC:1,             /* Bit 0, 1:allow EI/DI instructions in user mode */
              MA:1;              /* Bit 1, Operate misalign access (if H/W supports) */
    };
} MCTL_T;

#ifndef __RH850G4MH__
typedef union {
    u32_T mcfg0;                 /* MCFG0, 32-bit access */
    struct {                     
        u32_T HVTE:1,            /* Bit 0, HVTRAP allowed (R) */
              HVCE:1,            /* Bit 1, HVCALL allowed  (must be 1!) (R) */
              HVP:1,             /* Bit 2, reserved, always 0 (R) */
              rsvd1:13,          /* Bit 3-15, reserved */
              SPID:8,            /* System protection number (h/w dependant!) */
              rsvd2:8;           /* Bit 24-31, reserved */
    };
} MCFG0_T;
#endif
typedef union {
    u32_T   sccfg;               /* SCCFG, 32-bit access */
    struct {                    
        u32_T   SIZE:8,          /* Bit 0-8, max. number of syscall entries */
                rsvd1:24;        /* Bit 8-31, resevred */
    };                          
} SCCFG_T;

typedef union {
    u32_T    ispr;               /* ISPR, 32-bit access */
    struct {
        u32_T     ISP:16,        /* Bit 0-15, interrupts priorities pending */
                  rsvd1:16;      /* Bit 16-31, resevred */
    };
} ISPR_T;


#ifdef __RH850G4MH__
typedef union {
    u32_T gmebase;              /* GMEBASE, 32-bit access */
    struct {                     
        u32_T   rint:1,         /* Bit 0, handler address reduction */
                  dv:1,         /* Bit 1, Indicates the EIINT interrupt handler address selection method */
               rsvd1:7,         /* Bit 2-8, reserved */
               ebase:23;        /* Bit 9-31, GMEBASE 8 to 2 are always set to 0 */
    };
} GMEBASE_T;

typedef union {
    u32_T mcfg0;                 /* MCFG0, 32-bit access */
    struct {                     
        u32_T HVTE:1,            /* Bit 0, HVTRAP allowed (R) */
              HVCE:1,            /* Bit 1, HVCALL allowed  (must be 1!) (R) */
              HVP:1,             /* Bit 2, reserved, always 0 (R) */
              rsvd1:13,          /* Bit 3-15, reserved */
              SPID:8,            /* System protection number (h/w dependant!) */
              rsvd2:8;           /* Bit 24-31, reserved */
    };
} MCFG0_T;

typedef union {
    u32_T   mpm;                 /* MPM, 32-bit access */
    struct {                     
        u32_T   MPE:1,           /* Bit 0, 1:Enable */
                SVP:1;           /* Bit 1, restrict access in SV mode */
    };
} MPM_T;

typedef union {
    const u32_T     mpcfg;           /* MPCFG, 32-bit read-only access */
    struct {                     
        const u32_T NMPUE:5,         /* Bit 0, 1:Enable */
                    rsvd1:11,        /* Bit 2-7, reserved */
                    ARCH:4;          /* Bit 1, restrict access in SV mode */
    };
} MPCFG_T;

typedef union {
    u32_T     mcr;             /* MPCFG, 32-bit read-only access */
    struct {                     
        u32_T   URE:1,         /* Indicates the read permission for user mode. */
                UWE:1,         /* Indicates the write permission for user mode */
                UXE:1,         /* Indicates the execution privilege for user mode. */
                SRE:1,         /* Indicates the write permission for supervisor mode */
                SWE:1,         /* Indicates the write permission for supervisor mode. */
                SXE:1,         /* Indicates the execution privilege for supervisor mode */
                rsvd1:2,       /* Global specification of an MPU area for all VMs. */
                OV:1;          /* OV=1, if the specified area includes 0x00000000 or 0x7FFFFFFF */
    };
} MCR_T;

typedef union {
    u32_T   pswh;                /* MCI, 32-bit access */
   struct {                     
        u32_T   rsvd1:8,         /* reserved */
                GPID:3,          /* GPID */
                rsvd2:20,        /* reserved */
                GM:1;            /* Bit 31: GM */
    };
} PSWH_T;

typedef union {
    u32_T   mci;                 /* MCI, 32-bit access */
    struct {                     
        u32_T   SPID:5;          /* Bit 0, 1:Enable */
    };
} MCI_T;


typedef union {
    u32_T   mpidx;               /* MCI, 32-bit access */
    struct {                     
        u32_T   IDX:5;           /* Bit 0, 1:Enable */
    };
} MPIDX_T;

typedef union {
    u32_T   mpat;               /* MPAT, Protection Area Attribute */
    struct {                     
        u32_T   UR:1,           /* Indicates the read permission for user mode. */
                UW:1,           /* Indicates the write permission for user mode */
                UX:1,           /* Indicates the execution privilege for user mode. */
                SR:1,           /* Indicates the write permission for supervisor mode */
                SW:1,           /* Indicates the write permission for supervisor mode. */
                SX:1,           /* Indicates the execution privilege for supervisor mode */
                rsvd1:1,        /* Global specification of an MPU area for all VMs. */
                E:1,            /* area n (n = 0 to 23) specified by MPIDX enabled (1) or disabled (0) */
                RG:1,           /* indicates the execution and read permissions for any SPID */
                WG:1,           /* This bit indicates the write permission for any SPID */
                RMPID:8,        /* indicates the execution and read permissions for SPID in MPIDn (n = 0 to 7) */
                WMPID:8;        /* indicates the write permissions for SPID in MPIDn (n = 0 to 7) */
    };
} MPAT_T;

#else
typedef union {
    u32_T   mpat;
    struct  {
        u32_T   UR:1,            /* Indicates the read permission for user mode. */
                UW:1,            /* Indicates the write permission for user mode */
                UX:1,            /* Indicates the execution privilege for user mode. */
                SR:1,            /* Indicates the write permission for supervisor mode */
                SW:1,            /* Indicates the write permission for supervisor mode. */
                SX:1,            /* Indicates the execution privilege for supervisor mode */
                G:1,             /* Global specification of an MPU area for all VMs. */
                                 /* If 1, the ASID bit has no effect on the area-matching */
                                 /* condition. */
                E:1,             /* This bit indicates whether area n is enabled or disabled. */
                rsvd1:8,         /* (Reserved for future expansion. Be sure to clear to 0.) */
                ASID:10,         /* These bits indicate the ASID value to be used as the */
                                 /* area match condition */
                rsvd2:6;         /* Reserved. Be sure to clear to 0.) */
    };
} MPAT_T;

typedef union {
    u32_T   mpm;                 /* MPM, 32-bit access */
    struct {                     
        u32_T   MPE:1,           /* Bit 0, 1:Enable */
                SVP:1,           /* Bit 1, restrict access in SV mode */
                rsvd1:6,         /* Bit 2-7, reserved */
                DR:1,            /* Bit 8, 1:enable read from address outside p-area */
                DW:1,            /* BIt 9, 1:enable write to address outside p-area */
                DX:1,            /* BIt 10, 1:enable execute from address outside p-area */
                rsvd2:21;        /* Bit 11-31, reserved */
    };
} MPM_T;
#endif


typedef union {
    u32_T   mprc;                /* MPRC, 32-bit access */
    struct {                    
        u32_T   E0:1,            /* Bit  0, protection area enable bits 1:enables */
                E1:1,            /* Bit  1 */
                E2:1,            /* Bit  2 */
                E3:1,            /* Bit  3 */
                E4:1,            /* Bit  4 */
                E5:1,            /* Bit  5 */
                E6:1,            /* Bit  6 */
                E7:1,            /* Bit  7 */
                E8:1,            /* Bit  8 */
                E9:1,            /* Bit  9 */
                E10:1,           /* Bit 10 */
                E11:1,           /* Bit 11 */
                E12:1,           /* Bit 12 */
                E13:1,           /* Bit 13 */
                E14:1,           /* Bit 14 */
                E15:1;           /* Bit 15 */
    };
} MPRC_T;

typedef union {
    u32_T   fpsr;                /* FPSR, 32-bit access */
    struct {                    
        u32_T   XPI:1,           /* Bit  0, preservation bit */
                XPU:1,           /* Bit  1, preservation bit */
                XPO:1,           /* Bit  2, preservation bit */
                XPZ:1,           /* Bit  3, preservation bit */
                XPV:1,           /* Bit  4, preservation bit */
                                 
                XEI:1,           /* Bit  5, enable bit */
                XEU:1,           /* Bit  6, enable bit */
                XEO:1,           /* Bit  7, enable bit */
                XEZ:1,           /* Bit  8, enable bit */
                XEV:1,           /* Bit  9, enable bit */
                                 
                XCI:1,           /* Bit 10, preservation bit */
                XCU:1,           /* Bit 11, preservation bit */
                XCO:1,           /* Bit 12, preservation bit */
                XCZ:1,           /* Bit 13, preservation bit */
                XCV:1,           /* Bit 14, preservation bit */
                                 
                rsvd1:1,         /* Bit 16, reserved */
                FS:1,            /* Bit 17, subnormal number enable */
                RM:2,            /* Bit 18-19, rounding mode control register */
                rsvd2:1,         /* Bit 20, reserved */
                PEM:1,           /* Bit 21, precise exception mode */
                IF:1,            /* Bit 22, flushing of subnormal numbers */
                FN:1,            /* Bit 23, flushing of subnormal numbers to nearest */
                CC:8;            /* Bit 24-31, comparison result condition bits */
    };                          
} FPSR_T;                       
                                
typedef union {                 
    u32_T   ictagl;              /* ICCTAGL, 32-bit access */
    struct {                    
        u32_T    V:1,            /* Bit 0, valid bit */
                 rsvd1:1,        /* Bit 1, reserved */
                 Lock:1,         /* Bit 2, lock bit */
                 rsvd2:1,        /* Bit 3, reserved */
                 LRU:2,          /* Bit 4-5, valid bit */
                 rsvd3:4,        /* Bit 6-9, reserved */
                 LPN:22;         /* Bit 10-31, physical page number */
    };                          
} ICCTAGL_T;                    
                                
typedef union {                 
    u32_T   ictagh;              /* ICCTAGH, 32-bit access */
    struct {                    
        u32_T    rsvd1:8,        /* Bit 0-7, reserved */
                 TAGECC:8,       /* Bit 8-15, TAG ECC */
                 DATAECC:8,      /* Bit 16-23, data ECC */
                 rsvd2:4,        /* Bit 24-27, reserved */
                 PT:1,           /* Bit 28, automatic ECC generation TAG */
                 WT:1,           /* Bit 29, automatic ECC generation TAG */
                 PD:1,           /* Bit 30, automatic ECC generation data */
                 WD:1;           /* Bit 31, automatic ECC generation data */
    };                          
} ICCTAGH_T;                    
                                
typedef union {                 
    u32_T   icctrl;              /* ICCTRL, 32-bit access */
    struct {                    
        u32_T    ICHEN:1,        /* Bit 0, Instruction cahe 0:invalid 1:valid */
                 ICHEMK:1,       /* Bit 1, mask cache error expceptions */
                 ICHEIV:1,       /* Bit 2, 1:automatically invalidate instr. cache */
                 rsvd1:5,        /* Bit 3-7, reserved */
                 ICHCLR:1,       /* Bit 8, 1:clear entire instr. cache */
                 rsvd2:7,        /* Bit 9-15, reserved set to 0*/
                 rsvd3:1,        /* Bit 16, reserved set to 1*/
                 D1EV:1,         /* Bit 17, ECC correction */
                 rsvd4:14;       /* Bit 31, automatic ECC generation data */
    };                          
} ICCTRL_T;                     
                                
typedef union {                 
    u32_T   iccfg;               /* ICCFG, 32-bit access */
    struct {                    
        u32_T    ICHWAY:4,       /* no. of ways in ICACHE */
                                 /* 0: no icache, 1: 1 way */
                                 /* 2: 2 ways,   4: 4 ways */
                                 /* 8: 8 ways */
                 ICHLINE:4,      /* no of lines/way in ICACHE */
                                 /* 0: no icache,  1: 32 lines */
                                 /* 2: 64 lines,   4: 128 lines */
                                 /* 8: 256 liness */
                                
                 ICHSIZE:7,      /* Size in kBytes of ICACHE */
                                 /*  0:  no icache,   1:  1 kByte */
                                 /*  2:  2 kBytes,    4:  4 kBytes */
                                 /*  8:  8 kBytes,   16: 16 kBytes */
                                 /* 32: 32 kBytes,   64: 64 kBytes */
                                
                 rsvd1:1,        /* Bit 15, reserved, always 0 */
                 rsvd2:1,        /* Bit 16, reserved, always 1 */
                 rsvd3:15;       /* Bit 17-31, reserved, always 0 */
    };                          
} ICCFG_T;                      
                                
typedef union {                 
    u32_T   icerr;               /* ICERR, 32-bit access */
    struct {                   
        u32_T    ICHERR:1,       /* Bit 0, is 1, if error occured */
                 rsvd1:1,        /* Bit 1, reserved, always 0 */
                 ICHET:1,        /* Bit 2, error in tag RAM */
                 ICHED:1,        /* Bit 3, error in data RAM */
                 ICHERQ:1,       /* Bit 4, chache error exception in progress */
                 ICHEIX:8,       /* Bit 5-12, retain cache index, if chache error occured */
                 ICHEWY:2,       /* Bit 13-14, retain way number, if chache error occured */
                 rsvd2:1,        /* Bit 15, reserved, always 0 */
                                
                 ERMDE:1,        /* Bit 16, Error exception notification mask : Data RAM 2-bit error */
                 ERMDC:1,        /* Bit 17, Error exception notification mask : Data RAM 1-bit correction */
                 ERMTE2:1,       /* Bit 18, Error exception notification mask : Tag RAM 2-bit error */
                 ERMTE1:1,       /* Bit 19, Error exception notification mask : Tag RAM 1-bit error */
                 ERMPBSE:1,      /* Bit 20, Error exception notification mask : WAY error */
                 ERMMH:1,        /* Bit 21, Error exception notification mask : Multi bit */
                                
                 rsvd3:2,        /* Bit 22-23, reserved, always 0 */
                                
                 ESDE:1,         /* Bit 24, Error status: Data RAM 2-bit error */
                 ESDC:1,         /* Bit 25, Error status: Data RAM 1-bit correction R/W Undefined */
                 ESTE2:1,        /* Bit 26, Error status: Tag RAM 2-bit error R/W Undefined */
                 ESTE1:1,        /* Bit 27, Error status: Tag RAM 1-bit error R/W Undefined */
                 ESPBSE:1,       /* Bit 28, Error status: WAY error R/W Undefined */
                 ESMH:1,         /* Bit 29, Error status: Multi hit R/W Undefined */
                 rsvd4:2,        /* Bit 30, reserved, always 0 */
                 CISTW:1;        /* Bit 31, Indicates an error in the destibnation way of CIST instr. */
    };
} ICERR_T;

typedef union {                 
    u32_T   cdbcr;               /* CDBCR, 32-bit access */
    struct {                   
        u32_T   CDBEN:1,         /* Bit 0, This bit specifies enables or disables of the data buffer. */
                                 /*        0: Data buffer disabled.  1: Data buffer enabled. */
                CDBCLR:1,        /* Bit 1, 1: data buffer is cleared. */
                rsvd1:30;        /* Bit 2-31, Reserved for future expansion. Be sure to set to 0. */ 
    };
} CDBCR_T;

#ifdef __RH850G4MH__
typedef union {
    u32_T   rbcr0;
    struct  {
        u32_T   BE:16,           /* This bit specifies the save mode.  0: Save mode 0 1: Save mode 1 */
                MD:1,            /* This bit indicates whether area n is enabled or disabled. */
                rsvd:15;         /* (Reserved for future expansion. Be sure to clear to 0.) */
    };
} RBCR0_T;


typedef union {
    u32_T   rbcr1;
    struct  {
        u32_T   NC:16,           /* These bits specify the values to be set to the PSW.ID bit when interrupt occurs */
                rsvd:16;         /* (Reserved for future expansion. Be sure to clear to 0.) */
    };
} RBCR1_T;

typedef union {
    u32_T   rbnr;
    struct  {
        u32_T   BN:5,            /* These bits indicate the number of the register bank to be used next. */
                rsvd:27;         /* (Reserved for future expansion. Be sure to clear to 0.) */
    };
} RBNR_T;

typedef union {
    u32_T   rbip;
    struct  {
        const u32_T rsvd:4;      /* These bits indicate the number of the register bank to be used next. */
              u32_T RBIP:28;     /* (Reserved for future expansion. Be sure to clear to 0.) */
    };
} RBIP_T;

#endif

/*
 * Groups
 */

/*      Name      Reg, Group        Comment */
#define EIPC          0, 0        /* Status save registers when acknowledging EI level exception SV */
#define EIPSW         1, 0        /* Status save registers when acknowledging EI level exception SV */
#define FEPC          2, 0        /* Status save registers when acknowledging FE level exception SV */
#define FEPSW         3, 0        /* Status save registers when acknowledging FE level exception SV */
#define PSW           5, 0        /* Program status word Note 1 */
#define EIIC         13, 0        /* EI level exception cause SV */
#define FEIC         14, 0        /* FE level exception cause SV */
#define CTPC         16, 0        /* CALLT execution status save register UM */
#define CTPSW        17, 0        /* CALLT execution status save register UM */
#define CTBP         20, 0        /* CALLT base pointer UM */
#define EIWR         28, 0        /* EI level exception working register SV */
#define FEWR         29, 0        /* FE level exception working register SV */
#define BSEL         31, 0        /* (Reserved for backward compatibility with V850E2 series)Note 2 SV */
                   
#ifdef __RH850G4MH__
#define SPID          0, 1        /* System protection identifier */
#define SPIDLIST      1, 1        /* System protection identifier */
#else
#define MCFG0         0, 1        /* Machine configuration SV */
#endif
#define RBASE         2, 1        /* Reset vector base address SV */
#define EBASE         3, 1        /* Exception handler vector address SV */
#define INTBP         4, 1        /* Base address of the interrupt handler table SV */
#define MCTL          5, 1        /* CPU control SV */
#define PID           6, 1        /* Processor ID SV */
#define SCCFG        11, 1        /* SYSCALL operation setting SV */
#define SCBP         12, 1        /* SYSCALL base pointer SV */
                   
#define HTCFG0        0, 2        /* Thread configuration SV */
#define MEA           6, 2        /* Memory error address SV */
#define ASID          7, 2        /* Address space ID SV */
#define MEI           8, 2        /* Memory error information SV */
#define ISPR         10, 2        /* Priority of interrupt being serviced SV */
#define PMR          11, 2        /* Interrupt priority masking SV */
#define ICSR         12, 2        /* Interrupt control status SV */
#define INTCFG       13, 2        /* Interrupt function setting SV */
#ifdef __RH850G4MH__
#define PLMR         14, 2        /* Interrupt function setting SV */
#endif

#define MPM            0, 5        /* Memory protection operation mode setting SV */
#define MPRC           1, 5        /* MPU region control SV */
#define MPBRGN         4, 5        /* MPU base region number SV */
#define MPTRGN         5, 5        /* MPU end region number SV */
#define MCA            8, 5        /* Memory protection setting check address SV */
#define MCS            9, 5        /* Memory protection setting check size SV */
#define MCC           10, 5        /* Memory protection setting check command SV */
#define MCR           11, 5        /* Memory protection setting check result SV */
                     
#define MPLA0          0, 6        /* Protection area minimum address SV */
#define MPUA0          1, 6        /* Protection area maximum address SV */
#define MPAT0          2, 6        /* Protection area attribute SV */
#define MPLA1          4, 6        /* Protection area minimum address SV */
#define MPUA1          5, 6        /* Protection area maximum address SV */
#define MPAT1          6, 6        /* Protection area attribute SV */
#define MPLA2          8, 6        /* Lower address of the protection area SV */
#define MPUA2          9, 6        /* Protection area maximum address SV */
#define MPAT2         10, 6        /* Protection area attribute SV */
#define MPLA3         12, 6        /* Protection area minimum address SV */
#define MPUA3         13, 6        /* Protection area maximum address SV */
#define MPAT3         14, 6        /* Protection area attribute SV */
#define MPLA4         16, 6        /* Protection area minimum address SV */
#define MPUA4         17, 6        /* Protection area maximum address SV */
#define MPAT4         18, 6        /* Protection area attribute SV */
#define MPLA5         20, 6        /* Protection area minimum address SV */
#define MPUA5         21, 6        /* Protection area maximum address SV */
#define MPAT5         22, 6        /* Protection area attribute SV */
#define MPLA6         24, 6        /* Protection area minimum address SV */
#define MPUA6         25, 6        /* Protection area maximum address SV */
#define MPAT6         26, 6        /* Protection area attribute SV */
#define MPLA7         28, 6        /* Protection area minimum address SV */
#define MPUA7         29, 6        /* Protection area maximum address SV */
#define MPAT7         30, 6        /* Protection area attribute SV */
#define MPLA8          0, 7        /* Protection area minimum address SV */
#define MPUA8          1, 7        /* Protection area maximum address SV */
#define MPAT8          2, 7        /* Protection area attribute SV */
#define MPLA9          4, 7        /* Protection area minimum address SV */
#define MPUA9          5, 7        /* Protection area maximum address SV */
#define MPAT9          6, 7        /* Protection area attribute SV */
#define MPLA10         8, 7        /* Protection area minimum address SV */
#define MPUA10         9, 7        /* Protection area maximum address SV */
#define MPAT10        10, 7        /* Protection area attribute SV */
#define MPLA11        12, 7        /* Protection area minimum address SV */
#define MPUA11        13, 7        /* Protection area maximum address SV */
#define MPAT11        14, 7        /* Protection area attribute SV */
#define MPLA12        16, 7        /* Protection area minimum address SV */
#define MPUA12        17, 7        /* Protection area maximum address SV */
#define MPAT12        18, 7        /* Protection area attribute SV */
#define MPLA13        20, 7        /* Protection area minimum address SV */
#define MPUA13        21, 7        /* Protection area maximum address SV */
#define MPAT13        22, 7        /* Protection area attribute SV */
#define MPLA14        24, 7        /* Protection area minimum address SV */
#define MPUA14        25, 7        /* Protection area maximum address SV */
#define MPAT14        26, 7        /* Protection area attribute SV */
#define MPLA15        28, 7        /* Protection area minimum address SV */
#define MPUA15        29, 7        /* Protection area maximum address SV */
#define MPAT15        30, 7        /* Protection area attribute SV */

#ifdef __RH850G4MH__
 #define RBCR0        15, 2        /* Register bank control 0 SV */
 #define RBCR1        16, 2        /* Register bank control 1 SV */
 #define RBNR         17, 2        /* Register bank number SV */
 #define RBIP         18, 2        /* Register bank initial pointer SV */
                                   
 #define TSCOUNTL      0, 11       /* Timestamp count L register UM*1 */
 #define TSCOUNTH      1, 11       /* Timestamp count H register UM*1 */
 #define TSCTRL        2, 11       /* Timestamp count control register SV */
 #define MPIDX        16, 5        /* Index of memory protection setting registers to be accessed */
 #define MPLA         20, 5        /* Index of memory protection setting registers to be accessed */
 #define MPUA         21, 5        /* Index of memory protection setting registers to be accessed */
 #define MPAT         22, 5        /* Index of memory protection setting registers to be accessed */
 #define MPID0        24, 5        /* SPID which can access protection area */
 #define MPID1        25, 5        /* SPID which can access protection area */
 #define MPID2        26, 5        /* SPID which can access protection area */
 #define MPID3        27, 5        /* SPID which can access protection area */
 #define MPID4        28, 5        /* SPID which can access protection area */
 #define MPID5        29, 5        /* SPID which can access protection area */
 #define MPID6        30, 5        /* SPID which can access protection area */
 #define MPID7        31, 5        /* SPID which can access protection area */
                      
 #define PMUMCTRL      8, 11       /* Performance counter user mode control register SV */
 #define PMGMCTRL      9, 11       /* Performance counter Guest mode control register */
#ifdef __E2X__
 #define PMCOUNT0     16, 11       /* Performance count 0 register UM*2 */
 #define PMCOUNT1     18, 11       /* Performance count 1 register UM*2 */
 #define PMCOUNT2     20, 11       /* Performance count 2 register UM*2 */
 #define PMCOUNT3     22, 11       /* Performance count 3 register UM*2 */
 #define PMCTRL0      17, 11       /* Performance count control 0 register UM*2 */
 #define PMCTRL1      19, 11       /* Performance count control 1 register UM*2 */
 #define PMCTRL2      21, 11       /* Performance count control 2 register UM*2 */
 #define PMCTRL3      23, 11       /* Performance count control 3 register UM* */
#endif

 #ifdef __U2B__      
  #define HVCFG       16, 1        /* Settings for hardware-assisted virtualization SV HV */
  #define GMCFG       17, 1        /* Settings for guest mode operation SV HV */
  #define PSWH        15, 0        /* Program status extension word SV HV */
  #define FEPSWH      18, 0        /* PSWH save registers when acknowledging FE level exception SV HV */
  #define EIPSWH      19, 0        /* PSWH save registers when acknowledging EI level exception SV HV */
  #define HVSB        20, 1        /* Information notification register for guest mode SV HV */

  #define GMEIPC       0, 9         
  #define GMEIPSW      1, 9        /* Status save registers when acknowledging EI level exception SV */
  #define GMFEPC       2, 9         
  #define GMFEPSW      3, 9        /* PSW save registers when acknowledging FE level exception SV HV */
  #define GMPSW        5, 9        /* Program status extension word SV HV */

  #define GMMEA        6, 9         
  #define GMMEI        8, 9         
  #define GMEIIC      13, 9
  #define GMFEIC      14, 9

  #define GMINTBP     20, 9         
  #define GMSPID      16, 9        /* System protection identifier */
  #define GMSPIDLIST  17, 9        /* System protection identifier */
  #define GMEBASE     19, 9        /* EBASE identifier */
  #define GMPLMR      25, 9        /* Interrupt function setting SV */
  #define GMEIWR      28, 9         
  #define GMFEWR      29, 9         


  #define PMCTRL0      0, 14      /* Performance count control 0 register */
  #define PMCTRL1      1, 14      /* Performance count control 1 register */
  #define PMCTRL2      2, 14      /* Performance count control 2 register */
  #define PMCTRL3      3, 14      /* Performance count control 3 register */
  #define PMCTRL4      4, 14      /* Performance count control 4 register */
  #define PMCTRL5      5, 14      /* Performance count control 5 register */
  #define PMCTRL6      6, 14      /* Performance count control 6 register */
  #define PMCTRL7      7, 14      /* Performance count control 7 register */
  #define PMCOUNT0    16, 14      /* Performance count 0 register */
  #define PMCOUNT1    17, 14      /* Performance count 1 register */
  #define PMCOUNT2    18, 14      /* Performance count 2 register */
  #define PMCOUNT3    19, 14      /* Performance count 3 register */
  #define PMCOUNT4    20, 14      /* Performance count 4 register */
  #define PMCOUNT5    21, 14      /* Performance count 5 register */
  #define PMCOUNT6    22, 14      /* Performance count 6 register */
  #define PMCOUNT7    23, 14      /* Performance count 7 register */
  #define PMSUBCND0    0, 15      /* Performance count subcondition 0 register */
  #define PMSUBCND1    1, 15      /* Performance count subcondition 1 register */
  #define PMSUBCND2    2, 15      /* Performance count subcondition 2 register */
  #define PMSUBCND3    3, 15      /* Performance count subcondition 3 register */
  #define PMSUBCND4    4, 15      /* Performance count subcondition 4 register */
  #define PMSUBCND5    5, 15      /* Performance count subcondition 5 register */
  #define PMSUBCND6    6, 15      /* Performance count subcondition 6 register */
  #define PMSUBCND7    7, 15      /* Performance count subcondition 7 register */

 #else  /* E2X */
 #endif                         
#endif                          
                                
                                
/* HAZARD RESOLUTION PROCEDURE FOR SYSTEM REGISTERS */

_INLINE_ void SET_PSW_USERMODE( void)
{
    PSW_T p;
    p.psw= __STSR( PSW); 
    p.UM= 1; 
    __LDSR( PSW, p.psw); 
    __SYNCI(); 
}

_INLINE_ void SET_FEPSW_SVMODE( void)
{ 
    PSW_T p;
    p.psw= __STSR( FEPSW); 
    p.UM= 0; 
    __LDSR( FEPSW, p.psw); 
    __SYNCI(); 
}

#ifndef __RH850G4MH__
_INLINE_ void SET_MCFG0_SPID( u32_T v)
{ 
    MCFG0_T p;
    p.mcfg0= __STSR( MCFG0); 
    p.SPID= v; 
    __LDSR( MCFG0, p.mcfg0); 
    __SYNCI(); 
}
#endif

_INLINE_ void SET_SCCFG( u32_T emax) 
{ 
    SCCFG_T p; 
    p.sccfg= __STSR( SCCFG); 
    p.SIZE= emax; 
    __LDSR( SCCFG, p.sccfg); 
    __SYNCP(); 
}


_INLINE_ void SET_ASID( u32_T v)   { __LDSR( ASID  , v); __SYNCP();}

_INLINE_ void SET_MPM   (u32_T v)  { __LDSR( MPM   , v); __SYNCI();}
_INLINE_ void SET_MPRC  (u32_T v)  { __LDSR( MPRC  , v); __SYNCI();}
_INLINE_ void SET_MPBRGN(u32_T v)  { __LDSR( MPBRGN, v); __SYNCI();}
_INLINE_ void SET_MPTRGN(u32_T v)  { __LDSR( MPTRGN, v); __SYNCI();}
_INLINE_ void SET_MCA   (u32_T v)  { __LDSR( MCA   , v); __SYNCI();}
_INLINE_ void SET_MCS   (u32_T v)  { __LDSR( MCS   , v); __SYNCI();}
_INLINE_ void SET_MCC   (u32_T v)  { __LDSR( MCC   , v); __SYNCI();}
_INLINE_ void SET_MCR   (u32_T v)  { __LDSR( MCR   , v); __SYNCI();}

_INLINE_ void SET_MPLA0(u32_T v)   { __LDSR( MPLA0 , v); __SYNCP();}
_INLINE_ void SET_MPUA0(u32_T v)   { __LDSR( MPUA0 , v); __SYNCP();}
_INLINE_ void SET_MPAT0(u32_T v)   { __LDSR( MPAT0 , v); __SYNCP();}
_INLINE_ void SET_MPLA1(u32_T v)   { __LDSR( MPLA1 , v); __SYNCP();}
_INLINE_ void SET_MPUA1(u32_T v)   { __LDSR( MPUA1 , v); __SYNCP();}
_INLINE_ void SET_MPAT1(u32_T v)   { __LDSR( MPAT1 , v); __SYNCP();}
_INLINE_ void SET_MPLA2(u32_T v)   { __LDSR( MPLA2 , v); __SYNCP();}
_INLINE_ void SET_MPUA2(u32_T v)   { __LDSR( MPUA2 , v); __SYNCP();}
_INLINE_ void SET_MPAT2(u32_T v)   { __LDSR( MPAT2 , v); __SYNCP();}
_INLINE_ void SET_MPLA3(u32_T v)   { __LDSR( MPLA3 , v); __SYNCP();}
_INLINE_ void SET_MPUA3(u32_T v)   { __LDSR( MPUA3 , v); __SYNCP();}
_INLINE_ void SET_MPAT3(u32_T v)   { __LDSR( MPAT3 , v); __SYNCP();}
_INLINE_ void SET_MPLA4(u32_T v)   { __LDSR( MPLA4 , v); __SYNCP();}
_INLINE_ void SET_MPUA4(u32_T v)   { __LDSR( MPUA4 , v); __SYNCP();}
_INLINE_ void SET_MPAT4(u32_T v)   { __LDSR( MPAT4 , v); __SYNCP();}
_INLINE_ void SET_MPLA5(u32_T v)   { __LDSR( MPLA5 , v); __SYNCP();}
_INLINE_ void SET_MPUA5(u32_T v)   { __LDSR( MPUA5 , v); __SYNCP();}
_INLINE_ void SET_MPAT5(u32_T v)   { __LDSR( MPAT5 , v); __SYNCP();}
_INLINE_ void SET_MPLA6(u32_T v)   { __LDSR( MPLA6 , v); __SYNCP();}
_INLINE_ void SET_MPUA6(u32_T v)   { __LDSR( MPUA6 , v); __SYNCP();}
_INLINE_ void SET_MPAT6(u32_T v)   { __LDSR( MPAT6 , v); __SYNCP();}
_INLINE_ void SET_MPLA7(u32_T v)   { __LDSR( MPLA7 , v); __SYNCP();}
_INLINE_ void SET_MPUA7(u32_T v)   { __LDSR( MPUA7 , v); __SYNCP();}
_INLINE_ void SET_MPAT7(u32_T v)   { __LDSR( MPAT7 , v); __SYNCP();}
_INLINE_ void SET_MPLA8(u32_T v)   { __LDSR( MPLA8 , v); __SYNCP();}
_INLINE_ void SET_MPUA8(u32_T v)   { __LDSR( MPUA8 , v); __SYNCP();}
_INLINE_ void SET_MPAT8(u32_T v)   { __LDSR( MPAT8 , v); __SYNCP();}
_INLINE_ void SET_MPLA9(u32_T v)   { __LDSR( MPLA9 , v); __SYNCP();}
_INLINE_ void SET_MPUA9(u32_T v)   { __LDSR( MPUA9 , v); __SYNCP();}
_INLINE_ void SET_MPAT9(u32_T v)   { __LDSR( MPAT9 , v); __SYNCP();}
_INLINE_ void SET_MPLA10(u32_T v)  { __LDSR( MPLA10, v); __SYNCP();}
_INLINE_ void SET_MPUA10(u32_T v)  { __LDSR( MPUA10, v); __SYNCP();}
_INLINE_ void SET_MPAT10(u32_T v)  { __LDSR( MPAT10, v); __SYNCP();}
_INLINE_ void SET_MPLA11(u32_T v)  { __LDSR( MPLA11, v); __SYNCP();}
_INLINE_ void SET_MPUA11(u32_T v)  { __LDSR( MPUA11, v); __SYNCP();}
_INLINE_ void SET_MPAT11(u32_T v)  { __LDSR( MPAT11, v); __SYNCP();}
_INLINE_ void SET_MPLA12(u32_T v)  { __LDSR( MPLA12, v); __SYNCP();}
_INLINE_ void SET_MPUA12(u32_T v)  { __LDSR( MPUA12, v); __SYNCP();}
_INLINE_ void SET_MPAT12(u32_T v)  { __LDSR( MPAT12, v); __SYNCP();}
_INLINE_ void SET_MPLA13(u32_T v)  { __LDSR( MPLA13, v); __SYNCP();}
_INLINE_ void SET_MPUA13(u32_T v)  { __LDSR( MPUA13, v); __SYNCP();}
_INLINE_ void SET_MPAT13(u32_T v)  { __LDSR( MPAT13, v); __SYNCP();}
_INLINE_ void SET_MPLA14(u32_T v)  { __LDSR( MPLA14, v); __SYNCP();}
_INLINE_ void SET_MPUA14(u32_T v)  { __LDSR( MPUA14, v); __SYNCP();}
_INLINE_ void SET_MPAT14(u32_T v)  { __LDSR( MPAT14, v); __SYNCP();}
_INLINE_ void SET_MPLA15(u32_T v)  { __LDSR( MPLA15, v); __SYNCP();}
_INLINE_ void SET_MPUA15(u32_T v)  { __LDSR( MPUA15, v); __SYNCP();}
_INLINE_ void SET_MPAT15(u32_T v)  { __LDSR( MPAT15, v); __SYNCP();}

_INLINE_ u32_T GET_EIIC(  void )   { return __STSR( EIIC  );  }
_INLINE_ u32_T GET_FEIC(  void )   { return __STSR( FEIC  );  }
_INLINE_ u32_T GET_EIPSW( void )   { return __STSR( EIPSW  ); }
_INLINE_ u32_T GET_FEPSW( void )   { return __STSR( FEPSW  ); }

_INLINE_ u32_T GET_ASID( void )    { return __STSR( ASID  ); }

_INLINE_ u32_T GET_MPM   (void )   { return __STSR( MPM   ); } 
_INLINE_ u32_T GET_MPRC  (void )   { return __STSR( MPRC  ); } 
_INLINE_ u32_T GET_MPBRGN(void )   { return __STSR( MPBRGN); } 
_INLINE_ u32_T GET_MPTRGN(void )   { return __STSR( MPTRGN); } 
_INLINE_ u32_T GET_MCA   (void )   { return __STSR( MCA   ); } 
_INLINE_ u32_T GET_MCS   (void )   { return __STSR( MCS   ); } 
_INLINE_ u32_T GET_MCC   (void )   { return __STSR( MCC   ); } 
_INLINE_ u32_T GET_MCR   (void )   { return __STSR( MCR   ); } 
_INLINE_ u32_T GET_PID   (void )   { return __STSR( PID   ); } 

_INLINE_ u32_T GET_MPLA0(void )    { return __STSR( MPLA0 ); }
_INLINE_ u32_T GET_MPUA0(void )    { return __STSR( MPUA0 ); }
_INLINE_ u32_T GET_MPAT0(void )    { return __STSR( MPAT0 ); }
_INLINE_ u32_T GET_MPLA1(void )    { return __STSR( MPLA1 ); }
_INLINE_ u32_T GET_MPUA1(void )    { return __STSR( MPUA1 ); }
_INLINE_ u32_T GET_MPAT1(void )    { return __STSR( MPAT1 ); }
_INLINE_ u32_T GET_MPLA2(void )    { return __STSR( MPLA2 ); }
_INLINE_ u32_T GET_MPUA2(void )    { return __STSR( MPUA2 ); }
_INLINE_ u32_T GET_MPAT2(void )    { return __STSR( MPAT2 ); }
_INLINE_ u32_T GET_MPLA3(void )    { return __STSR( MPLA3 ); }
_INLINE_ u32_T GET_MPUA3(void )    { return __STSR( MPUA3 ); }
_INLINE_ u32_T GET_MPAT3(void )    { return __STSR( MPAT3 ); }
_INLINE_ u32_T GET_MPLA4(void )    { return __STSR( MPLA4 ); }
_INLINE_ u32_T GET_MPUA4(void )    { return __STSR( MPUA4 ); }
_INLINE_ u32_T GET_MPAT4(void )    { return __STSR( MPAT4 ); }
_INLINE_ u32_T GET_MPLA5(void )    { return __STSR( MPLA5 ); }
_INLINE_ u32_T GET_MPUA5(void )    { return __STSR( MPUA5 ); }
_INLINE_ u32_T GET_MPAT5(void )    { return __STSR( MPAT5 ); }
_INLINE_ u32_T GET_MPLA6(void )    { return __STSR( MPLA6 ); }
_INLINE_ u32_T GET_MPUA6(void )    { return __STSR( MPUA6 ); }
_INLINE_ u32_T GET_MPAT6(void )    { return __STSR( MPAT6 ); }
_INLINE_ u32_T GET_MPLA7(void )    { return __STSR( MPLA7 ); }
_INLINE_ u32_T GET_MPUA7(void )    { return __STSR( MPUA7 ); }
_INLINE_ u32_T GET_MPAT7(void )    { return __STSR( MPAT7 ); }
_INLINE_ u32_T GET_MPLA8(void )    { return __STSR( MPLA8 ); }
_INLINE_ u32_T GET_MPUA8(void )    { return __STSR( MPUA8 ); }
_INLINE_ u32_T GET_MPAT8(void )    { return __STSR( MPAT8 ); }
_INLINE_ u32_T GET_MPLA9(void )    { return __STSR( MPLA9 ); }
_INLINE_ u32_T GET_MPUA9(void )    { return __STSR( MPUA9 ); }
_INLINE_ u32_T GET_MPAT9(void )    { return __STSR( MPAT9 ); }
_INLINE_ u32_T GET_MPLA10(void )   { return __STSR( MPLA10); }
_INLINE_ u32_T GET_MPUA10(void )   { return __STSR( MPUA10); }
_INLINE_ u32_T GET_MPAT10(void )   { return __STSR( MPAT10); }
_INLINE_ u32_T GET_MPLA11(void )   { return __STSR( MPLA11); }
_INLINE_ u32_T GET_MPUA11(void )   { return __STSR( MPUA11); }
_INLINE_ u32_T GET_MPAT11(void )   { return __STSR( MPAT11); }
_INLINE_ u32_T GET_MPLA12(void )   { return __STSR( MPLA12); }
_INLINE_ u32_T GET_MPUA12(void )   { return __STSR( MPUA12); }
_INLINE_ u32_T GET_MPAT12(void )   { return __STSR( MPAT12); }
_INLINE_ u32_T GET_MPLA13(void )   { return __STSR( MPLA13); }
_INLINE_ u32_T GET_MPUA13(void )   { return __STSR( MPUA13); }
_INLINE_ u32_T GET_MPAT13(void )   { return __STSR( MPAT13); }
_INLINE_ u32_T GET_MPLA14(void )   { return __STSR( MPLA14); }
_INLINE_ u32_T GET_MPUA14(void )   { return __STSR( MPUA14); }
_INLINE_ u32_T GET_MPAT14(void )   { return __STSR( MPAT14); }
_INLINE_ u32_T GET_MPLA15(void )   { return __STSR( MPLA15); }
_INLINE_ u32_T GET_MPUA15(void )   { return __STSR( MPUA15); }
_INLINE_ u32_T GET_MPAT15(void )   { return __STSR( MPAT15); }


/* Access restrictions, write only if PSW.ID=1 */
_INLINE_ void SET_PSW_EBV( void)
{ 
    PSW_T p;
    __DI() ;
    p.psw= __STSR( PSW); 
    p.EBV= 1;
    __LDSR( PSW, p.psw);
}

_INLINE_ void SET_MCTL_MA( u32_T v)
{ 
    MCTL_T p;
    p.mctl= __STSR( MCTL);
    p.MA=!!v;
    __LDSR( MCTL, p.mctl);
    __SYNCP();
}

#ifndef __RH850G4MH__
_INLINE_ void SET_MCTL_UIC( u32_T v)
{ 
    MCTL_T p;
    p.mctl= __STSR( MCTL);
    p.UIC=!!v;
    __LDSR( MCTL, p.mctl);
    __SYNCP();
}
#endif

_INLINE_ void SET_EBASE( u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( EBASE, v);
    __RIR( __t);
}

_INLINE_ void SET_INTBP( u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( INTBP, v);
    __RIR( __t);
}

_INLINE_ void SET_ISPR(u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( ISPR, v);
    __RIR( __t);
}

_INLINE_ void SET_PMR(u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( PMR, v);
    __RIR( __t);
}

_INLINE_ void SET_ICSR( u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( ICSR, v);
    __RIR( __t);
}

_INLINE_ void SET_INTCFG( u32_T v)
{ 
    u32_T __t=__DIR(); 
    __LDSR( INTCFG, v);
    __RIR( __t);
}

#if defined( __RH850G3M__) | defined( __RH850G4MH__)
/* 
 * RH850 G3M related handling 
 */


/* FPU Operation Registers */
/*      Name       Reg, Group       Comment */
#define FPSR         6, 0        /* (See 3.4 FPU Function Registers) CU0 and SV */
#define FPEPC        7, 0        /* (See 3.4 FPU Function Registers) CU0 and SV */
#define FPST         8, 0        /* (See 3.4 FPU Function Registers) CU0 */
#define FPCC         9, 0        /* (See 3.4 FPU Function Registers) CU0 */
#define FPCFG       10, 0        /* (See 3.4 FPU Function Registers) CU0 */
#define FPEC        11, 0        /* (See 3.4 FPU Function Registers) CU0 and SV */

/* Cache Operation Function Registers */
/*      Name       Reg, Group       Comment */
#define ICTAGL      16, 4        /* Instruction cache tag Lo access SV */
#define ICTAGH      17, 4        /* Instruction cache tag Hi access SV */
#define ICDATL      18, 4        /* Instruction cache data Lo access SV */
#define ICDATH      19, 4        /* Instruction cache data Hi access SV */
#define ICCTRL      24, 4        /* Instruction cache control SV */
#define ICCFG       26, 4        /* Instruction cache configuration SV */
#define ICERR       28, 4        /* Instruction cache error SV */

#define CDBCR       24,13        /* Data buffer control register */

_INLINE_ void SET_FPSR(u32_T v) { __LDSR( FPSR ,v); __SYNCP();}
_INLINE_ void SET_FPEP(u32_T v) { __LDSR( FPEPC,v); __SYNCP();}
_INLINE_ void SET_FPST(u32_T v) { __LDSR( FPST ,v); __SYNCP();}
_INLINE_ void SET_FPCC(u32_T v) { __LDSR( FPCC ,v); __SYNCP();}
_INLINE_ void SET_FPCF(u32_T v) { __LDSR( FPCFG,v); __SYNCP();}
_INLINE_ void SET_FPEC(u32_T v) { __LDSR( FPEC ,v); __SYNCP();}

_INLINE_ void SET_PSW_ENABLEFPU( void)
{ 
    PSW_T p;
    p.psw= __STSR( PSW); 
    p.CU0= 1; 
    __LDSR( PSW, p.psw); 
    __SYNCI(); 
}

_INLINE_ void SET_FPSR_PEM( void)
{ 
    FPSR_T p;
    __SYNCP(); 
    p.fpsr= __STSR( FPSR); 
    p.PEM= 1; 
    __LDSR( FPSR, p.fpsr); 
    __SYNCP(); 
}

_INLINE_ void CLR_FPSR_PEM( void)
{ 
    FPSR_T p;
    __SYNCP(); 
    p.fpsr= __STSR( FPSR); 
    p.PEM= 0; 
    __LDSR( FPSR, p.fpsr); 
    __SYNCP(); 
}

#if defined( __RH850G3M__)
/* G3M and later starts counting from 1 to 7 */
_INLINE_ u32_T GetPEID( void)
{
	u32_T	__t;
	__t= (__STSR( 0, 2) >>16) & 0x7;
	return __t;
}
#endif  /* __RH850G3M__ */
#endif  /* __RH850G3M__, RH850G4MH */

#ifdef __RH850G4MH__
/*
 * Register bank definitions (maximum values) 
 */
#define MAX_N_OF_RNB    (16)
#ifdef RB_MODE
 #if (RB_MODE==1)
  #define MAX_RNBREGS   (0x60)
 #endif 
#else
#define RB_MODE         (1)
 #define MAX_RNBREGS    (0x90)
#endif

_INLINE_ u32_T GetPEID( void);

/* G4MH and later starts counting from 0 to 7 */
_INLINE_ u32_T GetPEID( void)
{
	u32_T	__t;
	__t= __STSR( 0, 2) & 0x7;
	return __t;
}

/* For bothn revisions of the core ... */
_INLINE_ void  SET_TSCOUNTL( u32_T v) {        __LDSR( TSCOUNTL, v); }
_INLINE_ void  SET_TSCOUNTH( u32_T v) {        __LDSR( TSCOUNTH, v); }
_INLINE_ void  SET_TSCTRL( void   )   {        __LDSR( TSCTRL, 1); }
_INLINE_ void  CLR_TSCTRL( void   )   {        __LDSR( TSCTRL, 0); }
_INLINE_ u32_T GET_TSCTRL( u32_T v)   { return __STSR( TSCTRL) & 0x3; }
_INLINE_ u32_T GET_TSCOUNTL( void)    { return __STSR( TSCOUNTL); }
_INLINE_ u32_T GET_TSCOUNTH( void)    { return __STSR( TSCOUNTH); }

#ifdef __E2X__
_INLINE_ void ENA_PMCTRL0( void)      {        __LDSR( PMCTRL0, __STSR( PMCTRL0)|1); }
_INLINE_ void ENA_PMCTRL1( void)      {        __LDSR( PMCTRL1, __STSR( PMCTRL1)|1); }
_INLINE_ void ENA_PMCTRL2( void)      {        __LDSR( PMCTRL2, __STSR( PMCTRL2)|1); }
_INLINE_ void ENA_PMCTRL3( void)      {        __LDSR( PMCTRL3, __STSR( PMCTRL3)|1); }
_INLINE_ void DIS_PMCTRL0( void)      {        __LDSR( PMCTRL0, __STSR( PMCTRL0) & 0xfffe); }
_INLINE_ void DIS_PMCTRL1( void)      {        __LDSR( PMCTRL1, __STSR( PMCTRL1) & 0xfffe); }
_INLINE_ void DIS_PMCTRL2( void)      {        __LDSR( PMCTRL2, __STSR( PMCTRL2) & 0xfffe); }
_INLINE_ void DIS_PMCTRL3( void)      {        __LDSR( PMCTRL3, __STSR( PMCTRL3) & 0xfffe); }
_INLINE_ void SET_PMCTRL0( u32_T v)   {        __LDSR( PMCTRL0, ((v&0xff)<<8)); }
_INLINE_ void SET_PMCTRL1( u32_T v)   {        __LDSR( PMCTRL1, ((v&0xff)<<8)); }
_INLINE_ void SET_PMCTRL2( u32_T v)   {        __LDSR( PMCTRL2, ((v&0xff)<<8)); }
_INLINE_ void SET_PMCTRL3( u32_T v)   {        __LDSR( PMCTRL3, ((v&0xff)<<8)); }
_INLINE_ u32_T GET_PMCTRL0( void)     { return __STSR( PMCTRL0); }
_INLINE_ u32_T GET_PMCTRL1( void)     { return __STSR( PMCTRL1); }
_INLINE_ u32_T GET_PMCTRL2( void)     { return __STSR( PMCTRL2); }
_INLINE_ u32_T GET_PMCTRL3( void)     { return __STSR( PMCTRL3); }
_INLINE_ u32_T GET_PMCOUNT0( void)    { return __STSR( PMCOUNT0); }
_INLINE_ u32_T GET_PMCOUNT1( void)    { return __STSR( PMCOUNT1); }
_INLINE_ u32_T GET_PMCOUNT2( void)    { return __STSR( PMCOUNT2); }
_INLINE_ u32_T GET_PMCOUNT3( void)    { return __STSR( PMCOUNT3); }
#endif

#ifdef __U2B__
_INLINE_ u32_T GET_PMCTRL0( void)     { return __STSR(PMCTRL0);}
_INLINE_ u32_T GET_PMCTRL1( void)     { return __STSR(PMCTRL1);}
_INLINE_ u32_T GET_PMCTRL2( void)     { return __STSR(PMCTRL2);}
_INLINE_ u32_T GET_PMCTRL3( void)     { return __STSR(PMCTRL3);}
_INLINE_ u32_T GET_PMCTRL4( void)     { return __STSR(PMCTRL4);}
_INLINE_ u32_T GET_PMCTRL5( void)     { return __STSR(PMCTRL5);}
_INLINE_ u32_T GET_PMCTRL6( void)     { return __STSR(PMCTRL6);}
_INLINE_ u32_T GET_PMCTRL7( void)     { return __STSR(PMCTRL7);}

_INLINE_ u32_T GET_PMCOUNT0( void)     { return __STSR(PMCOUNT0);}
_INLINE_ u32_T GET_PMCOUNT1( void)     { return __STSR(PMCOUNT1);}
_INLINE_ u32_T GET_PMCOUNT2( void)     { return __STSR(PMCOUNT2);}
_INLINE_ u32_T GET_PMCOUNT3( void)     { return __STSR(PMCOUNT3);}
_INLINE_ u32_T GET_PMCOUNT4( void)     { return __STSR(PMCOUNT4);}
_INLINE_ u32_T GET_PMCOUNT5( void)     { return __STSR(PMCOUNT5);}
_INLINE_ u32_T GET_PMCOUNT6( void)     { return __STSR(PMCOUNT6);}
_INLINE_ u32_T GET_PMCOUNT7( void)     { return __STSR(PMCOUNT7);}

_INLINE_ void SET_PMCTRL0( u32_T v)   { __LDSR(PMCTRL0, v);}
_INLINE_ void SET_PMCTRL1( u32_T v)   { __LDSR(PMCTRL1, v);}
_INLINE_ void SET_PMCTRL2( u32_T v)   { __LDSR(PMCTRL2, v);}
_INLINE_ void SET_PMCTRL3( u32_T v)   { __LDSR(PMCTRL3, v);}
_INLINE_ void SET_PMCTRL4( u32_T v)   { __LDSR(PMCTRL4, v);}
_INLINE_ void SET_PMCTRL5( u32_T v)   { __LDSR(PMCTRL5, v);}
_INLINE_ void SET_PMCTRL6( u32_T v)   { __LDSR(PMCTRL6, v);}
_INLINE_ void SET_PMCTRL7( u32_T v)   { __LDSR(PMCTRL7, v);}

_INLINE_ void ENA_PMCTRL0( void)      { __LDSR( PMCTRL0, __STSR( PMCTRL0) | 1); }
_INLINE_ void ENA_PMCTRL1( void)      { __LDSR( PMCTRL1, __STSR( PMCTRL1) | 1); }
_INLINE_ void ENA_PMCTRL2( void)      { __LDSR( PMCTRL2, __STSR( PMCTRL2) | 1); }
_INLINE_ void ENA_PMCTRL3( void)      { __LDSR( PMCTRL3, __STSR( PMCTRL3) | 1); }
_INLINE_ void ENA_PMCTRL4( void)      { __LDSR( PMCTRL4, __STSR( PMCTRL4) | 1); }
_INLINE_ void ENA_PMCTRL5( void)      { __LDSR( PMCTRL5, __STSR( PMCTRL5) | 1); }
_INLINE_ void ENA_PMCTRL6( void)      { __LDSR( PMCTRL6, __STSR( PMCTRL6) | 1); }
_INLINE_ void ENA_PMCTRL7( void)      { __LDSR( PMCTRL7, __STSR( PMCTRL7) | 1); }

_INLINE_ void DIS_PMCTRL0( void)      { __LDSR( PMCTRL0, __STSR( PMCTRL0) & 0xfffe); }
_INLINE_ void DIS_PMCTRL1( void)      { __LDSR( PMCTRL1, __STSR( PMCTRL1) & 0xfffe); }
_INLINE_ void DIS_PMCTRL2( void)      { __LDSR( PMCTRL2, __STSR( PMCTRL2) & 0xfffe); }
_INLINE_ void DIS_PMCTRL3( void)      { __LDSR( PMCTRL3, __STSR( PMCTRL3) & 0xfffe); }
_INLINE_ void DIS_PMCTRL4( void)      { __LDSR( PMCTRL4, __STSR( PMCTRL4) & 0xfffe); }
_INLINE_ void DIS_PMCTRL5( void)      { __LDSR( PMCTRL5, __STSR( PMCTRL5) & 0xfffe); }
_INLINE_ void DIS_PMCTRL6( void)      { __LDSR( PMCTRL6, __STSR( PMCTRL6) & 0xfffe); }
_INLINE_ void DIS_PMCTRL7( void)      { __LDSR( PMCTRL7, __STSR( PMCTRL7) & 0xfffe); }

_INLINE_ void CLR_PMCOUNT0( void)     { __LDSR( PMCOUNT0, 0); }
_INLINE_ void CLR_PMCOUNT1( void)     { __LDSR( PMCOUNT1, 0); }
_INLINE_ void CLR_PMCOUNT2( void)     { __LDSR( PMCOUNT2, 0); }
_INLINE_ void CLR_PMCOUNT3( void)     { __LDSR( PMCOUNT3, 0); }
_INLINE_ void CLR_PMCOUNT4( void)     { __LDSR( PMCOUNT4, 0); }
_INLINE_ void CLR_PMCOUNT5( void)     { __LDSR( PMCOUNT5, 0); }
_INLINE_ void CLR_PMCOUNT6( void)     { __LDSR( PMCOUNT6, 0); }
_INLINE_ void CLR_PMCOUNT7( void)     { __LDSR( PMCOUNT7, 0); }

_INLINE_ u32_T GET_HVCFG ( void)     { return __STSR( HVCFG );}
_INLINE_ u32_T GET_GMCFG ( void)     { return __STSR( GMCFG );}
_INLINE_ u32_T GET_PSWH  ( void)     { return __STSR( PSWH  );}
_INLINE_ u32_T GET_FEPSWH( void)     { return __STSR( FEPSWH);}
_INLINE_ u32_T GET_EIPSWH( void)     { return __STSR( EIPSWH);}
_INLINE_ u32_T GET_HVSB  ( void)     { return __STSR( HVSB  );}
_INLINE_ u32_T GET_PMUMCTRL( void)   { return __STSR( PMUMCTRL); }
_INLINE_ u32_T GET_PMGMCTRL( void)   { return __STSR( PMGMCTRL); }
_INLINE_ u32_T GET_GMEBASE( void)    { return __STSR( GMEBASE); }

_INLINE_ void SET_HVCFG ( u32_T v)   { __LDSR( HVCFG , v);}
_INLINE_ void SET_GMCFG ( u32_T v)   { __LDSR( GMCFG , v);}
_INLINE_ void SET_PSWH  ( u32_T v)   { __LDSR( PSWH  , v);}
_INLINE_ void SET_FEPSWH( u32_T v)   { __LDSR( FEPSWH, v);}
_INLINE_ void SET_EIPSWH( u32_T v)   { __LDSR( EIPSWH, v);}
_INLINE_ void SET_HVSB  ( u32_T v)   { __LDSR( HVSB  , v);}
_INLINE_ void SET_PMUMCTRL( u32_T v) { __LDSR( PMUMCTRL, __STSR( PMUMCTRL) | v); }
_INLINE_ void SET_PMGMCTRL( u32_T v) { __LDSR( PMGMCTRL, __STSR( PMGMCTRL) | v); }

_INLINE_ void DIS_PMUMCTRL( u32_T v) { __LDSR( PMUMCTRL, __STSR( PMUMCTRL) & ~v); }
_INLINE_ void DIS_PMGMCTRL( u32_T v) { __LDSR( PMGMCTRL, __STSR( PMGMCTRL) & ~v); }

_INLINE_ void SET_GMEBASE_ADDR( u32_T v)
{
    GMEBASE_T __t;
    __t.gmebase= GET_GMEBASE();
    __t.ebase= v;
    __LDSR( GMEBASE, __t.gmebase);
}

_INLINE_ void SET_EIPSWH_GM( void)
{ 
    PSWH_T p;
    p.GM= 1;
    __LDSR( EIPSWH, p.pswh); 
    __SYNCI(); 
}

_INLINE_ void SET_EIPC( u32_T a)
{ 
    __LDSR( EIPC, a); 
    __SYNCI(); 
}


#endif  /* __U2B__ */

#define CLR_PMCOUNT(n)	CLR_PMCOUNT##n()
#define GET_PMCOUNT(n)	GET_PMCOUNT##n()
#define GET_PMCTRL(n)	GET_PMCTRL##n()
#define DIS_PMCTRL(n)	DIS_PMCTRL##n()
#define ENA_PMCTRL(n)	ENA_PMCTRL##n()
#define SET_PMCTRL(n)	SET_PMCTRL##n(x)


_INLINE_ u32_T GET_SPID( void)         { return __STSR( SPID);}
_INLINE_ void SET_SPID( u32_T v)       { __LDSR( SPID, v); __SYNCI();}
                                       
_INLINE_ void ENA_MPE      ( void)     { __LDSR( MPM   , 1); __SYNCI();}
_INLINE_ void DIS_MPE      ( void)     { __LDSR( MPM, __STSR( MPM) & 0xfffe); }
_INLINE_ void ENA_MPESVP   ( void)     { __LDSR( MPM   , 3); __SYNCI();}
_INLINE_ void DIS_MPESVP   ( void)     { __LDSR( MPM, __STSR( MPM) & 0xfffc); }

_INLINE_ void SET_MPID0( u32_T v)      { __LDSR( MPID0, v); __SYNCI();}
_INLINE_ void SET_MPID1( u32_T v)      { __LDSR( MPID1, v); __SYNCI();}
_INLINE_ void SET_MPID2( u32_T v)      { __LDSR( MPID2, v); __SYNCI();}
_INLINE_ void SET_MPID3( u32_T v)      { __LDSR( MPID3, v); __SYNCI();}
_INLINE_ void SET_MPID4( u32_T v)      { __LDSR( MPID4, v); __SYNCI();}
_INLINE_ void SET_MPID5( u32_T v)      { __LDSR( MPID5, v); __SYNCI();}
_INLINE_ void SET_MPID6( u32_T v)      { __LDSR( MPID6, v); __SYNCI();}
_INLINE_ void SET_MPID7( u32_T v)      { __LDSR( MPID7, v); __SYNCI();}
_INLINE_ void SET_MPIDX( u32_T v)      { __LDSR( MPIDX, v); __SYNCI();}
_INLINE_ void SET_MPLA( u32_T v)       { __LDSR( MPLA , v); __SYNCI();}
_INLINE_ void SET_MPUA( u32_T v)       { __LDSR( MPUA , v); __SYNCI();}
_INLINE_ void SET_MPAT( u32_T v)       { __LDSR( MPAT , v);}

_INLINE_ u32_T GET_MPLA( void)         { return __STSR( MPLA);}
_INLINE_ u32_T GET_MPUA( void)         { return __STSR( MPUA);}
_INLINE_ u32_T GET_MPAT( void)         { return __STSR( MPAT);}
                                       
_INLINE_ u32_T GET_PSW ( void)         { return __STSR( PSW );}
_INLINE_ void SET_PSW( u32_T v)        { __LDSR( EIPSW, v);}
_INLINE_ void SET_EIPSW( u32_T v)      { __LDSR( EIPSW, v);}
_INLINE_ void SET_FEPSW( u32_T v)      { __LDSR( FEPSW, v);}

_INLINE_ void  SET_TSCOUNT( u64_T v)
{
    __LDSR( TSCOUNTL, v.l);
    __LDSR( TSCOUNTH, v.h);
}

_INLINE_ void  CLR_TSCOUNT( void)
{
    __LDSR( TSCOUNTL, 0ul);
    __LDSR( TSCOUNTH, 0ul);
}

_INLINE_ u64_T GET_TSCOUNT( void)
{
    u64_T   __t;
    __t.h= __STSR( TSCOUNTH);
    __t.l= __STSR( TSCOUNTL);
    return __t;
}

#endif /* __RH850G4MH__ */

#ifdef __ghs
#pragma ghs endnomisra
#endif  /* __ghs */
#endif
