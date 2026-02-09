#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "can.h"

void can_user_init(CAN_HandleTypeDef hcan);

#endif
