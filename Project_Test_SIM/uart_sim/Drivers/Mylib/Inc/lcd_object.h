#ifndef __LCD_OBJECT_H
#define	__LCD_OBJECT_H

#include "stm32l1xx_hal.h"
#include "CLCD.h"

typedef struct
{
	char *Object;
	uint16_t col;
	uint16_t rol;
	uint16_t state;
}LCD_Object_Display;

void LCD_Display_Running_OR_Setup(CLCD_Name* LCD, LCD_Object_Display *Name_Object_Run,
                                             LCD_Object_Display *Name_Object_Stop);

void LCD_Display_Esc(CLCD_Name* LCD,uint16_t setupCount,
																LCD_Object_Display *Name_Object1,
                                LCD_Object_Display *Name_Object2,
                                LCD_Object_Display *Name_Object3);

void LCD_Display_Time1_Time2_Time3(CLCD_Name* LCD,uint16_t setupCount,
																		 LCD_Object_Display *Name_Object1,
                                     LCD_Object_Display *Name_Object2,
                                     LCD_Object_Display *Name_Object3);

void LCD_Display_Time(CLCD_Name* LCD,LCD_Object_Display *Name_Object1,
                                     LCD_Object_Display *Name_Object2,
                                     LCD_Object_Display *Name_Object3);

void LCD_Send_Data(CLCD_Name* LCD, LCD_Object_Display *Name_Object);

#endif
