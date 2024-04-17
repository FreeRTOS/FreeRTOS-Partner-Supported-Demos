#ifndef MAIN_CORE_H
#define MAIN_CORE_H

// Section: Included Files

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "main_core_types.h"

/**
 * @ingroup  main_coredriver
 * @brief    This routine initializes the MSI driver.
 *           This routine must be called before any other MSI routine is called.
 *           This routine should only be called once during system initialization.
 * @param    none
 * @return   none  
 */
void MAIN_CORE_Initialize(void);

/**
 * @ingroup  main_coredriver
 * @brief    Deinitializes MAIN_CORE to POR values.
 * @param    none
 * @return   none 
 */
void MAIN_CORE_Deinitialize(void);

/**
 * @ingroup  main_coredriver
 * @brief    This routine generates interrupt to MAIN_CORE.
 * @param    none
 * @return   none  
 */
void MAIN_CORE_InterruptRequestGenerate(void);

/**
 * @ingroup  main_coredriver
 * @brief    This routine returns the status of interrupt request acknowledge from the  MAIN_CORE.
 * @param    none
 * @return   True  - when MAIN_CORE has acknowledged Main MSI interrupt request.
 * @return   False - when MAIN_CORE has not acknowledged Main MSI interrupt request.  
 */       
bool MAIN_CORE_IsInterruptRequestAcknowledged(void);    

/**
 * @ingroup  main_coredriver
 * @brief    This routine clears interrupt to MAIN_CORE.
 * @param    none
 * @return   none  
 */        
void MAIN_CORE_InterruptRequestComplete(void);    

/**
 * @ingroup  main_coredriver
 * @brief    This routine returns the status of interrupt request from the MAIN_CORE.
 * @param    none
 * @return   True  - when MAIN_CORE has issued interrupt to Secondary Core.
 * @return   False - when MAIN_CORE has not issued interrupt to Secondary Core.  
 */     
bool MAIN_CORE_IsInterruptRequested(void);    

/**
 * @ingroup  main_coredriver
 * @brief    This routine acknowledges interrupt received from Main MSI.
 * @param    none
 * @return   none  
 */    
void MAIN_CORE_InterruptRequestAcknowledge(void);    

/**
 * @ingroup  main_coredriver
 * @brief    This routine clears acknowledge for interrupt received from Main MSI.
 * @param    none
 * @return   none  
 */       
void MAIN_CORE_InterruptRequestAcknowledgeComplete(void);

/**
 * @ingroup  main_coredriver
 * @brief    This routine returns Main system status.
 * @param    none
 * @return   MAIN_CORE_SYSTEM_STATUS: Enum which provides the status of MAIN_CORE.  
 */      
enum MAIN_CORE_SYSTEM_STATUS MAIN_CORE_SystemStatusGet(void);    

/**
 * @ingroup    main_coredriver
 * @brief      This routine reads FIFO data sent from the Main Core.
 * @param[in]  *pData      - Pointer to an array to store the read data
 * @param[in]  wordCount   - Number of words to be read.  
 * @return     Number of words read.  
 */
uint16_t MAIN_CORE_FIFORead(uint16_t *pData, uint16_t wordCount);    

/**
 * @ingroup    main_coredriver
 * @brief      This routine transfers FIFO data to Main Core.
 * @pre        MAIN_CORE_Initialize() should be called before calling this routine.
 * @param[in]  *pData      - Pointer to an array which has data for transmission.
 * @param[in]  wordCount   - Number of words to be transfer.  
 * @return     Number of words transferred.  
 */
uint16_t MAIN_CORE_FIFOWrite(uint16_t *pData, uint16_t wordCount);

/** 
 * @ingroup  main_coredriver
 * @brief    This inline function checks whether the status of Read FIFO is full. 
 *           last write by Secondary core to Read FIFO (RFDATA) was into the last free location
 * @return   true   -    Read FIFO is full
 * @return   false  -    Read FIFO is not full
 */ 
inline static bool MAIN_CORE_FIFOReadIsFull(void)
{
    return(SI1FIFOCSbits.SRFFULL);
}

