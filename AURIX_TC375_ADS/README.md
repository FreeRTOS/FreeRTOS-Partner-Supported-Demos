# FreeRTOS Port for Infineon AURIX™ TC3xx Devices

## Getting Started with FreeRTOS on AURIX™ TC3xx
All the currently available examples/demos are based on the [AURIX™ Development Studio (ADS)](https://www.infineon.com/cms/en/product/promopages/aurix-development-studio/), which includes a free TASKING compiler and debugger. The examples are built for the entry level [AURIX™ TC375 Lite Kit](https://www.infineon.com/cms/en/product/evaluation-boards/kit_a2g_tc375_lite/) but should run on any TC3xx device/hardware with minor changes. All demos use the [Infineon Low Level Drivers (iLLDs)](https://www.infineon.com/cms/en/tools/aurix-embedded-sw/aurix-illd-drivers/), but a separate download is not required since they are included in ___AURIX™ Development Studio___

There are two demos available in this folder:
### Blinky
A simple blinky code example which blinks an LED cyclically in a FreeRTOS task.
- ___FreeRTOSConfig.h___ is located under the ___Configurations/___ folder
- The `main(...)` entry point for the program is in ___Cpu0_Main.c___
- The source of the task which blinks the LED can be found in ___Blinky.c___

### Gpt12_Printf
Creates one task which blinks `LED1` cyclically and polls `BUTTON1` and toggles the state of `LED2` whenever it is pressed. A second task blocks on `ulTaskNotifyTake(...)` and is notified from a Timer (GPT12) interrupt and then prints a message to the serial port using `printf(...)`
- ___FreeRTOSConfig.h___ is located under the ___Configurations/___ folder
- The `main(...)` entry point for the program is in ___Cpu0_Main.c___
- The source of the task which polls `BUTTON1` and controls `LED1` and `LED2`, can be found in ___Blinky.c___
- The GPT12 timer setup, the corresponding ISR and the task which prints a message to the serial port after each timer interrupt, can be found in ___Timer_Gpt12.c___

## Pre-requisites
- Download and install the latest version of ___AURIX™ Development Studio___ [from here](https://softwaretools.infineon.com/tools/com.ifx.tb.tool.aurixide) or install it using the [Infineon Developer Center Launcher](https://www.infineon.com/cms/en/design-support/tools/utilities/infineon-developer-center-idc-launcher/?redirId=102702)
- The demos were developed for the ___AURIX™ TC375 Lite Kit___, which comes with an onboard miniWiggler USB debug interface and also contains a built-in serial interface (this is the serial port used by default for the ___iLLD_TC375_ADS_Timer_Interrupt___ demo). More details about the ___AURIX™ TC375 Lite Kit___ can be found [here](https://www.infineon.com/cms/en/product/evaluation-boards/kit_a2g_tc375_lite/)
    - The demos, however, can be ported to any TC3xx board with minimal changes
    - The port pins used for LEDs and the BUTTON are defined using macros at the top of the files which uses them, e.g.
    ```C
    #define LED_1       IfxPort_P00_5       /* Port/Pin for LED 1     */
    #define LED_2       IfxPort_P00_6       /* Port/Pin for LED 2     */
    #define BUTTON_1    IfxPort_P00_7       /* Port/Pin for BUTTION 1 */
    ```
  
    - The serial port (ASCLINx) module, port pins and baudrate configuration can be found at the top of ___Cpu0_Main.c___#, i.e.
    ```C
    /* ASCLIN0 module and UART TX/RX pin selection */
    SERIALIO_t SERIALIO =
    {
        .asclin = &MODULE_ASCLIN0,
        .tx_pin = &IfxAsclin0_TX_P14_0_OUT,
        .rx_pin = &IfxAsclin0_RXA_P14_1_IN
    };
    ```

## Creating an AURIX™ Development Studio Project
### 1. __File__ → __New__ → __New AURIX Project__, type in a name for the project and click "__Next__"  
<img src="images/ads-new-project-0.png">
<br>
<img src="images/ads-new-project-1.png">

### 2. In the right column titled "__Board__", select the "__AURIX TC375 lite Kit__" and click "__Finish__"  
<img src="images/ads-new-project-2.png">

### 3. Go to the folder where the project was created and copy over the contents of any demo folder to the root of the project folder, e.g.  
- The project folder can be accessed as shown below...  
<img src="images/ads-new-project-3.png">

- Opening the project folder will show something like this...  
<img src="images/ads-new-project-4.png">

- Copy over the demo contents, for instance, the "__Blinky__" demo...  
<img src="images/blinky-example.png">

### 4. Add the FreeRTOS Kernel and the corresponding AURIX™ TC3xx portables in a folder called "__FreeRTOS-Kernel__". The AURIX™ TC3xx FreeRTOS port used for these demos is available in the ___Tasking/AURIX_TC3xx___ folder in the [___Partner Supported Ports___ repository](https://github.com/FreeRTOS/FreeRTOS-Kernel-Partner-Supported-Ports)

### 5. The final directory structure should look, for example, something like this:  
<img src="./images/directory-structure.png">  

### 6. Open __AURIX™ Development Studio__ and refresh the project:  
<img src="./images/ads-refresh-project.png">

### 7. Select a heap implementation and exclude the rest from the build as shown below: 
<img src="./images/ads-heap-selection.png">

### 8. A TC375 FreeRTOS example that works out-of-the-box is also available in the ___Infineon Code Examples Repository___, which can be accessed using the ___Import___ menu inside __AURIX™ Development Studio__.
<img src="images/ads-import-icer.png" width=500px>
<img src="images/ads-ifx-code-examples-repo.png" width=750px>

## Run and Test
- Compile the code using the _**Build Active Project**_ button (![](./images/build_activeproj.gif)) in the toolbar or by right-clicking the project name and selecting _**Build Project**_ (if it is not already the active project, right click on the respective demo project and click ___Set Active Project___)
- Connect the lite kit to the PC using a micro-USB cable
- Click the **Debug Active Project** button (<img src="./images/debug_activeproj.gif"/>) to flash and debug the project. When the "Debug Condifuration" dialog pops-up, double-click the "__winIDEA Debugger__" to create a debug configuration and click **Close**. Now click **Debug Active Project** button  (<img src="./images/debug_activeproj.gif"/>) and wait for the debugger window to open  
<img src="images/ads-debugger-config.png">

Once the debugger opens, the code will stop at a default startup breakpoint, click <img src="./images/debug_resume.gif" height=15px width=20px/> or press F5 to continue.

## References  

AURIX™ Development Studio is available online:  
- <https://www.infineon.com/aurixdevelopmentstudio>  
- Use the "Import..." function to get access to more code examples  

AURIX™ TC3xx User Manual:
- Part 1: <https://www.infineon.com/dgdl/Infineon-AURIX_TC3xx_Part1-UserManual-v02_00-EN.pdf?fileId=5546d462712ef9b701717d3605221d96>
- Part 2: <https://www.infineon.com/dgdl/Infineon-AURIX_TC3xx_Part2-UserManual-v02_00-EN.pdf?fileId=5546d462712ef9b701717d35f8541d94>

FreeRTOS Quick Start Guide:
- <https://www.freertos.org/FreeRTOS-quick-start-guide.html>

More code examples can be found on the GIT repository:  
- <https://github.com/Infineon/AURIX_code_examples>  

For additional trainings, visit our webpage:  
- <https://www.infineon.com/aurix-expert-training>  

For questions and support, use the AURIX™ Forum:  
- <https://community.infineon.com/t5/AURIX/bd-p/AURIX>  
