#include "user_LCD_object.h"

uint32_t check_time1=0;
uint32_t check_time2=0;
uint32_t check_time3=0;

uint32_t check_SS=0;
uint16_t check_MM=0;
uint16_t check_HH=0;

char HH[3],MM[3],SS[3];

char stamp_time1[9];
char stamp_time2[9];
char stamp_time3[9];

LCD_Object_Display LCD_Running={"Running ",0,0,0};
LCD_Object_Display LCD_Setup={"SETUP  ",0,0,0};

LCD_Object_Display LCD_SetupT1={"T1:         Giay",0,1,1};
LCD_Object_Display LCD_SetupT2={"T2:         Giay",0,1,1};
LCD_Object_Display LCD_SetupT3={"T3:         Giay",0,1,1};

LCD_Object_Display LCD_Time_HH={HH,8,0,1};
LCD_Object_Display LCD_Time_MM={MM,11,0,1};
LCD_Object_Display LCD_Time_SS={SS,14,0,1};

LCD_Object_Display LCD_Time1={stamp_time1,4,2,1};
LCD_Object_Display LCD_Time2={stamp_time2,4,2,1};
LCD_Object_Display LCD_Time3={stamp_time3,4,2,1};

void LCD_Change_State_Setup_T1_T2_T3(uint32_t stampTime1, uint32_t stampTime2, uint32_t stampTime3)
{
	if (stampTime1!=check_time1)
	{
		check_time1=stampTime1;
		LCD_Time1.state=1;
	}
	
	if (stampTime2!=check_time2)
	{
		check_time2=stampTime2;
		LCD_Time2.state=1;
	}
	
	if (stampTime3!=check_time3)
	{
		check_time3=stampTime3;
		LCD_Time3.state=1;
	}
}

void LCD_Change_State_Time_HH_MM_SS(uint16_t hh, uint16_t mm, uint32_t ss)
{
	if(check_SS!=ss)
	{
		check_SS=ss;
		LCD_Time_SS.state=1;
	}
	
	if(check_MM!=mm)
	{
		check_MM=mm;
		LCD_Time_MM.state=1;
	}
	if(check_HH!=hh)
	{
		check_HH=hh;
		LCD_Time_HH.state=1;
	}
}

void UintTime_To_CharTime_HH_MM_SS(uint16_t hh, uint16_t mm, uint32_t ss)
{
	Variable_To_Char_Time(HH, hh);
	Variable_To_Char_Time(MM, mm);
	Variable_To_Char_Time(SS, ss);
}

void UintTime_To_CharTime_T1_T2_T3(uint32_t stampTime1, uint32_t stampTime2, uint32_t stampTime3)
{
	Variable_To_Char(stamp_time1, stampTime1);
	Variable_To_Char(stamp_time2, stampTime2);
	Variable_To_Char(stamp_time3, stampTime3);
}

void USER_LCD_Display_Time(CLCD_Name* LCD)
{
	LCD_Display_Time(LCD, &LCD_Time_HH, &LCD_Time_MM, &LCD_Time_SS);
}

void USER_LCD_Display_Running_OR_Setup(uint16_t State)
{
	if(State==0)
	{
		if(LCD_Running.state==0) LCD_Setup.state=1;
	}
	if(State==1)
	{
		if(LCD_Setup.state==0) LCD_Running.state=1;
	}
}

void USER_LCD_Display_Running(CLCD_Name* LCD, uint16_t setupCount)
{
	LCD_Display_Running_OR_Setup(LCD, &LCD_Running, &LCD_Setup);
	LCD_Display_Esc(LCD, setupCount ,&LCD_SetupT1, &LCD_SetupT2, &LCD_SetupT3);
	LCD_Display_Time1_Time2_Time3(LCD, setupCount ,&LCD_Time1, &LCD_Time2, &LCD_Time3);
}
void USER_LCD_Display_Setup(CLCD_Name* LCD, uint16_t setupCount)
{
	LCD_Display_Running_OR_Setup(LCD, &LCD_Setup, &LCD_Running);
	LCD_Display_Esc(LCD, setupCount ,&LCD_SetupT1, &LCD_SetupT2, &LCD_SetupT3);
	LCD_Display_Time1_Time2_Time3(LCD, setupCount ,&LCD_Time1, &LCD_Time2, &LCD_Time3);
}

void USER_LCD_Change_Setup(void)
{
	  LCD_SetupT1.state=1;
		LCD_SetupT2.state=1;
		LCD_SetupT3.state=1;
		
		LCD_Time1.state=1;
		LCD_Time2.state=1;
		LCD_Time3.state=1;
}



