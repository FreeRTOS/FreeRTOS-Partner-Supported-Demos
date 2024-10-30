-------------------------------------------------------------------------------
-- Module      = coldreset.850
-- Version     = E1.0.0g
--               manually created
--
-------------------------------------------------------------------------------
--                                  COPYRIGHT
-------------------------------------------------------------------------------
-- Copyright (c) 2021 by Renesas Electronics Europe GmbH,
--               a company of the Renesas Electronics Corporation
-------------------------------------------------------------------------------
-- Purpose:      Startup Code
--
-------------------------------------------------------------------------------
--
-- Warranty Disclaimer
--
-- Because the Product(s) is licensed free of charge, there is no warranty
-- of any kind whatsoever and expressly disclaimed and excluded by Renesas,
-- either expressed or implied, including but not limited to those for
-- non-infringement of intellectual property, merchantability and/or
-- fitness for the particular purpose.
-- Renesas shall not have any obligation to maintain, service or provide bug
-- fixes for the supplied Product(s) and/or the Application.
--
-- Each User is solely responsible for determining the appropriateness of
-- using the Product(s) and assumes all risks associated with its exercise
-- of rights under this Agreement, including, but not limited to the risks
-- and costs of program errors, compliance with applicable laws, damage to
-- or loss of data, programs or equipment, and unavailability or
-- interruption of operations.
--
-- Limitation of Liability
--
-- In no event shall Renesas be liable to the User for any incidental,
-- consequential, indirect, or punitive damage (including but not limited
-- to lost profits) regardless of whether such liability is based on breach
-- of contract, tort, strict liability, breach of warranties, failure of
-- essential purpose or otherwise and even if advised of the possibility of
-- such damages. Renesas shall not be liable for any services or products
-- provided by third party vendors, developers or consultants identified or
-- referred to the User by Renesas in connection with the Product(s) and/or
-- the Application.
--
-------------------------------------------------------------------------------
-- Environment:
--              Device:         All RH850 Mutlicore
--              IDE:            GHS Multi V7.1.4 or later
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Revision Control History:
-- E1.0.0g :   23th-Aug-2021    : Initial Version
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-- Definition of PE number
-------------------------------------------------------------------------------
  .set PE0        0
  .set PE1        1
  .set PE2        2
  .set PE3        3

-------------------------------------------------------------------------------
-- Jump to input address if the PE number is correct
-------------------------------------------------------------------------------
.macro jmpIfPEn addr, pe
    stsr  0,    r1, 2
    andi  0x7,  r1, r1
    cmp   pe,   r1
    bne   2f
 1: mov   addr, r1
    jmp   [r1]
 2:
    .endm
