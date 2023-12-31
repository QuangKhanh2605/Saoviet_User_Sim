#ifndef __FLASH_MEMORY_
#define __FLASH_MEMORY_

#include "stm32l1xx_hal.h"

#define FLASH_BYTE_OF_PAGE             1024
#define LENGTH_BYTE_OF_THE_NEWS        32    
#define LENGTH_BYTE_OF_THE_NEWS_FLASH  ((LENGTH_BYTE_OF_THE_NEWS%4==0) ? (LENGTH_BYTE_OF_THE_NEWS) : (LENGTH_BYTE_OF_THE_NEWS/4+1)*4)

#define FLASH_ADDR_PAGE_NEWS_START     ((uint32_t)0x08000000 + FLASH_BYTE_OF_PAGE*150) //Page 100
#define FLASH_ADDR_PAGE_NEWS_END       ((uint32_t)0x08000000 + FLASH_BYTE_OF_PAGE*253) //Page 253

#define FLASH_ADDR_PAGE_253            ((uint32_t)0x08000000 + FLASH_BYTE_OF_PAGE*253) //Page 253

#define FLASH_ADDR_PAGE_254            ((uint32_t)0x08000000 + FLASH_BYTE_OF_PAGE*254) //Page 254
#define FLASH_ADDR_PAGE_255            ((uint32_t)0x08000000 + FLASH_BYTE_OF_PAGE*255) //Page 255

#define FLASH_USER_START_ADDR          FLASH_ADDR_PAGE_254
#define FLASH_USER_END_ADDR            FLASH_ADDR_PAGE_255 

void FLASH_WriteNews(uint32_t addr_start_write,char News[], uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write);
void FLASH_WriteNews_Earse(uint32_t addr_start_write,char News[], uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write);
void FLASH_ReadNews(uint32_t addr_start_read,char News[]);

uint32_t FLASH_ReadData32(uint32_t addr);
void FLASH_WritePage(uint32_t startPage, uint32_t endPage,uint32_t check, uint32_t data1,uint32_t data2,uint32_t data3);
void FLASH_Write_Addr_Page_Write_Read(uint32_t addr_start_save, uint32_t addr_read, uint32_t addr_write);
#endif
