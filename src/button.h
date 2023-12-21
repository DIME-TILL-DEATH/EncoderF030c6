#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif


void    ButtonEncoder_Enable(uint8_t irq);
void    ButtonEncoder_press();
uint8_t ButtonEncoder_isPress();


// Инициализация двух ног входа номера комплекта bank/preset
void BankPresetItem_init();
// Возвращает прочитанный с двух ног номер комплекта bank/preset
uint8_t BankPresetItem_get();


//------------------------------------------------
// Внешняя кнопка - инициализация
void ButtonExtern_init();
// Внешняя кнопка - чтение нажатия
uint8_t ButtonExtern_get();


//------------------------------------------------
// Комплект конфигурации(сохраненный номер preset/bank) - инициализация
void ComplectConfig_init();
// Комплект конфигурации - чтение нномера конфигурации: 0,1,2,3
uint8_t ComplectConfig_get();


//------------------------------------------------
// Инициализация Дип-переключателя "Простой Режим" (нет предыдущего состояния preset/bank)
void ModePlain_init();
// Чтение Дип-переключателя "Простой Режим" (нет предыдущего состояния preset/bank)
uint8_t ModePlain_get();


//------------------------------------------------
// Инициализация Дип-переключателя "Режим столбика"
// Номер preset/bank отображается в виде нескольких светодиодов.
// Первый preset в виде двух светиков, второй preset в виде трех светиков.
void ModeBar_init();
// Чтение Дип-переключателя "Режим столбика"
uint8_t ModeBar_get();



//------------------------------------------------
// Инициализация - Режим разворота на 180 градусов
void Mode180_init();
// Чтение Режим разворота на 180 градусов
uint8_t Mode180_get();


#ifdef __cplusplus
}
#endif

#endif
