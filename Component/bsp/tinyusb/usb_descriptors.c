#include "tusb.h"

//--------------------------------------------------------------------+
// 设备描述符
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = 0xCafe, // 你的 PID
    .idProduct          = 0x4002, // 你的 VID
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};


uint8_t const * tud_descriptor_device_cb(void) {
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// 配置描述符 (CDC 示例)
//--------------------------------------------------------------------+
enum { ITF_NUM_CDC = 0, ITF_NUM_CDC_DATA, ITF_NUM_TOTAL };
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)
#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82

uint8_t const desc_configuration[] = {
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
  (void) index;
  return desc_configuration;
}

//--------------------------------------------------------------------+
// 字符串描述符
//--------------------------------------------------------------------+
char const* string_desc_arr [] = {
  (const char[]) { 0x09, 0x04 }, // 0: 语言 ID (English)
  "TinyUSB",                     // 1: Manufacturer
  "TinyUSB Device(Created by CM-Robot Club)",              // 2: Product
  "123456",                      // 3: Serials
};



static uint16_t _desc_str[64];



uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void) langid;
  uint8_t chr_count;

  if ( index == 0 ) {
    // 语言 ID 索引
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  } else {
    // 2. 严格的索引越界检查
    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];
    
    // 3. 获取长度并限制在缓冲区范围内 (32字 - 1头部 = 31字符)
    chr_count = (uint8_t) strlen(str);
    if ( chr_count > 63 ) chr_count = 31;

    // 4. 将 ASCII 转换为 UTF-16LE 
    for(uint8_t i=0; i<chr_count; i++) {
      _desc_str[1+i] = (uint16_t) str[i];
    }
  }

  // 5. 构造描述符头部：字节 0 为长度，字节 1 为类型 (0x03)
  // 长度计算公式：(字符数 * 2) + 2 字节头部
  _desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

  return _desc_str;
}
