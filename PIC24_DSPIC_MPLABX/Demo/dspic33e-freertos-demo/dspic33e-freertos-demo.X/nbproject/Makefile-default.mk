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
FINAL_IMAGE=${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../../../Source/croutine.c ../../../Source/portable/MemMang/heap_1.c ../../../Source/list.c ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S ../../../Source/queue.c ../../../Source/tasks.c ../../Common/Minimal/BlockQ.c ../../Common/Minimal/blocktim.c ../../Common/Minimal/comtest.c ../../Common/Minimal/crflash.c ../../Common/Minimal/integer.c ../main.c ../ParTest/ParTest.c ../serial/serial.c ../timertest.c ../lcd.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/449926602/croutine.o ${OBJECTDIR}/_ext/1884096877/heap_1.o ${OBJECTDIR}/_ext/449926602/list.o ${OBJECTDIR}/_ext/1343266892/port.o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o ${OBJECTDIR}/_ext/449926602/queue.o ${OBJECTDIR}/_ext/449926602/tasks.o ${OBJECTDIR}/_ext/1163846883/BlockQ.o ${OBJECTDIR}/_ext/1163846883/blocktim.o ${OBJECTDIR}/_ext/1163846883/comtest.o ${OBJECTDIR}/_ext/1163846883/crflash.o ${OBJECTDIR}/_ext/1163846883/integer.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/809743516/ParTest.o ${OBJECTDIR}/_ext/821501661/serial.o ${OBJECTDIR}/_ext/1472/timertest.o ${OBJECTDIR}/_ext/1472/lcd.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/449926602/croutine.o.d ${OBJECTDIR}/_ext/1884096877/heap_1.o.d ${OBJECTDIR}/_ext/449926602/list.o.d ${OBJECTDIR}/_ext/1343266892/port.o.d ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d ${OBJECTDIR}/_ext/449926602/queue.o.d ${OBJECTDIR}/_ext/449926602/tasks.o.d ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d ${OBJECTDIR}/_ext/1163846883/blocktim.o.d ${OBJECTDIR}/_ext/1163846883/comtest.o.d ${OBJECTDIR}/_ext/1163846883/crflash.o.d ${OBJECTDIR}/_ext/1163846883/integer.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/809743516/ParTest.o.d ${OBJECTDIR}/_ext/821501661/serial.o.d ${OBJECTDIR}/_ext/1472/timertest.o.d ${OBJECTDIR}/_ext/1472/lcd.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/449926602/croutine.o ${OBJECTDIR}/_ext/1884096877/heap_1.o ${OBJECTDIR}/_ext/449926602/list.o ${OBJECTDIR}/_ext/1343266892/port.o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o ${OBJECTDIR}/_ext/449926602/queue.o ${OBJECTDIR}/_ext/449926602/tasks.o ${OBJECTDIR}/_ext/1163846883/BlockQ.o ${OBJECTDIR}/_ext/1163846883/blocktim.o ${OBJECTDIR}/_ext/1163846883/comtest.o ${OBJECTDIR}/_ext/1163846883/crflash.o ${OBJECTDIR}/_ext/1163846883/integer.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/809743516/ParTest.o ${OBJECTDIR}/_ext/821501661/serial.o ${OBJECTDIR}/_ext/1472/timertest.o ${OBJECTDIR}/_ext/1472/lcd.o

# Source Files
SOURCEFILES=../../../Source/croutine.c ../../../Source/portable/MemMang/heap_1.c ../../../Source/list.c ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S ../../../Source/queue.c ../../../Source/tasks.c ../../Common/Minimal/BlockQ.c ../../Common/Minimal/blocktim.c ../../Common/Minimal/comtest.c ../../Common/Minimal/crflash.c ../../Common/Minimal/integer.c ../main.c ../ParTest/ParTest.c ../serial/serial.c ../timertest.c ../lcd.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512MU810
MP_LINKER_FILE_OPTION=,--script=p33EP512MU810.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/449926602/croutine.o: ../../../Source/croutine.c  .generated_files/flags/default/f6cc9509ac15dbab4ae8c8d7ea75a8fc16bce45a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/croutine.c  -o ${OBJECTDIR}/_ext/449926602/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/croutine.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1884096877/heap_1.o: ../../../Source/portable/MemMang/heap_1.c  .generated_files/flags/default/189b0bfe9a1b9d804730cf2f16ddb8412c28ccae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1884096877" 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/_ext/1884096877/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1884096877/heap_1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/list.o: ../../../Source/list.c  .generated_files/flags/default/6edfb81e52a6da1f223b41013f3f04ff2f52e58e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/list.c  -o ${OBJECTDIR}/_ext/449926602/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/list.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1343266892/port.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/31366bf698b959ae9d17716c58bccc2ac2048731 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1343266892/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/port.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/queue.o: ../../../Source/queue.c  .generated_files/flags/default/e0a8bca7f7428bb847b8a9c36263cc89a08852b1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/queue.c  -o ${OBJECTDIR}/_ext/449926602/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/queue.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/tasks.o: ../../../Source/tasks.c  .generated_files/flags/default/c23bcb1c5500156a17dbdb3524dd28636cfd2f77 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/tasks.c  -o ${OBJECTDIR}/_ext/449926602/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/tasks.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/BlockQ.o: ../../Common/Minimal/BlockQ.c  .generated_files/flags/default/a740ae151b6f145133ebf84df2a82405ebce89f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/BlockQ.c  -o ${OBJECTDIR}/_ext/1163846883/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/BlockQ.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/blocktim.o: ../../Common/Minimal/blocktim.c  .generated_files/flags/default/12e8b1ab7f7b6b37436ce42dafd35d5a1a70858c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/blocktim.c  -o ${OBJECTDIR}/_ext/1163846883/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/blocktim.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/comtest.o: ../../Common/Minimal/comtest.c  .generated_files/flags/default/1cc9a8e816c9eb23bcd6808a4cb415ea8c88313d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/comtest.c  -o ${OBJECTDIR}/_ext/1163846883/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/comtest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/crflash.o: ../../Common/Minimal/crflash.c  .generated_files/flags/default/a3fa857c7b1f83b564ba779482535a2496a2e784 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/crflash.c  -o ${OBJECTDIR}/_ext/1163846883/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/crflash.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/integer.o: ../../Common/Minimal/integer.c  .generated_files/flags/default/fca8a8ef75dacdf5e8422d29ec8d25b936fd8488 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/integer.c  -o ${OBJECTDIR}/_ext/1163846883/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/integer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/16464657b665c0de37ed103ccca0d553c2684b75 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/809743516/ParTest.o: ../ParTest/ParTest.c  .generated_files/flags/default/520fb595739d575b5b7275fbdb3187f1b9fc3e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/809743516" 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o.d 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ParTest/ParTest.c  -o ${OBJECTDIR}/_ext/809743516/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/809743516/ParTest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/821501661/serial.o: ../serial/serial.c  .generated_files/flags/default/9789dcaa883518859904464d2a47a5e622c3e793 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/821501661" 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../serial/serial.c  -o ${OBJECTDIR}/_ext/821501661/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/821501661/serial.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/timertest.o: ../timertest.c  .generated_files/flags/default/806deb01f02c909121b589319cff8981fa74e395 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timertest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timertest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../timertest.c  -o ${OBJECTDIR}/_ext/1472/timertest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/timertest.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/lcd.o: ../lcd.c  .generated_files/flags/default/7d5e996d503272a4f37fed438c4720cdadf9a7b1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lcd.c  -o ${OBJECTDIR}/_ext/1472/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/lcd.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/449926602/croutine.o: ../../../Source/croutine.c  .generated_files/flags/default/eae0bed899e52016017e179a03eda5fbb62d9ca6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/croutine.c  -o ${OBJECTDIR}/_ext/449926602/croutine.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/croutine.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1884096877/heap_1.o: ../../../Source/portable/MemMang/heap_1.c  .generated_files/flags/default/80ad7f666f9d738ea7f67279779567c54a43ac2d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1884096877" 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1884096877/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MemMang/heap_1.c  -o ${OBJECTDIR}/_ext/1884096877/heap_1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1884096877/heap_1.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/list.o: ../../../Source/list.c  .generated_files/flags/default/c2e2e23605b0126bc4f29c4641017fc1d4a86a15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/list.c  -o ${OBJECTDIR}/_ext/449926602/list.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/list.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1343266892/port.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  .generated_files/flags/default/fa00806b818acd5715a7d7f4c8d04f8accbe4c4d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/port.c  -o ${OBJECTDIR}/_ext/1343266892/port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/port.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/queue.o: ../../../Source/queue.c  .generated_files/flags/default/4a6293ff06790a62545cbcb16cf9a5da8f0c15fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/queue.c  -o ${OBJECTDIR}/_ext/449926602/queue.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/queue.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/449926602/tasks.o: ../../../Source/tasks.c  .generated_files/flags/default/c1ae2a20d0aff407861751af9b747e2f44b720d3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/449926602" 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/449926602/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../../Source/tasks.c  -o ${OBJECTDIR}/_ext/449926602/tasks.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/449926602/tasks.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/BlockQ.o: ../../Common/Minimal/BlockQ.c  .generated_files/flags/default/c4bcd10ad23385d1fc1b8d1b195d8f628a735a71 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/BlockQ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/BlockQ.c  -o ${OBJECTDIR}/_ext/1163846883/BlockQ.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/BlockQ.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/blocktim.o: ../../Common/Minimal/blocktim.c  .generated_files/flags/default/592067eb233d93b92dd8251d779e9a169892665b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/blocktim.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/blocktim.c  -o ${OBJECTDIR}/_ext/1163846883/blocktim.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/blocktim.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/comtest.o: ../../Common/Minimal/comtest.c  .generated_files/flags/default/d6574ce42b41470d6866ea7b3c4fc6ca1dfbbcf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/comtest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/comtest.c  -o ${OBJECTDIR}/_ext/1163846883/comtest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/comtest.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/crflash.o: ../../Common/Minimal/crflash.c  .generated_files/flags/default/72069fbc26c738b18621752595852853580ec8da .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/crflash.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/crflash.c  -o ${OBJECTDIR}/_ext/1163846883/crflash.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/crflash.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1163846883/integer.o: ../../Common/Minimal/integer.c  .generated_files/flags/default/12a90149035bfe2ff7ddc8e9279005f679a006ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1163846883" 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1163846883/integer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Common/Minimal/integer.c  -o ${OBJECTDIR}/_ext/1163846883/integer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1163846883/integer.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  .generated_files/flags/default/2d322666c9896d9c50f99c09ed8a889d9a974f9e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../main.c  -o ${OBJECTDIR}/_ext/1472/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/809743516/ParTest.o: ../ParTest/ParTest.c  .generated_files/flags/default/e9d48c26ecb4f4d63961c28e39efcea4bb4d0ef7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/809743516" 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o.d 
	@${RM} ${OBJECTDIR}/_ext/809743516/ParTest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../ParTest/ParTest.c  -o ${OBJECTDIR}/_ext/809743516/ParTest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/809743516/ParTest.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/821501661/serial.o: ../serial/serial.c  .generated_files/flags/default/a54be43f3687b8fa8ec2989f50f9c4fc2d9de381 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/821501661" 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/821501661/serial.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../serial/serial.c  -o ${OBJECTDIR}/_ext/821501661/serial.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/821501661/serial.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/timertest.o: ../timertest.c  .generated_files/flags/default/b894dc924d10a71bf4357ecac0b221d8829d929c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/timertest.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/timertest.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../timertest.c  -o ${OBJECTDIR}/_ext/1472/timertest.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/timertest.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1472/lcd.o: ../lcd.c  .generated_files/flags/default/6e836646772cb9c7e972bb8aad28509bdf4cd4a1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../lcd.c  -o ${OBJECTDIR}/_ext/1472/lcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1472/lcd.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -ffunction-sections -O2 -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -msmart-io=1 -Wall -msfr-warn=off   -fno-schedule-insns -fno-schedule-insns2  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  .generated_files/flags/default/157e36a0fabe46e48f742991a7ca1746797be840 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  -o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d"  -D__DEBUG   -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o: ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  .generated_files/flags/default/50973376d05c51fd168fd811f9b06913950cd599 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1343266892" 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d 
	@${RM} ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../../Source/portable/MPLAB/PIC24_dsPIC/portasm_dsPIC.S  -o ${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.d"  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    -I".." -I"../../../../../Demo/dsPIC_MPLAB" -I"../../../../Demo/dsPIC_MPLAB" -I"../../../../Source/include" -I"../../../../include" -I"../../../Common/include" -I"../../../Source/include" -I"../../../include" -I"../../Common/include" -I"../../Demo/dsPIC_MPLAB" -I"../../include" -I"../FileSystem" -I"../include" -I"." -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1343266892/portasm_dsPIC.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)  -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--heap=0,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=0,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/dspic33e-freertos-demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}
