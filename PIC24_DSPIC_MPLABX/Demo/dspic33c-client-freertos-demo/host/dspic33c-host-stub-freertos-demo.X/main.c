#include <xc.h>
#include <libpic30.h>
#include <string.h>
#include "sec_core1.h"

#pragma config FNOSC = FRC    //Master Oscillator Source Selection->Internal Fast RC (FRC)
#pragma config ICS = PGD2     //Master ICD Communication Channel Select bits->Communicate on PGC3 and PGD3

// FCFGPRA0
#pragma config CPRA0 = SLV1    //Pin RA0 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRA1 = SLV1    //Pin RA1 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRA2 = SLV1    //Pin RA2 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRA3 = SLV1    //Pin RA3 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRA4 = SLV1    //Pin RA4 Ownership Bits->Slave 1 core owns pin.

// FCFGPRB0
#pragma config CPRB0 = SLV1    //Pin RB0 Ownership Bits->Master core owns pin.
#pragma config CPRB1 = SLV1    //Pin RB1 Ownership Bits->Master core owns pin.
#pragma config CPRB2 = SLV1    //Pin RB2 Ownership Bits->Master core owns pin.
#pragma config CPRB3 = SLV1    //Pin RB3 Ownership Bits->Master core owns pin.
#pragma config CPRB4 = SLV1    //Pin RB4 Ownership Bits->Master core owns pin.
#pragma config CPRB5 = SLV1    //Pin RB5 Ownership Bits->Master core owns pin.
#pragma config CPRB6 = SLV1    //Pin RB6 Ownership Bits->Master core owns pin.
#pragma config CPRB7 = SLV1    //Pin RB7 Ownership Bits->Master core owns pin.
#pragma config CPRB8 = SLV1    //Pin RB8 Ownership Bits->Master core owns pin.
#pragma config CPRB9 = SLV1    //Pin RB9 Ownership Bits->Master core owns pin.
#pragma config CPRB10 = SLV1    //Pin RB10 Ownership Bits->Master core owns pin.
#pragma config CPRB11 = SLV1    //Pin RB11 Ownership Bits->Master core owns pin.
#pragma config CPRB12 = SLV1    //Pin RB12 Ownership Bits->Master core owns pin.
#pragma config CPRB13 = SLV1    //Pin RB13 Ownership Bits->Master core owns pin.
#pragma config CPRB14 = SLV1    //Pin RB14 Ownership Bits->Master core owns pin.
#pragma config CPRB15 = SLV1    //Pin RB15 Ownership Bits->Master core owns pin.

// FCFGPRC0
#pragma config CPRC0 = SLV1    //Pin RC0 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC1 = SLV1    //Pin RC1 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC2 = SLV1    //Pin RC2 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC3 = SLV1    //Pin RC3 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC4 = SLV1    //Pin RC4 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC5 = SLV1    //Pin RC5 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC6 = SLV1    //Pin RC6 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC7 = SLV1    //Pin RC7 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC8 = SLV1    //Pin RC8 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC9 = SLV1    //Pin RC9 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC10 = SLV1    //Pin RC10 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC11 = SLV1    //Pin RC11 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC12 = SLV1    //Pin RC12 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC13 = SLV1    //Pin RC13 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC14 = SLV1    //Pin RC14 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRC15 = SLV1    //Pin RC15 Ownership Bits->Slave 1 core owns pin.

// FCFGPRE0
#pragma config CPRE0 = SLV1    //Pin RE0 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE1 = SLV1    //Pin RE1 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE2 = SLV1    //Pin RE2 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE3 = SLV1    //Pin RE3 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE4 = SLV1    //Pin RE4 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE5 = SLV1    //Pin RE5 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE6 = SLV1    //Pin RE6 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE7 = SLV1    //Pin RE7 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE8 = SLV1    //Pin RE8 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE9 = SLV1    //Pin RE9 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE10 = SLV1    //Pin RE10 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE11 = SLV1    //Pin RE11 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE12 = SLV1    //Pin RE12 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE13 = SLV1    //Pin RE13 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE14 = SLV1    //Pin RE14 Ownership Bits->Slave 1 core owns pin.
#pragma config CPRE15 = SLV1    //Pin RE15 Ownership Bits->Slave 1 core owns pin.


#pragma config S1FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled
#pragma config S1ICS = PGD3        //SLAVE ICD Communication Channel Select bits->Communicate on PGC2 and PGD2
#pragma config S1FNOSC = FRC       //Oscillator Source Selection->Internal Fast RC (FRC)


#pragma config MBXM0 = M2S    //Mailbox 0 data direction->Mailbox register configured for Main data write (Main to Secondary data transfer)
#pragma config MBXM1 = S2M    //Mailbox 1 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM2 = S2M    //Mailbox 2 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM3 = S2M    //Mailbox 3 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM4 = S2M    //Mailbox 4 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM5 = S2M    //Mailbox 5 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM6 = S2M    //Mailbox 6 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM7 = S2M    //Mailbox 7 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM8 = S2M    //Mailbox 8 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM9 = S2M    //Mailbox 9 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM10 = S2M    //Mailbox 10 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM11 = S2M    //Mailbox 11 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM12 = S2M    //Mailbox 12 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM13 = S2M    //Mailbox 13 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM14 = S2M    //Mailbox 14 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)
#pragma config MBXM15 = S2M    //Mailbox 15 data direction->Mailbox register configured for Main data read (Secondary to Main data transfer)

