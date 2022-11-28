
# Device type (HS/GP)
DEVICE_TYPE?=GP

ifeq ($(OS),Windows_NT)
	HOME_DIR?=C:
	TOOLS_PATH?=C:/ti
	CCS_PATH?=$(TOOLS_PATH)/ccs1120/ccs
    CCS_ECLIPSE=$(CCS_PATH)/eclipse/eclipsec
    CYGWIN_PATH?=$(CCS_PATH)/utils/cygwin
    MKDIR=$(CYGWIN_PATH)/mkdir -p
    RMDIR=$(CYGWIN_PATH)/rm -rf
    RM=$(CYGWIN_PATH)/rm -f
    COPY=$(CYGWIN_PATH)/cp
    TOUCH=$(CYGWIN_PATH)/touch
    PATHSEP=\\
	CHMOD=$(CYGWIN_PATH)/echo
    SHELL=cmd.exe
	CGT_GCC_AARCH64_PATH=$(TOOLS_PATH)/gcc-arm-9.2-2019.12-mingw-w64-i686-aarch64-none-elf
    NODE=$(TOOLS_PATH)/nodejs/node.exe
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		export HOME_DIR?=$(HOME)
		export TOOLS_PATH?=$(HOME)/ti
		export MKDIR=mkdir -p
        export RMDIR=rm -rf
        export RM=rm -f
        export COPY=cp
        export TOUCH=touch
        export PATHSEP=/
        export CHMOD=chmod
		export CGT_GCC_AARCH64_PATH=$(TOOLS_PATH)/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf
        export NODE=$(TOOLS_PATH)/node-v12.18.4-linux-x64/bin/node
	endif
endif

FREERTOS_CLONE=$(HOME_DIR)/FreeRTOS/FreeRTOS
FREERTOS_SRC=$(FREERTOS_CLONE)/Source
FREERTOS_PORTABLE=$(FREERTOS_CLONE)/Source/portable/ThirdParty/Partner-Supported-Ports/TI/CORTEX_A53_64-BIT_TI_AM64_SMP
PORT_SRC=$(FREERTOS_CLONE)/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_A53_64-BIT_TI_AM64_SMP

# Path to the signing tools, keys etc
SIGNING_TOOL_PATH=$(TI_PORT_DEMO_ROOT)/tools/boot/signing

ROM_DEGENERATE_KEY:=$(SIGNING_TOOL_PATH)/rom_degenerateKey.pem
APP_SIGNING_KEY=$(ROM_DEGENERATE_KEY)

