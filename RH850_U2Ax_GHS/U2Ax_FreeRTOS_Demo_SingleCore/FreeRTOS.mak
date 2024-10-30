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
COMMON_SAMPLE_CORE_PATH =U:\

################################################################################
# Maros for build
#
LIBRARIES_TO_BUILD =

OBJECT_OUTPUT_PATH = U:\obj\ghs

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
include U:\ghs\make\ghs_rh850_defs.mak
include U:\ghs\make\ghs_rh850_rules.mak

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
include U:\ghs\make\FreeRTOS_Common_Sample.mak
include U:\startup\make\ghs\startup_rh850_U2Ax_defs.mak
include U:\startup\make\ghs\startup_rh850_U2Ax_rules.mak

################################################################################
# Path to Linker Directives Files
#
LNKFILE = U:\linker\App_U2A_Sample.ld
LNKFILE_DB = U:\linker\App_U2A_Sample_db.ld
LNKFILE_COMMON = U:\linker\App_Common_Sample.ld

################################################################################
# Include generated source files list to the global "C" file list              #
################################################################################
#
ASM_INCLUDE_PATH +=
ASM_INCLUDE_PATH += U:\startup\include

CC_INCLUDE_PATH +=
CC_INCLUDE_PATH += U:\freertos_port
CC_INCLUDE_PATH += U:\bsp\bsp_irq
CC_INCLUDE_PATH += U:\bsp\common
CC_INCLUDE_PATH += U:\freertos_config
CC_INCLUDE_PATH += U:\FreeRTOS_Kernel\Common\include
CC_INCLUDE_PATH += U:\FreeRTOS_Kernel\Source\include
CC_INCLUDE_PATH += U:\ghs\include
CC_INCLUDE_PATH += U:\src

CC_SRC_PATH +=
CC_SRC_PATH += U:\startup
CC_SRC_PATH += U:\freertos_port
CC_SRC_PATH += U:\src
CC_SRC_PATH += U:\bsp\bsp_irq
CC_SRC_PATH += U:\FreeRTOS_Kernel\Common\Minimal
CC_SRC_PATH += U:\FreeRTOS_Kernel\Source
CC_SRC_PATH += U:\FreeRTOS_Kernel\Source\portable\MemMang

CPP_INCLUDE_PATH +=
CPP_INCLUDE_PATH += U:\src
CPP_INCLUDE_PATH += U:\bsp\bsp_irq
CPP_INCLUDE_PATH += U:\ghs\include
CPP_INCLUDE_PATH += U:\bsp\common
CPP_INCLUDE_PATH += U:\freertos_config
CPP_INCLUDE_PATH += U:\FreeRTOS_Kernel\Common\include
CPP_INCLUDE_PATH += U:\FreeRTOS_Kernel\Source\include

ASM_FILES_TO_BUILD  += \
                 U:\freertos_port\portasm.850 \
                 U:\startup\coldreset.850 \
                 U:\startup\U2Ax_startup_PE0.850 \
                 U:\startup\U2Ax_startup_PEn.850 \
                 U:\src\RegTest.850
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
