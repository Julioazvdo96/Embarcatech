#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2812b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7

// Definição dos pinos do Joystick
#define EixoX 27
#define EixoY 26

// Definição dos pinos dos botões
#define BUTTON_A 5
#define BUTTON_B 6

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

void jogo_da_velha(int r, int g, int b, int ms){
    for(int i=0; i<=24; i++){
        if(i!=0 && i!=2 && i!=4 && i!=10 && i!=12 && i!=14 && i!=20 && i!=22 && i!=24){
            npSetLED(i, r, g, b);
            npWrite(); // Escreve os dados nos LEDs.
        }
        sleep_ms(ms);
    }
}

void inicializacao_matriz_led(){
    for (int i = 0; i<2; i++){
        jogo_da_velha(30, 0, 0, 10);
        jogo_da_velha(30, 30, 0, 10); 
        jogo_da_velha(0, 30, 0, 10);
        jogo_da_velha(0, 30, 30, 10);
        jogo_da_velha(0, 0, 30, 10);
        jogo_da_velha(30, 0, 30, 10);
        }

        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 50);
        sleep_ms(1000);
        npClear();
        sleep_ms(1000);
        jogo_da_velha(30, 30, 30, 50);
        sleep_ms(1000);
        
        jogo_da_velha(30,30,0,50);
}


int verificaColuna(int matriz[3][3], int matrizPosicoes[3][3]){
    for(int jogador = 1 ; jogador <= 2; jogador++){
        for(int i=0; i<=2; i++){
            if(matriz[0][i] == jogador && matriz[1][i] == jogador && matriz[2][i] == jogador){
                npClear();
                sleep_ms(1000);
                npSetLED(matrizPosicoes[0][i], 30, 30, 30);
                npSetLED(matrizPosicoes[1][i], 30, 30, 30);
                npSetLED(matrizPosicoes[2][i], 30, 30, 30);
                npWrite();
                sleep_ms(2000);
                npClear();
                sleep_ms(1000);
                return 1;
            }
        }
    }
    return 0;
}

int verificaLinha(int matriz[3][3], int matrizPosicoes[3][3]){
    for(int jogador = 1 ; jogador <= 2; jogador++){
        for(int i=0; i<=2; i++){
            if(matriz[i][0] == jogador && matriz[i][1] == jogador && matriz[i][2] == jogador){
                npClear();
                sleep_ms(1000);
                npSetLED(matrizPosicoes[i][0], 30, 30, 30);
                npSetLED(matrizPosicoes[i][1], 30, 30, 30);
                npSetLED(matrizPosicoes[i][2], 30, 30, 30);
                npWrite();
                sleep_ms(2000);
                npClear();
                sleep_ms(1000);
                return 1;
            }
        }
    }
    return 0;
}

int verificaDiagonal(int matriz[3][3], int matrizPosicoes[3][3]){
    for(int jogador = 1 ; jogador <= 2; jogador++){
        if(matriz[0][0] == jogador && matriz[1][1] == jogador && matriz[2][2] == jogador){
            npClear();
            sleep_ms(1000);
            npSetLED(matrizPosicoes[0][0], 30, 30, 30);
            npSetLED(matrizPosicoes[1][1], 30, 30, 30);
            npSetLED(matrizPosicoes[2][2], 30, 30, 30);
            npWrite();
            sleep_ms(2000);
            npClear();
            sleep_ms(1000);
            return 1;
        }
        else if(matriz[0][2] == jogador && matriz[1][1] == jogador && matriz[2][0] == jogador){
            npClear();
            sleep_ms(1000);
            npSetLED(matrizPosicoes[0][2], 30, 30, 30);
            npSetLED(matrizPosicoes[1][1], 30, 30, 30);
            npSetLED(matrizPosicoes[2][0], 30, 30, 30);
            npWrite();
            sleep_ms(2000);
            npClear();
            sleep_ms(1000);
            return 1;
        }
    }
    return 0;
}

int verificaCompleta(int matriz[3][3], int matrizPosicoes[3][3]){
    for(int i=0;i<3;i++){
        for(int j=0; j<3; j++){
            if(matriz[i][j]==1 || matriz[i][j]==2){
                continue;
            }
            else{
                return 0;
            }
        }
    }
    npClear();
    sleep_ms(1000);
    for(int i=0;i<3;i++){
        for(int j=0; j<3; j++){
        npSetLED(matrizPosicoes[i][j], 30, 30, 30);
        }
    }
    npWrite();
    sleep_ms(2000);
    npClear();
    sleep_ms(1000);
    return 1;
}

