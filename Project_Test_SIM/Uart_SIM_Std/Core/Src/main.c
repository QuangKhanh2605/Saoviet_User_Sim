/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "user_sim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RECONNECT_SIM         -1
#define CONFIG_SIM             0
#define GETTIME_SIM            1
#define CHECK_CONNECT_SIM      2
#define SEND_DATA_SERVER_SIM   3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
UART_BUFFER sUart1 = 
{
	.huart = &huart1,
};

UART_BUFFER sUart3 = 
{
	.huart = &huart3,
};

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

REAL_TIME RTC_Current={0};

uint8_t get_RTC=0;          //Xac nhan viec lay thoi gian tu mang di dong

int8_t module_sim_step=0;  //Trang thai dang lam viec Module Sim

uint32_t Systick_MS=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Set_RTC(REAL_TIME RTC_Current);
void TimeCurrent_And_PackingNews(void);
void Module_SIM(void);

int8_t SendData_Control_Sim(void);
int8_t Check_Connect_Error_Sim(void);
int8_t Config_Module_Sim(void);
int8_t Get_Real_Time_Sim(void);
int8_t Reconnect_Server_Sim(void);
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
  MX_USART3_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(sUart1.huart,&sUart1.buffer,1);
	HAL_UART_Receive_IT(sUart3.huart,&sUart1.buffer,1);
	Get_Addr_Read_Write();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		/* Lam viec voi Module Sim */
		Systick_MS=HAL_GetTick();
		Module_SIM();

		/*Lay thoi gian tu RTC*/
		TimeCurrent_And_PackingNews();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1)==0x2608)
	{
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x2608);
	}
  /* USER CODE END RTC_Init 2 */

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
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Module_SIM(void)
{
	int8_t check_module_sim_step=0;
	switch(module_sim_step)
	{
		case RECONNECT_SIM:
			check_module_sim_step = Reconnect_Server_Sim();
			if(check_module_sim_step == 1) module_sim_step = GETTIME_SIM;
			if(check_module_sim_step ==-1) module_sim_step = CONFIG_SIM;
			break; 
		
		case CONFIG_SIM:
			check_module_sim_step = Config_Module_Sim();
			if(check_module_sim_step == 1) module_sim_step = GETTIME_SIM;
			break;
		
		case GETTIME_SIM:
			check_module_sim_step = Get_Real_Time_Sim();
			if(check_module_sim_step == 1) module_sim_step++;
			if(check_module_sim_step ==-1) module_sim_step = CONFIG_SIM;
			break;
		
		case CHECK_CONNECT_SIM:
			check_module_sim_step = Check_Connect_Error_Sim();
			if(check_module_sim_step == -1) module_sim_step = RECONNECT_SIM;
			else module_sim_step++;
			break;
		
		case SEND_DATA_SERVER_SIM:
			check_module_sim_step = SendData_Control_Sim();
			if(check_module_sim_step == -1) module_sim_step = CONFIG_SIM;
			else if(check_module_sim_step == 0)  module_sim_step = CHECK_CONNECT_SIM;
			else module_sim_step++;
			break;
			
		default:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
			module_sim_step = GETTIME_SIM;
			break;
	}
}

/*
	@brief  Cau hinh Module Sim
	@return (1) Hoan thanh 
	@return (0) Chua hoan thanh
*/
int8_t Config_Module_Sim(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	Reset_Cipsend();
	if(Config_SIM(&sUart1, &sUart3) == 1)
	{
		if(sDate.Year == 0) get_RTC=1;
		return 1;
	}
	return 0;
}

/*
	@brief  Kiem tra ket noi Module Sim
	@return (1) Co ket noi
	@return (-1) Mat ket noi
*/
int8_t Check_Connect_Error_Sim(void)
{
	if(Check_Disconnect_Error(&sUart1, &sUart3) == 1) // Kiem tra mat ket noi
	{
		Delete_Buffer(&sUart3);
		return -1;
	}
	return 1;
}

/*
	@brief  Ket noi lai Module Sim voi Server
	@return (-1) Khong ket noi duoc sau 3 lan 
	@return (1)  Ket noi thanh cong
	@return (0)  Chua hoan thanh
*/
int8_t Reconnect_Server_Sim(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	Reset_Cipsend();
	int8_t connect_server = Connect_Server_SIM(&sUart1, &sUart3); // Ket noi lai Module Sim voi Server
	if(connect_server == 1) return 1;
	if(connect_server == -1)return -1;
	return 0;
}

