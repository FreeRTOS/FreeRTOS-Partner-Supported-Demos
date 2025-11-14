################################################################################
# Common part of the Makefile to compile and build the sample application      #
# Software Component (For Test purposes only)                                  #
# Compatible with GNU Make 3.81 for Win32.                                     #
################################################################################

################################################################################
# Subroutine to convert Module Name To All Upper Case for Naming convention.
uc = $(subst a,A,$(subst b,B,$(subst c,C,$(subst d,D,$(subst e,E,$(subst f,F,$(subst g,G,$(subst h,H,$(subst i,I,$(subst j,J,$(subst k,K,$(subst l,L,$(subst m,M,$(subst n,N,$(subst o,O,$(subst p,P,$(subst q,Q,$(subst r,R,$(subst s,S,$(subst t,T,$(subst u,U,$(subst v,V,$(subst w,W,$(subst x,X,$(subst y,Y,$(subst z,Z,$1))))))))))))))))))))))))))
################################################################################

################################################################################
# Subroutine to convert Module Name To Lower Case
#
lc = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))
################################################################################

# Get name of the current application
#
CURRENT_APPL = App_FreeRTOS

# Get the project directory into variable "PROJECT_ROOT"
PROJECT_ROOT := $(subst /,\,$(abspath ../../../../..))

# Get the current working directory into variable "SPECIFIC_APPL_ROOT_PATH"
SPECIFIC_APPL_ROOT_PATH := $(subst /,\,$(abspath .))

# Set the Cpu Core Based on Variant (g4mh):
CPU_CORE = g4mh
################################################################################

################################################################################
# Macros for common path
#
EXE = $(CURRENT_APPL).$(EXE_FILE_SUFFIX)

# Get the common sample application directory into variable "COMMON_SAMPLE_CORE_PATH"
COMMON_SAMPLE_CORE_PATH =$(SPECIFIC_APPL_ROOT_PATH)\

################################################################################
# Maros for build
#
LIBRARIES_TO_BUILD =

OBJECT_OUTPUT_PATH = $(SPECIFIC_APPL_ROOT_PATH)\obj\ghs

CC_FILES_TO_BUILD =
ASM_FILES_TO_BUILD =
CC_SRC_PATH =
CC_INCLUDE_PATH =
CPP_INCLUDE_PATH =
ASM_INCLUDE_PATH =
MAKE_CLEAN_RULES =
MAKE_COMPILE_RULES =
MAKE_DEBUG_RULES =

MAKE_DEBUG_RULES += debug_base_make

.PHONY: $(MAKE_CLEAN_RULES) $(MAKE_COMPILE_RULES) \
	$(MAKE_DEBUG_RULES) \
################################################################################

################################################################################
# GHS Compiler for rh850
#

# ghs defs
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
CFLAGS= -c -g -cpu=rh850$(CPU_CORE) -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -additional_sda_reg=0 -rh850_abi=ghs2014 -MMD -G -no_rosda
################################################################################

################################################################################
# Assembler Flag
AFLAGS= -c -g -cpu=rh850$(CPU_CORE) -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -additional_sda_reg=0 -rh850_abi=ghs2014 -MMD -G -no_rosda
################################################################################

################################################################################
# Linker Flag
LFLAGS= -g -cpu=rh850$(CPU_CORE) -gsize -prepare_dispose -inline_prologue -sda=all -passsource -Wundef -no_callt -reserve_r2 --short_enum --prototype_errors --diag_error 193 -dual_debug -large_sda --no_commons -shorten_loads -shorten_moves -Wshadow -ignore_callt_state_in_interrupts -delete -entry=_RESET_VECTOR -additional_sda_reg=0 -rh850_abi=ghs2014 -G -no_rosda
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
# End of ghs defs

# ghs rules
#######################################################################
# REGISTRY
#
CC_FILES_TO_BUILD +=

MAKE_DEBUG_RULES += debug_ghs_makefile

#######################################################################
# Command to print debug information                                  #
#######################################################################
debug_ghs_makefile:
	@echo COMPILER_INSTALL_DIR = $(COMPILER_INSTALL_DIR)
	@echo CC = $(CC)
	@echo CFLAGS = $(CFLAGS)
	@echo MAKELIB = $(MAKELIB)

#######################################################################
#end of ghs rules



###############################################################################
# CPU_Option
#
ifeq ($(CPU0_OPTION),CPU0_USED)
AFLAGS += -DPE0_USED
endif

ifeq ($(CPU1_OPTION),CPU1_USED)
AFLAGS += -DPE1_USED
CFLAGS += -DRUN_PE1
CFLAGS += -DRUN_OTHER_PE
endif

###############################################################################
# Max CPU Flag
#
ifeq ($(TOTAL_CPU),2)
AFLAGS += -DMAX_PE1
endif
################################################################################

