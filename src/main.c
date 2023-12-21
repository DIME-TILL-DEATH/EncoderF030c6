/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "stm32f0xx_hal.h"
#include "led.h"
#include "button.h"
#include "config.h"
#include "main.h"


/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim3;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

// перейти к следующему preset/bank
static void increment(uint8_t* preset, uint8_t* bank)
{
    if (*preset < 3) {
        *preset += 1;
    } else {
        *preset = 0;
        if (*bank < 3) {
            *bank += 1;
        } else {
            *bank = 0;
        }
    }
}
// перейти к предыдущему preset/bank
static void decrement(uint8_t* preset, uint8_t* bank)
{
    if (*preset > 0) {
        *preset -= 1;
    } else {
        *preset = 3;
        if (*bank > 0) {
            *bank -= 1;
        } else {
            *bank = 3;
        }
    }
}
// поменять местами
static void swap(uint8_t* num, uint8_t* prev)
{
    uint8_t tmp = *num;
    *num = *prev;
    *prev = tmp;
}

uint8_t modePlain = 0;      // 1 -"Простой Режим" (нет предыдущего состояния preset/bank)
uint8_t modeBar = 0;        // 1 -"Режим столбика"
uint8_t mode180 = 0;        // 1 -"Режим разворота на 180 градусов"

uint8_t preset  = 0;        // текущий      preset
uint8_t bank    = 0;        // текущий      bank

uint8_t presetNext = 0;     // следующий    preset
uint8_t bankNext   = 0;     // следующий    bank

uint8_t presetPrev = 0;     // предыдущий   preset
uint8_t bankPrev   = 0;     // предыдущий   bank

uint8_t externPress = 0;    // флаг нажатия внешней кнопки
uint8_t externPressPrev = 0;// предыдущее состояние нажатия внешней кнопки
//========================================================================
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM3_Init();
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);

    ComplectConfig_init();
    uint8_t nCfg = 0xff;
    GetConfig_PresetBank(nCfg, &preset, &bank, &presetPrev, &bankPrev);
    presetNext  = 0xff;   // пока нет следующего состояния
    bankNext    = 0xff;

    ModePlain_init();
    ModeBar_init();
    Mode180_init();
    modePlain = ModePlain_get();
    modeBar = ModeBar_get();
    mode180 = Mode180_get();

    PresetBankToState();
    LEDs_show();
    LEDs_out_show();

    LEDS_init();
    LEDS_out_init();

    ButtonEncoder_Enable(1);
    ButtonExtern_init();
    BankPresetItem_init();

    uint32_t cnt = TIM3->CNT;
    uint32_t cntPrev = cnt;

    uint32_t delay_ms = 0;
    uint8_t cntInc = 0;
    uint8_t cntDec = 0;
    while (1)
    {
        uint8_t cfgNew = ComplectConfig_get();
        if (cfgNew != nCfg) {
            nCfg = cfgNew;
            GetConfig_PresetBank(nCfg, &preset, &bank, &presetPrev, &bankPrev);
            PresetBankToState();
            LEDs_show();
            LEDs_out_show();
        }
        if (delay_ms > 0) {
            delay_ms--;
        }
        cnt = TIM3->CNT;
        if (cnt != cntPrev) {
            if (cnt > cntPrev) {
                cntInc++;
            } else {
                cntDec++;
            }
            cntPrev = cnt;
        }
        if (cntInc >= 2 || cntDec >= 2) {
            if ((presetNext == 0xff) || (bankNext == 0xff)) {   // Нет следующего состояния
                presetNext  = preset;
                bankNext    = bank;
            }
            if (cntInc > cntDec) {
                increment(&presetNext, &bankNext);
            } else {
                decrement(&presetNext, &bankNext);
            }
            PresetBankToState();
            cntInc = cntDec = 0;
            delay_ms = 10;
        }
        uint8_t press = ButtonEncoder_isPress();    /// Кнопка Енкодера
        if (!press) {           // основная кнопка не нажата
            externPress = ButtonExtern_get();
            if (!externPressPrev && externPress) {  // нажата внешняя кнопка
                press = 1;      // Короткое нажатие
            }
            externPressPrev = externPress;
        }
        if (press) {
            if (press == 2) {           /// Длинное нажатие
                if ((presetNext != 0xff) || (bankNext != 0xff)) {   // Есть следующее состояние
                    presetPrev  = preset;
                    bankPrev    = bank;
                    preset      = presetNext;
                    bank        = bankNext;
                    presetNext  = 0xff;                         // нет следующего состояния
                    bankNext    = 0xff;
                }
                PutConfig_PresetBank(nCfg, preset, bank, presetPrev, bankPrev);
                LEDs_off();             // Отключим все светодиоды, - подтверждение, что команда принята и в flash сохранено
                HAL_Delay(200);
            } else if (press == 1) {    /// Короткое нажатие
                if ((presetNext != 0xff) || (bankNext != 0xff)) {   // Есть следующее состояние
                    presetNext  = 0xff;                             // отменяем следующее состояние
                    bankNext    = 0xff;
                } else {
                    if ((presetPrev != 0xff) || (bankPrev != 0xff)) {   // Есть предыдущее состояние
                        swap(&preset, &presetPrev);                 // текущий меняем с предыдущим
                        swap(&bank , &bankPrev);
                    }
                }
            }
            PresetBankToState();
        }
        LEDs_show();
        LEDs_out_show();
        HAL_Delay(1);
    }
}

//  Clock Configuration
//------------------------------------
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 255;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BANK0_GPIO_Port, LED_BANK0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_BANK0_Pin */
  GPIO_InitStruct.Pin = LED_BANK0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BANK0_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
