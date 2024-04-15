#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../Source/croutine.c ../../../Source/portable/MemMang/heap_1.c ../../../Source/list.c ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c ../../../Source/queue.c ../../../Source/tasks.c ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S ../../Common/Minimal/BlockQ.c ../../Common/Minimal/blocktim.c ../../Common/Minimal/comtest.c ../../Common/Minimal/crflash.c ../../Common/Minimal/integer.c ../main.c ../ParTest/ParTest.c ../serial/serial.c ../lcd.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/449926602/croutine.o ${OBJECTDIR}/_ext/1884096877/heap_1.o ${OBJECTDIR}/_ext/449926602/list.o ${OBJECTDIR}/_ext/1343266892/port.o ${OBJECTDIR}/_ext/449926602/queue.o ${OBJECTDIR}/_ext/449926602/tasks.o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o ${OBJECTDIR}/_ext/1163846883/BlockQ.o ${OBJECTDIR}/_ext/1163846883/blocktim.o ${OBJECTDIR}/_ext/1163846883/comtest.o ${OBJECTDIR}/_ext/1163846883/crflash.o ${OBJECTDIR}/_ext/1163846883/integer.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/809743516/ParTest.o ${OBJECTDIR}/_ext/821501661/serial.o ${OBJECTDIR}/_ext/1472/lcd.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/449926602/croutine.o.d ${OBJECTDIR}/_ext/1884096877/heap_1.o.d ${OBJECTDIR}/_ext/449926602/list.o.d ${OBJECTDIR}/_ext/1343266892/port.o.d ${OBJECTDIR}/_ext/449926602/queue.o.d ${OBJECTDIR}/_ext/449926602/tasks.o.d ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d ${OBJECTDIR}/_ext/1163846883/blocktim.o.d ${OBJECTDIR}/_ext/1163846883/comtest.o.d ${OBJECTDIR}/_ext/1163846883/crflash.o.d ${OBJECTDIR}/_ext/1163846883/integer.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/809743516/ParTest.o.d ${OBJECTDIR}/_ext/821501661/serial.o.d ${OBJECTDIR}/_ext/1472/lcd.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/449926602/croutine.o ${OBJECTDIR}/_ext/1884096877/heap_1.o ${OBJECTDIR}/_ext/449926602/list.o ${OBJECTDIR}/_ext/1343266892/port.o ${OBJECTDIR}/_ext/449926602/queue.o ${OBJECTDIR}/_ext/449926602/tasks.o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o ${OBJECTDIR}/_ext/1163846883/BlockQ.o ${OBJECTDIR}/_ext/1163846883/blocktim.o ${OBJECTDIR}/_ext/1163846883/comtest.o ${OBJECTDIR}/_ext/1163846883/crflash.o ${OBJECTDIR}/_ext/1163846883/integer.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/809743516/ParTest.o ${OBJECTDIR}/_ext/821501661/serial.o ${OBJECTDIR}/_ext/1472/lcd.o

