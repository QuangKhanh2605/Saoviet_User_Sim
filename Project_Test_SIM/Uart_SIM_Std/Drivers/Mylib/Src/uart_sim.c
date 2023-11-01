#include "uart_sim.h"

uint8_t time_10_check_command=0;

uint8_t time_3_Uart_SIM=0;

uint8_t count_Transmit_Sim=0;

uint8_t check_Transmit=0;
uint32_t get_systick_command=0;
uint8_t check_timeOut=0;

uint32_t get_systick_Setup_Sim = 0;
uint8_t  check_systick_Setup_Sim =0;

uint8_t SimConfig_step=0;
uint8_t SimConnect_step=0;
uint8_t time_3_Connect_SIM_step=0;

uint8_t check_systick_CCLK=0;
uint32_t get_systick_CCLK=0;
uint8_t time_3_CCLK=0;

int8_t Setup_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3);
int8_t Transmit_CFUN(UART_BUFFER *sUart1, UART_BUFFER *sUart3);

int8_t Config_TCP(UART_BUFFER *sUart1,UART_BUFFER *sUart3);
int8_t Config_SMS(UART_BUFFER *sUart1, UART_BUFFER *sUart3);
int8_t Config_Uart_Sim(UART_BUFFER *sUart1, UART_BUFFER *sUart3);
int8_t Check_Command_Config(UART_BUFFER *sUart1,UART_BUFFER *sUart3, char* command,char* response, uint32_t Time_Resend_Command);

/*
	@brief  Truyen chuoi tu Uart1 den Uart3 neu dung thi se hien thi qua Uart
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@param  response chuoi phan hoi de so sanh
	@retval None
*/
int8_t Compare_Uart1_RX_Uart3_TX_True_Display(UART_BUFFER *sUart1, UART_BUFFER *sUart3,char* response)
{
	uint8_t answer = 0;
	if(Check_CountBuffer_Complete_Uart1(sUart1)==1)
	{
		Transmit_Data_Uart(*sUart1->huart,sUart1->sim_rx);	
		HAL_UART_Transmit(sUart3->huart, (uint8_t*)sUart1->sim_rx, (uint8_t)strlen(sUart1->sim_rx), 1000);
		Delete_Buffer(sUart1);
	}
	
	if(Check_CountBuffer_Complete_Uart3(sUart3)==1)
	{	
		if(strstr(sUart3->sim_rx,response) != NULL) 
		{
			Transmit_Data_Uart(*sUart1->huart,sUart3->sim_rx);
			Delete_Buffer(sUart3);
			answer = 1;
		}
		else 
		{
			answer = 0;
		}	
	}
	return answer;
}

/*
	@brief  Truyen chuoi tu Uart1 den Uart3 va hien thi qua Uart
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@param  response chuoi phan hoi de so sanh
	@retval None
*/
int8_t Compare_Uart1_RX_Uart3_TX(UART_BUFFER *sUart1, UART_BUFFER *sUart3,char* response)
{
	uint8_t answer = 0;
	if(Check_CountBuffer_Complete_Uart1(sUart1)==1)
	{
		Transmit_Data_Uart(*sUart1->huart,sUart1->sim_rx);	
		HAL_UART_Transmit(sUart3->huart, (uint8_t*)sUart1->sim_rx, (uint8_t)strlen(sUart1->sim_rx), 1000);
		Delete_Buffer(sUart1);
	}
	
	if(Check_CountBuffer_Complete_Uart3(sUart3)==1)
	{	
		Transmit_Data_Uart(*sUart1->huart,sUart3->sim_rx);
		if(strstr(sUart3->sim_rx,response) != NULL) 
		{
			answer = 1;
		}
		else 
		{
			answer = 0;
		}
		Delete_Buffer(sUart3);
	}
	return answer;
}

