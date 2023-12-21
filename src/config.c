#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_flash.h"
#include "config.h"

static uint32_t cntErrorErase    = 0;
static uint32_t cntErrorProgram  = 0;
//----------------------------------
// Variable used for Erase procedure
static FLASH_EraseInitTypeDef EraseInitStruct;
//---------------------------------------------
static HAL_StatusTypeDef FLASH_ErasePage(uint32_t addr, uint32_t countPages)
{
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = addr;
    EraseInitStruct.NbPages     = countPages;
    uint32_t PAGEError = 0;
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);
    return status;
}

static uint32_t GetConfig(uint32_t adrFlash)
{
    uint32_t cfgPrev = 0x0;
    uint32_t adr = adrFlash;
    for (int32_t i = 0; i < FLASH_PAGE_SIZE/sizeof(uint32_t); i++, adr += 4) {
        uint32_t cfg = *((uint32_t*)adr);
        if (cfg == 0xffffffff) {    // €чейка стерта
            return cfgPrev;         // возвращаем предыдущую
        }
        cfgPrev = cfg;
    }
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef status = FLASH_ErasePage(adrFlash, 1);
    if (status != HAL_OK) {
        cntErrorErase++;
    }
    uint32_t cfg = 0x0;
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adrFlash, cfg);
    if (status != HAL_OK) {
        cntErrorProgram++;
    }
    HAL_FLASH_Lock();
    return cfg;
}

static void PutConfig(uint32_t adrFlash, uint32_t cfg)
{
    HAL_StatusTypeDef status;
    HAL_FLASH_Unlock();
    uint32_t adr = adrFlash;
    for (int32_t i = 0; i < FLASH_PAGE_SIZE/sizeof(uint32_t); i++, adr += 4) {
        uint32_t data32 = *((uint32_t*)adr);
        if (data32 == 0xffffffff) {
            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adr, cfg);
            if (status != HAL_OK) {
                cntErrorProgram++;
            }
            HAL_FLASH_Lock();
            return;
        }
    }
    status = FLASH_ErasePage(adrFlash, 1);
    if (status != HAL_OK) {
        cntErrorErase++;
    }
    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adrFlash, cfg);
    if (status != HAL_OK) {
        cntErrorProgram++;
    }
    HAL_FLASH_Lock();
}

static  uint32_t getAddressFlash(uint8_t nCfg)
{
    switch (nCfg) {
    case 0:     return ADDR_FLASH_PAGE_15;
    case 1:     return ADDR_FLASH_PAGE_16;
    case 2:     return ADDR_FLASH_PAGE_17;
    default:
    case 3:     return ADDR_FLASH_PAGE_18;
    }
}
void GetConfig_PresetBank(uint8_t nCfg, uint8_t* preset, uint8_t* bank, uint8_t* presetPrev, uint8_t* bankPrev)
{
    uint32_t adrFlash = getAddressFlash(nCfg);
    uint32_t cfg = GetConfig(adrFlash);
    *preset     = (cfg     ) & 0x03;
    *bank       = (cfg >> 2) & 0x03;
    *presetPrev = (cfg >> 4) & 0x03;
    *bankPrev   = (cfg >> 6) & 0x03;
}

void PutConfig_PresetBank(uint8_t nCfg, uint8_t preset, uint8_t bank, uint8_t presetPrev, uint8_t bankPrev)
{
    uint32_t adrFlash = getAddressFlash(nCfg);
    uint32_t cfg = 0x0;
    cfg |= preset;
    cfg |= (bank        << 2);
    cfg |= (presetPrev  << 4);
    cfg |= (bankPrev    << 6);
    PutConfig(adrFlash, cfg);
}
