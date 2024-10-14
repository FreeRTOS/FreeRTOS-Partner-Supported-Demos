### Makefile to build the FreeRTOS demo ###

# Override the compiler options if desired by specifying CFLAGS on the make
# command line. For example,
#
#    make all CFLAGS="-Os -g" to optimize for size.

# Build target (options: sim, ml605, kc705)

TARGET      = sim

# Tools

CC          = xt-clang
AS          = xt-clang
ISS         = xt-run
XT_CORE     = $(patsubst %-params,%,$(notdir $(shell xt-clang --show-config=core)))
CONFIGDIR   = $(shell xt-clang --show-config=config)

# For platform-specific commands

include $(CONFIGDIR)/misc/hostenv.mk

# Source code and build locations

CWDIR       = $(subst /,$(S),$(CURDIR))
DROOT       = $(CWDIR)
SRCDIR      = $(DROOT)$(S)common$(S)application_code$(S)cadence_code
CFGDIR      = $(DROOT)$(S)common$(S)config_files
OSROOT      = $(abspath $(DROOT)$(S)..$(S)..$(S)..$(S)..$(S)Source$(S)portable$(S)ThirdParty$(S)Partner-Supported-Ports$(S)Cadence$(S)Xtensa)
BLDROOT     = $(DROOT)$(S)build
BLDDIR      = $(BLDROOT)$(S)$(XT_CORE)
OSINCDIR    = $(abspath $(OSROOT)$(S)..$(S)..$(S)..$(S)..$(S)..$(S)include)
COMMDIR     = $(abspath $(DROOT)$(S)..$(S)..$(S)..$(S)Common)
COMMINCDIR  = $(abspath $(COMMDIR)$(S)include)

# Source files

COMM_FILES  = \
    assert.c

TEST_FILES  = \
    example.c            \
    timer_test1.c        \
    timer_test2.c        \
    perf_test.c          \
    var_freq.c           \
    xt_alloca.c          \
    xt_coproc.c          \
    xt_clib.c            \
    xt_intr.c

ALL_FILES	= $(COMM_FILES) $(TEST_FILES)

STRESS_BASE_O	= \
    IntQueueTimer.o      \
    TestRunner.o         \
    RegTests.o           \
    regtest_xtensa.o     \
    stress.o

STRESS_COMM_C	= \
    AbortDelay.c         \
    BlockQ.c             \
    EventGroupsDemo.c    \
    GenQTest.c           \
    IntQueue.c           \
    IntSemTest.c         \
    MessageBufferAMP.c   \
    MessageBufferDemo.c  \
    PollQ.c              \
    QPeek.c              \
    QueueOverwrite.c     \
    QueueSet.c           \
    QueueSetPolling.c    \
    StaticAllocation.c   \
    StreamBufferDemo.c   \
    StreamBufferInterrupt.c \
    TaskNotify.c         \
    TaskNotifyArray.c    \
    TimerDemo.c          \
    blocktim.c           \
    countsem.c           \
    death.c              \
    dynamic.c            \
    flop.c               \
    integer.c            \
    recmutex.c           \
    semtest.c

ifeq ($(MPU),1)
TEST_FILES += mpu_basic.c
endif

EXE         = $(addprefix $(BLDDIR)/,$(patsubst %.c,%.exe,$(TEST_FILES)))
COMM_OBJS   = $(addprefix $(BLDDIR)/,$(patsubst %.c,%.o,$(COMM_FILES)))
OBJS        = $(addprefix $(BLDDIR)/,$(patsubst %.c,%.o,$(ALL_FILES)))
SRC_FILES   = $(TEST_FILES)

STRESS_O    = \
	$(addprefix $(BLDDIR)/stress/,$(STRESS_BASE_O)) \
	$(addprefix $(BLDDIR)/stress/Common/Minimal/,$(patsubst %.c,%.o,$(STRESS_COMM_C)))

# Search paths

vpath %.c $(SRCDIR)

# Output files

OSLIB       = $(BLDDIR)$(S)libfreertos.a
OSLIB_VERIF = $(BLDDIR)$(S)stress$(S)libfreertos.a

