#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "button.h"


static uint32_t buttonEncoder_tick = 0;
//------------------------------------
void ButtonEncoder_Enable(uint8_t irq)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_9;           // port B9 - BUTTON_1
    gpio.Pull   = GPIO_NOPULL;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = irq ? GPIO_MODE_IT_FALLING : GPIO_MODE_INPUT;  // � ��������� ��� ������� ��������� �� ���������� ������ �� ���������� ������
    HAL_GPIO_Init(GPIOB, &gpio);
    if (irq) {
        HAL_NVIC_SetPriority((IRQn_Type)(EXTI4_15_IRQn), 0x0F, 0); // Enable and set Button EXTI Interrupt to the lowest priority
        HAL_NVIC_EnableIRQ(  (IRQn_Type)(EXTI4_15_IRQn));
    } else {
        HAL_NVIC_DisableIRQ(  (IRQn_Type)(EXTI4_15_IRQn));
    }
}
/// ������ ��������
/// ���������� 0 -�� ������, 1 -�������� �������, 2 -������� �������
///=================================================================
uint8_t ButtonEncoder_isPress()
{
    if (!buttonEncoder_tick) { /// ��� �� ������ (������ ����������)
        return 0;
    }
    uint8_t bit = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
    uint8_t press = (bit==0) ? 1 : 0;
    if (press) {    // ������
        uint32_t tick = HAL_GetTick();
        if ((tick - buttonEncoder_tick) < 500) {
            return 0;
        }
        buttonEncoder_tick = 0;
        return 2;   // ������� ������� (> 500 ����)
    } else {
        buttonEncoder_tick = 0;
        return 1;   // �������� ������� (������)
    }
}

void ButtonEncoder_press()
{
    buttonEncoder_tick = HAL_GetTick();
}

// ������������� ���� ��� ����� ������ ��������� bank/preset
//==============================================================
void BankPresetItem_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_12 | GPIO_PIN_14;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &gpio);
}

// ���������� ����������� � ���� ��� ����� ��������� bank/preset
uint8_t BankPresetItem_get()
{
    uint8_t bit0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    uint8_t bit1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
    uint8_t item = (bit0 << 1) | bit1;
    return item;
}

//-------------------------------
// ������� ������ - �������������
void ButtonExtern_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_13;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &gpio);
}
// ������� ������ - ������ �������
uint8_t ButtonExtern_get()
{
    uint8_t press = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
    return !press;
}

//-------------------------------
// �������� ������������(����������� ����� preset/bank) - �������������
void ComplectConfig_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_12 | GPIO_PIN_14;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &gpio);
}
// �������� ������������ - ������ ������� ������������: 0,1,2,3
uint8_t ComplectConfig_get()
{
    uint8_t bit1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
    uint8_t bit0 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
    return (bit1 << 1) | bit0;
}

//-------------------------------
// ������������� ���-������������� "������� �����" (��� ����������� ��������� preset/bank)
void ModePlain_init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_9;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &gpio);
}
// ������ ���-������������� "������� �����" (��� ����������� ��������� preset/bank)
uint8_t ModePlain_get()
{
    uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9);
    return bit;
}

//-------------------------------
// ������������� ���-������������� "����� ��������"
// ����� preset/bank ������������ � ���� ���������� �����������.
// ������ preset � ���� ���� ��������, ������ preset � ���� ���� ��������.
void ModeBar_init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_10;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &gpio);
}
// ������ ���-������������� "����� ��������"
uint8_t ModeBar_get()
{
    uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
    return !bit;
}

//------------------------------------------------
// ������������� - ����� ��������� �� 180 ��������
void Mode180_init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio;
    gpio.Pin    = GPIO_PIN_8;
    gpio.Pull   = GPIO_PULLUP;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    gpio.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA, &gpio);
}
// ������ ����� ��������� �� 180 ��������
uint8_t Mode180_get()
{
    uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
    return !bit;
}