int main() {
    uint adc_x, adc_y;

    uint pos_atual = 12;
    uint pos_anterior = 25;

    uint jogadores = 1;
    int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;
    int apertados[3][3]={{25,25,25},{25,25,25},{25,25,25}};
    int tabuleiro[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    
    stdio_init_all();

    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);

    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);

    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);

    adc_init();
    adc_gpio_init(EixoX);
    adc_gpio_init(EixoY);

    npInit(LED_PIN);


    while(true){
        npClear();
        //inicializacao_matriz_led();
        pos_atual = 12;
        pos_anterior = 25;
        jogadores = 1;

        npSetLED(pos_atual,30,0,30);
        npWrite();
        while (flag1!=1 && flag2 !=1 && flag3 != 1 && flag4 != 1) {

            adc_select_input(1);
            adc_x = adc_read();
            adc_select_input(0);
            adc_y = adc_read();

            
                if(adc_x < 50){
                    if(pos_atual != 24 && pos_atual != 14 && pos_atual != 4){
                        pos_anterior = pos_atual;
                        pos_atual = pos_atual + 2;
                    }
                    else{
                        continue;
                    }
                }
                else if(adc_x > 4050){
                    if(pos_atual != 20 && pos_atual != 10 && pos_atual != 0){
                        pos_anterior = pos_atual;
                        pos_atual = pos_atual - 2;
                    }
                    else{
                        continue;
                    }
                }
                else if(adc_y<50){
                    if(pos_atual != 4 && pos_atual != 2 && pos_atual != 0){
                        pos_anterior = pos_atual;
                        pos_atual = pos_atual - 10;
                    }
                    else{
                        continue;
                    }
                }
                else if(adc_y>4050){
                    if(pos_atual != 24 && pos_atual != 22 && pos_atual != 20){
                        pos_anterior = pos_atual;
                        pos_atual = pos_atual + 10;
                    }
                    else{
                        continue;
                    }
                }
                else if(gpio_get(BUTTON_A)==0 || gpio_get(BUTTON_B)==0){
                    sleep_ms(200);
                    switch(pos_atual){
                        case 0:
                        if(apertados[0][0]==25){
                            apertados[0][0] = 0;
                            if(jogadores == 1){
                                tabuleiro[0][0] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[0][0] = 2;
                                jogadores = 1;
                            }
                        }   
                        break;
                        case 2:
                        if(apertados[0][1]==25){
                            apertados[0][1] = 2;
                            if(jogadores == 1){
                                tabuleiro[0][1] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[0][1] = 2;
                                jogadores = 1;
                            }
                        }    
                        break;
                        case 4:
                        if(apertados[0][2]==25){
                            apertados[0][2] = 4;
                            if(jogadores == 1){
                                tabuleiro[0][2] = 1;
                                jogadores = 2;

                            }
                            else if(jogadores == 2){
                                tabuleiro[0][2] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 10:
                        if(apertados[1][0]==25){
                            apertados[1][0] = 10;
                            if(jogadores == 1){
                                tabuleiro[1][0] = 1;
                                jogadores = 2;
                                
                            }
                            else if(jogadores == 2){
                                tabuleiro[1][0] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 12:
                        if(apertados[1][1]==25){
                            apertados[1][1] = 12;
                            if(jogadores == 1){
                                tabuleiro[1][1] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[1][1] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 14:
                        if(apertados[1][2]==25){
                            apertados[1][2] = 14;
                            if(jogadores == 1){
                                tabuleiro[1][2] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[1][2] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 20:
                        if(apertados[2][0]==25){
                            apertados[2][0] = 20;
                            if(jogadores == 1){
                                tabuleiro[2][0] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[2][0] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 22:
                        if(apertados[2][1]==25){
                            apertados[2][1] = 22;
                            if(jogadores == 1){
                                tabuleiro[2][1] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[2][1] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        case 24:
                        if(apertados[2][2]==25){
                            apertados[2][2] = 24;
                            if(jogadores == 1){
                                tabuleiro[2][2] = 1;
                                jogadores = 2;
                            }
                            else if(jogadores == 2){
                                tabuleiro[2][2] = 2;
                                jogadores = 1;
                            }
                        }
                        break;
                        default:
                        continue;
                    }
                }
                else{
                    continue;
                }
                npSetLED(pos_anterior,0,0,0);
                for(int i=0; i<3; i++){
                    for(int j=0; j<3; j++){
                        if(tabuleiro[i][j]==1){
                        npSetLED(apertados[i][j],30,0,0);
                        }
                        else if(tabuleiro[i][j]==2){
                            npSetLED(apertados[i][j],0,0,30);
                        }
                    }
                }
                npSetLED(pos_atual,30,0,30);
                npWrite();
                flag1 = verificaColuna(tabuleiro, apertados);
                flag2 = verificaLinha(tabuleiro, apertados);
                flag3 = verificaDiagonal(tabuleiro, apertados);
                if(flag1 == 0 && flag2==0 && flag3 ==0 ){
                    flag4 = verificaCompleta(tabuleiro, apertados);
                }
                sleep_ms(200);
        }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            tabuleiro[i][j] = 0;
            apertados[i][j] = 25;
            }
        }
    flag1=0;
    flag2=0;
    flag3=0;
    flag4=0;
    }
}