#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
/* C++ USER CODE BEGIN  */

/* C++ USER CODE END   */
#endif // __cplusplu

#ifdef __cplusplus
extern "C" {
#endif



void can_user_init(CAN_HandleTypeDef *hcan);
void can_user_send_msg(CAN_HandleTypeDef *hcan, uint32_t id, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
