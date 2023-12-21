#ifndef __LED_H
#define __LED_H


#ifdef __cplusplus
extern "C" {
#endif

void LEDS_init();
void LEDs_off();
void LEDs_show();

void LEDS_out_init();   /// FS1, FS2, FS3, FS4
void LEDs_out_show();

void PresetBankToState();

void preset_bank_show();

#ifdef __cplusplus
}
#endif

#endif