################################################################################
# Modules to be included in the project                                        #
################################################################################
# Include make file for module
include $(SPECIFIC_APPL_ROOT_PATH)\ghs\make\FreeRTOS_Common_Sample.mak

################################################################################
# Path to Linker Directives Files
#
LNKFILE = $(SPECIFIC_APPL_ROOT_PATH)\linker\App_U2A_Sample.ld
LNKFILE_COMMON = $(SPECIFIC_APPL_ROOT_PATH)\linker\App_Common_Sample.ld

################################################################################
# Include generated source files list to the global "C" file list              #
################################################################################
#
ASM_INCLUDE_PATH +=
ASM_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\startup\include

CC_INCLUDE_PATH +=
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\common
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\freertos_config
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\include
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\include
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\ghs\include
CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\src

CC_SRC_PATH +=
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\startup
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\src
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source
CC_SRC_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\MemMang

CPP_INCLUDE_PATH +=
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\src
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\common
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\freertos_config
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\include
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\include

ASM_FILES_TO_BUILD  += \
                 $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x\portasm.850 \
                 $(SPECIFIC_APPL_ROOT_PATH)\startup\coldreset.850 \
                 $(SPECIFIC_APPL_ROOT_PATH)\startup\U2Ax_startup_PE0.850 \
                 $(SPECIFIC_APPL_ROOT_PATH)\startup\U2Ax_startup_PEn.850 \
                 $(SPECIFIC_APPL_ROOT_PATH)\src\RegTest.850
###############################################################################
# REGISTRY device sample app
#

ASM_FILES_TO_BUILD	+=
OBJECTS_LINK_ONLY +=
MAKE_CLEAN_RULES +=
MAKE_DEBUG_RULES += debug_common_appl_makefile
###############################################################################
# Command to print debug information
debug_common_appl_makefile:
	@echo COMMON_SAMPLE_CORE_PATH = $(COMMON_SAMPLE_CORE_PATH)
################################################################################
# End of Include sample app source files list to the global "C" file list      #
################################################################################

################################################################################
# Search path for source and header files (For Make utility)                   #
################################################################################
VPATH=$(CC_SRC_PATH) $(CC_INCLUDE_PATH) $(OBJECT_OUTPUT_PATH) $(ASM_INCLUDE_PATH)

################################################################################
# Create list of object files from source files                                #
################################################################################
OBJECT_FILES_BUILT := $(subst \,/, $(CC_FILES_TO_BUILD))
OBJECT_FILES_BUILT := $(subst :,/, $(OBJECT_FILES_BUILT))
OBJECT_FILES_BUILT := $(notdir $(OBJECT_FILES_BUILT))
OBJECT_FILES_BUILT := $(subst .$(C_FILE_SUFFIX),.$(OBJ_FILE_SUFFIX), $(OBJECT_FILES_BUILT))
OBJECT_PATH_FILES_BUILT := $(addprefix $(OBJECT_OUTPUT_PATH)\, $(OBJECT_FILES_BUILT))

################################################################################
# Create list of object files from assembly files                              #
################################################################################
ASM_OBJECT_FILES_BUILT := $(subst \,/, $(ASM_FILES_TO_BUILD))
ASM_OBJECT_FILES_BUILT := $(subst :,/, $(ASM_OBJECT_FILES_BUILT))
ASM_OBJECT_FILES_BUILT := $(notdir $(ASM_OBJECT_FILES_BUILT))
ASM_OBJECT_FILES_BUILT :=  \
                  $(subst .$(ASM_FILE_SUFFIX),.$(ASM_OBJ_FILE_SUFFIX), $(ASM_OBJECT_FILES_BUILT))
ASM_OBJECT_PATH_FILES_BUILT := $(ASM_OBJECT_FILES_BUILT)

ASM_OBJECT_PATH_FILES_BUILT := \
                  $(addprefix $(OBJECT_OUTPUT_PATH)\, $(ASM_OBJECT_FILES_BUILT))

OBJECT_PATH_FILES_BUILT += $(ASM_OBJECT_PATH_FILES_BUILT)

################################################################################
# Include auto-generated dependencies                                          #
################################################################################
ifeq ($(findstring clean,$(MAKECMDGOALS)),)
OBJECT_DEPS := $(OBJECT_PATH_FILES_BUILT)
OBJECT_DEPS := $(OBJECT_DEPS:.$(OBJ_FILE_SUFFIX)=.d)
OBJECT_DEPS := $(OBJECT_DEPS:.$(ASM_OBJ_FILE_SUFFIX)=.d)
-include $(OBJECT_DEPS)
endif

################################################################################
# Remove the path from Source file names                                       #
################################################################################
CC_FILES_BUILT := $(subst \,/, $(CC_FILES_TO_BUILD))
CC_FILES_BUILT := $(subst :,/, $(CC_FILES_BUILT))
CC_FILES_BUILT := $(notdir $(CC_FILES_BUILT))

