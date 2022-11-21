
export TI_PORT_DEMO_ROOT?=$(abspath ../../)
include $(TI_PORT_DEMO_ROOT)/imports.mak

CG_TOOL_ROOT=$(CGT_GCC_AARCH64_PATH)

CC=$(CG_TOOL_ROOT)/bin/aarch64-none-elf-gcc
LNK=$(CG_TOOL_ROOT)/bin/aarch64-none-elf-gcc
STRIP=$(CG_TOOL_ROOT)/bin/aarch64-none-elf-strip
OBJCOPY=$(CG_TOOL_ROOT)/bin/aarch64-none-elf-objcopy
ifeq ($(OS), Windows_NT)
	PYTHON=python
else
	PYTHON=python3
endif

OUTNAME:=test_demo.out

BOOTIMAGE_PATH=$(abspath .)
BOOTIMAGE_NAME:=test_demo.appimage
BOOTIMAGE_NAME_XIP:=test_demo.appimage_xip
BOOTIMAGE_NAME_SIGNED:=test_demo.appimage.signed
BOOTIMAGE_RPRC_NAME:=test_demo.rprc
BOOTIMAGE_RPRC_NAME_XIP:=test_demo.rprc_xip
BOOTIMAGE_RPRC_NAME_TMP:=test_demo.rprc_tmp
BOOTIMAGE_NAME_HS:=test_demo.appimage.hs
BOOTIMAGE_NAME_HS_FS:=test_demo.appimage.hs_fs
TARGETS := $(BOOTIMAGE_NAME)
ifeq ($(DEVICE_TYPE), HS)
   TARGETS += $(BOOTIMAGE_NAME_HS)
endif

FILES_common := \
	main.c \
	ti_drivers_config.c \
	ti_drivers_open_close.c \
	ti_board_config.c \
	ti_board_open_close.c \
	ti_dpl_config.c \
	ti_pinmux_config.c \
	ti_power_clock_config.c \

TEST_FILES := \
	AbortDelay.c \
	BlockQ.c \
	blocktim.c \
	countsem.c \
	death.c \
	dynamic.c \
	EventGroupsDemo.c \
	flop.c \
	GenQTest.c \
	integer.c \
	IntQueue.c \
	IntQueueTimer.c \
	IntSemTest.c \
	MessageBufferAMP.c \
	MessageBufferDemo.c \
	PollQ.c \
	QPeek.c \
	QueueOverwrite.c \
	QueueSet.c \
	QueueSetPolling.c \
	recmutex.c \
	RegTests.c \
	semtest.c \
	StaticAllocation.c \
	StreamBufferDemo.c \
	StreamBufferInterrupt.c \
	TaskNotifyArray.c \
	TaskNotify.c \
	TestRunner.c \
	TimerDemo.c \

FILES_PATH_common = \
	configs \
	$(FREERTOS_CLONE)/Demo/Common/Minimal \

INCLUDES_common := \
	-I$(FREERTOS_SRC)/include \
	-I${PORT_SRC} \
	-I${PORT_SRC}/kernel/config/am64x/a53-smp \
	-Iconfigs \
	-I. \
	-I$(FREERTOS_CLONE)/Demo/Common/include \
	-I$(FREERTOS_PORTABLE)

DEFINES_common := \
	-DSOC_AM64X \
	-DOS_FREERTOS \
	-DSMP_FREERTOS \

CFLAGS_common := \
	-mcpu=cortex-a53+fp+simd \
	-mabi=lp64 \
	-mcmodel=large \
	-mstrict-align \
	-mfix-cortex-a53-835769 \
	-mfix-cortex-a53-843419 \
	-Wall \
	-Werror \
	-g \
	-Wno-int-to-pointer-cast \
	-Wno-pointer-to-int-cast \
	-Wno-unused-but-set-variable \
	-fdata-sections \
	-ffunction-sections \

CFLAGS_debug := \
	-D_DEBUG_=1 \

LNK_FILES_common = \
	linker.cmd \

LIBS_PATH_common = \
	-L${PORT_SRC}/kernel/lib \
	-L${PORT_SRC}/drivers/lib \
	-L$(CGT_GCC_AARCH64_PATH)/lib \

LIBS_common = \
	-l:freertos-smp.am64x.a53.gcc-aarch64.lib \
	-l:drivers.am64x.a53.gcc-aarch64.lib \

LFLAGS_common = \
	-Wl,-static \
	-Wl,--gc-sections \
	-Wl,--build-id=none \
	-lstdc++ \
	-lgcc \
	-lm \
	-lc \
	-lrdimon \


LIBS_NAME = \
	freertos-smp.am64x.a53.gcc-aarch64.lib \
	drivers.am64x.a53.gcc-aarch64.lib \

LIBS_PATH_NAME = \
	${PORT_SRC}/kernel/lib \
	${PORT_SRC}/drivers/lib \
	$(CGT_GCC_AARCH64_PATH)/lib \

