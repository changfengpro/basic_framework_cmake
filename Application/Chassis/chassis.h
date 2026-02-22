#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "bsp_can.h"
#include "djimotor.h"

#ifdef __cplusplus
/* C++ USER CODE BEGIN  */

/* C++ USER CODE END   */
#endif // __cplusplu


#ifdef __cplusplus
extern "C" {
#endif

void Chassis_Init(CAN_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif

#endif
