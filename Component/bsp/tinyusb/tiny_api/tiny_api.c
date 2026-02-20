#include "tiny_api.h"

void cdc_task(void)
{
  // 检查是否有数据可读
  if ( tud_cdc_available() )
  {
    uint8_t buf[64];
    // 读取接收到的数据
    uint32_t count = tud_cdc_read(buf, sizeof(buf));

    // 原样发回 (Echo)
    tud_cdc_write("Received: ", 10);
    tud_cdc_write(buf, count);
    tud_cdc_write_flush();
  }
}