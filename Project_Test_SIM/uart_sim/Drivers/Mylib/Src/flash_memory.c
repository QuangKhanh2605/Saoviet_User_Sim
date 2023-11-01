#include "Flash_Memory.h"

uint32_t startpage = FLASH_USER_START_ADDR;
void FLASH_WritePage(uint32_t startPage, uint32_t endPage,uint32_t check, uint32_t data1,uint32_t data2,uint32_t data3)
{
  HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = startPage;
	EraseInit.NbPages = (endPage - startPage)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage , check);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 4, data1); //4 byte dau tien
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 8, data2); // 4byte tiep theo
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startPage + 12, data3); // 4byte tiep theo
  HAL_FLASH_Lock();
}

void FLASH_WriteNews(uint32_t addr_start_write,char News[])
{
  HAL_FLASH_Unlock();
	uint8_t length_tmp = BYTE_OF_THE_NEWS/4;
	uint32_t tmp[length_tmp];
	for(uint8_t i=0;i<length_tmp;i++)
	{
		tmp[i]=News[i];
		for(uint8_t j=i*4;j<i*4+4;j++)
		{
			tmp[i]=tmp[i]<< 8 | News[j];
		}
	}
	for(uint8_t i=0;i<length_tmp;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_write , tmp[i]);
		(addr_start_write)=(addr_start_write)+4;
	}
  HAL_FLASH_Lock();
}

void FLASH_ReadNews(uint32_t addr_start_read,char News[])
{
	uint8_t length_tmp = BYTE_OF_THE_NEWS/4;
	uint32_t tmp[length_tmp];
	for(uint8_t i=0;i<length_tmp;i++)
	{
		tmp[i] = *(__IO uint32_t *)(addr_start_read);
	(addr_start_read)=(addr_start_read)+4;
	}
	for(uint8_t i=0;i<length_tmp;i++)
	{
		uint8_t drop = 32-8;
		for(uint8_t j=i*4;j<i*4+4;j++)
		{
			News[j]=tmp[i] >> drop; 
			drop=drop-8;
		}
	}
}

uint32_t FLASH_ReadData32(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
}

void FLASH_WriteNews_Earse(uint32_t addr_start_write,char News[])
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = addr_start_write;
	EraseInit.NbPages = (1024)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	
	uint8_t length_tmp = BYTE_OF_THE_NEWS/4;
	uint32_t tmp[length_tmp];
	for(uint8_t i=0;i<length_tmp;i++)
	{
		tmp[i]=News[i];
		for(uint8_t j=i*4;j<i*4+4;j++)
		{
			tmp[i]=tmp[i]<< 8 | News[j];
		}
	}
	for(uint8_t i=0;i<length_tmp;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_write , tmp[i]);
		(addr_start_write)=(addr_start_write)+4;
	}
  HAL_FLASH_Lock();
}

