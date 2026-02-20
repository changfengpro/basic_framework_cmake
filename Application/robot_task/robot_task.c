#include "robot_task.h"
#include "tiny_api.h"

osThreadId TinyUSBTaskHandle;

void OSTaskInit()
{
    osThreadDef(TinyUSBTask, StartTinyUSBTask, osPriorityAboveNormal, 0, 1024);
    TinyUSBTaskHandle = osThreadCreate(osThread(TinyUSBTask), NULL);
}

__attribute__((noreturn)) void StartTinyUSBTask(void const * argument)
{
    for(;;)
    {
        tud_task();
        static uint32_t start_ms = 0;
        if (HAL_GetTick() - start_ms > 1000) {
            start_ms = HAL_GetTick();
            
            // 检查 CDC 接口是否已连接（DTR 信号已建立）
            if (tud_cdc_connected()) {
                tud_cdc_write_str("Hello World from STM32!\r\n");
                tud_cdc_write_flush(); // 强制刷新缓冲区发送
            }
        }

        // 4. 调用接收处理函数
        cdc_task();
        osDelay(1);
    }
}