FILES := $(FILES_common) $(TEST_FILES)
ASMFILES := $(ASMFILES_common)
FILES_PATH := $(FILES_PATH_common)
CFLAGS := $(CFLAGS_common)
DEFINES := $(DEFINES_common)
INCLUDES := $(INCLUDES_common)
LIBS := $(LIBS_common)
LIBS_PATH := $(LIBS_PATH_common)
LFLAGS := $(LFLAGS_common)
LNK_FILES := $(LNK_FILES_common)

OUTDIR := out/
OBJDIR := obj/
OBJS := $(FILES:%.c=%.obj)
OBJS += $(ASMFILES:%.S=%.obj)
DEPS := $(FILES:%.c=%.d)

vpath %.obj $(OBJDIR)
vpath %.c $(FILES_PATH)
vpath %.S $(FILES_PATH)
vpath %.lib $(LIBS_PATH_NAME)
vpath %.a $(LIBS_PATH_NAME)

$(OBJDIR)/%.obj %.obj: %.c
	@echo  Compiling: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(OUTNAME): $<
	$(CC) -c $(CFLAGS) $(INCLUDES) $(DEFINES) -MMD -MT $@ -o $(OBJDIR)/$@ $<

$(OBJDIR)/%.obj %.obj: %.S
	@echo  Compiling: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(LIBNAME): $<
	$(CC) -c -x assembler-with-cpp $(CFLAGS) $(INCLUDES) $(DEFINES) -o $(OBJDIR)$@ $<

all: $(TARGETS)


$(OUTNAME): $(OBJS) $(LNK_FILES) $(LIBS_NAME)
	@echo  .
	@echo  Linking: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $@ ...
	$(LNK) $(LIBS_PATH) -Wl,-Map=$(OUTDIR)/$(OUTNAME:%.out=%.map) -o $(OUTDIR)/$@ -Wl,--start-group $(LFLAGS) $(addprefix $(OBJDIR), $(OBJS)) $(LIBS) -Wl,--end-group -Wl,-T,$(LNK_FILES)
	@echo  Linking: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $@ Done !!!
	@echo  .

clean:
	@echo  Cleaning: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(OUTNAME) ...
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(OUTDIR)
	$(RM) $(OUTDIR)$(OUTNAME)
	$(RM) $(OUTDIR)$(BOOTIMAGE_NAME)
	$(RM) $(OUTDIR)$(BOOTIMAGE_NAME_XIP)
	$(RM) $(OUTDIR)$(BOOTIMAGE_NAME_SIGNED)
	$(RM) $(OUTDIR)$(BOOTIMAGE_NAME_HS)
	$(RM) $(OUTDIR)$(BOOTIMAGE_NAME_HS_FS)
	$(RM) $(OUTDIR)$(BOOTIMAGE_RPRC_NAME)
	$(RM) $(OUTDIR)$(BOOTIMAGE_RPRC_NAME_XIP)

scrub:
	@echo  Scrubing: am64x:a53ss0-0:freertos-smp:gcc-aarch64 test_demo ...
	$(RMDIR) obj
ifeq ($(OS),Windows_NT)
	$(RM) $(OUTDIR)*.out
	$(RM) $(OUTDIR)*.map
	$(RM) $(OUTDIR)*.appimage*
	$(RM) $(OUTDIR)*.rprc*
	$(RM) $(OUTDIR)*.tiimage*
	$(RM) $(OUTDIR)*.bin
else
	$(RM) $(OUTDIR)*.out
	$(RM) $(OUTDIR)*.map
	$(RM) $(OUTDIR)*.appimage*
	$(RM) $(OUTDIR)*.rprc*
	$(RM) $(OUTDIR)*.tiimage*
	$(RM) $(OUTDIR)*.bin
endif
	$(RMDIR) generated

$(OBJS): | $(OBJDIR)

$(OUTNAME): | $(OUTDIR)

$(OBJDIR):
	$(MKDIR) $@

$(OUTDIR):
	$(MKDIR) $@

#
# Generation of boot image which can be loaded by Secondary Boot Loader (SBL)
#
ifeq ($(OS),Windows_NT)
EXE_EXT=.exe
endif
ifeq ($(OS),Windows_NT)
  BOOTIMAGE_CERT_GEN_CMD=powershell -executionpolicy unrestricted -command $(TI_PORT_DEMO_ROOT)/tools/boot/signing/x509CertificateGen.ps1
else
  BOOTIMAGE_CERT_GEN_CMD=$(TI_PORT_DEMO_ROOT)/tools/boot/signing/x509CertificateGen.sh
endif
BOOTIMAGE_TEMP_OUT_FILE=temp_stdout.txt

BOOTIMAGE_CERT_KEY=$(APP_SIGNING_KEY)

BOOTIMAGE_CORE_ID_a53ss0-0 = 0
BOOTIMAGE_CORE_ID_a53ss0-1 = 1
SBL_RUN_ADDRESS=0x70000000
SBL_DEV_ID=55