// FMBXHS1
#pragma config MBXHSA = MBX0    //Mailbox handshake protocol block A register assignment->MSIxMBXD0 assigned to mailbox handshake protocol block A
#pragma config MBXHSB = MBX1    //Mailbox handshake protocol block B register assignment->MSIxMBXD1 assigned to mailbox handshake protocol block B
#pragma config MBXHSC = MBX15    //Mailbox handshake protocol block C register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block C
#pragma config MBXHSD = MBX15    //Mailbox handshake protocol block D register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block D

// FMBXHS2
#pragma config MBXHSE = MBX15    //Mailbox handshake protocol block E register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block E
#pragma config MBXHSF = MBX15    //Mailbox handshake protocol block F register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block F
#pragma config MBXHSG = MBX15    //Mailbox handshake protocol block G register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block G
#pragma config MBXHSH = MBX15    //Mailbox handshake protocol block H register assignment->MSIxMBXD15 assigned to mailbox handshake protocol block H

// FMBXHSEN
#pragma config HSAEN = ON    //Mailbox A data flow control protocol block enable->Mailbox data flow control handshake protocol block enabled
#pragma config HSBEN = ON    //Mailbox B data flow control protocol block enable->Mailbox data flow control handshake protocol block enabled
#pragma config HSCEN = OFF    //Mailbox C data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.
#pragma config HSDEN = OFF    //Mailbox D data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.
#pragma config HSEEN = OFF    //Mailbox E data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.
#pragma config HSFEN = OFF    //Mailbox F data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.
#pragma config HSGEN = OFF    //Mailbox G data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.
#pragma config HSHEN = OFF    //Mailbox H data flow control protocol block enable->Mailbox data flow control handshake protocol block disabled.

// FS1OSCSEL
#pragma config S1IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FS1OSC
#pragma config S1OSCIOFNC = OFF    //Secondary OSC2 Pin Function bit->OSC2 is clock output
#pragma config S1PLLKEN = S1PLLKEN_ON    //S1PLLKEN->reserved as S1PLLKEN_ON

// FS1WDT
#pragma config S1RWDTPS = PS1048576    //Run Mode Watchdog Timer Post Scaler select bits->1:1048576
#pragma config S1RCLKSEL = LPRC    //Watchdog Timer Clock Select bits->Always use LPRC
#pragma config S1WINDIS = ON    //Watchdog Timer Window Enable bit->Watchdog Timer operates in Non-Window mode
#pragma config S1WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config S1SWDTPS = PS1048576    //Sleep Mode Watchdog Timer Post Scaler select bits->1:1048576
#pragma config S1FWDTEN = ON_SW    //Watchdog Timer Enable bit->WDT controlled via WDTCON.ON bit

// FS1ICD
#pragma config S1ISOLAT = ON    //Isolate the Secondary core subsystem from the Main subsystem during Debug->The Secondary can operate (in debug mode) even if the SLVEN bit in the MSI is zero.
#pragma config S1NOBTSWP = OFF    //BOOTSWP Instruction Enable/Disable bit->BOOTSWP instruction is disabled

// FS1DEVOPT
#pragma config S1ALTI2C1 = OFF    //Alternate I2C1 Pin bit->I2C1 mapped to SDA1/SCL1 pins
#pragma config S1SPI1PIN = PPS    //S1 SPI1 Pin Select bit->Secondary SPI1 uses I/O remap (PPS) pins
#pragma config S1SSRE = OFF    //Secondary Secondary Reset Enable->Secondary generated resets will not reset the Secondary Enable Bit in the MSI module
#pragma config S1MSRE = OFF    //Main Secondary Reset Enable->The Main software oriented RESET events (RESET Op-Code, Watchdog timeout, TRAP reset, illegalInstruction) will not cause the Secondary subsystem to reset.

// FS1ALTREG
#pragma config S1CTXT1 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 1 bits->Not Assigned
#pragma config S1CTXT2 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 2 bits->Not Assigned
#pragma config S1CTXT3 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 3 bits->Not Assigned
#pragma config S1CTXT4 = OFF    //Specifies Interrupt Priority Level (IPL) Associated to Alternate Working Register 4 bits->Not Assigned

#define MESSAGE_COUNT 5
uint16_t dataSend[MSI1_ProtocolA_SIZE]; //for Protocol size refer sec_core_types.h file

int main() {
    
    uint16_t messageIndex = 0;
    uint16_t message[MESSAGE_COUNT] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE};
    SEC_CORE1_Initialize();
    
    //Program and enable secondary core
    SEC_CORE1_Program();
    SEC_CORE1_Start();

    while (1) {
        
        dataSend[0] = message[messageIndex%MESSAGE_COUNT];
        
        SEC_CORE1_ProtocolWrite(MSI1_ProtocolA, (uint16_t*) dataSend);
        SEC_CORE1_InterruptRequestGenerate();
        while (!SEC_CORE1_IsInterruptRequestAcknowledged());
        SEC_CORE1_InterruptRequestComplete();
        while (SEC_CORE1_IsInterruptRequestAcknowledged());
        
        messageIndex++;
    }

}


