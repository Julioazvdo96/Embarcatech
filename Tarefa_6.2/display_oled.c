#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

//Adicionando os headers das funções criadas com base nos projetos do github do bitdoglab
#include "func/buzzer.h"
#include "func/joy.h"
#include "func/led_rgb.h"
#include "func/menu.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Definição dos pinos do Joystick
#define EixoY 26
#define Button 22

#define BUZZER_PIN 21


void inicializacao(){
	stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

	//Inicialização do botão do joystick
	gpio_init(Button);
    gpio_set_dir(Button, GPIO_IN); 
    gpio_pull_up(Button);

	//Inicialização do ADC
	adc_init();
    adc_gpio_init(EixoY);
}

int main(){

	inicializacao();

	uint pos_atual = 1, adc_y, flag_button=0, opcao1=0, opcao3=0;

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    zeraDisplay(frame_area);

    while(true) {
		if(flag_button==0){
			if (pos_atual == 1) {
				zeraDisplay(frame_area);
				Menu1(frame_area);
			} 
			else if (pos_atual == 2) {
				zeraDisplay(frame_area);
				Menu2(frame_area);
			} 
			else if (pos_atual == 3) {
				zeraDisplay(frame_area);
				Menu3(frame_area);
			}
		}
		else if(flag_button==1){
			flag_button=0;
			if(pos_atual==1){
				Menu4(frame_area);
				sleep_ms(1000);
				joystick_loop(opcao1);
				if(opcao1==0){
					opcao1=1;
				}
				Menu1(frame_area);
			}
			else if(pos_atual==2){
				Menu4(frame_area);
				sleep_ms(1000);
				play_star_wars(BUZZER_PIN);
				Menu2(frame_area);
			}
			else if(pos_atual==3){
				Menu4(frame_area);
				sleep_ms(1000);
				LED_RGB(opcao3);
				if(opcao3==0){
					opcao3=1;
				}
				Menu3(frame_area);
			}
		}

        do{
		adc_select_input(0);
        adc_y = adc_read();

		if (adc_y < 50) {
			if (pos_atual != 3) {
				pos_atual++;
			}
		}
		else if (adc_y > 4050) {
			if (pos_atual != 1) {
				pos_atual--;
			}
		}
		else if(gpio_get(Button)==0){
			flag_button=1;
		}
		sleep_ms(100);
		}while(adc_y>50 && adc_y<4050 && flag_button!=1);
    }

    return 0;
}