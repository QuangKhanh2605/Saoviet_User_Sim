#ifndef __UART_SIM_
#define __UART_SIM_

#include "string.h"
#include "stm32l1xx_hal.h"
#include "user_uart.h"

#define On_Off_Sim GPIO_PIN_13
#define Pin_PWKEY  GPIO_PIN_0
#define Pin_RESET  GPIO_PIN_5

#define TIME_ON_SIM                        23000
#define TIME_RESEND_COMMAND_IF_NOT_RECEIVE 500

#define TIME_RESEND_COMMAND_MODULE_SIM     500
#define TIME_RESEND_COMMAND_CONNECT_SIM    10000

#define LENGTH_BUFFER_UART 5000

typedef struct
{
	uint8_t Send_Data_Server;
	uint8_t Year;
	uint8_t Month;
	uint8_t Date;
	uint8_t Hour;
	uint8_t Minutes;
	uint8_t Seconds;
}REAL_TIME;
													 
int8_t Compare_Uart1_RX_Uart3_TX(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3,char* response);	
int8_t Compare_Uart1_RX_Uart3_TX_True_Display(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3,char* response);
int8_t Uart1_To_Uart3(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3);

int8_t Wait_SMS_Receive(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3,char* response);

void Config_SIM_Before_While(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, uint32_t runTime );

int8_t Connect_Server_SIM(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3);
int8_t Check_Disconnect_Error(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3);
int8_t Config_SIM_In_While(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3);

void Get_Real_Time(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, REAL_TIME *RTC_Current,uint8_t *check_connect, uint8_t *Get_RTC, uint8_t *Get_RTC_Complete);
#endif
