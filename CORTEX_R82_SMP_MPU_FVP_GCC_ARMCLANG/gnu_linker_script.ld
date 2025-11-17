/*
 * Copyright (c) 2021-2025 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Memory regions */
MEMORY
{
    ROM (rwx) : ORIGIN = 0x80000000, LENGTH = 4M /* System ROM */
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 256M /* System RAM */
}

/* Sections */
SECTIONS
{
    . = ORIGIN(ROM);  /* Place text at ROM base (0x8000_0000) */

    /* Code section */
    . = ALIGN(64);
    .privileged_functions : ALIGN(64)
    {
        __privileged_functions_start__ = .;
        KEEP(*(.vectors))  /* Vector table */
        KEEP(*(.init))
        KEEP(*(.fini))
        *(privileged_functions)
        . = ALIGN(64);
        __privileged_functions_end__ = .;
        /* Adding 64 bytes pad at the end of the region to avoid end address for . privileged_functions and
         * start address of .syscalls_flash sections to have different MPU region attributes with the same address.
         */
        . = . + 0x40;
    } > ROM

    .syscalls_flash : ALIGN(64)
    {
        __syscalls_flash_start__ = .;
        *(freertos_system_calls)
        . = ALIGN(64);
        __syscalls_flash_end__ = .;
    } > ROM

    .unprivileged_flash : ALIGN(64)
    {
        __unprivileged_flash_start__ = .;
        *(.text*)          /* Code */
        *(.rodata*)        /* Read-only data */
        . = ALIGN(64);
        __unprivileged_flash_end__ = .;
    } > ROM

    .privileged_sram :
    {
        __privileged_sram_start__ = .;
        __data_start__ = .;
        *(privileged_data)
        . = ALIGN(64);
        __privileged_sram_end__ = .;
    } > RAM AT> ROM /* load in ROM, run in RAM */
    _sidata = LOADADDR(.privileged_sram);

    .unprivileged_sram :
    {
        __unprivileged_sram_start__ = .;
        *(.data*)
        *(+RW)
        . = ALIGN(64);
        __unprivileged_sram_end__ = .;
        __data_end__ = .;
    } > RAM AT> ROM /* load in ROM, run in RAM */

    /* Uninitialized data (BSS) */
    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(64);
        __bss_end__ = .;
    } > RAM

    PROVIDE(end = __bss_end__); /* Alias for `_end`, commonly used in `_sbrk()` */

    /* Stack section */
    .stack (NOLOAD) : ALIGN(16)
    {
        . += 0x10000; /* 64KB stack */
    } > RAM

    /* Heap section (can be used for dynamic memory allocation) */
    .heap (NOLOAD) :
    {
        . += 0x20000; /* 128KB heap */
    } > RAM
}

/* Provide symbols for startup code */
PROVIDE(__el1_stack = ADDR(.stack));
PROVIDE(_el1_stack_end = ADDR(.stack) + SIZEOF(.stack));
