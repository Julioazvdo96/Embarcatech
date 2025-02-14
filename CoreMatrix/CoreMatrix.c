#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2812b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição de pixel GRB
struct pixel_t {
 uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
* Inicializa a máquina PIO para controle da matriz de LEDs.
*/
void npInit(uint pin) {
 // Cria programa PIO.
 uint offset = pio_add_program(pio0, &ws2818b_program);
 np_pio = pio0;

 // Toma posse de uma máquina PIO.
 sm = pio_claim_unused_sm(np_pio, false);
 if (sm < 0) {
   np_pio = pio1;
   sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
 }

 // Inicia programa na máquina PIO obtida.
 ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

 // Limpa buffer de pixels.
 for (uint i = 0; i < LED_COUNT; ++i) {
   leds[i].R = 0;
   leds[i].G = 0;
   leds[i].B = 0;
 }
}

/**
* Atribui uma cor RGB a um LED.
*/
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
 leds[index].R = r;
 leds[index].G = g;
 leds[index].B = b;
}

/**
* Limpa o buffer de pixels.
*/
void npClear() {
 for (uint i = 0; i < LED_COUNT; ++i)
   npSetLED(i, 0, 0, 0);
}

/**
* Escreve os dados do buffer nos LEDs.
*/
void npWrite() {
 // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
 for (uint i = 0; i < LED_COUNT; ++i) {
   pio_sm_put_blocking(np_pio, sm, leds[i].G);
   pio_sm_put_blocking(np_pio, sm, leds[i].R);
   pio_sm_put_blocking(np_pio, sm, leds[i].B);
 }
}

void MatrixCore(int func, int prod){
    int cont[5] = {0, 0, 0, 0, 0};
    
    int leds_config[5][5] = {
        {4, 5, 14, 15, 14},  // LEDS Produto 1
        {3, 6, 13, 16, 23},  // LEDS Produto 2
        {2, 7, 12, 17, 22},  // LEDS Produto 3
        {1, 8, 11, 18, 21},  // LEDS Produto 4
        {0, 9, 10, 19, 20}   // LEDS Produto 5
    };

    // Inicializa os LEDs
    npInit(LED_PIN);

    while(1) {
        if (func == 1) {
            if (cont[prod] < 5) {
                npSetLED(leds_config[prod][cont[prod]], 255, 0, 0); // Configura cor vermelha
                cont[prod]++;
            }
            else if (prod == 6) {
                for (int i = 0; i < 5; i++) {
                    npSetLED(leds_config[i][cont[prod]], 0, 0, 0); // Limpa LED
                    cont[i]++;
                }
            }
            else {
                // beep negativo; adicionar função beep negativo
            }
        }
        else if (func == 2) {
            if (cont[prod] > 0) {
                cont[prod]--;
                npSetLED(leds_config[prod][cont[prod]], 0, 0, 0); // Apaga o LED
            }
            else if (prod == 6) {
                for (int i = 0; i < 5; i++) {
                    cont[i]++;
                    npSetLED(leds_config[i][cont[prod]], 0, 0, 0); // Limpa LED
                }
            }
            else {
                // beep negativo; adicionar função beep negativo
            }
        }
        else if (func == 3) {
            if (cont[prod] > 0) {
                do {
                    cont[prod]--;
                    npSetLED(leds_config[prod][cont[prod]], 0, 0, 0);
                } while (cont[prod] != 0); // Corrigido para sair quando o contador for 0
            }
            else if (prod == 6) {
                npClear();
            }
            else {
                // beep negativo; adicionar função beep negativo
            }
        }
        npWrite(); // Escreve a atualização para os LEDs
    }
    return 0;
}
