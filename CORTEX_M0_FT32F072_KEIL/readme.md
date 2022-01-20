# Overview  

This directory contains an KEIL demo project (ARM compiler V6.16) for FT32F072X8 board equipped with FT32F072X8 microcontroller (64 KB Flash, 8 KB SRAM and the FT32F072XB is going to come into the market which 128 KB Flash, 24 KB SRAM ).FT32F072X8 is one of the 32-bit microcontroller which is designed by Fremont Micro Devices Corporation. Go for the hardware page [here](https://www.fremontmicro.com/product/32%20bit%20mcu/arm%20core_1/index.aspx).  

The project comes with 2 different demos, selectable by a define in the "config.h" file. Each demo has its own main_demo_name.c  files. Follow the instructions on the RTOS port documentation page for details about how to setup the target hardware, download and execute the demo application.

# Standar Test Demo  

Add a define in the "config.h" file as follows  
#define   mainCREATE_DEMO_ONLY	0  
Main_standartest is a test project follow per the instructions in this page [here](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/portable/ThirdParty/README.md).    
And it use the USART to printf,all the test output no errors.  

# Main_demo  

The whole functionality is included in the main-demo.c file by using the following tasks.    
InitTask is a task to initialize the mcu peripherals,Such as initialize the GPIO as LED light, initialize USART,ADC and TIMER, then delete itself.  
LowproTask is low priorities task,It handles low-priority work,such as the LED_Flash fun.  
HighProTask is high priorities task,It handles high-priority work,such as the key pressing and the usart sending and receiving.  
And tasks communicate through queues