MULTI_CORE_IMAGE_GEN = $(NODE) $(TI_PORT_DEMO_ROOT)/tools/boot/multicoreImageGen/multicoreImageGen.js
OUTRPRC_CMD = $(NODE) $(TI_PORT_DEMO_ROOT)/tools/boot/out2rprc/elf2rprc.js
APP_IMAGE_SIGN_CMD = $(TI_PORT_DEMO_ROOT)/tools/boot/signing/appimage_x509_cert_gen.py

ifeq ($(OS),Windows_NT)
  XIPGEN_CMD=$(TI_PORT_DEMO_ROOT)/tools/boot/xipGen/xipGen.exe
else
  XIPGEN_CMD=$(TI_PORT_DEMO_ROOT)/tools/boot/xipGen/xipGen.out
endif

MULTI_CORE_IMAGE_PARAMS = \
	$(OUTDIR)$(BOOTIMAGE_RPRC_NAME)@$(BOOTIMAGE_CORE_ID_a53ss0-0) \
	$(OUTDIR)$(BOOTIMAGE_RPRC_NAME)@$(BOOTIMAGE_CORE_ID_a53ss0-1) \

MULTI_CORE_IMAGE_PARAMS_XIP = \
	$(OUTDIR)$(BOOTIMAGE_RPRC_NAME_XIP)@$(BOOTIMAGE_CORE_ID_a53ss0-0) \
	$(OUTDIR)$(BOOTIMAGE_RPRC_NAME_XIP)@$(BOOTIMAGE_CORE_ID_a53ss0-1) \

$(BOOTIMAGE_NAME): $(OUTNAME)
	@echo  Boot image: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(BOOTIMAGE_PATH)/$@ ...
ifneq ($(OS),Windows_NT)
	$(CHMOD) a+x $(XIPGEN_CMD)
endif
	$(OUTRPRC_CMD) $(OUTDIR)/$(OUTNAME) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(COPY) $(BOOTIMAGE_RPRC_NAME) $(OUTDIR)/$(BOOTIMAGE_RPRC_NAME_TMP)
	$(RM) $(BOOTIMAGE_RPRC_NAME)
	$(XIPGEN_CMD) -i $(OUTDIR)$(BOOTIMAGE_RPRC_NAME_TMP) -o $(OUTDIR)$(BOOTIMAGE_RPRC_NAME) -x $(OUTDIR)$(BOOTIMAGE_RPRC_NAME_XIP) --flash-start-addr 0x60000000 -v > $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID $(SBL_DEV_ID) --out $(OUTDIR)$(BOOTIMAGE_NAME) $(MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID $(SBL_DEV_ID) --out $(OUTDIR)$(BOOTIMAGE_NAME_XIP) $(MULTI_CORE_IMAGE_PARAMS_XIP) >> $(BOOTIMAGE_TEMP_OUT_FILE)
# Sign the appimage for HS-FS using appimage signing script
	$(PYTHON) $(APP_IMAGE_SIGN_CMD) --bin $(OUTDIR)$(BOOTIMAGE_NAME) --authtype 1 --key $(APP_SIGNING_KEY) --output $(OUTDIR)$(BOOTIMAGE_NAME_HS_FS)
	$(RM) $(OUTDIR)$(BOOTIMAGE_RPRC_NAME_TMP)
	$(RM) $(BOOTIMAGE_TEMP_OUT_FILE)
	@echo  Boot image: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(BOOTIMAGE_PATH)/$@ Done !!!
	@echo  .
	@echo  Boot image: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(BOOTIMAGE_PATH)/$(BOOTIMAGE_NAME_HS_FS) Done !!!
	@echo  .

$(BOOTIMAGE_NAME_HS): $(BOOTIMAGE_NAME)
ifeq ($(DEVICE_TYPE), HS)
# Sign the appimage using appimage signing script
ifeq ($(ENC_ENABLED),no)
	@echo Boot image signing: Encryption is disabled.
	$(PYTHON) $(APP_IMAGE_SIGN_CMD) --bin $(BOOTIMAGE_NAME) --authtype 1 --key $(APP_SIGNING_KEY) --output $(BOOTIMAGE_NAME_HS)
else
	@echo Boot image signing: Encryption is enabled.
	$(PYTHON) $(APP_IMAGE_SIGN_CMD) --bin $(BOOTIMAGE_NAME) --authtype 1 --key $(APP_SIGNING_KEY) --enc y --enckey $(APP_ENCRYPTION_KEY) --output $(BOOTIMAGE_NAME_HS)
	$(RM) $(BOOTIMAGE_NAME)-enc
endif
	@echo  Boot image: am64x:a53ss0-0:freertos-smp:gcc-aarch64 $(BOOTIMAGE_PATH)/$(BOOTIMAGE_NAME_HS) Done !!!
	@echo  .
endif
-include $(addprefix $(OBJDIR)/, $(DEPS))

