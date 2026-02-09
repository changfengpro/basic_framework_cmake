register_component(
    SRCS 
        "Chassis.cpp"
    INCLUDE_DIRS 
        "."  
    REQUIRES 
        "stm32cubemx" # 依赖 HAL 库定义
        "bsp_can" # 依赖 CAN 驱动组件
        "DJIMotor"
)