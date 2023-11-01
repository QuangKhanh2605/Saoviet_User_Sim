#include "user_sim.h"

const uint32_t maxTime=9999999;
char News[BYTE_OF_THE_NEWS];
uint32_t count_news=0;
uint32_t getTick_error_cipsend=0;
uint8_t check_cipsend=0;
uint8_t send_data=0;

char Sim_Control[4]= "CTL+";

uint32_t flash_addr_write=FLASH_ADDR_PAGE_NEWS_START;
uint32_t flash_addr_read =FLASH_ADDR_PAGE_NEWS_START;
uint32_t flash_page_write=FLASH_ADDR_PAGE_NEWS_START;
uint32_t flash_page_read =FLASH_ADDR_PAGE_NEWS_START;

//LinkedList List_News;
//uint32_t Length_News=0;
//void Linked_List_Init(void)
//{
//	InitializeList( &List_News);
//}

void Control_Write_News_Flash(void);
void Control_Read_News_Flash(void);
void Write_Data_News(REAL_TIME *RTC_Current, char News[], uint32_t lengthNews, uint32_t countNews);
int8_t Error_Cipsend(void);

int8_t SendData_Server(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, REAL_TIME *RTC_Current)
{
	if(Error_Cipsend() == 1) return -1;
	
	if(send_data == 1)
	{
		if(check_cipsend == 0) 
		{
			if(RTC_Current->Send_Data_Server == 1)
			{
				send_data = 0;
				return 0;
			}
			//Length_News=Length_LinkedList(&List_News);
			if(flash_addr_read != flash_addr_write)
			{
				Transmit_Data_Uart(*rx_uart3->huart,"AT+CIPSEND=1,32");
				Transmit_Data_Uart(*rx_uart1->huart,"AT+CIPSEND=1,32");
				check_cipsend=1;
				getTick_error_cipsend = HAL_GetTick();
			}
		}
		if(check_cipsend == 1)
		{
			if(Check_CountBuffer_Complete_Uart3(rx_uart3)==1)
			{
				if(Check_Disconnect_Error(rx_uart1, rx_uart3)!=1) 
				{
					if(strstr(rx_uart3->sim_rx,">") != NULL) 
					{
						FLASH_ReadNews(flash_addr_read, News);
						Transmit_Data_Uart(*rx_uart1->huart, rx_uart3->sim_rx);
						Transmit_Data_Uart(*rx_uart3->huart,News);
						Transmit_Data_Uart(*rx_uart1->huart,News);
						//Delete_Node(&List_News, List_News.head);
						check_cipsend = 2;
						getTick_error_cipsend = HAL_GetTick();
					}
					if(Check_Receive_sendData_Control(rx_uart1,rx_uart3)==1) Delete_Buffer(rx_uart3);
				}
				else
				{
					check_cipsend = 0;
					return 1;
				}
			}
		}
			
		if(check_cipsend==2)
		{
			if(Check_CountBuffer_Complete_Uart3(rx_uart3)==1)
			{
				if(Check_Disconnect_Error(rx_uart1, rx_uart3)!=1) 
				{
					if(strstr(rx_uart3->sim_rx,"SUCCESS") != NULL) 
					{
						Transmit_Data_Uart(*rx_uart1->huart, rx_uart3->sim_rx);
						if(Check_Receive_sendData_Control(rx_uart1,rx_uart3)==1) Delete_Buffer(rx_uart3);
						Control_Read_News_Flash();
						check_cipsend=0;
						send_data=0;
					}
					if(strstr(rx_uart3->sim_rx,"FAIL") != NULL) 
					{
						check_cipsend = 0;
						Delete_Buffer(rx_uart3);
						return 1;
					}
				}
				else
				{
					check_cipsend = 0;
					return 1;
				}
			}
		}
	}
	return 0;
}

void Reset_SendData(void)
{
	check_cipsend=0;
}

void Packing_News(REAL_TIME *RTC_Current)
{
	if(check_cipsend == 0)
	{
		send_data = 0;
	}
	
	if(send_data == 0)
	{
		//Length_News=Length_LinkedList(&List_News);
		if(RTC_Current->Send_Data_Server == 1 )
		{
			RTC_Current->Send_Data_Server = 0;
			count_news++;	
			Write_Data_News(RTC_Current, News, BYTE_OF_THE_NEWS, count_news);
			//Add_Tail(&List_News, News);
			if(flash_addr_write == flash_page_write)
			{
				FLASH_WriteNews_Earse(flash_addr_write, News);
			}
			else
			{
				FLASH_WriteNews(flash_addr_write,News);
			}
			Control_Write_News_Flash();
			send_data = 1;
		}
		
		if(flash_addr_read != flash_addr_write )
		{
			send_data = 1;
		}
	}
}

