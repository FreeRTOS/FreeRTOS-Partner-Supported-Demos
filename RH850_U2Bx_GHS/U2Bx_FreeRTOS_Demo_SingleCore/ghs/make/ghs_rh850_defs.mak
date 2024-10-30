################################################################################
# INTERNAL REQUIRED CONFIGURATION
#
# COMPILER_INSTALL_DIR
#

################################################################################
# REQUIRED (in base_make)
#
# CC
# LINKER
# DBLINKER
# CONVERTER
# GHS Compiler Driver for compiling source files
CC="$(COMPILER_INSTALL_DIR)\ccrh850.exe"

# GHS Compiler Driver for assembling the startup files
ASM="$(COMPILER_INSTALL_DIR)\ccrh850.exe"

# GHS Compiler Driver for linking the object files and startup code
LINKER="$(COMPILER_INSTALL_DIR)\ccrh850.exe"

# GHS Linker for linking the object files without startup code
DBLINKER="$(COMPILER_INSTALL_DIR)\elxr.exe"

# GHS Code converter to generate motorola S-Record file
CONVERTER="$(COMPILER_INSTALL_DIR)\gsrec.exe"
################################################################################

################################################################################
# Compiler Flag
CFLAGS= -c -g -cpu=rh850$(CPU_CORE) -rh850_fxu -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -additional_sda_reg=0 -rh850_abi=ghs2014 -MMD -G -no_rosda
################################################################################

################################################################################
# Assembler Flag
AFLAGS= -c -g -cpu=rh850$(CPU_CORE) -rh850_fxu -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -additional_sda_reg=0 -rh850_abi=ghs2014 -MMD -G -no_rosda
################################################################################

################################################################################
# Linker Flag
LFLAGS= -g -cpu=rh850$(CPU_CORE) -rh850_fxu -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -entry=_RESET_VECTOR -additional_sda_reg=0 -rh850_abi=ghs2014 -G -no_rosda
################################################################################

################################################################################
SFLAGS= -S3
################################################################################

ASM_FILE_SUFFIX = 850
OBJ_FILE_SUFFIX = o
ASM_OBJ_FILE_SUFFIX = ao
LST_FILE_SUFFIX = lst
PRE_FILE_SUFFIX = pre
MAP_FILE_SUFFIX = map
S_RECORD_SUFFIX = s37
EXE_FILE_SUFFIX = out
C_FILE_SUFFIX = c

################################################################################
# REGISTRY
#
CPP_INCLUDE_PATH +=
ASM_INCLUDE_PATH +=

################################################################################
# Directory to store the Assembly List file and the Preprocessor file          #
################################################################################
export TEXTPATH=$(OBJECT_OUTPUT_PATH)
export OBJPATH=$(OBJECT_OUTPUT_PATH)

################################################################################
