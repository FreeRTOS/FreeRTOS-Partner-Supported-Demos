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
    /* ROM is device flash mapped at 0x8000_0000 on FVP; RAM begins at 0x0 */
    ROM (rx)  : ORIGIN = 0x80000000, LENGTH = 4M   /* Flash memory (ROM) */
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 256M /* System RAM */
}

/* Sections */
SECTIONS
{
    . = ORIGIN(ROM);  /* Place text at ROM base (0x8000_0000) */

    /* Code section */
    . = ALIGN(8);
    .text : ALIGN(8)
    {
        KEEP(*(.vectors))  /* Vector table */
        *(.text*)          /* Code */
        *(.rodata*)        /* Read-only data */
        KEEP(*(.init))
        KEEP(*(.fini))

        . = ALIGN(8);
    } > ROM

    /* Initialized data section */
    .data :
    {
        __data_start__ = .;
        *(.data*)
        . = ALIGN(8);
        __data_end__ = .;
    } > RAM AT> ROM /* load in ROM, run in RAM */
    _sidata = LOADADDR(.data);

    /* Uninitialized data (BSS) */
    .bss :
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(8);
        __bss_end__ = .;
    } > RAM

    /* Define the end of allocated memory for `_sbrk()` */
    _end = .; /* Marks the end of static memory */
    PROVIDE(end = .); /* Alias for `_end`, commonly used in `_sbrk()` */

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
