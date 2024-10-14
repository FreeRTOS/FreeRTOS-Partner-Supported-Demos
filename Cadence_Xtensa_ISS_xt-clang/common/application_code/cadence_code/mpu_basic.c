/*
 * FreeRTOS Kernel V11.0.0
 * Copyright (c) 2024 Cadence Design Systems, Inc.
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/*
 * This file demonstrates the use of FreeRTOS-MPU.  It creates tasks in both
 * User mode and Privileged mode, and using both the xTaskCreate() and
 * xTaskCreateRestricted() API functions. Total of 4 tasks. Each task will check
 * if it can access different memories. Privileged tasks will check if the MPU
 * has expected attributes. Restricted tasks will check if their private memories
 * have expected attributes.
 * NOTE: MUST compile with -O0 to allow the limited processing in exception handler
 * which expects not-bundled s32i, s32i.n, l32i, l32i.n instructions only.
 */

/* Standard includes. */
#include "string.h"
#include "stdio.h"
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include "stream_buffer.h"
#include "xtensa_api.h"
#include <xtensa/xtutil.h>

/* Parameters that are passed into the third and fourth register check tasks
 * solely for the purpose of ensuring parameters are passed into tasks correctly. */
#define configREG_TASK1_TEST_PARAMETER      ( ( void * ) 0x11112222 )
#define configREG_TASK2_TEST_PARAMETER      ( ( void * ) 0x11113333 )
#define configREG_TASK3_TEST_PARAMETER      ( ( void * ) 0x12345678 )
#define configREG_TASK4_TEST_PARAMETER      ( ( void * ) 0x87654321 )/* Definitions for the messages that can be sent to the check task. */

/*-----------------------------------------------------------*/

/* Misc constants. */
#define mainDONT_BLOCK                    ( 0 )

/* GCC specifics. */
#define mainALIGN_TO( x )                __attribute__((aligned(x)))

/* The period of the timer must be less than the rate at which
configPRINT_SYSTEM_STATUS messages are sent to the check task - otherwise the
check task will think the timer has stopped. */
#define mainTIMER_PERIOD                pdMS_TO_TICKS( 2000 )

#include <xtensa/config/core.h>
#include <xtensa/core-macros.h>
/* Read MPU to observe settings for each tread so they can be compared with
 * what's expected. However, the full testing doing automated invalid/valid
 * accesses should be done.
 */
void read_mpu_map(void)
{
  char* name = pcTaskGetName(NULL);
  xthal_MPU_entry   mpumap[XCHAL_MPU_ENTRIES];
  xthal_read_map(mpumap);
  int i;

  for (i = 0; i < XCHAL_MPU_ENTRIES; i++){
     xt_printf("%s: mpu[%d]: as:%x, at:%x\n", name, i, mpumap[i].as, mpumap[i].at);
  }
}

static void restrictedUserTask1(void *arg);
static void restrictedPrivTask1(void *arg);

/*
 * Prototype for a task created in Privileged mode using the original
 * vTaskCreate() API function.  The task demonstrates the characteristics of
 * such a task, before simply deleting itself.
 */
static void PrivTask1( void *pvParameters );

/*
 * Just configures any clocks and IO necessary.
 */
static void prvSetupHardware( void );

/*-----------------------------------------------------------*/

extern uint32_t privileged_functions_start[];
extern uint32_t privileged_functions_end[];
extern uint32_t privileged_data_start[];
extern uint32_t privileged_data_end[];
extern uint32_t _text_start[];

/* Declare three arrays - an MPU region will be created for each array
using the TaskParameters_t structure below.  THIS IS JUST TO DEMONSTRATE THE
MPU FUNCTIONALITY, the data is not used by the check tasks primary function
of monitoring the reg test tasks and printing out status information.
*/

