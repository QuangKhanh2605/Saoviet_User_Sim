#ifndef __USER_UART_
#define __USER_UART_

#include "stm32l1xx_hal.h"
#include "string.h"

#define	COMPLETE_RECEIVE_UART_TIME_MS_BAUDRATE_1200         9
#define	COMPLETE_RECEIVE_UART_TIME_MS_BAUDRATE_2400         4
#define	COMPLETE_RECEIVE_UART_TIME_MS_BAUDRATE_4800         2
#define COMPLETE_RECEIVE_UART_TIME_MS_BAUDRATE_9600_115200  1

#define LENGTH_BUFFER_UART 5000

typedef struct
{
	UART_HandleTypeDef* huart;
	uint8_t buffer;
	uint16_t countBuffer;
	char sim_rx[LENGTH_BUFFER_UART];
}UART_BUFFER;

int8_t Check_CountBuffer_Complete_Uart1(UART_BUFFER *sUart);
int8_t Check_CountBuffer_Complete_Uart3(UART_BUFFER *sUart);

void Transmit_Data_Uart(UART_HandleTypeDef huart,char data[]);
void Transmit_Data_Uart_Length(UART_HandleTypeDef huart, char data[], uint16_t length);
void Delete_Buffer(UART_BUFFER *sUart);
#endif
