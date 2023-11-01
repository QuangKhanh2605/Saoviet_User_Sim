#ifndef __USER_UART_
#define __USER_UART_

#include "stm32l1xx_hal.h"
#include "string.h"
typedef struct
{
	UART_HandleTypeDef* huart;
	uint8_t buffer;
	uint16_t countBuffer;
	char sim_rx[5000];
}UART_BUFFER;

int8_t Check_CountBuffer_Complete_Uart1(UART_BUFFER *rx_uart);
int8_t Check_CountBuffer_Complete_Uart3(UART_BUFFER *rx_uart);
int8_t Check_Rx_Complete(UART_BUFFER *rx_uart);

void Transmit_Data_Uart(UART_HandleTypeDef huart,void* data);
void Delete_Buffer(UART_BUFFER *rx_uart);
#endif