/*
	@brief  Truyen chuoi tu Uart1 den Uart3 
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@param  response chuoi phan hoi de so sanh
	@retval None
*/
int8_t Uart1_To_Uart3(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	uint8_t answer = 0;
	if(Check_CountBuffer_Complete_Uart1(sUart1)==1)
	{
		Transmit_Data_Uart(*sUart1->huart,sUart1->sim_rx);	
		HAL_UART_Transmit(sUart3->huart, (uint8_t*)sUart1->sim_rx, (uint8_t)strlen(sUart1->sim_rx), 1000);
		Delete_Buffer(sUart1);
	}
  if(Check_CountBuffer_Complete_Uart3(sUart3)==1)
	{
		Transmit_Data_Uart(*sUart1->huart,sUart3->sim_rx);
		Delete_Buffer(sUart3);
		answer=1;
	}
	return answer;
}

/*
	@brief  Cau hinh SMS Module SIM
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (-1) Loi
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Config_SMS(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Config_SMS=0;
	switch (count_Transmit_Sim)
	{
		case 0:
			check_Config_SMS = Check_Command_Config(sUart1, sUart3, "AT+CMGF=1", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_SMS ==1)
			{
				count_Transmit_Sim++;
			}
			else if(check_Config_SMS==-1)
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
		default:
			check_Config_SMS = Check_Command_Config(sUart1, sUart3, "AT+CNMI=1,2", "OK",TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_SMS ==1)
			{
				count_Transmit_Sim=0;
				return 1;
			}
			else if(check_Config_SMS ==-1)
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	}
	return 0;
}

/*
	@brief  Cau hinh TCP Module SIM
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (-1) Loi
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Config_TCP(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Config_TCP = 0;
	switch (count_Transmit_Sim)
	{
		case 0:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3, "AT+CIPTIMEOUT=30000,20000,40000,50000", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_TCP ==1)
			{
				count_Transmit_Sim++;
			}
			else if(check_Config_TCP ==-1)
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
		case 1:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3, "AT+CIPMODE=0", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_TCP==1)
			{
				count_Transmit_Sim++;
			}
			else if(check_Config_TCP ==-1)
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
	
		case 2:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3, "AT+NETOPEN", "SUCCESS", TIME_RESEND_COMMAND_CONNECT_SIM);
			if(check_Config_TCP ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_TCP ==-1) 
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
		case 3:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3, "AT+IPADDR", "SUCCESS", TIME_RESEND_COMMAND_CONNECT_SIM);
			if(check_Config_TCP ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_TCP ==-1) 
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
		case 4:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3 , "AT+CIPRXGET=0,1", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_TCP ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_TCP ==-1) 
			{
				count_Transmit_Sim = 0;
				return -1;
			}
			break;

		default:
			check_Config_TCP = Check_Command_Config(sUart1, sUart3, "AT+CIPOPEN=1,\"TCP\",\"113.190.240.47\",7577,0", "SUCCESS", TIME_RESEND_COMMAND_CONNECT_SIM);
			if(check_Config_TCP ==1)
			{
				count_Transmit_Sim=0;
				return 1;
			}
			if(check_Config_TCP ==-1) 
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	}
	return 0;
}

/*
	@brief  Truyen CFUN=4 va CFUN=1
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (-1) Loi
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Transmit_CFUN(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Transmit_CFUN=0;
	switch (count_Transmit_Sim)
	{
		case 0:
			check_Transmit_CFUN = Check_Command_Config(sUart1 ,sUart3, "AT+CFUN=4", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Transmit_CFUN ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Transmit_CFUN ==-1) 
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	
		default:
			check_Transmit_CFUN = Check_Command_Config(sUart1, sUart3, "AT+CFUN=1", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Transmit_CFUN ==1)
			{
				count_Transmit_Sim=0;
				return 1;
			}
			if(check_Transmit_CFUN ==-1) 
			{
				count_Transmit_Sim=0;
				return -1;
			}
			break;
	}
	return 0;
}

/*
	@brief  Cau hinh ban dau cho Module SIM
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (-1) Loi
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Config_Uart_Sim(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Config_Uart_Sim=0;
	switch (count_Transmit_Sim)
	{
		case 0:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				return -1;
			}
			break;
	
		case 1:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CIPCLOSE=1", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 2:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CPIN?", "OK",TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 3:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CICCID", "OK", TIME_RESEND_COMMAND_MODULE_SIM) ;
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 4:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CSQ", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 5:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CGREG?", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 6:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CGATT?", "CGATT: 1", TIME_RESEND_COMMAND_CONNECT_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				time_3_Uart_SIM=0;
				count_Transmit_Sim=0;
				return 1;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				count_Transmit_Sim=7;
			}
			break;
	
		case 7:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CFUN=4", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim++;
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				time_3_Uart_SIM=0;
				return -1;
			}
			break;
	
		default:
			check_Config_Uart_Sim = Check_Command_Config(sUart1, sUart3, "AT+CFUN=1", "OK", TIME_RESEND_COMMAND_MODULE_SIM);
			if(check_Config_Uart_Sim ==1)
			{
				count_Transmit_Sim=1;
				time_3_Uart_SIM++;
				if(time_3_Uart_SIM >= 3) 
				{
					time_3_Uart_SIM=0;
					count_Transmit_Sim=0;
					return -1;
				}
			}
			if(check_Config_Uart_Sim ==-1) 
			{
				time_3_Uart_SIM=0;
				return -1;
			}
			break;
	}
	
	if(time_3_Uart_SIM >= 3) 
	{
		time_3_Uart_SIM=0;
		count_Transmit_Sim=0;
		return -1;
	}
	return 0;
}

/*
	@brief  Gui va kiem tra phan hoi cua Command
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@param  command lenh gui toi Module Sim
	@param  response phan hoi tu Module Sim
	@param  Time_Resend_Command thoi gian gui lai lenh command neu khong thanh cong
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
	@return (-1) Loi. Gui 10 lan khong thanh cong
*/
int8_t Check_Command_Config(UART_BUFFER *sUart1, UART_BUFFER *sUart3, char* command, char* response, uint32_t Time_Resend_Command)
{
	if(check_Transmit==0 )
	{
		Transmit_Data_Uart(*sUart3->huart, command);
		Transmit_Data_Uart(*sUart1->huart, command);
		check_Transmit=1;
		check_timeOut=1;
		get_systick_command = HAL_GetTick();
	}
	
	if(check_Transmit==1)
	{
		if(Check_CountBuffer_Complete_Uart3(sUart3)==1)
		{
			Transmit_Data_Uart(*sUart1->huart, sUart3->sim_rx );
			if(strstr(sUart3->sim_rx,response) != NULL) 
			{
				time_10_check_command=0;
				Delete_Buffer(sUart3);
				check_timeOut=1;
				return 1;
			}
			else
			{
				Delete_Buffer(sUart3);
				check_timeOut=2;
			}
		}
	}
	
	if(check_timeOut==1)
	{
		if(HAL_GetTick() - get_systick_command > TIME_RESEND_COMMAND_IF_NOT_RECEIVE)
		{
			time_10_check_command++;
			check_Transmit=0;
			check_timeOut=0;
		}
	}
	
	if(check_timeOut==2)
	{
		if(HAL_GetTick() - get_systick_command > Time_Resend_Command)
		{
			time_10_check_command++;
			check_Transmit=0;
			check_timeOut=0;
		}
	}
	if(time_10_check_command > 10)
	{
		time_10_check_command = 0;
		return -1;
	}
	return 0;
}

