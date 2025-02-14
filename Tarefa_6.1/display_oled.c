#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"


//Configuração dos pinos da comunicação I2C com o OLED
#define I2C_SDA_PIN 14 
#define I2C_SCL_PIN 15 
 
//Configuração dos pinos do LED RGB
#define LED_R_PIN 13
#define LED_B_PIN 12
#define LED_G_PIN 11

#define BTN_A_PIN 5

int A_state = 0;

// Configuração do display OLED 
#define OLED_WIDTH 128 
#define OLED_HEIGHT 32 

void verde_OLED(struct render_area frame_area) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    
    char *text[] = {
    "                ",
    " SINAL ABERTO   ",
    "                ",
    "ATRAVESSAR COM  ",
    "                ",
    "   CUIDADO !    "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++){
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}


void amarelo_OLED(struct render_area frame_area) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    char *text[] = {
    "                ",
    "   SINAL DE     ",
    "                ",
    "   ATENCAO      ",
    "                ",
    "  PREPARE1SE    "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++){
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void vermelho_OLED(struct render_area frame_area) {
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
    char *text[] = {
    "                ",
    " SINAL FECHADO  ",
    "                ",
    "   AGUARDE      ",
    "                ",
    "                "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++){
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void init_oled() { 
    i2c_init(i2c1, ssd1306_i2c_clock * 1000); 
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C); 
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C); 
    gpio_pull_up(I2C_SDA_PIN); 
    gpio_pull_up(I2C_SCL_PIN); 
    ssd1306_init(); 
} 

void SinalAberto() { 
    gpio_put(LED_R_PIN, 0); 
    gpio_put(LED_G_PIN, 1); 
    gpio_put(LED_B_PIN, 0); 
} 
 
void SinalAtencao() { 
    gpio_put(LED_R_PIN, 1); 
    gpio_put(LED_G_PIN, 1); 
    gpio_put(LED_B_PIN, 0); 
} 
 
void SinalFechado() { 
    gpio_put(LED_R_PIN, 1); 
    gpio_put(LED_G_PIN, 0); 
    gpio_put(LED_B_PIN, 0); 
} 

//Código abaixo é de propriedade do professor Moroni, mostrado na aula síncrona do dia 20/01/2025 em https://youtu.be/bnYUuMSVnHg?t=2395
int WaitWithRead(int timeMS){
    for (int i=0; i<timeMS;i=i+100){
        A_state = !gpio_get(BTN_A_PIN);
        if(A_state == 1){
            return 1;
        }
        sleep_ms(100);
    }
    return 0;
}


int main() { 
    // Inicialização de LEDs e Botão 
    gpio_init(LED_R_PIN); 
    gpio_set_dir(LED_R_PIN, GPIO_OUT); 
    gpio_init(LED_G_PIN); 
    gpio_set_dir(LED_G_PIN, GPIO_OUT); 
    gpio_init(LED_B_PIN); 
    gpio_set_dir(LED_B_PIN, GPIO_OUT); 
 
    gpio_init(BTN_A_PIN); 
    gpio_set_dir(BTN_A_PIN, GPIO_IN); 
    gpio_pull_up(BTN_A_PIN); 
 
    // Inicialização do OLED 
    init_oled(); 

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
 
    while (true) { 
        SinalFechado(); 
        vermelho_OLED(frame_area);
        int A_state = WaitWithRead(8000); 
 
        if (A_state) { 
            SinalAtencao(); 
            amarelo_OLED(frame_area);
            sleep_ms(5000); 
 
            SinalAberto(); 
            verde_OLED(frame_area);
            sleep_ms(10000); 
        } else { 
            SinalAtencao(); 
            amarelo_OLED(frame_area);
            sleep_ms(2000); 
 
            SinalAberto(); 
            verde_OLED(frame_area);
            sleep_ms(8000); 
        } 
    } 
 
    return 0; 
}
