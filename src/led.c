#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "countof.h"
#include "led.h"

#define STATE_OFF   0
#define STATE_ON    1
#define STATE_BLINK 2
#define STATE_HALF  3

static uint8_t statePreset[4];
static uint8_t stateBank[4];

static void presetBank_toState()
{
    uint8_t isBlink = 0;
    for (int pos = 0; pos < 4; pos++ ) {
        if (preset == pos) {                statePreset[pos] = STATE_ON;
        } else if (presetNext == pos) {     statePreset[pos] = STATE_BLINK;     isBlink++;
        } else if (presetPrev == pos) {     statePreset[pos] = modePlain ? STATE_OFF : STATE_HALF;
        } else {                            statePreset[pos] = STATE_OFF;
        }

        if (bank == pos) {                  stateBank[pos] = STATE_ON;
        } else if (bankNext == pos) {       stateBank[pos] = STATE_BLINK;       isBlink++;
        } else if (bankPrev == pos) {       stateBank[pos] = modePlain ? STATE_OFF : STATE_HALF;
        } else {                            stateBank[pos] = STATE_OFF;
        }
    }
    if (isBlink) {
        for (int pos = 0; pos < 4; pos++ ) {
            if (statePreset[pos] == STATE_HALF) {   statePreset[pos] = STATE_OFF;   }
            if (stateBank[pos]   == STATE_HALF) {   stateBank[pos] = STATE_OFF;     }
        }
    }
}

void PresetBankToState()
{
    if (modeBar) {
        uint8_t modeEdit = ((presetNext != 0xff) && (preset != presetNext)) ||  // есть следующее состояние и оно не текущее - режим редактирования
                           ((bankNext != 0xff) && (bank != bankNext)) ? 1 : 0;
        if (modeEdit) {
            presetBank_toState();
        } else {
            for (int pos = 0; pos < 4; pos++ ) {
                statePreset[pos]    = pos <= preset ? STATE_ON : STATE_OFF;
                stateBank[pos]      = pos <= bank   ? STATE_ON : STATE_OFF;
            }

            if (!modePlain) {
                if ((presetPrev != 0xff) && (presetPrev != preset)) {   // есть предыдущее состояние
                    statePreset[presetPrev] = STATE_HALF;
                }
                if ((bankPrev != 0xff) && (bankPrev != bank)) {         // есть предыдущее состояние
                    stateBank[bankPrev] = STATE_HALF;
                }
            }
        }
    } else {
        presetBank_toState();
    }
}

uint8_t blink = 0;
uint8_t iBlink = 0;
uint8_t half = 0;
int8_t  iHalf = 0;
//----------------------------
uint8_t stateToOn(uint8_t state)
{
    switch (state) {
    case STATE_OFF:     return 0;
    case STATE_ON:      return 1;
    case STATE_BLINK:   return blink;
    case STATE_HALF:    return half;
    default:            return 0;
    }
}

static void show_Preset(uint8_t preset, uint8_t state)
{
    switch (preset) {
    case 0:
        if (mode180)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, state);
        else            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,  state);
        break;
    case 1:
        if (mode180)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,  state);
        else            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, state);
        break;
    case 2:
        if (mode180)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,  state);
        else            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, state);
        break;
    case 3:
        if (mode180)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,  state);
        else            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, state);
        break;
    }
}
static void show_Bank(uint8_t bank, uint8_t state)
{
    switch (bank) {
    case 0:
        if (mode180)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, state);
        else            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,  state);
        break;
    case 1:
        if (mode180)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  state);
        else            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, state);
        break;
    case 2:
        if (mode180)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  state);
        else            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, state);
        break;
    case 3:
        if (mode180)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,  state);
        else            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, state);
        break;
    }
}

void LEDs_show()
{
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,  stateToOn(statePreset[0]));   // preset: 0
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  stateToOn(statePreset[1]));   // preset: 1
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  stateToOn(statePreset[2]));   // preset: 2
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, stateToOn(statePreset[3]));   // preset: 3
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,  stateToOn(stateBank[0]));     // bank: 0
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,  stateToOn(stateBank[1]));     // bank: 1
//    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,  stateToOn(stateBank[2]));     // bank: 2
//    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, stateToOn(stateBank[3]));     // bank: 3
    show_Preset(0, stateToOn(statePreset[0]));   // preset: 0
    show_Preset(1, stateToOn(statePreset[1]));   // preset: 1
    show_Preset(2, stateToOn(statePreset[2]));   // preset: 2
    show_Preset(3, stateToOn(statePreset[3]));   // preset: 3
    show_Bank(  0, stateToOn(stateBank[0]));     // bank: 0
    show_Bank(  1, stateToOn(stateBank[1]));     // bank: 1
    show_Bank(  2, stateToOn(stateBank[2]));     // bank: 2
    show_Bank(  3, stateToOn(stateBank[3]));     // bank: 3
    if (++iBlink >= 50) {
        iBlink = 0;
        blink = !blink;
    }
    if (--iHalf <= 0) {
        half = !half;
        if (half) {
            iHalf = 1;
        } else {
            iHalf = 10;
        }
    }
}

void LEDs_off()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,  0);   // preset: 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  0);   // preset: 1
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  0);   // preset: 2
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);   // preset: 3
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0,  0);   // bank: 0
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,  0);   // bank: 1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,  0);   // bank: 2
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, 0);   // bank: 3
}

void LEDS_init()
{
    GPIO_InitTypeDef gpio;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpio.Pin    = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
    gpio.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio.Pull   = GPIO_NOPULL;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7, GPIO_PIN_RESET);       // LED OFF

    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio.Pin    = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_11;
    gpio.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio.Pull   = GPIO_NOPULL;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &gpio);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_11,  GPIO_PIN_RESET);       // LEDS OFF
}

//==================================================
// Инициализация передачи номера bank/preset наружу
// PS1, PS2, PS3, PS4
//==================================================
void LEDS_out_init()
{
    GPIO_InitTypeDef gpio;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    gpio.Pin    = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    gpio.Mode   = GPIO_MODE_OUTPUT_PP;
    gpio.Pull   = GPIO_NOPULL;
    gpio.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,  GPIO_PIN_RESET);     // LEDS OFF
}

// Передача номера bank/preset наружу
void LEDs_out_show()
{
    uint8_t _preset, _bank;
    if ((presetNext != 0xff) || (bankNext != 0xff)) {   // Есть следующее состояние
       _preset = presetNext;
       _bank = bankNext;
    } else {
       _preset = preset;
       _bank = bank;
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, (_preset & 0x01) ? 0 : 1);  // инверсно
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, (_preset & 0x02) ? 0 : 1);  // инверсно

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, (_bank & 0x01) ? 0 : 1);    // инверсно
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, (_bank & 0x02) ? 0 : 1);    // инверсно
}