/*
	@brief  Bat Module Sim
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Setup_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	uint32_t run_Time=0;
	if(check_systick_Setup_Sim == 0)
	{
		get_systick_Setup_Sim = HAL_GetTick();
		check_systick_Setup_Sim = 1;
	}
	if(check_systick_Setup_Sim ==1)
	{
		if(HAL_GetTick() - get_systick_Setup_Sim<=TIME_ON_SIM)
		{
			run_Time=(HAL_GetTick() - get_systick_Setup_Sim )/1000;
			if(run_Time<=2)
			{
				HAL_GPIO_WritePin(GPIO_PIN_ON_OFF_SIM, PIN_ON_OFF_SIM,GPIO_PIN_RESET);
			}
			
			if(run_Time>2 && run_Time<=3)
			{
				HAL_GPIO_WritePin(GPIO_PIN_PWKEY_SIM, PIN_PWKEY_SIM,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIO_PIN_RESET_SIM, PIN_RESET_SIM,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIO_PIN_ON_OFF_SIM, PIN_ON_OFF_SIM,GPIO_PIN_SET);
			}
			
			if(run_Time>3 && run_Time <=6)
			{
				HAL_GPIO_WritePin(GPIO_PIN_PWKEY_SIM, PIN_PWKEY_SIM,GPIO_PIN_SET);
			}
			if(run_Time>6 && run_Time <=21)
			{
				HAL_GPIO_WritePin(GPIO_PIN_PWKEY_SIM, PIN_PWKEY_SIM,GPIO_PIN_RESET);
			}
		}
		else
		{
			check_systick_Setup_Sim = 0;
			Delete_Buffer(sUart3);	
			return 1;
		}
	}
	return 0;
}

/*
	@brief  Toan bo cau hinh Module SIM.
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Config_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Config_SIM=0;
	switch(SimConfig_step)
	{
		case 0:
			SimConfig_step++;
			Transmit_Data_Uart(*sUart1->huart, "Setup Module Sim");
			break;

		case 1:
			check_Config_SIM = Setup_SIM(sUart1,sUart3);
			if(check_Config_SIM==1) SimConfig_step++;
			break;
		
		case 2:
			check_Config_SIM = Config_Uart_Sim(sUart1,sUart3);
			if(check_Config_SIM == 1)      SimConfig_step++;
			else if(check_Config_SIM ==-1) SimConfig_step=0;
			break;

		case 3:
			check_Config_SIM = Config_SMS(sUart1, sUart3);
			if(check_Config_SIM == 1)      SimConfig_step++;
			else if(check_Config_SIM ==-1) SimConfig_step=0;
			break;
		
		default :
			check_Config_SIM = Config_TCP(sUart1, sUart3);
			if(check_Config_SIM==1) 
			{
				SimConfig_step=0;
				return 1;
			}
			else if(check_Config_SIM==-1) 
			{
				SimConfig_step=0;
			}
			break;
	}
	return 0;
}

/*
	@brief  Ket noi Module Sim voi Server
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (-1) Loi
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
*/
int8_t Connect_Server_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	int8_t check_Connect_Server_SIM=0;
	switch(SimConnect_step)
	{
		case 0:
			time_3_Connect_SIM_step++;
			SimConnect_step++;
			break;
		
		case 1:
			check_Connect_Server_SIM = Transmit_CFUN(sUart1,sUart3);
			if(check_Connect_Server_SIM ==1)       SimConnect_step++;
			else if(check_Connect_Server_SIM ==-1) SimConnect_step=0;
			break;
		
		case 2:
			check_Connect_Server_SIM = Config_Uart_Sim(sUart1,sUart3);
			if(check_Connect_Server_SIM ==1)       SimConnect_step++;
			else if(check_Connect_Server_SIM ==-1) SimConnect_step=0;
			break;

		case 3:
			check_Connect_Server_SIM = Config_SMS(sUart1, sUart3);
			if(check_Connect_Server_SIM ==1)       SimConnect_step++;
			else if(check_Connect_Server_SIM ==-1) SimConnect_step=0;
			break;
		
		default:
			check_Connect_Server_SIM = Config_TCP(sUart1, sUart3);
			if(check_Connect_Server_SIM==1) 
			{
				SimConnect_step=0;
				time_3_Connect_SIM_step=0;
				return 1;
			}
			else if(check_Connect_Server_SIM==-1) 
			{
				SimConnect_step=0;
			}
			break;
	}
	if(time_3_Connect_SIM_step > 3)
	{
		SimConnect_step=0;
		time_3_Connect_SIM_step=0;
		return -1;
	}
	return 0;
}

