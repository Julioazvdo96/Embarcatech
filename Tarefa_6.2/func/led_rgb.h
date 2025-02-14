#ifndef LED_RGB_H
#define LED_RGB_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define Button 22 // Botão do analogico
#define LED 12                  // Pino do LED conectado
#define PERIOD 2000              // Período do PWM (valor máximo do contador)
#define DIVIDER_PWM 16.0         // Divisor fracional do clock para o PWM
#define LED_STEP 100             // Passo de incremento/decremento para o duty cycle do LED


void setup_led();
int LED_RGB(uint opcao3);

#endif 