void Control_Write_News_Flash(void)
{
	if(flash_addr_write + BYTE_OF_THE_NEWS > flash_page_write + FLASH_BYTE_OF_PAGE)
	{
		flash_page_write = flash_page_write + FLASH_BYTE_OF_PAGE;
		flash_addr_write = flash_page_write;
	}
	else
	{
		flash_addr_write = flash_addr_write + BYTE_OF_THE_NEWS;
		if(flash_addr_write == flash_page_write + FLASH_BYTE_OF_PAGE)
		{
			flash_page_write = flash_page_write + FLASH_BYTE_OF_PAGE;
		}
	}
	
	if(flash_page_write == FLASH_ADDR_PAGE_NEWS_END )
	{
		flash_page_write = FLASH_ADDR_PAGE_NEWS_START;
		flash_addr_write = flash_page_write;
	}	
	
	if(flash_addr_write <= flash_addr_read)
	{
		if(flash_page_write == flash_page_read)
		{
			flash_page_read = flash_page_read + FLASH_BYTE_OF_PAGE;
			flash_addr_read = flash_page_read ;
			if(flash_page_read == FLASH_ADDR_PAGE_NEWS_END )
			{
				flash_page_read = FLASH_ADDR_PAGE_NEWS_START;
				flash_addr_read = flash_page_read;
			}
		}
	}
}

void Control_Read_News_Flash(void)
{
	if(flash_addr_read + BYTE_OF_THE_NEWS > flash_page_read + FLASH_BYTE_OF_PAGE)
	{
		flash_page_read = flash_page_read + FLASH_BYTE_OF_PAGE;
		flash_addr_read = flash_page_read;
	}
	else
	{
		flash_addr_read = flash_addr_read + BYTE_OF_THE_NEWS;
		if(flash_addr_read == flash_page_read + FLASH_BYTE_OF_PAGE)
		{
			flash_page_read = flash_page_read + FLASH_BYTE_OF_PAGE;
		}
	}
	
	if(flash_page_read == FLASH_ADDR_PAGE_NEWS_END )
	{
		flash_page_read = FLASH_ADDR_PAGE_NEWS_START;
		flash_addr_read = flash_page_read;
	}
}

int8_t Error_Cipsend(void)
{
	if(check_cipsend == 1)
	{
		if(HAL_GetTick() - getTick_error_cipsend > TIME_ERROR_CIPSEND) 
		{
			check_cipsend = 0;
			return 1;
		}
	}
	
	if(check_cipsend == 2)
	{
		if(HAL_GetTick() - getTick_error_cipsend > TIME_ERROR_CIPSEND) 
		{
			check_cipsend = 0;
			return 1;
		}
	}
	return 0;
}

int8_t Receive_Control_Setup(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, uint8_t *check_connect, uint32_t *time1, uint32_t *time2,uint32_t *time3)
{
	if(Check_CountBuffer_Complete_Uart3(rx_uart3)==1)
	{
		if(Check_Disconnect_Error(rx_uart1, rx_uart3)!=1) 
		{
			if(strstr(rx_uart3->sim_rx,Sim_Control) != NULL)
			{
				Transmit_Data_Uart(*rx_uart1->huart, rx_uart3->sim_rx);
				if(strstr(rx_uart3->sim_rx,"ONLED5") != NULL)
				{
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
				}
				if(strstr(rx_uart3->sim_rx,"OFFLED5") != NULL)
				{
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
				}
				if(strstr(rx_uart3->sim_rx,"SETUP") != NULL)
				{
					if(Receive_SMS_Setup(rx_uart3->sim_rx, time1, time2, time3)==1)
					{
					  if(Check_Receive_sendData_Control(rx_uart1,rx_uart3)==1) Delete_Buffer(rx_uart3);
						return 1;
					}
				}
				if(strstr(rx_uart3->sim_rx,"RESET") != NULL)
				{
					if(Check_Receive_sendData_Control(rx_uart1,rx_uart3)==1) Delete_Buffer(rx_uart3);
					return 1;
				}
				if(Check_Receive_sendData_Control(rx_uart1,rx_uart3)==1) Delete_Buffer(rx_uart3);
			}	
		}
		else
		{
			*check_connect=1;
		}
	}
	return 0;
}


