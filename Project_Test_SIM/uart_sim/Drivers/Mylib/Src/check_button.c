
/*

Add "HAL_SYSTICK_IRQHandler();" To "Systick_Handler" In "stm32L1xx_it.c"

*/

#include "check_Button.h"

uint16_t check_BT_run_up=0;
uint16_t check_BT_run_down=0;
uint16_t check_BT_run_esc=0;

uint16_t run_BT_time=0;
uint16_t run_BT_begin=0;

uint16_t run_BT_Esc_time=0;

const uint16_t run_BT_end=500;
const uint16_t run_BT_irq=200;
//const uint32_t MINISECOND_OF_THE_49_DAY=4233600000;

uint32_t SYSTICK_count_ms=0;

uint32_t GET_SYSTICK_MS(void)
{
	if (SYSTICK_count_ms >= MINISECOND_OF_THE_49_DAY)
	{
		SYSTICK_count_ms=0;
	}
	return SYSTICK_count_ms;
}

void BT_Press_Click_Up(uint16_t *BT_up, uint32_t *ptr_stamp)
{
	if(*BT_up == 1)
	{
		if(*ptr_stamp < MAX_TIME) (*ptr_stamp)++;
		*BT_up=2;
	}
}

void BT_Press_Click_Down(uint16_t *BT_down, uint32_t *ptr_stamp)
{
	if(*BT_down == 1)
	{
		if(*ptr_stamp > 0)(*ptr_stamp)--;
		 *BT_down=2;
	}
}

void BT_Press_Hold_Up( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint32_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_up=1;
		if (run_BT_time>run_BT_irq)
		{
			run_BT_time=0;
			(*ptr_stamp)++;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==1) 
	{
		check_BT_run_up=0;
	}
}

void BT_Press_Hold_Down( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint32_t *ptr_stamp)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0) 
	{
		check_BT_run_down=1;
		if (run_BT_time>run_BT_irq && (*ptr_stamp)>0)
		{
			run_BT_time=0;
			(*ptr_stamp)--;
		}
	}
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==1) 
	{
		check_BT_run_down=0;
	}
}

void BT_Press_Hold_Esc( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pinx, uint16_t *State, uint16_t BT_up, uint16_t BT_down)
{
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==0 && *State==0 && BT_up==0 && BT_down==0) 
	{
	 check_BT_run_esc=1;
	 if (run_BT_Esc_time>2000)
	 {
		*State=1;
		 run_BT_Esc_time=0;
		 check_BT_run_esc=0;
	 }
	}
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pinx)==1 || BT_up==1 || BT_down==1)
	{
		run_BT_Esc_time=0;
		check_BT_run_esc=0;
	}
}
	
void HAL_SYSTICK_Callback(void)
{
	SYSTICK_count_ms++;
	if(check_BT_run_esc==1)
		run_BT_Esc_time++;
	
	if(check_BT_run_up == 1 || check_BT_run_down == 1) 
	{
		run_BT_begin++;
	}
	else 
	{
		run_BT_begin=0;
		run_BT_time=0;
	}
	
	if(run_BT_begin>run_BT_end) run_BT_time++;
}
