#ifndef CORE_MATRIX_H
#define CORE_MATRIX_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "func/ws2812b.pio.h"

//Chamada da variável global externa a esse arquivo Cont
extern int cont[5];


// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7


struct pixel_t {
    uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
extern npLED_t leds[LED_COUNT];



// Funções da biblioteca
void npInit(uint pin);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear();
void npWrite();
int CoreMatrix(int func, int prod, int flag_coreMatrix);
void ws2818b_program_init(PIO pio, uint sm, uint offset, uint pin, float freq); 

#endif 