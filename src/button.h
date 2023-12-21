#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif


void    ButtonEncoder_Enable(uint8_t irq);
void    ButtonEncoder_press();
uint8_t ButtonEncoder_isPress();


// ������������� ���� ��� ����� ������ ��������� bank/preset
void BankPresetItem_init();
// ���������� ����������� � ���� ��� ����� ��������� bank/preset
uint8_t BankPresetItem_get();


//------------------------------------------------
// ������� ������ - �������������
void ButtonExtern_init();
// ������� ������ - ������ �������
uint8_t ButtonExtern_get();


//------------------------------------------------
// �������� ������������(����������� ����� preset/bank) - �������������
void ComplectConfig_init();
// �������� ������������ - ������ ������� ������������: 0,1,2,3
uint8_t ComplectConfig_get();


//------------------------------------------------
// ������������� ���-������������� "������� �����" (��� ����������� ��������� preset/bank)
void ModePlain_init();
// ������ ���-������������� "������� �����" (��� ����������� ��������� preset/bank)
uint8_t ModePlain_get();


//------------------------------------------------
// ������������� ���-������������� "����� ��������"
// ����� preset/bank ������������ � ���� ���������� �����������.
// ������ preset � ���� ���� ��������, ������ preset � ���� ���� ��������.
void ModeBar_init();
// ������ ���-������������� "����� ��������"
uint8_t ModeBar_get();



//------------------------------------------------
// ������������� - ����� ��������� �� 180 ��������
void Mode180_init();
// ������ ����� ��������� �� 180 ��������
uint8_t Mode180_get();


#ifdef __cplusplus
}
#endif

#endif
