#ifndef __USER_LCD_OBJECT_
#define __USER_LCD_OBJECT_

#include "LCD_object.h"
#include "convert_variable.h"

void LCD_Change_State_Setup_T1_T2_T3(uint32_t stampTime1, uint32_t stampTime2, uint32_t stampTime3);
void LCD_Change_State_Time_HH_MM_SS(uint16_t hh, uint16_t mm, uint32_t ss);

void UintTime_To_CharTime_HH_MM_SS(uint16_t hh, uint16_t mm, uint32_t ss);
void UintTime_To_CharTime_T1_T2_T3(uint32_t stampTime1, uint32_t stampTime2, uint32_t stampTime3);


void USER_LCD_Display_Time(CLCD_Name* LCD);
void USER_LCD_Display_Running_OR_Setup(uint16_t State);

void USER_LCD_Display_Running(CLCD_Name* LCD, uint16_t setupCount);
void USER_LCD_Display_Setup(CLCD_Name* LCD, uint16_t setupCount);

void USER_LCD_Change_Setup(void);

#endif
