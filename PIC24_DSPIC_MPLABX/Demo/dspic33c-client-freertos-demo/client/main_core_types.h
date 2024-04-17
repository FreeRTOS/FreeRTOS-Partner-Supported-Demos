#ifndef MAIN_CORE_TYPES_H
#define MAIN_CORE_TYPES_H

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_SYSTEM_STATUS
 @brief     Defines the status enumeration for main_core.
*/
enum MAIN_CORE_SYSTEM_STATUS
{
    MAIN_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE = 0,   /**< main_core status is in Low power mode */
    MAIN_CORE_SYSTEM_STATUS_SLEEP_MODE            = 1,   /**< main_core status is in Sleep mode */
    MAIN_CORE_SYSTEM_STATUS_IDLE_MODE             = 2,   /**< main_core status is in Idle mode */
    MAIN_CORE_SYSTEM_STATUS_RUNNING_STATE         = 3,   /**< main_core status is in Running state */
    MAIN_CORE_SYSTEM_STATUS_IN_RESET_STATE        = 4    /**< main_core status is in Reset state */
};

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_PROTOCOLS
 @brief     Defines the list of Protocols configured for MAIN_CORE_driver. 
 @note      These Protocols are required to create the buffer by the application
*/
enum MAIN_CORE_PROTOCOLS
{
    MSI1_ProtocolA,      /**< Custom-name of Protocol A for read operation only */
    MSI1_ProtocolB,      /**< Custom-name of Protocol B for write operation only */
};

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_PROTOCOL_SIZE
 @brief     Defines the size for Protocols.
 @note      Size not required for the driver but will be required for application to create the buffers
*/
enum MAIN_CORE_PROTOCOL_SIZE
{
    MSI1_ProtocolA_SIZE = 1,     /**< Size of Protocol A in 16-bit word*/
    MSI1_ProtocolB_SIZE = 1,     /**< Size of Protocol B in 16-bit word*/
};

#endif    /* MAIN_CORE_TYPES_H */