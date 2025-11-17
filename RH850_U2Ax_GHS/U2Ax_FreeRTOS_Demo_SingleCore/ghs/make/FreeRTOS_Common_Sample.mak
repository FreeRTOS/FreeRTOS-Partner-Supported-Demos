################################################################################
# Makefile to compile and build the Sample application with the AUTOSAR GPT    #
# Driver Component (For Test purposes only)                                    #
# Compatible with GNU Make 3.81 for Win32.                                     #
################################################################################

################################################################################
# Definitions of global environment variables                                  #
################################################################################

# Get the name of the SRECORD file
CURRENT_APPL_SRECORD = $(CURRENT_APPL)

################################################################################
# Driver Component

# renesas defs
###############################################################################
# REGISTRY
#
SSC_PLUGINS += renesas_
renesas_DEPENDENT_PLUGINS =

CC_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq \
                   $(SPECIFIC_APPL_ROOT_PATH)\freertos_config \
                   $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\include \
                   $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\include \
                   $(SPECIFIC_APPL_ROOT_PATH)\FreeRTOS_Kernel\freertos_port\GHS\U2x \
                   $(SPECIFIC_APPL_ROOT_PATH)\src
CC_SRC_PATH +=     $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq \
                   $(SPECIFIC_APPL_ROOT_PATH)\freertos_config \
                   $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\include \
                   $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\include \
                   $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x \
                   $(SPECIFIC_APPL_ROOT_PATH)\src
CPP_INCLUDE_PATH += $(SPECIFIC_APPL_ROOT_PATH)\bsp\bsp_irq \
                    $(SPECIFIC_APPL_ROOT_PATH)\freertos_config \
                    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\include \
                    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\include \
                    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x \
                    $(SPECIFIC_APPL_ROOT_PATH)\src
# end of renesas defs


# renesas rules
###############################################################################
# REGISTRY
#

LIBRARIES_TO_BUILD += renesas_lib

renesas_lib_FILES = \
    $(SPECIFIC_APPL_ROOT_PATH)\src\main.c \
    $(SPECIFIC_APPL_ROOT_PATH)\src\RegTestsTask.c \
    $(SPECIFIC_APPL_ROOT_PATH)\src\TestRunner.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x\port.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\timers.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\tasks.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\queue.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\stream_buffer.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\list.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\event_groups.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\croutine.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\MemMang\heap_4.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\semtest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\integer.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\flop.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\dynamic.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\death.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\BlockQ.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\recmutex.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\PollQ.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\crhook.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\crflash.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\countsem.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\blocktim.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TimerDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TaskNotifyArray.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TaskNotify.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StreamBufferInterrupt.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StreamBufferDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StaticAllocation.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueSetPolling.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueSet.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueOverwrite.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QPeek.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\MessageBufferDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\MessageBufferAMP.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\IntSemTest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\GenQTest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\EventGroupsDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\AbortDelay.c

CC_FILES_TO_BUILD += \
    $(SPECIFIC_APPL_ROOT_PATH)\src\main.c \
    $(SPECIFIC_APPL_ROOT_PATH)\src\RegTestsTask.c \
    $(SPECIFIC_APPL_ROOT_PATH)\src\TestRunner.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\ThirdParty\Partner-Supported-Ports\GHS\U2x\port.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\timers.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\tasks.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\queue.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\stream_buffer.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\list.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\event_groups.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\croutine.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Source\portable\MemMang\heap_4.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\semtest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\recmutex.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\integer.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\flop.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\dynamic.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\death.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\crhook.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\crflash.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\countsem.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\blocktim.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TimerDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TaskNotifyArray.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\TaskNotify.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StreamBufferInterrupt.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StreamBufferDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\StaticAllocation.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueSetPolling.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueSet.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QueueOverwrite.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\QPeek.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\PollQ.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\MessageBufferDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\MessageBufferAMP.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\IntSemTest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\GenQTest.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\EventGroupsDemo.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\BlockQ.c \
    $(SPECIFIC_APPL_ROOT_PATH)\..\..\..\..\..\Demo\Common\Minimal\AbortDelay.c

OBJECTS_LINK_ONLY +=

###############################################################################
MAKE_DEBUG_RULES += debug_makefile

# end of renesas rules


################################################################################
# Command to generate standalone database                                      #
################################################################################
$(MODULE_DB).$(S_RECORD_SUFFIX):$(MODULE_DB).$(OBJ_FILE_SUFFIX)
	@echo *********************************************************************
	@echo Building the standalone database ...
	$(DBLINKER) $(LNKFILE_COMMON) \
	"$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(OBJ_FILE_SUFFIX)" \
	-map="$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(MAP_FILE_SUFFIX)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(EXE_FILE_SUFFIX)"
	@echo Generating Motorola S-Record file...
	$(CONVERTER) $(SFLAGS) "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(EXE_FILE_SUFFIX)" \
	-o "$(OBJECT_OUTPUT_PATH)\$(MODULE_DB).$(S_RECORD_SUFFIX)"
	@echo Done ...

################################################################################
# End of the Base Make script                                                  #
################################################################################
