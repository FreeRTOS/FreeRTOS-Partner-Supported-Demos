#include <stddef.h>
#include "main_core_types.h"
#include "main_core.h"


void MAIN_CORE_Initialize(void)
{
    //MRSTIE disabled; MTSIACK disabled; STMIRQ disabled; RFITSEL Trigger data valid interrupt when 1st FIFO entry is written by Slave; 
    SI1CON = 0x0U;    
}

void MAIN_CORE_Deinitialize(void)
{
    SI1CON = 0x0U;    
}

void MAIN_CORE_InterruptRequestGenerate(void)
{
    SI1CONbits.STMIRQ = 1U;
}

bool MAIN_CORE_IsInterruptRequestAcknowledged(void)
{
    return(SI1STATbits.STMIACK);
    
}

void MAIN_CORE_InterruptRequestComplete(void)
{
    SI1CONbits.STMIRQ = 0U; 
}
 
bool MAIN_CORE_IsInterruptRequested(void)
{
    return(SI1STATbits.MTSIRQ);
}
void MAIN_CORE_InterruptRequestAcknowledge(void)
{
    SI1CONbits.MTSIACK = 1U;
}

void MAIN_CORE_InterruptRequestAcknowledgeComplete(void)
{
    SI1CONbits.MTSIACK = 0U;
}
 
enum MAIN_CORE_SYSTEM_STATUS MAIN_CORE_SystemStatusGet(void)
{
    enum MAIN_CORE_SYSTEM_STATUS systemStatus = MAIN_CORE_SYSTEM_STATUS_RUNNING_STATE;
    
    if(SI1STATbits.MSTRST)
    {
        systemStatus = MAIN_CORE_SYSTEM_STATUS_IN_RESET_STATE;
    }
    else
    {
        switch(SI1STATbits.MSTPWR)
        {
            case 0: systemStatus = MAIN_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE;
                    break;
            case 1: systemStatus = MAIN_CORE_SYSTEM_STATUS_IDLE_MODE;
                    break;
            case 2: systemStatus = MAIN_CORE_SYSTEM_STATUS_SLEEP_MODE;
                    break;
            default:systemStatus = MAIN_CORE_SYSTEM_STATUS_RUNNING_STATE;
                    break;             
        }
    }
    return systemStatus;
}

uint16_t MAIN_CORE_FIFORead(uint16_t *pData, uint16_t wordCount)
{
    uint16_t readCountStatus = 0U;
    
    while(wordCount)
    {
        if(!MAIN_CORE_FIFOReadIsEmpty())
        {
            *pData++ = SRMWFDATA;
            wordCount--;
            readCountStatus++;
        }
        else
        {
            break;
        }
    }
    return readCountStatus;  
}

uint16_t MAIN_CORE_FIFOWrite(uint16_t *pData, uint16_t wordCount)
{
    uint16_t writeCountStatus = 0U;
    
    while(wordCount)
    {
        if(!MAIN_CORE_FIFOWriteIsFull())
        {
            SWMRFDATA = *pData++;
            wordCount--;
            writeCountStatus++;
        }
        else
        {
            break;
        }
    }
    return writeCountStatus;
}
 
bool MAIN_CORE_ProtocolWrite(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData)
{
    bool status = false;
    switch(protocolName)
    {
        case MSI1_ProtocolB:
            if(MAIN_CORE_ProtocolIsEmpty(MSI1_ProtocolB))
            {
                SI1MBX1D = pData[0];
                status = true;
            }    
            else
            {
                status = false;
            }
            break;
        default:
            break; 
    }
    return status;    
}

bool MAIN_CORE_ProtocolRead(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData)
{
    bool status = false;   
    switch(protocolName)
    {
        case MSI1_ProtocolA:
            if(MAIN_CORE_ProtocolIsFull(MSI1_ProtocolA))
            {
                pData[0] = SI1MBX0D;

                status = true;
            }
            else
            {
                status = false;
            }
            break;
        default:
            break; 
	}
    return status;
}










