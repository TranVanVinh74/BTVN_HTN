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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_BUFFER_SIZE 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

uint8_t rx_data;

char rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_index = 0;

volatile uint8_t command_ready = 0;

/* Trạng thái LED */
uint8_t led_on = 0;

/* Mức PWM gần nhất, mặc định 50% */
uint8_t pwm_percent = 50;

char tx_buffer[100];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

void Set_PWM(uint8_t percent);
void Process_Command(void);
void UART_Send_String(char *str);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Hàm đặt duty PWM từ 0 -> 100% */
void Set_PWM(uint8_t percent)
{
    uint32_t compare;

    if(percent > 100)
    {
        percent = 100;
    }

    /*
     * ARR = 999
     * => tổng 1000 count
     *
     * 10%  = 100
     * 50%  = 500
     * 100% = 1000
     */
    compare = percent * 10;

    __HAL_TIM_SET_COMPARE(&htim2,
                          TIM_CHANNEL_1,
                          compare);
}


/* Gửi chuỗi qua UART */
void UART_Send_String(char *str)
{
    HAL_UART_Transmit(&huart1,
                      (uint8_t *)str,
                      strlen(str),
                      1000);
}


/* Xử lý lệnh nhận được */
void Process_Command(void)
{
    /* ---------- ON ---------- */
    if(strcmp(rx_buffer, "ON") == 0)
    {
        led_on = 1;

        /* Khôi phục mức PWM gần nhất */
        Set_PWM(pwm_percent);

        UART_Send_String("LED ON\r\n");
    }

    /* ---------- OFF ---------- */
    else if(strcmp(rx_buffer, "OFF") == 0)
    {
        led_on = 0;

        /* Tắt LED thực tế */
        Set_PWM(0);

        UART_Send_String("LED OFF\r\n");
    }

    /* ---------- PWM:xx ---------- */
    else if(strncmp(rx_buffer, "PWM:", 4) == 0)
    {
        int value;

        value = atoi(&rx_buffer[4]);

        if(value >= 0 && value <= 100)
        {
            /*
             * Luôn cập nhật giá trị PWM đã cấu hình,
             * kể cả khi LED đang OFF.
             */
            pwm_percent = (uint8_t)value;

            /*
             * Chỉ thay đổi độ sáng thực tế
             * nếu LED đang ON.
             */
            if(led_on)
            {
                Set_PWM(pwm_percent);
            }

            sprintf(tx_buffer,
                    "PWM = %d%%\r\n",
                    pwm_percent);

            UART_Send_String(tx_buffer);
        }
        else
        {
            UART_Send_String("PWM invalid\r\n");
        }
    }

    /* ---------- Status ---------- */
    else if(strcmp(rx_buffer, "Status") == 0)
    {
        if(led_on)
        {
            sprintf(tx_buffer,
                    "LED: ON, PWM: %d%%\r\n",
                    pwm_percent);
        }
        else
        {
            sprintf(tx_buffer,
                    "LED: OFF, PWM: %d%%\r\n",
                    pwm_percent);
        }

        UART_Send_String(tx_buffer);
    }

    /* ---------- Sai lệnh ---------- */
    else
    {
        UART_Send_String("Unknown command\r\n");
    }
}


/* Callback UART nhận xong 1 byte */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        /* Nếu gặp dấu ! -> kết thúc lệnh */
        if(rx_data == '!')
        {
            rx_buffer[rx_index] = '\0';

            command_ready = 1;

            /*
             * Không nhận tiếp ngay ở đây.
             * Chờ main xử lý xong lệnh rồi nhận tiếp.
             */
        }
        else
        {
            /*
             * Chỉ lưu nếu buffer chưa đầy.
             */
            if(rx_index < RX_BUFFER_SIZE - 1)
            {
                rx_buffer[rx_index] = rx_data;
                rx_index++;
            }

            /*
             * Tiếp tục nhận byte tiếp theo.
             */
            HAL_UART_Receive_IT(&huart1,
                                &rx_data,
                                1);
        }
    }
}

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
  MX_TIM2_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

  /*
   * Start PWM TIM2 CH1.
   */
  HAL_TIM_PWM_Start(&htim2,
                    TIM_CHANNEL_1);

  /*
   * Trạng thái ban đầu:
   * LED OFF.
   */
  Set_PWM(0);

  /*
   * Bắt đầu nhận UART byte đầu tiên bằng interrupt.
   */
  HAL_UART_Receive_IT(&huart1,
                      &rx_data,
                      1);

  UART_Send_String("System Ready\r\n");

  /* USER CODE END 2 */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    if(command_ready == 1)
    {
        /*
         * Xử lý lệnh vừa nhận.
         */
        Process_Command();

        /*
         * Chuẩn bị nhận lệnh mới.
         */
        rx_index = 0;

        command_ready = 0;

        /*
         * Bật lại UART interrupt để nhận lệnh tiếp theo.
         */
        HAL_UART_Receive_IT(&huart1,
                            &rx_data,
                            1);
    }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource =
      RCC_SYSCLKSOURCE_PLLCLK;

  RCC_ClkInitStruct.AHBCLKDivider =
      RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider =
      RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB2CLKDivider =
      RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct,
                          FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */

  htim2.Instance = TIM2;

  /*
   * Timer clock = 72 MHz
   *
   * fPWM =
   * 72 MHz / ((71 + 1) * (999 + 1))
   *
   * = 1 kHz
   */
  htim2.Init.Prescaler = 71;

  htim2.Init.CounterMode =
      TIM_COUNTERMODE_UP;

  htim2.Init.Period = 999;

  htim2.Init.ClockDivision =
      TIM_CLOCKDIVISION_DIV1;

  htim2.Init.AutoReloadPreload =
      TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger =
      TIM_TRGO_RESET;

  sMasterConfig.MasterSlaveMode =
      TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(
          &htim2,
          &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /*
   * PWM Channel 1
   */
  sConfigOC.OCMode =
      TIM_OCMODE_PWM1;

  sConfigOC.Pulse = 0;

  sConfigOC.OCPolarity =
      TIM_OCPOLARITY_HIGH;

  sConfigOC.OCFastMode =
      TIM_OCFAST_DISABLE;

  if (HAL_TIM_PWM_ConfigChannel(
          &htim2,
          &sConfigOC,
          TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

  HAL_TIM_MspPostInit(&htim2);
}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */

  huart1.Instance = USART1;

  huart1.Init.BaudRate = 115200;

  huart1.Init.WordLength =
      UART_WORDLENGTH_8B;

  huart1.Init.StopBits =
      UART_STOPBITS_1;

  huart1.Init.Parity =
      UART_PARITY_NONE;

  huart1.Init.Mode =
      UART_MODE_TX_RX;

  huart1.Init.HwFlowCtl =
      UART_HWCONTROL_NONE;

  huart1.Init.OverSampling =
      UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

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

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */