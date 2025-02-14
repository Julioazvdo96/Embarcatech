#ifndef JOY_H
#define JOY_H

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Definição dos pinos usados para o joystick e LEDs
#define VRX 26
#define VRY 27
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define SW 22

#define LED_B 12
#define LED_R 13
#define DIVIDER_PWM 16.0
#define PERIOD1 4096

#define Button 22

// Variáveis globais de controle do PWM
extern uint16_t led_b_level;
extern uint16_t led_r_level;
extern uint slice_led_b;
extern uint slice_led_r;

// Funções
void setup_joystick();
void setup_pwm_led(uint led, uint *slice, uint16_t level);
void setup_joy();
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);
void joystick_loop(uint opcao1);

#endif // JOYSTICK_H
