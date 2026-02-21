#include "djimotor.h"

// 构造函数实现
void DJIMotor::DJIMotor_Init(CAN_HandleTypeDef* hcan, uint32_t can_id) 
{
    _hcan = hcan;
    _can_id = can_id;
    _mode = MOTOR_STOP;
    _now_angle_raw = 0;
    _now_speed_raw = 0;
    _target_current = 0;
    _target_speed = 0.0f;
}

// 设置目标速度
void DJIMotor::Set_Target_Speed(float speed) 
{
    _target_speed = speed;
    _mode = MOTOR_SPEED;
}

// 解析 CAN 反馈数据
void DJIMotor::Update_Feedback(uint8_t data[8]) 
{
    _now_angle_raw = (uint16_t)(data[0] << 8 | data[1]);
    _now_speed_raw = (int16_t)(data[2] << 8 | data[3]);
}


// Getter 实现
int16_t DJIMotor::Get_Output_Current() const { return _target_current; }
CAN_HandleTypeDef* DJIMotor::Get_Hcan() const { return _hcan; }
uint32_t DJIMotor::Get_ID() const { return _can_id; }
