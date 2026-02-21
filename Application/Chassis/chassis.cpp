#include "Chassis.h"

DJIMotor motor;

void Chassis_Init(CAN_HandleTypeDef *hcan)
{
    can_user_init(hcan);
    motor.DJIMotor_Init(hcan, 0x200);     // 初始化一个 ID 为 0x200 的电机

}