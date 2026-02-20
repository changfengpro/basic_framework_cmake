file(GLOB SOURCES "*.c" "*.cpp")
register_component(
    SRCS 
        ${SOURCES}
    INCLUDE_DIRS 
        "."                 # "."代表当前文件夹包含头文件
    REQUIRES 
        "stm32cubemx" # 依赖 HAL 库定义
        "tinyusb"     # 依赖 TinyUSB 库定义
        
)