# Source Files
SOURCEFILES=../../../Source/croutine.c ../../../Source/portable/MemMang/heap_1.c ../../../Source/list.c ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c ../../../Source/queue.c ../../../Source/tasks.c ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S ../../Common/Minimal/BlockQ.c ../../Common/Minimal/blocktim.c ../../Common/Minimal/comtest.c ../../Common/Minimal/crflash.c ../../Common/Minimal/integer.c ../main.c ../ParTest/ParTest.c ../serial/serial.c ../lcd.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CH128MP508
MP_LINKER_FILE_OPTION=,--script=p33CH128MP508.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/449926602/croutine.o: ../../../Source/croutine.c  .generated_files/flags/default/f4e5cd21011ce2b8d5739160681ca6287aecc808 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/croutine.c  -o ${OBJECTDIR}/_ext/449926602/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/croutine.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1884096877/heap_1.o: ../../../Source/portable/MemMang/heap_1.c  .generated_files/flags/default/35b7d226d863ed8aea8e74e6d8fa574cf27c8c77 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1884096877" 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/_ext/1884096877/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1884096877/heap_1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/list.o: ../../../Source/list.c  .generated_files/flags/default/23dc8cf9134ad5235b88aa891cefa18e5ada66f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/list.c  -o ${OBJECTDIR}/_ext/449926602/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/list.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1343266892/port.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/bd76fbfe5b48715fbb1fe65af5e4a2814706f1d2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1343266892/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/port.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/queue.o: ../../../Source/queue.c  .generated_files/flags/default/3fb85e74b92ca22dc658959b14a129408e4aaea0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/queue.c  -o ${OBJECTDIR}/_ext/449926602/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/queue.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/tasks.o: ../../../Source/tasks.c  .generated_files/flags/default/e281f612682a4e3743c5484a6fb338a6b4ca4561 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/tasks.c  -o ${OBJECTDIR}/_ext/449926602/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/tasks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/BlockQ.o: ../../Common/Minimal/BlockQ.c  .generated_files/flags/default/e3905edc74f698a40fffaac956974f3af9dba1c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/BlockQ.c  -o ${OBJECTDIR}/_ext/1163846883/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/BlockQ.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/blocktim.o: ../../Common/Minimal/blocktim.c  .generated_files/flags/default/e98e235d1c907e7466ba0c6ad18c40df95d4c5f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/blocktim.c  -o ${OBJECTDIR}/_ext/1163846883/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/blocktim.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/comtest.o: ../../Common/Minimal/comtest.c  .generated_files/flags/default/43c0ff7f6e500f79420d6388d1db60d0e02c3538 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/comtest.c  -o ${OBJECTDIR}/_ext/1163846883/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/comtest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/crflash.o: ../../Common/Minimal/crflash.c  .generated_files/flags/default/be708d2e57507cca4ffc987e3a1d7d3ecc152378 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/crflash.c  -o ${OBJECTDIR}/_ext/1163846883/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/crflash.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/integer.o: ../../Common/Minimal/integer.c  .generated_files/flags/default/b2df4f25400aa58e7917c86ffde87c4401d5705d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/integer.c  -o ${OBJECTDIR}/_ext/1163846883/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/integer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/8bbd2b048f80c3b904bc055b1774649c3ef97cd1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/809743516/ParTest.o: ../ParTest/ParTest.c  .generated_files/flags/default/3b9ccfeed48c0cc6b5928dedb68c626a26114bbf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/809743516" 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o.d 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ParTest/ParTest.c  -o ${OBJECTDIR}/_ext/809743516/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/809743516/ParTest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/821501661/serial.o: ../serial/serial.c  .generated_files/flags/default/53b50ba84310f151d3a958eff362db52b81bb753 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/821501661" 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../serial/serial.c  -o ${OBJECTDIR}/_ext/821501661/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/821501661/serial.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/lcd.o: ../lcd.c  .generated_files/flags/default/820b457e503c26f700e513f454d1c40457f7a9d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lcd.c  -o ${OBJECTDIR}/_ext/1472/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/lcd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/449926602/croutine.o: ../../../Source/croutine.c  .generated_files/flags/default/2939a6c84723b6c3b5052cf928fdc3e92f5769fa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/croutine.c  -o ${OBJECTDIR}/_ext/449926602/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/croutine.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1884096877/heap_1.o: ../../../Source/portable/MemMang/heap_1.c  .generated_files/flags/default/a9705e4ec16f8e712ef3c5fc1491caca0832d214 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1884096877" 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/_ext/1884096877/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1884096877/heap_1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/list.o: ../../../Source/list.c  .generated_files/flags/default/e3d4b62c9d3f8d5e18657c41f5d0a61728d13b7d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/list.c  -o ${OBJECTDIR}/_ext/449926602/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/list.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1343266892/port.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/2e5c262c321811ce6d36fd80c2b3eed88dc522e8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1343266892/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/port.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/queue.o: ../../../Source/queue.c  .generated_files/flags/default/1cc7efd2797bcc5ff23fc33ef1f7c8c0375f3d56 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/queue.c  -o ${OBJECTDIR}/_ext/449926602/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/queue.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/tasks.o: ../../../Source/tasks.c  .generated_files/flags/default/67148d80eeabc83e1cd5ce07dc7d30219568d68f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/tasks.c  -o ${OBJECTDIR}/_ext/449926602/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/tasks.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/BlockQ.o: ../../Common/Minimal/BlockQ.c  .generated_files/flags/default/77000ff8d05dd5a9d24125de21b5119107262635 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/BlockQ.c  -o ${OBJECTDIR}/_ext/1163846883/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/BlockQ.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/blocktim.o: ../../Common/Minimal/blocktim.c  .generated_files/flags/default/f483cc01125d053e6eb53b4294790ce902d40393 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/blocktim.c  -o ${OBJECTDIR}/_ext/1163846883/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/blocktim.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/comtest.o: ../../Common/Minimal/comtest.c  .generated_files/flags/default/93b1e151ee73bb5cc08b02dbbb0f1bf37b36f6f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/comtest.c  -o ${OBJECTDIR}/_ext/1163846883/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/comtest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/crflash.o: ../../Common/Minimal/crflash.c  .generated_files/flags/default/4da9aae5cf5c24554953658b2fd27d498fd50e66 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/crflash.c  -o ${OBJECTDIR}/_ext/1163846883/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/crflash.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/integer.o: ../../Common/Minimal/integer.c  .generated_files/flags/default/7d0aac6609d8c0c9dad83835d94bad49f3631c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/integer.c  -o ${OBJECTDIR}/_ext/1163846883/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/integer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/37851c724441bd5f69971af19f8f834aca370f45 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/809743516/ParTest.o: ../ParTest/ParTest.c  .generated_files/flags/default/dce763287bec624823c6b846d38861187e8ca5bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/809743516" 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o.d 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ParTest/ParTest.c  -o ${OBJECTDIR}/_ext/809743516/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/809743516/ParTest.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/821501661/serial.o: ../serial/serial.c  .generated_files/flags/default/3ea01915d8bb3233a88346e269acc10e768498b6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/821501661" 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../serial/serial.c  -o ${OBJECTDIR}/_ext/821501661/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/821501661/serial.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/lcd.o: ../lcd.c  .generated_files/flags/default/a1521be392f5fc1a767cb252d6483fdd8d0bc03d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lcd.c  -o ${OBJECTDIR}/_ext/1472/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/lcd.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O0 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  .generated_files/flags/default/1b29b0d0d53066cf6a80ca5f2ae2dab6ec3935d7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  -o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d"  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  .generated_files/flags/default/ea88332c364f0ffd9f66d89c4fe588f4fbfbe9f2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  -o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)    -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/dspic33c-host-freertos-demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}
