/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "CLCD.h"
#include "Relay_Led.h"
#include "user_LCD_object.h"
#include "user_check_button.h"
#include "uart_sim.h"
#include "user_sim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RELAY1 GPIO_PIN_2 
#define RELAY2 GPIO_PIN_3
#define RELAY3 GPIO_PIN_0
#define RELAY4 GPIO_PIN_1
#define LED5   GPIO_PIN_0
#define LED6   GPIO_PIN_1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint32_t runTime=0;
uint32_t time1=10; //so giay cho an
uint32_t time2=10;	//so phut giua 2 chu ky ban
uint32_t time3=5; //thoi gian giua 2 moto vang va chinh luong 

uint16_t State=1;
uint16_t checkState=0;
uint16_t countState=0;
uint16_t setupCount=1;
uint16_t check_Power_OFF=0;
uint8_t check_config=0;
uint8_t check_connect=0;

//gio phut giay
uint16_t hh, mm, ss;

CLCD_Name LCD;

UART_BUFFER rx_uart3;
UART_BUFFER rx_uart1;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

REAL_TIME RTC_Current={0};
uint8_t get_RTC=0;
uint8_t get_RTC_complete=0;
uint8_t run_Time_Current=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Delay_1ms(void);
void Delay_s(int time);
void Check_BT_Callback(void);
void Check_Test(void);
void Set_Time(uint16_t *hh, uint16_t *mm, uint16_t *ss);
void Run_Feed_Shrimp(void);
void BT_Check_Up_Down(void);
void Display_Time(void);
void Read_Flash(void);
void Check_SMS_Receive(void);
void Set_RTC(REAL_TIME RTC_Current);
void Module_SIM(void);
void SendData_Control_SIM(void);
void Check_Disconnect_Error_SIM(void);
void Time_Current(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{
  /* USER CODE BEGIN 1 */
	rx_uart1.huart=&huart1;
	rx_uart3.huart=&huart3;
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
  MX_USART3_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	//Linked_List_Init();
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_UART_Receive_IT(&huart3,&rx_uart3.buffer,1);
	HAL_UART_Receive_IT(&huart1,&rx_uart1.buffer,1);
	
	CLCD_4BIT_Init(&LCD, 16,2, GPIOC, GPIO_PIN_8,GPIOC, GPIO_PIN_7,
                             GPIOC, GPIO_PIN_6,GPIOB, GPIO_PIN_15,
                             GPIOB, GPIO_PIN_14,GPIOB, GPIO_PIN_12);
	//Config_SIM_Before_While(&rx_uart1, &rx_uart3, runTime);
	Read_Flash();
	CLCD_SetCursor(&LCD, 0,0);
	CLCD_WriteString(&LCD, "        00:00:00");
	
	//Receive_SMS_Setup("+CMT: +84966674796,23/03/14,09:34:14+28 SETUP T1=  100  t3:199", &time1, &time2, &time3);
	Run_Begin(&setupCount, time1, time2, time3);
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//Check_SMS_Receive();
		Module_SIM();
		
		Display_Time();
		Check_BT_Callback();
		Time_Current();
		if(State==0 )
		{
			Check_Test();
			BT_Esc_Exit_Setup(&State, &setupCount, &time1, &time2, &time3);
			USER_LCD_Display_Running_OR_Setup(State);
			if(setupCount!=3) BT_Check_Up_Down();
			USER_LCD_Display_Setup(&LCD, setupCount);
		}
		
		if(State==1) 
		{
			Run_Feed_Shrimp();
			USER_LCD_Display_Running_OR_Setup(State);
			USER_LCD_Display_Running(&LCD, setupCount);
			Check_Test();
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 4;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x2608);
	}
  /* USER CODE END RTC_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC0 PC1 PC2
                           PC3 PC5 PC6 PC7
                           PC8 PC9 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA8 PA9 PA10 PA11
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB12 PB13 PB14
                           PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void Module_SIM(void)
{
	if(check_config==0)
	{
		check_config=Config_SIM_In_While(&rx_uart1, &rx_uart3);
		if(check_config == 1)
		{
			if(RTC_Current.Year == 0)
			{
				get_RTC=1;
			}
			check_connect =0;
		}
	}
	else
	{
		Check_Disconnect_Error_SIM();
		SendData_Control_SIM();
	}	
}

void Check_Disconnect_Error_SIM(void)
{
	if(Compare_Uart1_RX_Uart3_TX_True_Display(&rx_uart1, &rx_uart3, "DISCONNECTED") == 1)
	{
		check_connect=1;
	}
	
	if(Compare_Uart1_RX_Uart3_TX_True_Display(&rx_uart1, &rx_uart3, "ERROR") == 1)
	{
		check_connect=1;
	}
	
	if(Compare_Uart1_RX_Uart3_TX_True_Display(&rx_uart1, &rx_uart3, "FAIL") == 1)
	{
		check_connect=1;
	}
	
	if(check_connect == 1)
	{
		Reset_SendData(); 
		int8_t connect_server = Connect_Server_SIM(&rx_uart1, &rx_uart3);
		if(connect_server == 1) check_connect =0;
		if(connect_server == -1) check_config =0;
	}	
}

void SendData_Control_SIM(void)
{
	if(check_connect == 0 && check_config == 1)
	{
		if(get_RTC == 1)
		{
			Get_Real_Time(&rx_uart1, &rx_uart3, &RTC_Current, &check_connect, &get_RTC, &get_RTC_complete );
		}
		else
		{
			if(Receive_Control_Setup(&rx_uart1, &rx_uart3, &check_connect, &time1, &time2, &time3 )==1)
			{
				Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
				Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
				runTime=0;
				countState=0;
				State=1;
				FLASH_WritePage(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, 1, time1, time2, time3);
				Run_Begin(&setupCount, time1, time2, time3);
			}
			int8_t check_send_data = SendData_Server(&rx_uart1, &rx_uart3, &RTC_Current);
			if(check_send_data == 1) check_connect = 1;
			if(check_send_data == -1) check_config = 0;
		}
		
		if(get_RTC_complete == 1)
		{
			Set_RTC(RTC_Current);
			get_RTC_complete=0;
			run_Time_Current=1;
		}
	}	
}

void Check_SMS_Receive(void)
{
	if(Check_CountBuffer_Complete_Uart3(&rx_uart3)==1)
	{
		if(Wait_SMS_Receive(&rx_uart1,&rx_uart3,"AT+RESET")==1) 
		{
			Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
			Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
			runTime=0;
			countState=0;
			//Delete_Buffer(&rx_uart3);
		}
		
		if(Wait_SMS_Receive(&rx_uart1,&rx_uart3,"SETUP")==1) 
		{
			if(Receive_SMS_Setup(rx_uart3.sim_rx, &time1, &time2, &time3)==1)
			{
			Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
			Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
			runTime=0;
			countState=0;
			State=1;
			FLASH_WritePage(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR, 1, time1, time2, time3);
			Run_Begin(&setupCount, time1, time2, time3);
			}
			//Run_Begin(&setupCount, time1, time2, time3);
			//Delete_Buffer(&rx_uart3);
		}
		Transmit_Data_Uart(*rx_uart1.huart,rx_uart3.sim_rx);
		Delete_Buffer(&rx_uart3);
	}
}


void Run_Feed_Shrimp(void)
{
	if(countState==0)
	{
		//Set_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		countState++;
		runTime=0;
	}
	if(countState==1 && runTime>=time3 )
	{
		//Set_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState++;
		runTime=0;
	}
	if(countState==2 && runTime>=time1 )
	{
		//Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState++;
		runTime=0;
	}
	if(countState==3 && runTime>=time3 )
	{
		//Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		countState++;
		runTime=0;
	}
	if(countState==4 && runTime>=time2)
	{
		runTime=0;
		countState=0;
	}
}
uint32_t k;
void Check_BT_Callback(void)
{
	Check_BT_ENTER(&State, &checkState, &setupCount, &time1, &time2, &time3);
	Check_BT_ESC(State, &setupCount);
	Check_BT_UP(State);
	Check_BT_DOWN(State);
		
	//Check_Test();
}

void Set_Time(uint16_t *hh, uint16_t *mm, uint16_t *ss)
{
	const uint16_t MINUTES_OF_THE_HOUR=60;
	const uint16_t SECOND_OF_THE_HOUR=60;
	const uint32_t SECOND_OF_THE_DAY=86400;
	
	*hh=runTime/(MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR);
	*mm=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR)/SECOND_OF_THE_HOUR;
	*ss=(runTime-(*hh)*MINUTES_OF_THE_HOUR*SECOND_OF_THE_HOUR-(*mm)*SECOND_OF_THE_HOUR);
	
	if(runTime>SECOND_OF_THE_DAY)
	{
	runTime=0; *hh=0; *mm=0; *ss=0;
	}
}

void Check_Test(void)
{
	if(checkState==1)
	{
		Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
		Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
		countState=0;
		runTime=0;
		checkState=0;
	}
	if(State==0)
	{
		Reset_Relay_Led(GPIOC, RELAY1, GPIOC, RELAY2,GPIOC, LED5);
			Reset_Relay_Led(GPIOA, RELAY3, GPIOA, RELAY4,GPIOC, LED6);
			countState=0;
			runTime=0;
	}
}

void Display_Time(void)
{
	Set_Time(&hh, &mm, &ss);
	LCD_Change_State_Time_HH_MM_SS(hh, mm, ss);
	UintTime_To_CharTime_HH_MM_SS(hh,  mm, ss);
	USER_LCD_Display_Time(&LCD);
}

void Read_Flash(void)
{
	check_Power_OFF=FLASH_ReadData32(FLASH_USER_START_ADDR);
	
	if(check_Power_OFF == 1)
	{
		time1=FLASH_ReadData32(FLASH_USER_START_ADDR + 4);
		time2=FLASH_ReadData32(FLASH_USER_START_ADDR + 8);
		time3=FLASH_ReadData32(FLASH_USER_START_ADDR + 12);
	}
}

void Time_Current(void)
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	if(sDate.Year != 0)
	{
		if(sTime.Seconds!=RTC_Current.Seconds)
		{
			RTC_Current.Send_Data_Server=1;
			RTC_Current.Seconds = sTime.Seconds;
			RTC_Current.Minutes = sTime.Minutes;
			RTC_Current.Hour = sTime.Hours;
			RTC_Current.Date = sDate.Date;
			RTC_Current.Month= sDate.Month;
			RTC_Current.Year = sDate.Year;
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
		Packing_News(&RTC_Current);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  UNUSED(htim);
	if(htim->Instance == htim2.Instance )
	{
		runTime++;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
	if(huart->Instance == huart3.Instance)
	{
		rx_uart3.sim_rx[(rx_uart3.countBuffer)++]= rx_uart3.buffer;
		HAL_UART_Receive_IT(&huart3,&rx_uart3.buffer,1);
	}
	
	if(huart->Instance == huart1.Instance)
	{
		rx_uart1.sim_rx[(rx_uart1.countBuffer)++]= rx_uart1.buffer;
		HAL_UART_Receive_IT(&huart1,&rx_uart1.buffer,1);
	}
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback could be implemented in the user file
   */
}

void Set_RTC(REAL_TIME RTC_Current)
{
	sTime.Hours=RTC_Current.Hour;
	sTime.Minutes=RTC_Current.Minutes;
	sTime.Seconds=RTC_Current.Seconds;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	
	sDate.Year=RTC_Current.Year;
	sDate.Month=RTC_Current.Month;
	sDate.Date=RTC_Current.Date;
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void Delay_1ms(void)
{
	__HAL_TIM_SetCounter(&htim2,0);
	while (__HAL_TIM_GetCounter(&htim2)<1000);
}
void Delay_s(int time)
{
	int i=0;
	for(i=0;i<time;i++)
	{
		Delay_1ms();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