/*
	@brief  Hoi va lay thoi gian thuc 
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@param  RTC_Current Struct luu thoi gian thuc
	@return (1) Hoan thanh
	@return (0) Chua hoan thanh
	@return (-1) Loi
*/
int8_t Get_Real_Time(UART_BUFFER *sUart1, UART_BUFFER *sUart3, REAL_TIME *RTC_Current)
{
	if(check_systick_CCLK == 0)
	{
		time_3_CCLK++;
		if(time_3_CCLK >= 3)
		{
			time_3_CCLK = 0;
			return -1;
		}
		Transmit_Data_Uart(*sUart3->huart,"AT+CCLK?");
		get_systick_CCLK = HAL_GetTick();
		check_systick_CCLK = 1;
	}
	if(check_systick_CCLK == 1)
	{
		if(HAL_GetTick() - get_systick_CCLK > TIME_RESEND_COMMAND_IF_NOT_RECEIVE)
		{
			check_systick_CCLK = 0;
		}
	}
	
	if(Check_CountBuffer_Complete_Uart3(sUart3)==1)
	{
		if(strstr(sUart3->sim_rx,"CCLK:") != NULL) 
		{
			time_3_CCLK = 0;
			Transmit_Data_Uart(*sUart1->huart, sUart3->sim_rx);
			for(uint16_t i=0; i<sUart3->countBuffer;i++)
			{
				if(sUart3->sim_rx[i] == '"' && sUart3->sim_rx[i+21] == '"')
				{
					RTC_Current->Seconds = (sUart3->sim_rx[i+16]- 48)*10 + sUart3->sim_rx[i+17] -48;
					RTC_Current->Minutes = (sUart3->sim_rx[i+13]- 48)*10 + sUart3->sim_rx[i+14] -48;
					RTC_Current->Hour    = (sUart3->sim_rx[i+10]- 48)*10 + sUart3->sim_rx[i+11] -48;
					
					RTC_Current->Date  = (sUart3->sim_rx[i+7]- 48)*10 + sUart3->sim_rx[i+8] -48;
					RTC_Current->Month = (sUart3->sim_rx[i+4]- 48)*10 + sUart3->sim_rx[i+5] -48;
					RTC_Current->Year  = (sUart3->sim_rx[i+1]- 48)*10 + sUart3->sim_rx[i+2] -48;
					
					check_systick_CCLK = 0;
					if(Check_Receive_sendData_Control(sUart1,sUart3)==1) Delete_Buffer(sUart3);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
	@brief  Phat hien mat ket noi voi server
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return (1) Mat ket noi
	@return (0) Khong mat ket noi
*/
int8_t Check_Disconnect_Error(UART_BUFFER *sUart1, UART_BUFFER *sUart3)
{
	if(Check_CountBuffer_Complete_Uart3(sUart3) == 1)
	{
		if(strstr(sUart3->sim_rx,"DISCONNECTED") != NULL) 
		{
			Transmit_Data_Uart(*sUart1->huart, sUart3->sim_rx);
			Delete_Buffer(sUart3);
			return 1;
		}
		
		if(strstr(sUart3->sim_rx,"ERROR") != NULL) 
		{
			Transmit_Data_Uart(*sUart1->huart, sUart3->sim_rx);
			Delete_Buffer(sUart3);
			return 1;
		}
	}
	return 0;
}

/*
	@brief  Kiem tra chuoi Uart nhan duoc truoc khi xoa
	@param  sUart1 va sUart3: struct cua Uart1 va Uart3
	@return answer so lan xuat hien chuoi can kiem tra trong chuong trinh
*/
int8_t Check_Receive_sendData_Control(UART_BUFFER *sUart1,UART_BUFFER *sUart3)
{
	uint8_t answer=0;
	if(strstr(sUart3->sim_rx,"SUCCESS") != NULL) 
	{
		answer++;
	}
	
	if(strstr(sUart3->sim_rx,"CTL+") != NULL) 
	{
		answer++;
	}
	
	if(strstr(sUart3->sim_rx,">") != NULL) 
	{
		answer++;
	}
	
	if(strstr(sUart3->sim_rx,"DISCONNECTED") != NULL) 
	{
		answer++;
	}
	
	if(strstr(sUart3->sim_rx,"ERROR") != NULL) 
	{
		answer++;
	}
	
	if(strstr(sUart3->sim_rx,"FAIL") != NULL) 
	{
		answer++;
	}

	if(strstr(sUart3->sim_rx,"CCLK:") != NULL) 
	{
		answer++;
	}
	return answer;
}




