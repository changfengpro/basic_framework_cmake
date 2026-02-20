#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tusb.h"





void StartTinyUSBTask(void const * argument);
void OSTaskInit();





#endif /* __RTOS_TASK_H */