char dummyARRAY1_1[ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_region1")));
char dummyARRAY1_2[ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_region1")));
char dummyARRAY2_1[ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_region2")));
char dummyARRAY2_2[ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_region2")));
char sharedARRAY  [ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_region3")));

/* First location in the 1st memory of a restricted will mirror exception count,
 * 2nd will mirror expected exception count and 3rd mirrors count of passes
 */
static volatile int* excp_cnt_p     = (int*)&sharedARRAY[4];
static volatile int* passes_p       = (int*)&sharedARRAY[8];

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
static int  exp_passes = 27;
#else
static int  exp_passes = 23;
#endif

/* Stack size for nonrestricted tasks. */
#define STACK_SIZE    ((XT_STACK_MIN_SIZE + 1024) / sizeof(StackType_t))

/* Define the constants used to allocate the reg test task stacks.  Note that
that stack size is defined in words, not bytes. Also note that this size has
to be a multiple of the MPU align size. */
#define mainTASK_STACK_SIZE    (((XT_STACK_MIN_SIZE + XCHAL_MPU_ALIGN - 1U) & (uint32_t)-XCHAL_MPU_ALIGN) / sizeof(StackType_t))

/* Declare the stacks that will be used by the reg test tasks.  The kernel will
automatically create an MPU region for the stack.  The stack alignment must
match its size, so if 128 words are reserved for the stack then it must be
aligned to ( 128 * 4 ) bytes. */
static portSTACK_TYPE restrictedUserTask1Stack[ mainTASK_STACK_SIZE ] mainALIGN_TO( XCHAL_MPU_ALIGN ) mainALIGN_TO( portSTACK_ALIGNMENT ) __attribute__((section("private_stack")));
static portSTACK_TYPE restrictedPrivTask1Stack[ mainTASK_STACK_SIZE ] mainALIGN_TO( XCHAL_MPU_ALIGN ) mainALIGN_TO( portSTACK_ALIGNMENT ) __attribute__((section("private_stack")));

/* Fill in a TaskParameters_t structure per reg test task to define the tasks. */
static const TaskParameters_t restrictedUserTask1Params =
{
  restrictedUserTask1,
  "restrictedUserTask1",
  mainTASK_STACK_SIZE,
  ( void * ) configREG_TASK1_TEST_PARAMETER,
  2,
  restrictedUserTask1Stack,
  {
    /* Base address   Length                   Parameters */
    { dummyARRAY1_1,  sizeof(dummyARRAY1_1),   XTHAL_AR_RWrw | XTHAL_MEM_WRITEBACK},
    { dummyARRAY2_1,  sizeof(dummyARRAY2_1),   XTHAL_AR_RWr  | XTHAL_MEM_WRITETHRU},
    { sharedARRAY,    sizeof(sharedARRAY),     XTHAL_AR_RWrw | XTHAL_MEM_WRITEBACK}
  }
};
/*-----------------------------------------------------------*/

static const TaskParameters_t restrictedPrivTask1Params =
{
  restrictedPrivTask1,
  "restrictedPrivTask1",
  mainTASK_STACK_SIZE,
  ( void * ) configREG_TASK2_TEST_PARAMETER,
  (4 | portPRIVILEGE_BIT ),
  restrictedPrivTask1Stack,
  {
    /* Base address   Length                  Rights/Type */
    { dummyARRAY1_2,  sizeof(dummyARRAY1_2),  XTHAL_AR_RW | XTHAL_MEM_WRITEBACK},
    { dummyARRAY2_2,  sizeof(dummyARRAY2_2),  XTHAL_AR_R  | XTHAL_MEM_WRITETHRU},
    { sharedARRAY,    sizeof(sharedARRAY),    XTHAL_AR_RW | XTHAL_MEM_WRITEBACK},
  }
};

int find_address_in_mpu(xthal_MPU_entry mpumap[], uint32_t addr)
{
  int i;
  for (i = XCHAL_MPU_ENTRIES-1; i >= 0; i--) {
     if (addr >= (uint32_t)((uint32_t)mpumap[i].as & ~0x1U))
        return i;
  }
  return -1;
}

#define TEST(exc, name, test) {                 \
      int expected = *excp_cnt_p + (exc);       \
      {test;};                                  \
      if ((*excp_cnt_p) != expected) { \
        printf("%s: expected:%d, got:%d\n", name, expected, *excp_cnt_p); \
        xt_printf("Exiting '%s': %s FAIL\n", __FUNCTION__, name); \
        exit(1);                                \
      }                                         \
      xt_printf("'%s': %s PASS\n", __FUNCTION__, name); \
      (*passes_p)++;                            \
      }


/* Check that privileged data/code and unused memory have correct attribute */
int check_mpu(char name[], uint32_t priv_code_attr, uint32_t priv_data_attr, uint32_t other_mem_attr)
{
  /* Check MPU expected access attr and memory type. */
  uint32_t type, attr, start;
  int32_t  mpu_index;
  xthal_MPU_entry   mpumap[XCHAL_MPU_ENTRIES];
  xthal_read_map(mpumap);

  //read_mpu_map();

  start = (uint32_t)privileged_data_start;

  mpu_index = find_address_in_mpu(mpumap, start);
  if (mpu_index < 0) {
      xt_printf("%s (line:%d): FAIL: Couldn't find address '%x' in mpumap\n", name,__LINE__, start);
      exit(1);
  }

  // check mpu settings for priv data
  type = XTHAL_MPU_ENTRY_GET_MEMORY_TYPE(mpumap[mpu_index]);
  attr = XTHAL_MPU_ENTRY_GET_ACCESS(mpumap[mpu_index]);

  if (attr != priv_data_attr) {
    xt_printf("%s (line:%d): FAIL: privileged data, got attribute %x, expected %x (index:%d)\n",  name, __LINE__, attr, priv_data_attr, mpu_index );
    exit(1);
  }
  xt_printf("%s (line:%d): Test priv data attribute PASS\n", name, __LINE__);
  (*passes_p)++;

  // check mpu settings for priv functions
  start = (uint32_t)privileged_functions_start;
  mpu_index = find_address_in_mpu(mpumap, start);
  if (mpu_index < 0) {
      xt_printf("%s (line:%d): FAIL: Couldn't find address '%x' in mpumap\n", name, __LINE__, start);
      exit(1);
  }
  type = XTHAL_MPU_ENTRY_GET_MEMORY_TYPE(mpumap[mpu_index]);
  attr = XTHAL_MPU_ENTRY_GET_ACCESS(mpumap[mpu_index]);
  if (attr != priv_code_attr) {
    xt_printf("%s (line:%d): FAIL: privileged code, got attribute %x, expected %x (index:%d)\n",  name, __LINE__, attr, priv_code_attr, mpu_index );
    exit(1);
  }
  xt_printf("%s (line:%d): Test priv code attribute PASS\n", name, __LINE__);
  (*passes_p)++;

  // check mpu settings for other memory
  start = (uint32_t)_text_start;

  mpu_index = find_address_in_mpu(mpumap, start);
  if (mpu_index < 0) {
      xt_printf("%s (line:%d): FAIL: Couldn't find address '%x' in mpumap\n", name, __LINE__, start);
      exit(1);
  }
  type = XTHAL_MPU_ENTRY_GET_MEMORY_TYPE(mpumap[mpu_index]);
  attr = XTHAL_MPU_ENTRY_GET_ACCESS(mpumap[mpu_index]);
  if (attr != other_mem_attr) {
    xt_printf("%s (line:%d): FAIL: other mem, got attribute %x, expected %x\n",  name, __LINE__, attr, other_mem_attr );
    exit(1);
  }
  xt_printf("%s (line:%d): TODO: check memory type 0x%x\n", name, __LINE__, type);
  xt_printf("%s (line:%d): Test other mem attribute PASS\n", name, __LINE__);
  (*passes_p)++;

  //TODO - priv DEVICE
  //TODO - user DEVICE
  //TODO - memory type check
  return 0;
}

/* Restricted threads only - check private memories attributes */
int check_private_mpu(TaskParameters_t* taskParams)
{
  int mpu_index, i;
  uint32_t access, type;

  xthal_MPU_entry   mpumap[XCHAL_MPU_ENTRIES];
  xthal_read_map(mpumap);

  for (i = 0; i < 3; i++) {
    uint32_t start = (uint32_t)taskParams->xRegions[i].pvBaseAddress;
    uint32_t exp_access = taskParams->xRegions[i].ulParameters & 0xFFF;
    uint32_t exp_type   = taskParams->xRegions[i].ulParameters & 0xFFFE000;

    mpu_index = find_address_in_mpu(mpumap, start);
    if (mpu_index < 0) {
        xt_printf("%s: FAIL (line:%d): Couldn't find address '%x' in mpumap\n", taskParams->pcName, __LINE__, start);
        //exit(1);
    }
    // now check the attribute/type
    type   = XTHAL_MPU_ENTRY_GET_MEMORY_TYPE(mpumap[mpu_index]);
    access = XTHAL_MPU_ENTRY_GET_ACCESS(mpumap[mpu_index]);
    if ( access != exp_access) {
       xt_printf("%s: FAIL (line:%d): Got access %x, expected %x (i:%d, index:%d, at:%x)\n",  taskParams->pcName, __LINE__, access, exp_access, i, mpu_index, mpumap[mpu_index].at);
       //exit(1);
    }
    xt_printf("%s: Test private memory %d access PASS\n", taskParams->pcName, i);
    (*passes_p)++;

    if ( XTHAL_ENCODE_MEMORY_TYPE(exp_type) != type) {
       xt_printf("%s: FAIL (line:%d): Got type %x, expected %x (i:%d, index:%d, at:%x)\n",  taskParams->pcName, __LINE__, type, exp_type, i, mpu_index, mpumap[mpu_index].at);
       //exit(1);
    }
    xt_printf("%s: Test private memory %d type PASS (%x)\n", taskParams->pcName, i, exp_type);
    (*passes_p)++;
  }
  return 0;
}

/* This task is created in restricted mode. It should have RWXrwx access to it's
 * private memory while all other memory should be RWXrx, thus only readable
 * from this task.
 */
static void restrictedUserTask1( void *arg )
{
  uint32_t readData;

  UNUSED(arg);
  puts("\nEntering restrictedUserTask1\n");

  /* check private memory attributes */
  //check_private_mpu((TaskParameters_t*)&restrictedUserTask1Params);

  /* Cannot write privileged data */
  TEST(1, "1st test", *( volatile uint32_t *) privileged_data_start = 0x1);
  /* Cannot write privileged code */
  TEST(1, "2nd test",  *( volatile uint32_t *) privileged_functions_start = 0x1)

  /* Cannot read privileged data */
  TEST(1, "3rd test",  readData = *( volatile uint32_t *) privileged_data_start);
  xt_printf("3rd test: readData 0x%x\n", readData);
  /* Cannot write privileged code */
  TEST(1, "4th test", readData = *( volatile uint32_t *) privileged_functions_start);
  xt_printf("4th test: readData 0x%x\n", readData);

  /* Cannot write it's own private priv R/W space */
  TEST(1, "5th test",  dummyARRAY1_2[0] = 1)

  /* Can read it's own private user r space */
  TEST(0, "6th test",  readData = dummyARRAY1_2[0])
  xt_printf("6th test: readData 0x%x\n", readData);

  /* TODO: FETCH from privileged functions */

  /* TODO: WRITE to a restricted thread private space and read from that thread */

  /* TODO: WRITE PRIV Device space */

  /* TODO: WRITE User Device space */

  /* FIXME - how to test that access to privileged memory works ?
   * Note that even User tasks can read priv. memory, thus can't fail here
   */

  xt_printf("Exiting restrictedUserTask1\n");
  
  // vTaskDelete is a privileged function beginning in v10.6 so can't be called here.
  // Instead, just yield and let idle task exit the test...
  // vTaskDelete( NULL );
  while (1) {
  xt_printf("restrictedUserTask1 yielding...\n");
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  return;
}

/* This task is created in restricted mode. It should have RWXrwx access to it's
 * private memory while all other memory should be RWXrx, thus  fully accessible
 * from this task.
 */
static void restrictedPrivTask1( void *arg )
{
  uint32_t readData;

  UNUSED(arg);
  puts("\nEntering restrictedPrivTask1\n");

  /* check general memory MPU */

  check_mpu((char*)__FUNCTION__ , portDFLT_KERNCODE_ACCESS, portDFLT_KERNDATA_ACCESS, portDFLT_UNUSED_MEM_ACCESS);

  /* check private memory attributes */
  check_private_mpu((TaskParameters_t*)&restrictedPrivTask1Params);

  /* Read privileged data */
  TEST(0, "1st test", readData = *( volatile uint32_t *) privileged_data_start)
  xt_printf("1st test: readData 0x%x\n", readData);

  /* Read privileged code */
  TEST(0, "2nd test", readData = *( volatile uint32_t *) privileged_functions_start);
  xt_printf("2nd test: readData 0x%x\n", readData);

  /* TODO: Write privileged code - no exception but how to test ? */
  puts("Exiting restrictedPrivTask1'\n");
  vTaskDelete( NULL );
}

/* This task is created in Priv mode using the original xTaskCreate() API
 * function.  It should have access to all the memory.
 */
static void PrivTask1( void *pvParameters )
{
  uint32_t readData;

  UNUSED(pvParameters);
  puts("\nEntering PrivTask1\n");
  read_mpu_map();

  check_mpu((char*)__FUNCTION__ , portDFLT_KERNCODE_ACCESS, portDFLT_KERNDATA_ACCESS, portDFLT_UNUSED_MEM_ACCESS);

  /* Read privileged data */
  TEST(0, "1st test", readData = *( volatile uint32_t *) privileged_data_start)
  xt_printf("1st test: readData 0x%x\n", readData);

  /* Read privileged code */
  TEST(0, "2nd test", readData = *( volatile uint32_t *) privileged_functions_start);
  xt_printf("2nd test: readData 0x%x\n", readData);

  /* Write privileged code - exception expected */
  TEST(1, "3rd test", *( volatile uint32_t *) privileged_functions_start = 1);

  puts("PrivTask1: Exiting\n");
  vTaskDelete( NULL );
  return;
}

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* This task is created in User mode using the xTaskCreateStatic() API.
 * Note that xTaskCreate() can no longer be used to create nonprivileged
 * tasks. It should have access only to its stack.
 */
static void staticUserTask1( void *pvParameters )
{
  uint32_t readData;

  UNUSED(pvParameters);
  puts("\nEntering staticUserTask1\n");

  /* Tests below can't increment *passed_p because this task doesn't
   * have write access there. Write attempts cause exceptions that
   * have special handler that increments *passed_p for this task.
   */
  /* Cannot write privileged data */
  TEST(1, "1st test", *( volatile uint32_t *) privileged_data_start = 0x1);
  /* Cannot write privileged code */
  TEST(1, "2nd test",  *( volatile uint32_t *) privileged_functions_start = 0x1)

  /* Cannot read privileged data */
  TEST(1, "3rd test",  readData = *( volatile uint32_t *) privileged_data_start);
  /* Cannot write privileged code */
  TEST(1, "4th test", readData = *( volatile uint32_t *) privileged_functions_start);

  puts("staticUserTask1: Exiting\n");
  vTaskDelete( NULL );
  return;
}
#endif

/* Idle task is created the last, at the lowest priority. Thus it'll running
 * after the other threads have finished. Simply exit with ret val zero.
 */
static void idleTask( void *pvParameters )
{
  UNUSED(pvParameters);
  puts("\nEntering idleTask\n");
  if (*passes_p != exp_passes) {
    xt_printf("TESTING FAILURE: #passes:%d, expected:%d\n", *passes_p, exp_passes);
    exit(1);
  }
  xt_printf("TESTING SUCEEDED, Test Passed\n");
  exit(0);
}

/*-----------------------------------------------------------*/

/* NOTE NOTE NOTE:
 * Testing prohibited accesses generates exceptions, which then have to skip the
 * the offending store/load from the tests above. Either 3 or 2 bytes are skipped.
 * We skip 2 only if it's s32i.n instruction, however, more tests will be added
 * possibly requiring changes here.
 */
/*  Typedefs for interrupt/exception handler function / function pointer. */
void privilegedStoreException(XtExcFrame *frame)
{
  uint32_t skip = 3;

  uint32_t* pcptr = (uint32_t*)(frame->pc & ~0x3);
  uint32_t index = frame->pc & 0x3;
  uint8_t instr = (pcptr[0] >> (index*8)) & 0xf;
  if (instr == 9) // s32i.n
    skip = 2;

  /* Move the PC past the illegal (3-byte) instruction */
  frame->pc += skip;
  if (frame->excvaddr == (long)passes_p) {
      xt_printf("Store Exception on attempt to increment passes_p, fixing up\n");
      (*passes_p)++;
  } else {
      (*excp_cnt_p)++;
      xt_printf("Store Exception (%d), skip:%d and return\n", (*excp_cnt_p), skip);
  }
}

void privilegedLoadException(XtExcFrame *frame)
{
  uint32_t skip = 3;

  uint32_t* pcptr = (uint32_t*)(frame->pc & ~0x3);
  uint32_t index = frame->pc & 0x3;
  uint8_t instr = (pcptr[0] >> (index*8)) & 0xf;
  if (instr == 8) // l32i.n
    skip = 2;

  /* Move the PC past the illegal (3-byte) instruction */
  frame->pc += skip;
  (*excp_cnt_p)++;
  // read_mpu_map();
  xt_printf("Load Exception (%d), skip:%d and return\n", (*excp_cnt_p), skip);
}

void privilegedLoadStoreException(XtExcFrame *frame)
{
  uint32_t skip = 3;

  uint32_t* pcptr = (uint32_t*)(frame->pc & ~0x3);
  uint32_t index = frame->pc & 0x3;
  uint8_t instr = (pcptr[0] >> (index*8)) & 0xf;
  if ((instr == 8) || (instr == 9)) // l32i.n || s32i.n
    skip = 2;

  /* Move the PC past the illegal (3-byte) instruction */
  frame->pc += skip;
  if (frame->excvaddr == (long)passes_p) {
      xt_printf("Load/Store Exception on attempt to increment passes_p, fixing up\n");
      (*passes_p)++;
  } else {
      (*excp_cnt_p)++;
      xt_printf("Load/Store Exception (%d), skip:%d and return\n", (*excp_cnt_p), skip);
  }
}

void setupMPU(void) {
  extern char private_region1_start[];
  extern char private_region2_start[];
  extern char private_region3_start[];
  extern char private_region1_end[];
  extern char private_region2_end[];
  extern char private_region3_end[];
  extern char private_stack_start[];
  extern char private_stack_end[];
  //extern char __stack[];

  uint32_t stack_size_words = (((uint32_t)private_stack_end & ~(XCHAL_MPU_ALIGN - 1U)) - (uint32_t)private_stack_start) / sizeof(int);

  MemoryRegion_t mpuPrivRegions[] =
  {
     { private_region1_start, (uint32_t)(private_region1_end - private_region1_start), 0},
     { private_region2_start, (uint32_t)(private_region2_end - private_region2_start), 0},
     { private_region3_start, (uint32_t)(private_region3_end - private_region3_start), 0}
  };

  vPortStoreTaskMPUSettings(NULL, mpuPrivRegions, (StackType_t *)private_stack_start, stack_size_words );
}

int main( void )
{
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
  /* Used by static user task below. */
  static StaticTask_t xTCB;
  static StackType_t uxStack[ 8192 ] mainALIGN_TO( XCHAL_MPU_ALIGN );
#endif

   puts("(MAIN)    Entering...\n");
  /* Install handler for prohibited access*/
#if XCHAL_HAVE_XEA2
  xt_set_exception_handler(EXCCAUSE_STORE_PROHIBITED, privilegedStoreException);
  xt_set_exception_handler(EXCCAUSE_LOAD_PROHIBITED,  privilegedLoadException);
#else
  xt_set_exception_handler(EXCCAUSE_MEMORY, privilegedLoadStoreException);
#endif

  puts("(MAIN)    prvSetupHardware\n");
  prvSetupHardware();

  puts("(MAIN)    setup MPU\n");
  setupMPU();
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }

  puts("(MAIN)    xTaskCreate PrivTask1\n");
  xTaskCreate(PrivTask1,                   /* The function that implements the task. */
              "PrivTask1",                 /* Text name for the task. */
              STACK_SIZE,                  /* Stack depth in words. */
              NULL,                        /* Task parameters. */
              ( 5 | portPRIVILEGE_BIT ),   /* Priority and mode. */
              NULL                         /* Handle. */
              );
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }

#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
  puts("(MAIN)    xTaskCreateStatic staticUserTask1\n");
  xTaskCreateStatic(staticUserTask1,     /* The function that implements the task. */
                    "staticUserTask1",     /* Text name for the task. */
                    STACK_SIZE,            /* Stack depth in words. */
                    NULL,                  /* Task parameters. */
                    ( 5 ),                 /* Priority and mode. */
                    uxStack,               /* Stack buffer. */
                    xTCB                   /* TCB buffer. */
                    );
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }
#endif

  puts("(MAIN)    xTaskCreateRestricted restrictedPrivTask1\n");
  xTaskCreateRestricted( &restrictedPrivTask1Params, NULL );
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }

  puts("(MAIN)    xTaskCreateRestricted restrictedUserTask1\n");
  xTaskCreateRestricted( &restrictedUserTask1Params, NULL );
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }

  xTaskCreate(idleTask,               /* The function that implements the task. */
              "idleTask",             /* Text name for the task. */
              configMINIMAL_STACK_SIZE,   /* Stack depth in words. */
              NULL,                   /* Task parameters. */
              0 | portPRIVILEGE_BIT,  /* Priority and mode (user in this case). */
              NULL                    /* Handle. */
              );
  if (xtMPUError) {
    printf("Thread Create FAILED!, error:%d\n", xtMPUError);
    exit(1);
  }

  /* Start the scheduler. */
  puts("(MAIN)    vTaskStartScheduler\n");
  vTaskStartScheduler();

  /* Will only get here if there was insufficient memory to create the idle
  task. */
  for( ;; );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


static void prvSetupHardware( void )
{
  puts("\nEntering prvSetupHardware\n");
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
  //puts("\nEntering vApplicationTickHook\n");
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
  puts("\nEntering vApplicationStackOverflowHook\n");
  /* If configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2 then this
  function will automatically get called if a task overflows its stack. */
  ( void ) pxTask;
  ( void ) pcTaskName;
  for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
  puts("\nEntering vApplicationMallocFailedHook\n");
  /* If configUSE_MALLOC_FAILED_HOOK is set to 1 then this function will
     be called automatically if a call to pvPortMalloc() fails.  pvPortMalloc()
  is called automatically when a task, queue or semaphore is created. */
  exit(-1);
}

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  puts("\nEntering vApplicationGetIdleTaskMemory\n");
  /* If the buffers to be provided to the Idle task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_stack")));

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
  state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  puts("\nEntering vApplicationGetTimerTaskMemory\n");
  /* If the buffers to be provided to the Timer task are declared inside this
  function then they must be declared static - otherwise they will be allocated on
  the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ] mainALIGN_TO( XCHAL_MPU_ALIGN ) __attribute__((section("private_stack")));

  /* Pass out a pointer to the StaticTask_t structure in which the Timer
  task's state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Timer task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
  Note that, as the array is necessarily of type StackType_t,
  configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/*-----------------------------------------------------------*/


