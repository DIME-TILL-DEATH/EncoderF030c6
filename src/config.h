#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f0xx.h"

#define ADDR_FLASH_PAGE_0   ((uint32_t)0x08000000) /* Base @ of Page 0, 1024 bytes */
#define ADDR_FLASH_PAGE_1   ((uint32_t)0x08000400) /* Base @ of Page 1, 1024 bytes */
#define ADDR_FLASH_PAGE_2   ((uint32_t)0x08000800) /* Base @ of Page 2, 1024 bytes */
#define ADDR_FLASH_PAGE_3   ((uint32_t)0x08000C00) /* Base @ of Page 3, 1024 bytes */
#define ADDR_FLASH_PAGE_4   ((uint32_t)0x08001000) /* Base @ of Page 4, 1024 bytes */
#define ADDR_FLASH_PAGE_5   ((uint32_t)0x08001400) /* Base @ of Page 5, 1024 bytes */
#define ADDR_FLASH_PAGE_6   ((uint32_t)0x08001800) /* Base @ of Page 6, 1024 bytes */
#define ADDR_FLASH_PAGE_7   ((uint32_t)0x08001C00) /* Base @ of Page 7, 1024 bytes */
#define ADDR_FLASH_PAGE_8   ((uint32_t)0x08002000) /* Base @ of Page 8, 1024 bytes */
#define ADDR_FLASH_PAGE_9   ((uint32_t)0x08002400) /* Base @ of Page 9, 1024 bytes */
#define ADDR_FLASH_PAGE_10  ((uint32_t)0x08002800) /* Base @ of Page 10, 1024 bytes */
#define ADDR_FLASH_PAGE_11  ((uint32_t)0x08002C00) /* Base @ of Page 11, 1024 bytes */
#define ADDR_FLASH_PAGE_12  ((uint32_t)0x08003000) /* Base @ of Page 12, 1024 bytes */
#define ADDR_FLASH_PAGE_13  ((uint32_t)0x08003400) /* Base @ of Page 13, 1024 bytes */
#define ADDR_FLASH_PAGE_14  ((uint32_t)0x08003800) /* Base @ of Page 14, 1024 bytes */
#define ADDR_FLASH_PAGE_15  ((uint32_t)0x08003C00) /* Base @ of Page 15, 1024 bytes */
#define ADDR_FLASH_PAGE_16  ((uint32_t)0x08004000) /* Base @ of Page 16, 1024 bytes */
#define ADDR_FLASH_PAGE_17  ((uint32_t)0x08004400) /* Base @ of Page 17, 1024 bytes */
#define ADDR_FLASH_PAGE_18  ((uint32_t)0x08004800) /* Base @ of Page 18, 1024 bytes */
#define ADDR_FLASH_PAGE_19  ((uint32_t)0x08004C00) /* Base @ of Page 19, 1024 bytes */
#define ADDR_FLASH_PAGE_20  ((uint32_t)0x08005000) /* Base @ of Page 20, 1024 bytes */
#define ADDR_FLASH_PAGE_21  ((uint32_t)0x08005400) /* Base @ of Page 21, 1024 bytes */
#define ADDR_FLASH_PAGE_23  ((uint32_t)0x08005800) /* Base @ of Page 22, 1024 bytes */
#define ADDR_FLASH_PAGE_24  ((uint32_t)0x08005C00) /* Base @ of Page 23, 1024 bytes */


void    GetConfig_PresetBank(uint8_t nCfg, uint8_t* preset, uint8_t* bank, uint8_t* presetPrev, uint8_t* bankPrev);
void    PutConfig_PresetBank(uint8_t nCfg, uint8_t preset, uint8_t bank, uint8_t presetPrev, uint8_t bankPrev);


#endif
