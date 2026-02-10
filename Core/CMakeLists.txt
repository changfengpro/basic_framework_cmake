file(GLOB SOURCES "Src/*.c" "Src/*.cpp")

register_component(
    SRCS 
        ${SOURCES}
    INCLUDE_DIRS 
        "Inc"            # 指向头文件所在目录
    REQUIRES 
        "stm32cubemx"
)