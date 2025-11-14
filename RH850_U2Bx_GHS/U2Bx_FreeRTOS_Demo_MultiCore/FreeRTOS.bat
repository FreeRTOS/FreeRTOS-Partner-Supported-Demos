@echo off
setlocal EnableDelayedExpansion
set Filename=%~n0

echo **************************************************************************
echo Run %Filename% ...
echo ***************************************************************************
echo.
IF "%GNUMAKE%" == "" (GOTO :noGnuMake) ELSE (GOTO :GnuMake)

:noGnuMake
echo If you have make.exe located in a different directory
echo then your standard path, please add a local
echo user variable named 'GNUMAKE' to your Windows
echo environment variables and specified the location
echo of make.exe by using this variable.
echo.
echo Your current path variable is
echo.
echo %path%
echo.
echo ***************************************************************************
GOTO :continue

:GnuMake
set path=%GNUMAKE%
echo Found user variable GNUMAKE ...
echo ***************************************************************************
echo Temporary modify path variable to be sure to use correct make, shell...
echo path = %path%
echo **************************************************************************
GOTO :continue

:continue

SETLOCAL

SET ROOT_FOLDER=.\

::Specify the Family and Micro Variant
SET MICRO_FAMILY=X2x

SET MICRO_VARIANT=U2Bx

::CPU Option
:: CPUn_OPTION is CPUn_USED when the CPUn used
SET CPU0_OPTION=CPU0_USED
SET CPU1_OPTION=CPU1_USED
SET CPU2_OPTION=CPU2_USED

IF "%COMPILER_INSTALL_DIR%"=="" (
SET COMPILER_INSTALL_DIR=C:\ghs\comp_202354
)

SET DEVICE_NAME=%1

IF "%1" NEQ "70255xAFABB" ^

 GOTO :Help
)

::To Set the Sub Variant based on the Device.
CALL :SUB_VARIANT

::To Set the Max CPU Number on the Device.
CALL :TOTAL_CPU_NUMBER

:: To convert the case of Module name entered to Uppercase for Display
CALL :CONV_VAR_to_LOC MODULE
SET MODULE_PCL=%MODULE:~0,1%
CALL :CONV_VAR_to_UPC MODULE_PCL
SET MODULE_PCL=%MODULE_PCL%%MODULE:~1%
CALL :CONV_VAR_to_UPC MODULE

If not exist "%ROOT_FOLDER%" (
echo Error! Root Folder %ROOT_FOLDER% doesn't exist.
GOTO :End
)

If not exist "%COMPILER_INSTALL_DIR%" (
echo Error! Compiler installed directory %COMPILER_INSTALL_DIR% doesn't exist.
GOTO :End
)

SET OPTION=no

IF "%OPTION%"=="no" (
echo ===========================================================================
echo                            CLEAN AND RE-BUILD
echo ===========================================================================
make -f FreeRTOS.mak clean
make -f FreeRTOS.mak clean_all
make -f FreeRTOS.mak App_FreeRTOS.out
)

echo.
echo ===========================================================================
echo                        BUILDING COMPLETED
echo ===========================================================================
GOTO :eof


:CONV_VAR_to_LOC
  FOR %%z IN (a b c d e f g h i j k l m n o p q r s t u v w x y z) DO CALL set %~1=%%%~1:%%z=%%z%%
GOTO :eof

:CONV_VAR_to_UPC
  FOR %%z IN (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) DO CALL set %~1=%%%~1:%%z=%%z%%
GOTO :eof


:Help
echo.
echo ===========================================================================
echo                        HELP TO BUILD SAMPLE APPLICATION
echo ===========================================================================
echo Usage:
echo.
echo FreeRTOS.bat DEVICE
echo DEVICE (*)                          -   Device Name to be compiled which is available :
echo.                                        U2B6       : 70255xAFABB
echo.
echo Note: (*) is mandantory input argument
echo ===========================================================================
goto :END

:SUB_VARIANT
SET MICRO_SUB_VARIANT=U2B6


:TOTAL_CPU_NUMBER
SET TOTAL_CPU=3

GOTO :eof

:Display_Env
echo Display Env Variables
echo =================================
echo DEVICE_NAME=%DEVICE_NAME%
echo =================================


:End

ENDLOCAL

@echo on
