/*---------------------------------------------------------------------------
 * Copyright (c) 2024 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *---------------------------------------------------------------------------*/

#include <stdio.h>

#include "main.h"

#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "cmsis_vio.h"

static osThreadId_t tid_thrLED;         // Thread id of thread: LED
static osThreadId_t tid_thrButton;      // Thread id of thread: Button
static osThreadId_t tid_thrWorker;      // Thread id of thread: Worker
static osThreadId_t tid_thrDisplay;     // Thread id of thread: Display
static osThreadId_t tid_thrNoAttr;      // Thread id of thread: NoAttr


/*-----------------------------------------------------------------------------
  thrLED: blink LED
 *----------------------------------------------------------------------------*/

 osThreadAttr_t attr_thrLED = {
  .name = "LED",
  .priority = osPriorityNormal,
  .stack_size = 512U
 };
 
 static __NO_RETURN void thrLED (void *argument) {
  uint32_t active_flag = 0U;
  
  (void)argument;

  for (;;) {
    if (osThreadFlagsWait(1U, osFlagsWaitAny, 0U) == 1U) {
      active_flag ^= 1U;
    }

    if (active_flag == 1U) {
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      vioSetSignal(vioLED1, vioLEDon);          // Switch LED1 on
      osDelay(100U);                            // Delay 100 ms
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      vioSetSignal(vioLED1, vioLEDoff);         // Switch LED1 off
      osDelay(100U);                            // Delay 100 ms
    }
    else {
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      osDelay(500U);                            // Delay 500 ms
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      osDelay(500U);                            // Delay 500 ms
    }
  }
}

/*-----------------------------------------------------------------------------
  thrButton: check Button state
 *----------------------------------------------------------------------------*/
 osThreadAttr_t attr_thrButton = {
  .name = "Button",
  .priority = osPriorityNormal,
  .stack_size = 512U
 };

static __NO_RETURN void thrButton (void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    state = (vioGetSignal(vioBUTTON0));           // Get pressed Button state
    if (state != last) {
      if (state == 1U) {
        osThreadFlagsSet(tid_thrLED, 1U);         // Set flag to thrLED
      }
      last = state;
    }
    osDelay(100U);
  }
}

/*-----------------------------------------------------------------------------
  thrWorker: do nothing
 *----------------------------------------------------------------------------*/
 osThreadAttr_t attr_thrWorker = {
  .name = "Worker",
  .priority = osPriorityNormal,
  .stack_size = 512U
 };
static __NO_RETURN void thrWorker (void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    osDelay(100U);
  }
}

/*-----------------------------------------------------------------------------
  thrAnotherDummy: do nothing
 *----------------------------------------------------------------------------*/
 osThreadAttr_t attr_thrDisplay = {
  .name = "Display",
  .priority = osPriorityNormal,
  .stack_size = 512U
 };
static __NO_RETURN void thrDisplay (void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    osDelay(100U);
  }
}

/*-----------------------------------------------------------------------------
  thrNoAttr: do nothing
 *----------------------------------------------------------------------------*/
static __NO_RETURN void thrNoAttr (void *argument) {
  uint32_t last = 0U;
  uint32_t state;

  (void)argument;

  for (;;) {
    osDelay(100U);
  }
}

/*-----------------------------------------------------------------------------
 * Application main thread
 *----------------------------------------------------------------------------*/
__NO_RETURN void app_main_thread (void *argument) {

  printf("Blinky example\n");

  tid_thrLED = osThreadNew(thrLED, NULL, &attr_thrLED);             // Create LED thread
  tid_thrButton = osThreadNew(thrButton, NULL, &attr_thrButton);    // Create Button thread
  tid_thrWorker = osThreadNew(thrWorker, NULL, &attr_thrWorker);    // Create Worker thread
  tid_thrDisplay = osThreadNew(thrDisplay, NULL, &attr_thrDisplay); // Create Display thread
  tid_thrNoAttr = osThreadNew(thrNoAttr, NULL, NULL);               // Create NoAttr thread
  
  for (;;) {                            // Loop forever
    osDelay(100U);
  }
}

/*-----------------------------------------------------------------------------
 * Application initialization
 *----------------------------------------------------------------------------*/
int app_main (void) {
  osKernelInitialize();                         /* Initialize CMSIS-RTOS2 */
  osThreadNew(app_main_thread, NULL, NULL);
  osKernelStart();                              /* Start thread execution */
  return 0;
}
