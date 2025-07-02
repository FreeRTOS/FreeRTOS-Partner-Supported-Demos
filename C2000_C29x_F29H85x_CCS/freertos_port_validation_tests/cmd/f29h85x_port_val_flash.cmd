--entry_point="code_start"


/* Specify the system memory map */

MEMORY
{
    SRAM_LDAx : o=0x200E0000, l=0x20000
    SRAM_LPAx : o=0x20100000, l=0x20000
    SRAM_CDAx : o=0x20120000, l=0x30000

    CERT      : o=0x10000000, l=0x001000
    FLASH_RP0 : o=0x10001000, l=0x0FF000
    FLASH_RP1 : o=0x10100000, l=0x100000
    FLASH_RP2 : o=0x10200000, l=0x100000
    FLASH_RP3 : o=0x10300000, l=0x100000
}


/* Specify the sections allocation into memory */

SECTIONS
{
    codestart     : {} > 0x10001000,    palign(8)
    cert          : {} > CERT,          palign(8)

    .text         : {} > FLASH_RP0,     palign(8)
    .binit        : {} > FLASH_RP0,     palign(8)
    .cinit        : {} > FLASH_RP0,     palign(8)
    .const        : {} > FLASH_RP0,     palign(8)
    .rodata       : {} > FLASH_RP0,     palign(8)
    .init_array   : {} > FLASH_RP0,     palign(8)

    .TI.ramfunc   : {} LOAD=FLASH_RP0, RUN=SRAM_LPAx, table(BINIT), palign(8)

    .data         : {} > SRAM_LDAx
    .bss          : {} > SRAM_CDAx
    .stack        : {} > SRAM_LDAx
    .sysmem       : {} > SRAM_CDAx
    .cio          : {} > SRAM_LDAx
}