# Target-specific options

ifeq ($(TARGET),sim)
DFLAGS      = -DXT_SIMULATOR
LSP         = sim
endif
ifeq ($(TARGET),ml605)
DFLAGS      = -DXT_BOARD
LSP         = xtml605-rt
endif
ifeq ($(TARGET),kc705)
DFLAGS      = -DXT_BOARD
LSP         = xtkc705-rt
endif

# Build with MPU support
ifeq ($(MPU),1)
LSPPATH     = $(BLDDIR)$(S)lsp$(S)$(LSP)
MPUFLAGS    = -DportUSING_MPU_WRAPPERS=1 -DportALIGN_SECTIONS
else
LSPPATH     = $(LSP)
endif

# Build options. Override CFLAGS from the make command line if desired.
# NOTE: Default LDFLAGS are set up to discard unused sections so that
# the final exe size is minimized.

INCS        = -I$(SRCDIR) -I$(CFGDIR) -I$(OSROOT) -I$(OSINCDIR) -I$(OSINCDIR)$(S)private -I$(BLDDIR)
DFLAGS     += -DSTANDALONE
ifeq ($(OVERLAY),1)
CFLAGS     ?= -Os
else
CFLAGS     ?= -O2
endif
CFLAGS     += -g -Wall -Werror
CCFLAGS     = $(CFLAGS) -ffunction-sections -mlongcalls $(DFLAGS) $(MPUFLAGS)
XX_CCFLAGS  = $(filter-out -Werror,$(CFLAGS)) -ffunction-sections -mlongcalls $(DFLAGS)
VERIF_CFLAGS= -DCONFIG_VERIF
LDFLAGS    ?= -Wl,--gc-sections $(CFLAGS)

CFLAGS_mpu_basic = -O0
CFLAGS_xt_coproc = -O3

# use make DEBUG=1 to get MPU debug prints
ifeq ($(DEBUG),1)
CFLAGS     += -DDEBUG
endif

# include dependency rules (generated using -MD)

