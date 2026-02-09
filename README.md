
---

# STM32 模块化开发框架 (基于 CMake & Ninja)

本项目是一个专为 STM32 设计的模块化开发框架，其核心思想借鉴了 **ESP-IDF** 的组件化管理方式。通过 CMake 脚本实现组件的自动化注册、依赖分发及跨平台构建。

## 1. 核心设计思想：组件化 (Component-Based)

本项目不仅仅是将代码分目录存放，而是将每个功能单元视为一个独立的“组件”。

* **物理隔离**：每个组件（如 `bsp_can` 或 `Chassis`）都拥有独立的文件夹和 `CMakeLists.txt`。
* **依赖透明**：组件之间的引用必须通过 `REQUIRES` 显式声明。
* **自动路径分发**：当你声明一个依赖时，构建系统会自动将该依赖的头文件路径（Include Paths）加入编译命令，开发者无需手动在根目录添加路径。

---

## 2. 目录架构说明

```text
.
├── Application/        # 高级应用逻辑层 (Chassis, Task, etc.)
├── Component/          # 底层硬件抽象/驱动层 (bsp_can, bsp_uart, etc.)
├── Core/               # MCU 启动逻辑及入口 (main.c, stm32f4xx_it.c)
├── Drivers/            # 官方 HAL 库及 CMSIS
├── cmake/              # 构建系统核心
│   ├── project.cmake   # 组件注册宏定义 (register_component)
│   └── gcc-arm-none-eabi.cmake  # 交叉编译器设置及优化参数
├── CMakeLists.txt      # 全局入口，负责组件自动扫描
└── CMakePresets.json   # 编译环境预设 (Debug/Release)

```

---

## 3. CMake 构建系统详解

### 3.1 自动化扫描机制

根目录 `CMakeLists.txt` 不再手动列出源文件，而是通过以下逻辑遍历目录：

1. 定义 `COMPONENT_DIRS`（如 `Application`, `Component` 等）。
2. 循环遍历这些文件夹及其一级子目录。
3. 如果子目录下存在 `CMakeLists.txt`，则通过 `add_subdirectory` 进入该组件。

### 3.2 组件注册宏 (`register_component`)

每个组件内部通过该宏进行“自我描述”，这是打通路径的关键：

```cmake
# 示例：Application/Chassis/CMakeLists.txt
register_component(
    SRCS 
        "Chassis.c"
    INCLUDE_DIRS 
        "."               # 导出当前路径作为公共头文件目录
    REQUIRES 
        "stm32cubemx"     # 依赖底层硬件库
        "bsp_can"         # 依赖特定的 BSP 组件
)

```

---

## 4. 优化等级与构建预设 (Presets)

通过 `CMakePresets.json`，你可以快速在不同模式间切换。本项目对 Release 模式进行了针对性优化：

| 预设模式 | 优化等级 | 特点 | 适用场景 |
| --- | --- | --- | --- |
| **Debug** | `-O0` | 不优化，保留完整调试信息 (`-g3`) | 逻辑调试、打断点、单步执行 |
| **Release** | **`-Os`** | **体积优化**，删除调试信息 (`-g0`) | 最终固件发布，节省 Flash 空间 |

### ⚠️ 重要：关于 `volatile`

在 **Release (`-Os`)** 模式下，编译器会进行指令重排或缓存变量。

* 所有在中断中修改的全局变量、或者指向硬件寄存器的指针（如轮询状态位），**必须使用 `volatile` 关键字**，否则编译器可能会将其优化为死循环。

---

## 5. 快速上手手册

### 5.1 增加一个新组件 (以 `bsp_spi` 为例)

1. 在 `Component/bsp/` 下创建文件夹 `bsp_spi`。
2. 创建 `bsp_spi.c` 和 `bsp_spi.h`。
3. 创建 `CMakeLists.txt` 并调用 `register_component`。
4. 由于目录嵌套，确保父目录 `Component/bsp/CMakeLists.txt` 中包含 `add_subdirectory(bsp_spi)`或者采用**下文 6. 注意事项** 中的多级目录自动化方式。

### 5.2 常用编译命令

```bash
# 传统编译方式

#Debug
cmake -B build/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
ninja -C build/Debug

#Release
cmake -B build/Release -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build/Release

##################################################################

# CMake Presets方式

# Debug

# 1. 配置 Debug 预设
cmake --preset Debug

# 2. 编译 (Ninja 会自动寻找 build/Debug 目录)
cmake --build --preset Debug

# Release
cmake --preset Release
cmake --build --preset Release

```
* 或者使用**vscode**中**CMake插件**以按键的方式编译
  ![](8b151d45-49a4-4293-bc0a-8a628d67f801.png)


---

## 6. 开发注意事项

* **依赖声明**：本框架采用静态库链接模式。在代码中 `#include` 某个头文件后，**必须**在该组件的 `CMakeLists.txt` 中的 `REQUIRES` 下显式声明，只有声明了 `REQUIRES`，CMake 才会将目标组件的 `INCLUDE_DIRS` 注入到当前组件的编译选项中。

* **新增多级目录自动化**：由于框架支持如 `Component/bsp/bsp_can` 这样的多级嵌套，为了避免手动在每一层写 `add_subdirectory`，建议采用以下自动化方案：
  * **中间层引导**：在 `Component/bsp` 等非组件的中间文件夹下，放置一个包含自动扫描逻辑的 `CMakeLists.txt`。
  * **通用扫描脚本**：（例如Component/bsp/CMakeLists.txt）
```cmake
# 自动遍历当前目录下所有包含 CMakeLists.txt 的子目录
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt)
        add_subdirectory(${child})
    endif()
endforeach()


