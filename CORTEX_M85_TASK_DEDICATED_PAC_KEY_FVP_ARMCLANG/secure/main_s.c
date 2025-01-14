/* Copyright 2023-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "SSE315.h"
#include "system_SSE315.h"
#include "platform_irq.h"
#include "core_cm85.h"
#include "bsp_serial.h"
#include <arm_cmse.h>
#include "RTE_Components.h"
#include CMSIS_device_header
#include "region_defs.h"
#include "secure_config.h"

/**
 * @brief Boots into the non-secure code.
 *
 */
static void prvBootNonSecure( void );

/*
 * This function pointer is meant to only hold non-secure function pointers.
 */
typedef void (*nsfptr_t)(void) __attribute__((cmse_nonsecure_call));

int main()
{
  bsp_serial_init();
  printf( "In secure world main ...\n" );
  /* As part of secure platform init, configure uart0 to non-secure accessible.
  */
  secure_platform_init();
  prvBootNonSecure();

  /* Non-secure software does not return, this code is not executed. */
  for( ; ; )
  {
  }
}

static void prvBootNonSecure( void )
{
  /* SCB_NS.VTOR points to the Non-secure vector table base address */
  SCB_NS->VTOR = (NS_CODE_START);

  /* Setup Main stack pointer of the non-secure code */
  uint32_t ns_msp = *(uint32_t *)(NS_CODE_START);
  __TZ_set_MSP_NS(ns_msp);

  /* The entry contains adddress of the Reset_handler (CMSIS-CORE) function */
  uintptr_t entry_ptr = *(uint32_t *)(NS_CODE_START + 4);

  /* Clear LSB of the function address to indicate the function-call
      will perform the switch from secure to non-secure */
  nsfptr_t ns_entry = (nsfptr_t)cmse_nsfptr_create(entry_ptr);

  /* All changes made to memory will be effective after this point */
  __DSB();
  __ISB();

  /* Call the non-secure Reset_Handler to jump to the non-secure binary */
  ns_entry();
}
