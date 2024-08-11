![image](../../../images/microchip.jpg) 

## dsPIC33A FreeRTOS Demo

## Summary

This folder contains the freeRTOS demo for dsPIC33AK128MC106 device. 
For this demo, MPLAB X and MPLAB XC-DSC are the preferred IDE and compiler respectively with which to build the FreeRTOS demos. 
The board to be used to run this demo is dsPIC33A Curiosity Platform Development Board.
The version of freeRTOS used in this demo is : freeRTOS v10.5

## Related Documentation

+ The FreeRTOS/Source directory contains the FreeRTOS source code, and contains
  its own readme file.

+ See http://www.freertos.org/a00017.html for full details of the directory 
  structure and information on locating the files you require.
  
+ See http://www.freertos.org/portpic24_dspic.html#DemoApp for full details above the demos.
  
The easiest way to use FreeRTOS is to start with one of the pre-configured demo 
application projects (found in the pic24-dspic33-freertos-demo/Demo directory).  That way you will
have the correct FreeRTOS source files included, and the correct include paths
configured.  Once a demo application is building and executing you can remove
the demo application file, and start to add in your own application source
files.

See also -
http://www.freertos.org/FreeRTOS-quick-start-guide.html
http://www.freertos.org/FAQHelp.html

## Software Used 

- MPLAB® X IDE v6.20.0 or newer (https://www.microchip.com/mplabx)
- MPLAB® XC-DSC v3.10.0 or newer (https://www.microchip.com/xcdsc) 
- Any of the serial terminal application. Example: Tera Term (https://ttssh2.osdn.jp/index.html.en)


## Hardware Used

- dsPIC33A Curiosity Platform Development Board (https://www.microchip.com/EV74H48A)
- dsPIC33AK128MC106 DIM (https://www.microchip.com/EV02G02A)

## Hardware Setup
- For UART communication task - short pins p100 and p102 on Jumper J10.

