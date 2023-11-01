#ifndef __CONVERT_VARIABLE_
#define __CONVERT_VARIABLE_

#include "stm32l1xx_hal.h"
#include "math.h"

#define LENGTH_MOD_FLOAT 1
#define ASCII_VALUE_NUMBER 48

void Uint_To_Float_Mod(uint32_t variable_uint, float *variable_float);
void Uint_To_Char_Time(char time[], uint16_t stamp);
void Uint_To_Char_Length(char time[], uint16_t stamp, uint16_t *lengthStamp);
void Uint_To_Char(char time[], uint32_t stamp);
void Float_To_Char(char time[], float stamp);
void Uint_To_Char_Sim(char time[], uint32_t stamp, uint16_t *location);
void Char_To_Uint_Time(char timeChar[], uint16_t length, uint32_t *timeUint);

void Variable_To_Char_Time(char time[], uint16_t stamp);
void Variable_To_Char_Length(char time[], uint16_t stamp, uint16_t *lengthStamp);
void Variable_To_Char(char time[], uint32_t stamp);
#endif