################################################################################
# Remove the path from Source file names                                       #
################################################################################
ASM_FILES_BUILT := $(subst \,/, $(ASM_FILES_TO_BUILD))
ASM_FILES_BUILT := $(subst :,/, $(ASM_FILES_BUILT))
ASM_FILES_BUILT := $(notdir $(ASM_FILES_BUILT))

################################################################################
# Prefix -I to all include paths (required for compiler)                       #
################################################################################
COMPILER_INCLUDE := $(patsubst %,-I%,$(CC_INCLUDE_PATH))
ASSEMBLER_INCLUDE := $(patsubst %,-I%,$(ASM_INCLUDE_PATH))
################################################################################
# Command to link all the generated object files and generate exe/S-Record     #
################################################################################
$(EXE):$(OBJECT_FILES_BUILT) $(ASM_OBJECT_FILES_BUILT) $(LNKFILE) $(LNKFILE_COMMON)
	@echo *********************************************************************
	@echo Building the executable ...
	$(LINKER) $(LNKFILE) $(LNKFILE_COMMON) $(LFLAGS) $(OBJECT_PATH_FILES_BUILT) \
	$(OBJECTS_LINK_ONLY) $(STD_LIBRARY) \
	-map="$(OBJECT_OUTPUT_PATH)\$(CURRENT_APPL).$(MAP_FILE_SUFFIX)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(EXE)"
	@echo Generating Motorola S-Record file ...
	$(CONVERTER) $(SFLAGS) "$(OBJECT_OUTPUT_PATH)\$(EXE)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(CURRENT_APPL_SRECORD).$(S_RECORD_SUFFIX)"
	@echo Done ...


################################################################################
# Command to generate object files from all dependent assembly files           #
################################################################################
%.ao : %.850
	@echo *********************************************************************
	@echo Assembling $< ...
	@echo Generating $(OBJECT_OUTPUT_PATH)\$@ ...
	$(ASM) $(AFLAGS) $(ASSEMBLER_INCLUDE) $(COMPILER_INCLUDE) $< -o $(OBJECT_OUTPUT_PATH)\$@
	@echo Done ...

################################################################################
# Command to generate object files from all dependent source files             #
################################################################################
#
%.o : %.c
	@echo *********************************************************************
	@echo Compiling $< ...
	@echo Generating $(OBJECT_OUTPUT_PATH)\$@ ...
	$(CC) $(CFLAGS) $(COMPILER_INCLUDE) $< -object_dir=$(OBJECT_OUTPUT_PATH)
	@echo Done ...

################################################################################
# Command to clean the generated files                                         #
################################################################################
clean: $(MAKE_CLEAN_RULES)

################################################################################
# Command to clean the generated object files                                  #
################################################################################
clean_objs:
	@del /Q $(OBJECT_OUTPUT_PATH)\*.$(OBJ_FILE_SUFFIX)

################################################################################
# Command to clean all the files in the output folder                          #
################################################################################
clean_all:
	@del /Q $(OBJECT_OUTPUT_PATH)\*.*

################################################################################
# Command to print debug information                                           #
################################################################################
debug_base_make:
	@echo PROJECT_ROOT = $(PROJECT_ROOT)
	@echo LIBRARIES_TO_BUILD = $(LIBRARIES_TO_BUILD)
	@echo OBJECTS_LINK_ONLY = $(OBJECTS_LINK_ONLY)
	@echo OBJECT_OUTPUT_PATH = $(OBJECT_OUTPUT_PATH)
	@echo CC_FILES_TO_BUILD = $(CC_FILES_TO_BUILD)
	@echo ASM_FILES_TO_BUILD = $(ASM_FILES_TO_BUILD)
	@echo OBJECT_PATH_FILES_BUILT = $(OBJECT_PATH_FILES_BUILT)
	@echo CC_INCLUDE_PATH = $(CC_INCLUDE_PATH)
	@echo CPP_INCLUDE_PATH = $(CPP_INCLUDE_PATH)
	@echo ASM_INCLUDE_PATH = $(ASM_INCLUDE_PATH)
	@echo MAKE_CLEAN_RULES = $(MAKE_CLEAN_RULES)
	@echo MAKE_COMPILE_RULES = $(MAKE_COMPILE_RULES)
	@echo MAKE_DEBUG_RULES = $(MAKE_DEBUG_RULES)
	@echo CC_FILES_BUILT = $(CC_FILES_BUILT)
	@echo OBJECT_FILES_BUILT = $(OBJECT_FILES_BUILT)
	@echo VPATH = $(VPATH)
	@echo EXE = $(EXE)
	@echo COMPILER_INCLUDE = $(COMPILER_INCLUDE)
	@echo ASSEMBLER_INCLUDE = $(ASSEMBLER_INCLUDE)
	@echo EXE_FILE_SUFFIX = $(EXE_FILE_SUFFIX)

##############################################################################
# End of the Base Make script                                                #
##############################################################################
