#ifndef __UART_SIM_
#define __UART_SIM_

#include "string.h"
#include "stm32l1xx_hal.h"
#include "user_uart.h"

#define GPIO_PIN_ON_OFF_SIM                GPIOB
#define GPIO_PIN_PWKEY_SIM                 GPIOB
#define GPIO_PIN_RESET_SIM                 GPIOC

#define PIN_ON_OFF_SIM                     GPIO_PIN_13
#define PIN_PWKEY_SIM                      GPIO_PIN_0
#define PIN_RESET_SIM                      GPIO_PIN_5

#define TIME_ON_SIM                        23000
#define TIME_RESEND_COMMAND_IF_NOT_RECEIVE 500

#define TIME_RESEND_COMMAND_MODULE_SIM     500
#define TIME_RESEND_COMMAND_CONNECT_SIM    10000

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
													 
int8_t Compare_Uart1_RX_Uart3_TX(UART_BUFFER *sUart1, UART_BUFFER *sUart3,char* response);	
int8_t Compare_Uart1_RX_Uart3_TX_True_Display(UART_BUFFER *sUart1, UART_BUFFER *sUart3,char* response);
int8_t Uart1_To_Uart3(UART_BUFFER *sUart1, UART_BUFFER *sUart3);

int8_t Connect_Server_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3);
int8_t Check_Disconnect_Error(UART_BUFFER *sUart1, UART_BUFFER *sUart3);
int8_t Config_SIM(UART_BUFFER *sUart1, UART_BUFFER *sUart3);

int8_t Get_Real_Time(UART_BUFFER *sUart1, UART_BUFFER *sUart3, REAL_TIME *RTC_Current);

int8_t Check_Receive_sendData_Control(UART_BUFFER *sUart1,UART_BUFFER *sUart3);
#endif
