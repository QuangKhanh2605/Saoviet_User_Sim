
#include "Relay_Led.h"

void Set_Relay_Led(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                   GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                   GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_SET);
}

void Reset_Relay_Led(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                     GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                     GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO3, GPIO_Pin3,GPIO_PIN_RESET);
}

void Set_Relay(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
               GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_SET);
}

void Reset_Relay(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                 GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2)
{
	HAL_GPIO_WritePin(GPIO1, GPIO_Pin1,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2, GPIO_Pin2,GPIO_PIN_RESET);
}
