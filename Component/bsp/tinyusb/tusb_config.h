#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef CDC_VIRTUAL_PORT

// 定义控制器类型 (例如 STM32 F4 为 OPT_MCU_STM32F4)
#define CFG_TUSB_MCU           OPT_MCU_STM32F4 

// 启用设备栈 (对应 tusb.h 中的 CFG_TUD_ENABLED)
#define CFG_TUD_ENABLED        1

//  定义端口 0 的模式为从机 (Device Mode)
#define CFG_TUSB_RHPORT0_MODE       OPT_MODE_DEVICE

#define CFG_TUD_ENDPOINT0_SIZE      64
#define CFG_TUD_DWC2_SLAVE_ENABLE 1

// 配置具体的类 (如 CDC)
#define CFG_TUD_CDC            1
#define CFG_TUD_CDC_RX_BUFSIZE 64
#define CFG_TUD_CDC_TX_BUFSIZE 64

#define BOARD_TUD_RHPORT            0

// 启用 Microsoft OS Descriptor 2.0 支持
#define CFG_TUD_VENDOR_MS_OS_20  1

#endif

#endif


#define KEY_BOARD


#ifdef KEY_BOARD

// MCU 及 硬件端口配置
#define CFG_TUSB_MCU             OPT_MCU_STM32F4 
#define CFG_TUSB_RHPORT0_MODE    OPT_MODE_DEVICE
#define CFG_TUD_DWC2_SLAVE_ENABLE 1
#define BOARD_TUD_RHPORT         0

// 核心功能配置
#define CFG_TUD_ENABLED          1
#define CFG_TUD_ENDPOINT0_SIZE   64

// 关键：开启 HID，关闭 CDC
#define CFG_TUD_HID              1
// #define CFG_TUD_CDC              0  // 必须为 0

// HID 缓冲区
#define CFG_TUD_HID_EP_BUFSIZE   16

#endif