uint8_t Receive_SMS_Setup(char *sim_rx, uint32_t *time1, uint32_t *time2, uint32_t *time3)
{
	uint16_t length=strlen(sim_rx);
	uint16_t length_time1=0;
	uint16_t length_time2=0;
	uint16_t length_time3=0;
	
	char char_time1[16];
	char char_time2[16];
	char char_time3[16];
	
	for(int i=0; i<length;i++)
	{
		if(sim_rx[i]=='T' || sim_rx[i]=='t')
		{
			if(sim_rx[i+2]=='=' || sim_rx[i+2]==':')
			{
				if(sim_rx[i+1]=='1')
				{
					for(int j=i+3;j<length;j++)
					{
						if(sim_rx[j] < '0' || sim_rx[j] > '9')
						{
							break;
						}
						char_time1[length_time1]=sim_rx[j];
						length_time1++;
					}
				}
				
				if(sim_rx[i+1]=='2')
				{
					for(int j=i+3;j<length;j++)
					{
						if(sim_rx[j] < '0' || sim_rx[j] > '9')
						{
							break;
						}
						char_time2[length_time2]=sim_rx[j];
						length_time2++;
					}
				}
				
				if(sim_rx[i+1]=='3')
				{
					for(int j=i+3;j<length;j++)
					{
						if(sim_rx[j] < '0' || sim_rx[j] > '9')
						{
							break;
						}
						char_time3[length_time3]=sim_rx[j];
						length_time3++;
					}
				}
			}
		}
	}
	Char_To_Uint(char_time1, time1, length_time1);
	Char_To_Uint(char_time2, time2, length_time2);
	Char_To_Uint(char_time3, time3, length_time3);
	if(length_time1 == 0 & length_time2 == 0 & length_time3 == 0 ) return 0;
	else return 1;
}

void Char_To_Uint(char *char_time,uint32_t *uint_time, uint16_t length)
{
	uint32_t time=0;
	for(int i=0;i<length;i++)
	{
		time=time+(char_time[i]-48)*pow(10,length-i-1);
	}
	if(time <= maxTime && length!=0) *uint_time=time;
}

void Write_Data_News(REAL_TIME *RTC_Current, char News[], uint32_t lengthNews, uint32_t countNews)
{
	uint16_t i=0;
	if(RTC_Current->Year<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Year, &i);
	News[i]='/';
	i++;
	
	if(RTC_Current->Month<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Month, &i);
	News[i]='/';
	i++;
	
	if(RTC_Current->Date<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Date, &i);
	
	News[i]=',';
	i++;
	if(RTC_Current->Hour<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Hour, &i);
	News[i]=':';
	i++;
	if(RTC_Current->Minutes<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Minutes, &i);
	News[i]=':';
	i++;
	if(RTC_Current->Seconds<10)
	{
		News[i]='0';
		i++;
	}
	Uint_To_Char_Sim(News, RTC_Current->Seconds, &i);
	News[i]=',';
	i++;
	Uint_To_Char_Sim(News, count_news, &i);
	while(i<lengthNews-1)
	{
		News[i]=' ';
		i++;
	}
	News[lengthNews-1]=0;
}

int8_t Check_Receive_sendData_Control(UART_BUFFER *rx_uart1,UART_BUFFER *rx_uart3)
{
	uint8_t answer=0;
	if(strstr(rx_uart3->sim_rx,"SUCCESS") != NULL) 
	{
		answer++;
	}
	
	if(strstr(rx_uart3->sim_rx,Sim_Control) != NULL) 
	{
		answer++;
	}
	
	if(strstr(rx_uart3->sim_rx,">") != NULL) 
	{
		answer++;
	}
	if(strstr(rx_uart3->sim_rx,"DISCONNECTED") != NULL) 
	{
		answer++;
	}
	if(strstr(rx_uart3->sim_rx,"ERROR") != NULL) 
	{
		answer++;
	}
	if(strstr(rx_uart3->sim_rx,"FAIL") != NULL) 
	{
		answer++;
	}
	return 1;
}



