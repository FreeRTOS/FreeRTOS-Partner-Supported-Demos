/**
 * TRAPS Generated Driver Header File 
 * 
 * @file      traps.h
 *            
 * @defgroup  trapsdriver Traps Driver
 *            
 * @brief     Traps driver with handler for all types of traps using dsPIC MCUs.           
 *
 * @skipline @version   PLIB Version 1.0.1-rc.1
 *            
 * @skipline  Device : dsPIC33AK128MC106
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef TRAPS_H
#define TRAPS_H

// Section: Included Files
#include <stdint.h>

// Section: Data Type Definitions

/**
 @ingroup  trapsdriver
 @enum     TRAPS_ERROR_CODE
 @brief    Defines the TRAPS error codes
*/
enum TRAPS_ERROR_CODE 
{
// Traps
    TRAPS_DMA_BUS_ERR = 0, /**< Bus error. */
    TRAPS_ILLEGALINSTRUCTION = 1, /**< Illegal instruction. */
    TRAPS_ADDRESS_ERR = 2, /**< Address error. */
    TRAPS_STACK_ERR = 3, /**< Stack error. */
    TRAPS_DIV0_ERR = 4, /**< Math error. */
    TRAPS_DMT_ERR = 5, /**< General error. */
    TRAPS_WDT_ERR = 6, /**< General error. */
};

// Section: Driver Interface Function

/**
 * @ingroup    trapsdriver
 * @brief      Stores the trap error code and waits forever.
 *             This is a weak attribute function. The user can 
 *             override and implement the same function without weak attribute.
 * @param[in]  code - trap error code
 * @return     none  
 */
void TRAPS_halt_on_error(uint16_t code);

#endif