-include $(wildcard $(BLDDIR)/*.d)

# Targets

all : os $(EXE)
check : $(addprefix check_,$(notdir $(EXE)))
check_% : $(BLDDIR)/%
	$(ISS) --exit_with_target_code $<

# NOTE: The small test needs its own version of the FreeRTOS library.
# So don't club it with the other tests, it has to be built on its own.

small : small_test

os : $(OSLIB)

# NOTE: The section alignment below can be matched to the target config's
# MPU minimum alignment, we just use 4K here because that is the max possible.

ifeq ($(HOSTTYPE),win)
SECT_PRIV := PLACE SECTIONS(privileged_data privileged_functions freertos_system_calls private_region1 private_region2 private_region3 private_stack) WITH_SECTION(.data)
SECT_ALIGN := ALIGN SECTIONS(privileged_data privileged_functions freertos_system_calls private_region1 private_region2 private_region3 private_stack) 4096
endif

ifeq ($(HOSTTYPE),unix)
SECT_PRIV := "PLACE SECTIONS(privileged_data privileged_functions freertos_system_calls private_region1 private_region2 private_region3 private_stack) WITH_SECTION(.data)"
SECT_ALIGN := "ALIGN SECTIONS(privileged_data privileged_functions freertos_system_calls private_region1 private_region2 private_region3 private_stack) 4096"
endif

# NOTE: Older xt-genldscripts does not support "ALIGN SECTIONS ..." so attempt to gracefully omit it
# if it exits with an error
$(LSPPATH)/.mkdir :
	$(MKPATH) $(LSPPATH)
	$(CP) $(subst /,$(S),$(CONFIGDIR))$(S)xtensa-elf$(S)lib$(S)$(LSP)$(S)memmap.xmm $(LSPPATH)
	$(CP) $(subst /,$(S),$(CONFIGDIR))$(S)xtensa-elf$(S)lib$(S)$(LSP)$(S)specs $(LSPPATH)
	echo $(SECT_PRIV) >> $(LSPPATH)$(S)memmap.xmm
	$(CP) $(LSPPATH)$(S)memmap.xmm $(LSPPATH)$(S)memmap.xmm.noalign
	echo $(SECT_ALIGN) >> $(LSPPATH)$(S)memmap.xmm
	-xt-genldscripts -b $(LSPPATH) > $(LSPPATH)$(S)sect_align.log 2>&1 || $(CP) $(LSPPATH)$(S)memmap.xmm.noalign $(LSPPATH)$(S)memmap.xmm
	xt-genldscripts -b $(LSPPATH) >> $(LSPPATH)$(S)sect_align.log 2>&1
	@echo "" > $@

$(BLDDIR)/.mkdir :
	@$(MKPATH) $(BLDDIR)
	@echo "" > $@

$(BLDDIR)/stress/.mkdir : $(BLDDIR)/.mkdir
	@$(MKPATH) $(BLDDIR)$(S)stress
	@$(MKPATH) $(BLDDIR)$(S)stress$(S)Common
	@$(MKPATH) $(BLDDIR)$(S)stress$(S)Common$(S)Minimal
	@echo "" > $@

ifeq ($(MPU),1)
MPULSP = $(LSPPATH)/.mkdir
lsptest : $(LSPPATH)/.mkdir
endif

$(EXE) : $(BLDDIR)/%.exe : $(BLDDIR)/%.o $(COMM_OBJS) $(OSLIB) $(MPULSP)
	$(CC) -o $@ $(filter %.o,$^) -mlsp=$(LSPPATH) $(LDFLAGS) $(OSLIB) -lxtutil -lm

$(OBJS) : $(BLDDIR)/%.o : %.c $(BLDDIR)/.mkdir $(OSLIB)
	$(CC) $(CCFLAGS) $(CFLAGS_$*) $(INCS) -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(BLDDIR)/stress/stress.exe : $(STRESS_O) $(OSLIB_VERIF) $(MPULSP)
	$(CC) -o $@ $(filter %.o,$^) -mlsp=$(LSPPATH) $(LDFLAGS) $(OSLIB_VERIF) -lxtutil -lm

$(BLDDIR)/stress/%.o : stress/%.c $(BLDDIR)/.mkdir $(BLDDIR)/stress/.mkdir $(OSLIB_VERIF)
	$(CC) $(XX_CCFLAGS) $(VERIF_CFLAGS) $(CFLAGS_$*) -I$(CFGDIR) -I$(OSROOT) -I$(OSINCDIR) -I$(OSINCDIR)$(S)private -I$(BLDDIR)$(S)stress -I$(COMMINCDIR) -Istress -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(BLDDIR)/stress/%.o : stress/%.S $(BLDDIR)/.mkdir $(BLDDIR)/stress/.mkdir $(OSLIB_VERIF)
	$(CC) $(XX_CCFLAGS) $(VERIF_CFLAGS) $(CFLAGS_$*) -I$(CFGDIR) -I$(OSROOT) -I$(OSINCDIR) -I$(OSINCDIR)$(S)private -I$(BLDDIR)$(S)stress -I$(COMMINCDIR) -Istress -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(BLDDIR)/stress/Common/Minimal/%.o : $(COMMDIR)/Minimal/%.c $(BLDDIR)/.mkdir $(BLDDIR)/stress/.mkdir $(OSLIB_VERIF)
	$(CC) $(XX_CCFLAGS) $(VERIF_CFLAGS) $(CFLAGS_$*) -I$(CFGDIR) -I$(OSROOT) -I$(OSINCDIR) -I$(OSINCDIR)$(S)private -I$(BLDDIR)$(S)stress -I$(COMMINCDIR) -Istress -MD -MF $(subst .o,.d,$@) -c -o $@ $<

$(OSLIB) ::
	$(MAKE) -f $(OSROOT)$(S)Makefile all TARGET=$(TARGET) CFLAGS="$(CFLAGS)" SRCROOT=$(OSROOT) BLDROOT=$(BLDROOT) MPU=$(MPU)

$(OSLIB_VERIF) ::
	$(MAKE) -f $(OSROOT)$(S)Makefile all TARGET=$(TARGET) CFLAGS="$(CFLAGS) $(VERIF_CFLAGS)" SRCROOT=$(OSROOT) BLDROOT=$(BLDROOT) BLDAUX="$(S)stress" MPU=$(MPU)

mpu_basic : $(BLDDIR)/mpu_basic.exe

example   : $(BLDDIR)/example.exe

timer2   : $(BLDDIR)/timer_test2.exe

perf_test : $(BLDDIR)/perf_test.exe

xt_coproc : $(BLDDIR)/xt_coproc.exe

xt_alloca : $(BLDDIR)/xt_alloca.exe

xt_clib   : $(BLDDIR)/xt_clib.exe

xt_intr   : $(BLDDIR)/xt_intr.exe

stress   : $(BLDDIR)/stress/stress.exe

# xt_clib uses asm-offsets.h, which is build as a side effect of $(OSLIB)
$(BLDDIR)/xt_clib.o : $(OSLIB)

small_test : $(BLDDIR)/.mkdir
	$(MAKE) --no-print-directory $(BLDDIR)/small_test.exe CFLAGS="$(CFLAGS) -DSMALL_TEST -DXT_USE_THREAD_SAFE_CLIB=0"


#------------------------------------------------------------------------------
# The overlay test requires some special handling and also a separate build of
# the FreeRTOS library. Use "make clean; make overlay" to build.
#------------------------------------------------------------------------------

OVLYLSP = $(BLDDIR)/olsp

ifeq ($(MPU),1)
$(OVLYLSP) :
	xt-regenlsps -mlsp=sim -b $@
	@echo OVERLAYS = 4 >> $@/memmap.xmm
	@echo OVERLAY_ALIGN = 4096 >> $@/memmap.xmm
	@echo $(SECT_PRIV) >> $@$(S)memmap.xmm
	$(CP) $@$(S)memmap.xmm $@$(S)memmap.xmm.noalign
	@echo $(SECT_ALIGN) >> $@$(S)memmap.xmm
	-xt-genldscripts -b $@ > $@$(S)sect_align.log 2>&1 || $(CP) $@$(S)memmap.xmm.noalign $@$(S)memmap.xmm
	xt-genldscripts -b $@ >> $@$(S)sect_align.log 2>&1
else
$(OVLYLSP) :
	xt-regenlsps -mlsp=sim -b $@
	@echo OVERLAYS = 4 >> $@/memmap.xmm
	@echo OVERLAY_ALIGN = 4096 >> $@/memmap.xmm
	xt-genldscripts -b $@
endif

# Must specify this object file to override default version in liboverlay.
FIX = $(BLDDIR)/xtensa_overlay_os_hook.o

$(BLDDIR)/overlay_test.exe : overlay_test.c $(SRCDIR)/$(COMM_FILES) $(OSLIB) $(OVLYLSP)
	$(CC) $(CCFLAGS) $(CFLAGS_$*) $(INCS) -o $@ $< $(SRCDIR)/$(COMM_FILES) $(FIX) $(OSLIB) -loverlay -lxtutil -mlsp=$(OVLYLSP)

overlay : $(BLDDIR)/.mkdir
	$(MAKE) --no-print-directory $(BLDDIR)/overlay_test.exe OVERLAY=1


clean :
	$(RM_R) $(BLDDIR)

clean_all :
	$(RM_R) $(BLDROOT)

.PHONY : all clean clean_all mpu_basic example perf_test xt_coproc xt_alloca xt_clib xt_intr

help :
	@echo Available options:
	@echo     "make all              -- build all tests for simulator"
	@echo     "make all TARGET=ml605 -- build all tests for ML605 FPGA board"
	@echo     "make all TARGET=kc705 -- build all tests for KC705 FPGA board"
	@echo     "make check            -- build and run all tests on simulator"
	@echo     "make clean            -- Clean build for current config"
	@echo     "make clean_all        -- Clean build for all configs"

