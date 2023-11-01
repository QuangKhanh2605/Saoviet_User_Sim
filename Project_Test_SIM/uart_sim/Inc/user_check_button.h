#ifndef __USER_CHECK_BUTTON_
#define __USER_CHECK_BUTTON_

#include "user_LCD_object.h"
#include "check_Button.h"
#include "Flash_Memory.h"

#define PIN_BT_ENTER GPIO_PIN_2
#define PIN_BT_UP    GPIO_PIN_3
#define PIN_BT_DOWN  GPIO_PIN_4
#define PIN_BT_ESC   GPIO_PIN_5

#define GPIO_BT_ENTER GPIOD
#define GPIO_BT_UP    GPIOB
#define GPIO_BT_DOWN  GPIOB
#define GPIO_BT_ESC   GPIOB

void Check_BT_ENTER(uint16_t *State,uint16_t *checkState, uint16_t *setupCount,uint32_t *time1, uint32_t *time2, uint32_t *time3);
void Check_BT_ESC(uint16_t State, uint16_t *setupCount);
void Check_BT_UP(uint16_t State);
void Check_BT_DOWN(uint16_t State);

void Run_Begin(uint16_t *setupCount, uint32_t time1, uint32_t time2, uint32_t time3);
void BT_Esc_Exit_Setup(uint16_t *State, uint16_t *setupCount,uint32_t *time1, uint32_t *time2, uint32_t *time3);



#endif