/*
	@brief  Gui du lieu len Server
	@return (-1) Gui khong thanh cong
	@return (1)  Gui thanh cong
	@return (0)  Chua gui xong
*/
int8_t SendData_Control_Sim(void)
{
	int8_t check_SendData_Server = SendData_Server(&sUart1, &sUart3, &RTC_Current);
	if(check_SendData_Server == -1) return -1;
	else if(check_SendData_Server == 1) return 1;
	return 0;
}

/*
	@brief  Lay thoi gian tu mang di dong
	@return (-1) Khong co ket noi
	@return (1)  Hoan thanh 
	@return (0)  Chua hoan thanh
*/
int8_t Get_Real_Time_Sim(void)
{
	if(get_RTC == 1)
	{
		int8_t check_Get_Real_Time = Get_Real_Time(&sUart1, &sUart3, &RTC_Current); // Lay thoi gian tu mang di dong
		if(check_Get_Real_Time == 1)
		{
			get_RTC=0;
			Set_RTC(RTC_Current);
			return 1;
		}
		if(check_Get_Real_Time == -1) 
		{
			return -1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

/*
	@brief  Lay thoi gian tu RTC, kiem tra va dong goi ban tin
	@retval None
*/
void TimeCurrent_And_PackingNews(void)
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // Lay thoi gian tu RTC
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN); // Lay ngay tu RTC
	if(sDate.Year != 0)
	{
		if(sTime.Seconds != RTC_Current.Seconds)
		{
			RTC_Current.Send_Data_Server=1;
			RTC_Current.Seconds = sTime.Seconds;
			RTC_Current.Minutes = sTime.Minutes;
			RTC_Current.Hour    = sTime.Hours;
			RTC_Current.Date    = sDate.Date;
			RTC_Current.Month   = sDate.Month;
			RTC_Current.Year    = sDate.Year;
			if(RTC_Current.Seconds==0)
			{
				RTC_Current.Send_Data_Server=1;
			}
			if(RTC_Current.Hour == 0 && RTC_Current.Minutes == 0 && RTC_Current.Seconds == 0)
			{
				get_RTC=1;
			}
		}
	}
	if(RTC_Current.Year != 0) 
	{
		Packing_News(&RTC_Current); // Dong goi ban tin 
	}
}

/*
	@brief  Nap thoi gian vao RTC
	@retval None
*/
void Set_RTC(REAL_TIME RTC_Current)
{
	sTime.Hours=RTC_Current.Hour;
	sTime.Minutes=RTC_Current.Minutes;
	sTime.Seconds=RTC_Current.Seconds;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // Nap thoi gian vao RTC
	
	sDate.Year=RTC_Current.Year;
	sDate.Month=RTC_Current.Month;
	sDate.Date=RTC_Current.Date;
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);	// Nap ngay vao RTC
}

/*
	@brief  Ngat nhan Uart
	@param  huart, Uart
	@retval None
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
	if(huart->Instance == huart3.Instance)
	{
		Check_CountBuffer_Complete_Uart3(&sUart3);
		if(sUart3.countBuffer < LENGTH_BUFFER_UART)
		{
			sUart3.sim_rx[(sUart3.countBuffer)++]= sUart3.buffer;
			if(sUart3.countBuffer == LENGTH_BUFFER_UART) sUart3.countBuffer=0;
		}
		HAL_UART_Receive_IT(&huart3,&sUart3.buffer,1);
	}
	
	if(huart->Instance == huart1.Instance)
	{
		Check_CountBuffer_Complete_Uart1(&sUart1);
		if(sUart1.countBuffer < LENGTH_BUFFER_UART)
		{
			sUart1.sim_rx[(sUart1.countBuffer)++]= sUart1.buffer;
			if(sUart1.countBuffer == LENGTH_BUFFER_UART) sUart1.countBuffer=0;
		}
		HAL_UART_Receive_IT(&huart1,&sUart1.buffer,1);
	}
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
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

#ifdef  USE_FULL_ASSERT
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
