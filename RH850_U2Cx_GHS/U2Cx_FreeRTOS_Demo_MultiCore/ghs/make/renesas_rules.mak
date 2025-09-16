###############################################################################
# REGISTRY
#

LIBRARIES_TO_BUILD += renesas_lib

renesas_lib_FILES = \
    U:\src\main.c \
    U:\src\RegTestsTask.c \
    U:\src\TestRunner.c \
    U:\freertos_port\port.c \
    U:\FreeRTOS_Kernel\Source\timers.c \
    U:\FreeRTOS_Kernel\Source\tasks.c \
    U:\FreeRTOS_Kernel\Source\queue.c \
    U:\FreeRTOS_Kernel\Source\stream_buffer.c \
    U:\FreeRTOS_Kernel\Source\list.c \
    U:\FreeRTOS_Kernel\Source\event_groups.c \
    U:\FreeRTOS_Kernel\Source\croutine.c \
    U:\FreeRTOS_Kernel\Source\portable\MemMang\heap_4.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flash_timer.c \
    U:\FreeRTOS_Kernel\Common\Minimal\semtest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\integer.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flop.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flash.c \
    U:\FreeRTOS_Kernel\Common\Minimal\dynamic.c \
    U:\FreeRTOS_Kernel\Common\Minimal\death.c \
    U:\FreeRTOS_Kernel\Common\Minimal\BlockQ.c \
    U:\FreeRTOS_Kernel\Common\Minimal\recmutex.c \
    U:\FreeRTOS_Kernel\Common\Minimal\comtest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\PollQ.c \
    U:\FreeRTOS_Kernel\Common\Minimal\crhook.c \
    U:\FreeRTOS_Kernel\Common\Minimal\crflash.c \
    U:\FreeRTOS_Kernel\Common\Minimal\countsem.c \
    U:\FreeRTOS_Kernel\Common\Minimal\comtest_strings.c \
    U:\FreeRTOS_Kernel\Common\Minimal\blocktim.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TimerDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TaskNotifyArray.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TaskNotify.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StreamBufferInterrupt.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StreamBufferDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StaticAllocation.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueSetPolling.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueSet.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueOverwrite.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QPeek.c \
    U:\FreeRTOS_Kernel\Common\Minimal\MessageBufferDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\MessageBufferAMP.c \
    U:\FreeRTOS_Kernel\Common\Minimal\IntSemTest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\IntQueue.c \
    U:\FreeRTOS_Kernel\Common\Minimal\GenQTest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\EventGroupsDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\AbortDelay.c 

CC_FILES_TO_BUILD += \
    U:\src\main.c \
    U:\src\RegTestsTask.c \
    U:\src\TestRunner.c \
    U:\freertos_port\port.c \
    U:\FreeRTOS_Kernel\Source\timers.c \
    U:\FreeRTOS_Kernel\Source\tasks.c \
    U:\FreeRTOS_Kernel\Source\queue.c \
    U:\FreeRTOS_Kernel\Source\stream_buffer.c \
    U:\FreeRTOS_Kernel\Source\list.c \
    U:\FreeRTOS_Kernel\Source\event_groups.c \
    U:\FreeRTOS_Kernel\Source\croutine.c \
    U:\FreeRTOS_Kernel\Source\portable\MemMang\heap_4.c \
    U:\FreeRTOS_Kernel\Common\Minimal\semtest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\recmutex.c \
    U:\FreeRTOS_Kernel\Common\Minimal\integer.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flop.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flash_timer.c \
    U:\FreeRTOS_Kernel\Common\Minimal\flash.c \
    U:\FreeRTOS_Kernel\Common\Minimal\dynamic.c \
    U:\FreeRTOS_Kernel\Common\Minimal\death.c \
    U:\FreeRTOS_Kernel\Common\Minimal\crhook.c \
    U:\FreeRTOS_Kernel\Common\Minimal\crflash.c \
    U:\FreeRTOS_Kernel\Common\Minimal\countsem.c \
    U:\FreeRTOS_Kernel\Common\Minimal\comtest_strings.c \
    U:\FreeRTOS_Kernel\Common\Minimal\comtest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\blocktim.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TimerDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TaskNotifyArray.c \
    U:\FreeRTOS_Kernel\Common\Minimal\TaskNotify.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StreamBufferInterrupt.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StreamBufferDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\StaticAllocation.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueSetPolling.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueSet.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QueueOverwrite.c \
    U:\FreeRTOS_Kernel\Common\Minimal\QPeek.c \
    U:\FreeRTOS_Kernel\Common\Minimal\PollQ.c \
    U:\FreeRTOS_Kernel\Common\Minimal\MessageBufferDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\MessageBufferAMP.c \
    U:\FreeRTOS_Kernel\Common\Minimal\IntSemTest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\IntQueue.c \
    U:\FreeRTOS_Kernel\Common\Minimal\GenQTest.c \
    U:\FreeRTOS_Kernel\Common\Minimal\EventGroupsDemo.c \
    U:\FreeRTOS_Kernel\Common\Minimal\BlockQ.c \
    U:\FreeRTOS_Kernel\Common\Minimal\AbortDelay.c

OBJECTS_LINK_ONLY +=

###############################################################################
MAKE_DEBUG_RULES += debug_makefile


