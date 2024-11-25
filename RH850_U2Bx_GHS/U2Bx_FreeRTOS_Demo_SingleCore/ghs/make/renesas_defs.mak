###############################################################################
# REGISTRY
#
SSC_PLUGINS += renesas_
renesas_DEPENDENT_PLUGINS =

CC_INCLUDE_PATH += U:\bsp\bsp_irq \
                   U:\bsp\common \
                   U:\freertos_config \
                   U:\FreeRTOS_Kernel\Common\include \
                   U:\FreeRTOS_Kernel\Source\include \
                   U:\freertos_port \
                   U:\src
CC_SRC_PATH +=     U:\bsp\bsp_irq \
                   U:\bsp\common \
                   U:\freertos_config \
                   U:\FreeRTOS_Kernel\Common\include \
                   U:\FreeRTOS_Kernel\Source\include \
                   U:\freertos_port \
                   U:\src
CPP_INCLUDE_PATH += U:\bsp\bsp_irq \
                   U:\bsp\common \
                   U:\freertos_config \
                   U:\FreeRTOS_Kernel\Common\include \
                   U:\FreeRTOS_Kernel\Source\include \
                   U:\freertos_port \
                   U:\src
