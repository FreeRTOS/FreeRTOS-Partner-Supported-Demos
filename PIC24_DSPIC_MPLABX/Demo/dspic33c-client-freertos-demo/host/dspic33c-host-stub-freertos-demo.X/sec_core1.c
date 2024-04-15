/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include <stddef.h>
#include <libpic30.h>
#include "sec_core_types.h"
#include "sec_core1.h"
#include "dspic33c_client_freertos_demo.h"

#define SECONDARY_IMAGE     dspic33c_client_freertos_demo
#define SECONDARY_NUMBER    1

/*
    Backward compatibility DFP package support
    dsPIC33CH-MP_DFP version: < v1.3.125
*/
#ifndef _MTSIRQ
#define _MTSIRQ _MSTIRQ
#endif

void SEC_CORE1_Initialize(void)
{
    //SRSTIE disabled; STMIACK disabled; MSTIRQ disabled; RFITSEL Trigger data valid interrupt when 1st FIFO entry is written by Slave; SLVEN disabled; 
    MSI1CON = 0x0U;    
    //RFEN disabled; WFEN disabled; 
    MSI1FIFOCS = 0x0U;    

    
}

void SEC_CORE1_Deinitialize(void)
{
    MSI1CON = 0x0U;    
    MSI1FIFOCS = 0x0U;    

}

void SEC_CORE1_Start(void)
{
    _start_secondary();
}

void SEC_CORE1_Program(void)               
{
    _program_secondary(SECONDARY_NUMBER,0,SECONDARY_IMAGE);
}

void SEC_CORE1_InterruptRequestGenerate(void)
{
    MSI1CONbits.MTSIRQ = 1U;
}

bool SEC_CORE1_IsInterruptRequestAcknowledged(void)
{
    return(MSI1STATbits.MTSIACK);
    
}

void SEC_CORE1_InterruptRequestComplete(void)
{
    MSI1CONbits.MTSIRQ = 0U;
}
 
bool SEC_CORE1_IsInterruptRequested(void)
{
    return(MSI1STATbits.STMIRQ);
}
void SEC_CORE1_InterruptRequestAcknowledge(void)
{
    MSI1CONbits.STMIACK = 1U;
}

void SEC_CORE1_InterruptRequestAcknowledgeComplete(void)
{
    MSI1CONbits.STMIACK = 0U;
}

enum SEC_CORE_RESET_CAUSE SEC_CORE1_GetResetCause(void)
{
    enum SEC_CORE_RESET_CAUSE resetCause;
    
    if(MSI1STATbits.SLVWDRST)
    {
        resetCause = SEC_CORE_RESET_CAUSE_WATCHDOG;
    }
    else
    {
        resetCause = SEC_CORE_RESET_CAUSE_UNSPECIFIED;
    }
    
    return resetCause;
}

void SEC_CORE1_ResetCauseClear(enum SEC_CORE_RESET_CAUSE resetCause)
{
    switch(resetCause)
    {
        case SEC_CORE_RESET_CAUSE_WATCHDOG :   MSI1STATbits.SLVWDRST = 0U;
                                                    break;
        default                         :
                                            break;
    }
}
 
enum SEC_CORE_SYSTEM_STATUS SEC_CORE1_SystemStatusGet(void)
{
    enum SEC_CORE_SYSTEM_STATUS systemStatus = SEC_CORE_SYSTEM_STATUS_RUNNING_STATE;
    
    if(MSI1STATbits.SLVRST)
    {
        systemStatus = SEC_CORE_SYSTEM_STATUS_IN_RESET_STATE;
    }
    else
    {
        switch(MSI1STATbits.SLVPWR)
        {
            case 0: systemStatus = SEC_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE;
                    break;
            case 1: systemStatus = SEC_CORE_SYSTEM_STATUS_IDLE_MODE;
                    break;
            case 2: systemStatus = SEC_CORE_SYSTEM_STATUS_SLEEP_MODE;
                    break;
            default:systemStatus = SEC_CORE_SYSTEM_STATUS_RUNNING_STATE;
                    break;             
        }
    }
    return systemStatus;
}

uint16_t SEC_CORE1_FIFORead(uint16_t *pData, uint16_t wordCount)
{
    uint16_t readCountStatus = 0U;
    
    while(wordCount)
    {
        if(!SEC_CORE1_FIFOReadIsEmpty())
        {
            *pData++ = MRSWFDATA;
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

uint16_t SEC_CORE1_FIFOWrite(uint16_t *pData, uint16_t wordCount)
{
    uint16_t writeCountStatus = 0U;
    
    while(wordCount)
    {
        if(!SEC_CORE1_FIFOWriteIsFull())
        {
            MWSRFDATA = *pData++;
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
 
bool SEC_CORE1_ProtocolWrite(enum SEC_CORE_PROTOCOLS protocolName, uint16_t *pData)
{
    bool status = false;
    switch(protocolName)
    {
        case MSI1_ProtocolA:
            if(SEC_CORE1_ProtocolIsEmpty(MSI1_ProtocolA))
            {
                MSI1MBX0D = pData[0];
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

bool SEC_CORE1_ProtocolRead(enum SEC_CORE_PROTOCOLS protocolName, uint16_t *pData)
{
    bool status = false;   
    switch(protocolName)
    {
        case MSI1_ProtocolB:
            if(SEC_CORE1_ProtocolIsFull(MSI1_ProtocolB))
            {
                pData[0] = MSI1MBX1D;

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




