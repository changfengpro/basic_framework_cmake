# 递归获取当前目录下所有的 .c 源文件
file(GLOB_RECURSE SOURCES "*.c" "*.cpp")

# 自动获取所有包含 .h 文件的目录路径（去重后作为 INCLUDE_DIRS）
file(GLOB_RECURSE HEADERS "*.h")
set(INCLUDE_PATHS "")
foreach(HDR_FILE ${HEADERS})
    get_filename_component(DIR_PATH ${HDR_FILE} DIRECTORY)
    list(APPEND INCLUDE_PATHS ${DIR_PATH})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_PATHS) # 去除重复路径

# 注册组件
register_component(
    SRCS 
        ${SOURCES}
    INCLUDE_DIRS 
        ${INCLUDE_PATHS}
    REQUIRES 
        "stm32cubemx"
)