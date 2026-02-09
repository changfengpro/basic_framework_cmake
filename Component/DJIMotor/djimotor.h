#ifndef __DJIMOTOR_H
#define __DJIMOTOR_H
#include "stdint.h"
#include "bsp_can.h"

/* C++ USER CODE BEGIN  */
#ifdef __cplusplus

// 电机控制模式
enum DJI_Motor_Control_Method {
    MOTOR_STOP = 0,
    MOTOR_SPEED,
    MOTOR_ANGLE
};

class DJIMotor {
public:
    // 构造函数
    void DJIMotor_Init(CAN_HandleTypeDef* hcan, uint32_t can_id);

    // 应用层接口：设置目标
    void Set_Target_Speed(float speed);
    
    // 驱动层接口：更新反馈 (由 BSP 接收中断调用)
    void Update_Feedback(uint8_t data[8]);

    // 数据获取接口 (供发送函数使用)
    int16_t Get_Output_Current() const;
    CAN_HandleTypeDef* Get_Hcan() const;
    uint32_t Get_ID() const;

private:
    CAN_HandleTypeDef* _hcan;
    uint32_t           _can_id;
    
    DJI_Motor_Control_Method _mode;

    // 反馈数据
    uint16_t _now_angle_raw;
    int16_t  _now_speed_raw;
    int16_t  _target_current; // 最终计算出的电流值

    float    _target_speed;
    // 这里未来可以添加 PID 成员对象：PID _speed_pid;
};

/* C++ USER CODE END   */
#endif // __cplusplu


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* __DJIMOTOR_H */