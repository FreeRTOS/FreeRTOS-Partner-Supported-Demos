/*
 * Copyright (c) 2009-2024 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 *-------- <<< Use Configuration Wizard in Context Menu >>> -------------------
 */

/*---------------------- Flash Configuration ----------------------------------
  <h> Flash Configuration
    <o0> Flash Base Address <0x0-0xFFFFFFFF:8>
    <o1> Flash Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/

__ROM_BASE = 0x11000000;
__ROM_SIZE = 0x001FFFFF;

/*--------------------- Embedded RAM Configuration ----------------------------
  <h> RAM Configuration
    <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
    <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
 -----------------------------------------------------------------------------*/

__RAM_BASE = 0x31000000;
__RAM_SIZE = 0x003FFFFE;

/*--------------------- Stack / Heap Configuration ----------------------------
  <h> Stack / Heap Configuration
    <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
    <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__STACK_SIZE = 0x00001000;
__HEAP_SIZE  = 0x00080000;

/* Variables used by FreeRTOS-MPU. */
_Privileged_Functions_Region_Size = 64K;
_Privileged_Sram_Region_Size = 16K;

__privileged_functions_start__ = __ROM_BASE;
__privileged_functions_end__ = __privileged_functions_start__ + _Privileged_Functions_Region_Size;

__SRAM_segment_start__ = __RAM_BASE;
__SRAM_segment_end__ = __SRAM_segment_start__ + __RAM_SIZE;

__privileged_sram_start__ = __SRAM_segment_start__;
__privileged_sram_end__ = __privileged_sram_start__ + _Privileged_Sram_Region_Size;

/*
 *-------------------- <<< end of configuration section >>> -------------------
 */

MEMORY
{
  FLASH (rx)  : ORIGIN = __ROM_BASE,  LENGTH = __ROM_SIZE
  RAM   (rwx) : ORIGIN = __RAM_BASE,  LENGTH = __RAM_SIZE
}

ENTRY(Reset_Handler)

SECTIONS
{

  /* Privileged functions - Section needs to be 32 byte aligned to satisfy MPU requirements. */
  .privileged_functions : ALIGN(32)
  {
      KEEP(*(.vectors))
      . = ALIGN(32);
      __privileged_functions_start__ = .;
      *(privileged_functions)
      . = ALIGN(32);
      /* End address must be the last address in the region, therefore, -1. */
      __privileged_functions_end__ = . - 1;
  } > FLASH

  /* FreeRTOS System calls - Section needs to be 32 byte aligned to satisfy MPU requirements. */
  .freertos_system_calls : ALIGN(32)
  {
      . = ALIGN(32);
      __syscalls_flash_start__ = .;
      *(freertos_system_calls)
      . = ALIGN(32);
      /* End address must be the last address in the region, therefore, -1. */
      __syscalls_flash_end__ = . - 1;
  } > FLASH

  /* FreeRTOS System calls - Section needs to be 32 byte aligned to satisfy MPU requirements. */
  .unprivileged_functions : ALIGN(32)
  {
      . = ALIGN(32);
      __unprivileged_functions_start__ = .;
      *(unprivileged_functions)
      . = ALIGN(32);
  } > FLASH

  /* Main Text Section - Section needs to be 32 byte aligned to satisfy MPU requirements. */
  .text : ALIGN(32)
  {
    . = ALIGN(32);
    __unprivileged_flash_start__ = .;
    *(.text*)
    /*
      * These are the old initialisation sections, intended to contain
      * naked code, with the prologue/epilogue added by crti.o/crtn.o
      * when linking with startup files. The standalone startup code
      * currently does not run these, better use the init arrays below.
    */
    KEEP(*(.init))
    KEEP(*(.fini))
    KEEP(*(.eh_frame))

    __copy_table_start__ = .;
    LONG (LOADADDR(.data))
    LONG (ADDR(.data))
    LONG (SIZEOF(.data) / 4)
    __copy_table_end__ = .;

    /* .zero.table */
    . = ALIGN(4);
    __zero_table_start__ = .;
    LONG (ADDR(.bss))
    LONG (SIZEOF(.bss) / 4)
    __zero_table_end__ = .;

    *(.rodata*)

    . = ALIGN(32);
    /* End address must be the last address in the region, therefore, -1. */
    __unprivileged_flash_end__ = . - 1;
  } > FLASH

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;

  /**
   * Location counter can end up 2byte aligned with narrow Thumb code but
   * __etext is assumed by startup code to be the LMA of a section in RAM
   * which must be 4byte aligned
   */
  __etext = ALIGN (4);

  .data : AT (__etext)
  {
    __data_start__ = .;

    /* Privileged data - It needs to be 32 byte aligned to satisfy MPU requirements. */
    . = ALIGN(32);
    __privileged_sram_start__ = .;
    *(privileged_data)
    . = ALIGN(32);
    /* End address must be the last address in the region, therefore, -1. */
    __privileged_sram_end__ = . - 1;

    *(vtable)
    *(.data)
    *(.data.*)

    . = ALIGN(4);
    *(tasks_share)
    /* All data end */

    __data_end__ = .;
  } > RAM

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM AT > RAM

  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > RAM
  PROVIDE(__RAM_segment_used_end__ = __HeapLimit);

  .stack (ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __StackLimit = .;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
    __StackTop = .;
  } > RAM
  PROVIDE(__stack = __StackTop);

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
