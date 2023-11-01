#ifndef __USER_LCD_H
#define	__USER_LCD_H
	
#include "CLCD.h"
#include "convert_variable.h"

void LCD_Running_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss);
void LCD_Running_X2(CLCD_Name* LCD, uint16_t time1, uint16_t time2, uint16_t time3);

void LCD_Setup_X1(CLCD_Name* LCD, uint16_t hh, uint16_t mm, uint16_t ss, uint16_t setupCount);
void LCD_Setup_X2(CLCD_Name* LCD, uint16_t time, uint16_t setupCount);



#endif
