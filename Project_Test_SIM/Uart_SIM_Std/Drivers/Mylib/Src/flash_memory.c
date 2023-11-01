#include "Flash_Memory.h"

//uint32_t startpage = FLASH_USER_START_ADDR;

/*
	@brief  Ghi du lieu vao Flash
	@param  StartPage page flash bat dau viet
	@param  endPage page flash ket thuc viet
	@param  data1, data2, data3 du lieu can ghi vao Flash
	@retval None
*/
void FLASH_WritePage(uint32_t startPage, uint32_t endPage, uint32_t check, uint32_t data1, uint32_t data2,uint32_t data3)
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

/*
	@brief  Ghi du lieu vao Flash
	@param  addr_start_write page flash bat dau viet
	@param  News ban tin viet vao Flash
	@param  addr_start_save page luu gia tri cua dia chi viet va doc 
	@param  addr_read dia chi doc
	@param  addr_write dia chi viet
	@retval None
*/
void FLASH_WriteNews(uint32_t addr_start_write, char News[], uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write)
{
  HAL_FLASH_Unlock();
	uint8_t length_tmp = LENGTH_BYTE_OF_THE_NEWS_FLASH/4;
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
	
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = addr_start_save;
	EraseInit.NbPages = (1024)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save , addr_read);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save + 4, addr_write); //4 byte dau tien
  HAL_FLASH_Lock();
}

/*
	@brief  Xoa va Ghi du lieu vao Flash
	@param  addr_start_write page flash bat dau viet
	@param  News ban tin viet vao Flash
	@param  addr_start_save page luu gia tri cua dia chi dang viet va dang doc 
	@param  addr_read dia chi doc
	@param  addr_write dia chi viet
	@retval None
*/
void FLASH_WriteNews_Earse(uint32_t addr_start_write, char News[], uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = addr_start_write;
	EraseInit.NbPages = (1024)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	
	uint8_t length_tmp = LENGTH_BYTE_OF_THE_NEWS_FLASH/4;
	uint32_t tmp[length_tmp];
	for(uint8_t i=0; i<length_tmp; i++)
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
	
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = addr_start_save;
	EraseInit.NbPages = (1024)/FLASH_PAGE_SIZE;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save , addr_read);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save + 4, addr_write); //4 byte dau tien
  HAL_FLASH_Lock();
}

/*
	@brief  Doc du lieu tu Flash
	@param  addr_start_read dia chi bat dau doc
	@param  News ban tin doc duoc tu Flash
	@retval None
*/
void FLASH_ReadNews(uint32_t addr_start_read, char News[])
{
	uint8_t length_tmp = LENGTH_BYTE_OF_THE_NEWS_FLASH/4;
	uint32_t tmp[length_tmp];
	for(uint8_t i=0; i<length_tmp; i++)
	{
		tmp[i] = *(__IO uint32_t *)(addr_start_read);
		(addr_start_read)=(addr_start_read)+4;
	}
	for(uint8_t i=0; i<length_tmp; i++)
	{
		uint8_t drop = 32-8;
		for(uint8_t j=i*4; j<i*4+4; j++)
		{
			News[j]=tmp[i] >> drop; 
			drop=drop-8;
		}
	}
}

/*
	@brief  Doc byte tu Flash
	@param  addr dia chi byte muon doc trong flash
	@return du lieu tai dia chi muon doc
*/
uint32_t FLASH_ReadData32(uint32_t addr)
{
	uint32_t data = *(__IO uint32_t *)(addr);
	return data;
}

/*
	@brief  Ghi dia chi dang doc va dang viet vao Flash
	@param  addr_start_save page ghi gia tri
	@param  addr_read dia chi doc
	@param  addr_write dia chi viet
	@retval None
*/
void FLASH_Write_Addr_Page_Write_Read(uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write)
{
  HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInit;
	EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInit.PageAddress = addr_start_save;
	EraseInit.NbPages = (1024)/FLASH_PAGE_SIZE;
	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInit, &PageError);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save , addr_read);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr_start_save + 4, addr_write); //4 byte dau tien
  HAL_FLASH_Lock();
}