/** 
 * @ingroup  main_coredriver
 * @brief    This inline function checks whether the status of Read FIFO is Empty. 
 *           Returns true if last read by Main core from Read FIFO (RFDATA) emptied the 
 *           FIFO of all valid data or FIFO is disabled (and initialized to the empty state)
 * @return   true   -    Read FIFO is Empty
 * @return   false  -    Read FIFO is not Empty (Read FIFO contains valid data not yet read by the Main core)
 */
inline static bool MAIN_CORE_FIFOReadIsEmpty(void)
{
    return(SI1FIFOCSbits.SRFEMPTY);
}

/** 
 * @ingroup  main_coredriver
 * @brief    This inline function checks whether the status of Write FIFO is Full. 
 *           Returns true if last write by Main core to Write FIFO (WFDATA) was into 
 *           the last free location
 * @return   true   -    Write FIFO is full
 * @return   false  -    Write FIFO is not full
 */ 
inline static bool MAIN_CORE_FIFOWriteIsFull(void)
{
    return(SI1FIFOCSbits.SWFFULL);
}

/** 
 * @ingroup  main_coredriver
 * @brief    This inline function checks whether the status of Write FIFO is Empty. 
 *           Returns true if last write by Main to Write FIFO (WFDATA) was into 
 *           the last free location
 * @return   true   -    Write FIFO is Empty
 * @return   false  -    Write FIFO is not Empty (Write FIFO contains valid data not yet read by the Secondary core)
 */ 
inline static bool MAIN_CORE_FIFOWriteIsEmpty(void)
{
    return(SI1FIFOCSbits.SWFEMPTY);
}

/**
 * @ingroup    main_coredriver
 * @brief      This routine writes data to mailbox.
 * @param[in]  protocolName - Selected protocol 
 * @param[in]  *pData      - Pointer to a structure which has data for transmission.  
 * @return     true   -    successful write
 * @return     false  -    unsuccessful write  
 */      
bool MAIN_CORE_ProtocolWrite(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData);

/** 
 * @ingroup    main_coredriver
 * @brief      This routine reads data from mailbox.
 * @param[in]  protocolName - Selected protocol 
 * @param[in]  *pData       - Pointer to a structure to store the read data  
 * @return     true   -    successful read
 * @return     false  -    unsuccessful read  
 */   
bool MAIN_CORE_ProtocolRead(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData);

/** 
 * @ingroup    main_coredriver
 * @brief      This inline function checks whether mailbox is full. 
 *             Returns true if New data are ready to read. Returns false when 
 *             No data are available to be read by Main core.
 * @param[in]  protocolName - Selected protocol 
 * @return     true   -    Protocol is full
 * @return     false  -    Protocol is empty 
 */ 
inline static bool MAIN_CORE_ProtocolIsFull(enum MAIN_CORE_PROTOCOLS protocolName)
{
    bool status = false;
    switch(protocolName)
    {
        case MSI1_ProtocolA:
            status =  SI1MBXSbits.DTRDYA;
            break;
        case MSI1_ProtocolB:
            status =  SI1MBXSbits.DTRDYB;
            break;
        default:
            break;   
    }  
    return status;   
}

/** 
 * @ingroup    main_coredriver
 * @brief      This inline function checks whether mailbox is empty. 
 *             Returns true if Protocol is empty indicating mailbox is ready to 
 *             write by Main core. returns false indicating write operation cannot 
 *             be performed by Main core.
 * @param[in]  protocolName - Selected protocol 
 * @return     true   -    Protocol is empty
 * @return     false  -    Protocol is full
 */ 
inline static bool MAIN_CORE_ProtocolIsEmpty(enum MAIN_CORE_PROTOCOLS protocolName)
{
    bool status = false;
    switch(protocolName)
    {
        case MSI1_ProtocolA:
            status =  !SI1MBXSbits.DTRDYA;
            break;
        case MSI1_ProtocolB:
            status =  !SI1MBXSbits.DTRDYB;
            break;
        default:
            break;   
    }  
    return status; 
}



#endif //MAIN_CORE_H



