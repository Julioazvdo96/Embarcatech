#include "corematrix.h"
#include <stdio.h>


npLED_t leds[LED_COUNT]; // Definição do buffer de LEDs

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

void ws2818b_program_init(PIO pio, uint sm, uint offset, uint pin, float freq) {
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    // Program configuration.
    pio_sm_config c = ws2818b_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin); // Uses sideset pins.
    sm_config_set_out_shift(&c, true, true, 8); // 8 bit transfers, right-shift.
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX); // Use only TX FIFO.
    float prescaler = clock_get_hz(clk_sys) / (10.f * freq); // 10 cycles per transmission, freq is frequency of encoded bits.
    sm_config_set_clkdiv(&c, prescaler);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}


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

int CoreMatrix(int func, int prod, int flag_coreMatrix) {

    int leds_matrix[5][5] = {
        {4, 5, 14, 15, 24},  // LEDS Produto 1
        {3, 6, 13, 16, 23},  // LEDS Produto 2
        {2, 7, 12, 17, 22},  // LEDS Produto 3
        {1, 8, 11, 18, 21},  // LEDS Produto 4
        {0, 9, 10, 19, 20}   // LEDS Produto 5
    };
    //npClear para garantir que a matriz vai iniciar sem luzes acesas
    npClear();
    //Flag para garantir que os componentes usados em CoreMatrix iniciarem apenas na primeira execução.
    if(flag_coreMatrix == 0){
        npInit(LED_PIN);
    }
    //Condicional para selecionar função de adição
    if (func == 1) {
        //Caso o número da variável prod seja diferente de 5 a função será aplicada apenas ao produto indicado por prod
        if (prod != 5) {
            if(cont[prod]<5){
                cont[prod]++;
            }
        }
        //Caso o número da variável prod seja igual a 5 a função será aplicada a todos os produtos
        else if (prod == 5) {
            for (int i = 0; i < 5; i++) {
                if(cont[i]<5){
                    cont[i]++;
                }
            }
        }
    }
    //Condicional para selecionar função de subtração
    else if (func == 2) {
        //Caso o número da variável prod seja diferente de 5 a função será aplicada apenas ao produto indicado por prod
        if (prod != 5) {
            if(cont[prod]>0){
                cont[prod]--;
            }
        }
        //Caso o número da variável prod seja igual a 5 a função será aplicada a todos os produtos
        else if (prod == 5) {
            for (int i = 0; i < 5; i++) {
                if(cont[i]>0){
                    cont[i]--;
                }
            }
        }
    }
    //Condicional para selecionar função de subtração
    else if (func == 3) {
        //Caso o número da variável prod seja diferente de 5 a função será aplicada apenas ao produto indicado por prod
        if (prod !=5) {
            do {
                cont[prod]--;
            } while (cont[prod] > 0); 
        }
        //Caso o número da variável prod seja igual a 5 a função será aplicada a todos os produtos
        else if (prod == 5) {
            for(int i=0; i<5; i++){
                cont[i]=0;
            }
            npClear();
        }   
    }

    //Lógica para imprimir na matriz de LED a contagem de produtos
    for(int i=0; i<5; i++){
        if(cont[i]>0){
            for(int j=0; j<cont[i]; j++){
                //Condicionais para diferenciar os produtos por cores
                if(i==0){
                    npSetLED(leds_matrix[i][j],30,0,0);
                }
                else if(i==1){
                    npSetLED(leds_matrix[i][j],0,30,0);
                }
                else if(i==2){
                    npSetLED(leds_matrix[i][j],0,0,30);
                }
                else if(i==3){
                    npSetLED(leds_matrix[i][j],30,30,0);
                }
                else if(i==4){
                    npSetLED(leds_matrix[i][j],30,0,30);
                }
            }
        }
    }
    
    npWrite(); // Escreve a atualização para os LEDs
}