#ifndef __RELAY_LED_
#define __RELAY_LED_

#include "stm32l1xx_hal.h"

void Set_Relay_Led(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                   GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                   GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);

void Reset_Relay_Led(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                     GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2,  
                     GPIO_TypeDef* GPIO3, uint16_t GPIO_Pin3);

void Set_Relay(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
               GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2);

void Reset_Relay(GPIO_TypeDef* GPIO1, uint16_t GPIO_Pin1, 
                 GPIO_TypeDef* GPIO2, uint16_t GPIO_Pin2);

#endif
