#ifndef __USER_SIM_
#define __USER_SIM_

#include "uart_sim.h"
#include "math.h"
#include "convert_variable.h"
//#include "linked_list.h"
#include "Flash_Memory.h"
#include "stdio.h"

#define TIME_ERROR_CIPSEND 1000

void Linked_List_Init(void);

uint8_t Receive_SMS_Setup(char *sim_rx,uint32_t *time1,uint32_t *time2,uint32_t *time3);
int8_t Receive_Control_Setup(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, uint8_t *check_connect, uint32_t *time1, uint32_t *time2,uint32_t *time3);
int8_t Check_Receive_sendData_Control(UART_BUFFER *rx_uart1,UART_BUFFER *rx_uart3);
void Reset_SendData(void);

void Char_To_Uint(char *char_time,uint32_t *uint_time, uint16_t length);
int8_t SendData_Server(UART_BUFFER *rx_uart1, UART_BUFFER *rx_uart3, REAL_TIME *RTC_Current);
void Packing_News(REAL_TIME *RTC_Current);

#endif

