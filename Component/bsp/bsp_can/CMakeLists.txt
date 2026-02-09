register_component(
    SRCS 
        "bsp_can.c"
    INCLUDE_DIRS 
        "."  # 因为 bsp_can.h 在同级目录
    REQUIRES 
        "stm32cubemx" # 依赖 HAL 库定义 [cite: 10]
)