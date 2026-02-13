/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_can.h"
#include "chassis.h"
#include "tusb.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* 定义按键状态 */
typedef enum {
  KEY_IDLE,
  KEY_WAIT_RELEASE_1,
  KEY_WAIT_PRESS_2,
  KEY_RELEASE_WAIT,  
  KEY_SEND_RELEASE   
} key_state_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DOUBLE_CLICK_TIME_MS 300
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void keyboard_task(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  tusb_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // HAL_Delay(1000);
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    tud_task();
    keyboard_task();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void keyboard_task(void) {
    static key_state_t state = KEY_IDLE;
    static uint32_t last_time = 0;
    static uint32_t press_timer = 0; 
    
    bool is_pressed = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);
    uint32_t current_time = HAL_GetTick();

    // 1. 软件防抖 (20ms)
    if (current_time - last_time < 20) return;
    
    // 2. 检查 USB 是否就绪
    if (!tud_hid_ready()) return;

    switch (state) {
        case KEY_IDLE:
            if (is_pressed) {
                state = KEY_WAIT_RELEASE_1;
                last_time = current_time;
            }
            break;

        case KEY_WAIT_RELEASE_1:
            if (!is_pressed) {
                // 第一次松开，开始计时 300ms 窗口
                press_timer = current_time;
                state = KEY_WAIT_PRESS_2;
                last_time = current_time;
            }
            break;

        case KEY_WAIT_PRESS_2:
            if (is_pressed) {
                // 【判定为双击】：发送 Ctrl + V
                uint8_t keycodes[6] = { HID_KEY_V, 0, 0, 0, 0, 0 };
                tud_hid_keyboard_report(0, KEYBOARD_MODIFIER_LEFTCTRL, keycodes);
                
                state = KEY_RELEASE_WAIT; // 跳转到统一的释放等待状态
                last_time = current_time;
            } 
            else if (current_time - press_timer > DOUBLE_CLICK_TIME_MS) {
                // 【判定为单击】：发送 Ctrl + C
                uint8_t keycodes[6] = { HID_KEY_C, 0, 0, 0, 0, 0 };
                tud_hid_keyboard_report(0, KEYBOARD_MODIFIER_LEFTCTRL, keycodes);
                
                // 单击因为此时手已经松开了，直接发送释放报告
                state = KEY_SEND_RELEASE; 
                last_time = current_time;
            }
            break;

        case KEY_RELEASE_WAIT:
            // 处理双击后的手指抬起
            if (!is_pressed) {
                tud_hid_keyboard_report(0, 0, NULL);
                state = KEY_IDLE;
                last_time = current_time;
            }
            break;

        case KEY_SEND_RELEASE:
            // 处理单击后的自动释放
            tud_hid_keyboard_report(0, 0, NULL);
            state = KEY_IDLE;
            last_time = current_time;
            break;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
