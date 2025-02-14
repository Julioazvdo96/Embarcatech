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



//PADRÕES

void cobra_cima(int r, int g, int b, int ms){
    for (int i = 0; i<LED_COUNT; i++){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        sleep_ms(ms);
    }
}

void cobra_baixo(int r, int g, int b, int ms){
    for (int i=24; i>=0; i--){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        sleep_ms(ms);
    }
}

void espiral_horario(int r, int g, int b, int ms){
    for (int i = 0; i<6; i++){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
    for (int i = 14; i<=15; i++){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
    for (int i = 24; i>=19; i--){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
    for (int i=10; i>=6; i--){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
    npSetLED(13, r, g, b);
    npWrite();
    printf("13\n");
    sleep_ms(ms);
    for (int i=16; i<=18; i++){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
    for (int i=11; i<=12; i++){
        npSetLED(i, r, g, b);
        npWrite(); // Escreve os dados nos LEDs.
        printf("%d\n", i);
        sleep_ms(ms);
    }
}

void furacao(int r, int g, int b, int ms){
    
    npSetLED(12, r, g, b);
    npSetLED(17, r, g, b);
    npSetLED(22, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(21, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(18, r, g, b);
    npSetLED(20, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(19, r, g, b);
    npWrite();
    sleep_ms(ms);
    
    npSetLED(11, r, g, b);
    npSetLED(10, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(9, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(8, r, g, b);
    npSetLED(0, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(1, r, g, b);
    npWrite();
    sleep_ms(ms);
    
    npSetLED(7, r, g, b);
    npSetLED(2, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(3, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(6, r, g, b);
    npSetLED(4, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(5, r, g, b);
    npWrite();
    sleep_ms(ms);
    
    npSetLED(14, r, g, b);
    npSetLED(13, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(15, r, g, b);
    npWrite();
    sleep_ms(ms);
    
    npSetLED(24, r, g, b);
    npSetLED(16, r, g, b);
    npWrite();
    sleep_ms(ms);

    npSetLED(23, r, g, b);
    npWrite();
    sleep_ms(ms);
}

void jogo_da_velha(int r, int g, int b, int ms){
    for(int i=0; i<=24; i++){
        if(i!=0 && i!=2 && i!=4 && i!=10 && i!=12 && i!=14 && i!=20 && i!=22 && i!=24){
            npSetLED(i, r, g, b);
            npWrite(); // Escreve os dados nos LEDs.
        }
        sleep_ms(ms);
    }
}

int main() {

 // Inicializa entradas e saídas.
 stdio_init_all();

 // Inicializa matriz de LEDs NeoPixel.
 npInit(LED_PIN);
 npClear();

 // Aqui, você desenha nos LEDs.
 //npSetLED(0, 30, 0, 0); // Define o LED de índice 0 para vermelho.
 //npSetLED(12, 0, 30, 0); // Define o LED de índice 12 (centro da matriz) para verde.




 // Não faz mais nada. Loop infinito.
    while (true) {
        for (int i = 0; i<5; i++){
        jogo_da_velha(30, 0, 0, 10);
        jogo_da_velha(30, 30, 0, 10); 
        jogo_da_velha(0, 30, 0, 10);
        jogo_da_velha(0, 30, 30, 10);
        jogo_da_velha(0, 0, 30, 10);
        jogo_da_velha(30, 0, 30, 10);
        }

        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 100);
        sleep_ms(1000);
        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 100);
        sleep_ms(1000);
        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 100);
        sleep_ms(1000);
        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 100);
        sleep_ms(1000);
        npClear();

    }   
}