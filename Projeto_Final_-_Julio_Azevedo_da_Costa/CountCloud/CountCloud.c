#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"

//Adicionando os headers das funções criadas com base nos projetos do github do bitdoglab
//#include "func/buzzer.h"
#include "func/menu.h"
#include "func/corematrix.h"
#include "func/wifi_embarcatech.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

// Definição dos pinos do Joystick
#define EixoY 26
#define EixoX 27
#define ButtonA 5
#define ButtonB 6

//Declaração do vetor global Cont, responsável por armazenar a contagem das peças
int cont[5] = {0,0,0,0,0};


//Declaração de variáveis e de funções utilizando os temporizadores para chamar a função wi-fi de 5 em 5 minutos
//e para servir de espera para o debounce dos botões e dos joysticks
volatile bool tempo_acabou = false;
volatile uint64_t tempo_limite = 0;

int64_t callback_temporizador(alarm_id_t id, void *user_data) {
    tempo_acabou = true;
    return 0;  // Retorna 0 para não repetir o timer
}

void iniciar_temporizador(uint tempo_ms) {
    tempo_acabou = false;
    add_alarm_in_ms(tempo_ms, callback_temporizador, NULL, false);
}

//Função de substituição ao sleep_ms, utiliza a função time_us_64, feita dessa forma para que a marcação de tempo
//do programa seja feita utilizando temporizador
void pausa(uint tempo_ms) {
    uint64_t tempo_atual = time_us_64() / 1000;
    if (tempo_atual < tempo_limite) {
        while (time_us_64() / 1000 < tempo_limite) {
			//Aqui poderia ser inserido um código para funcionar enquanto a pausa estivesse ativa.
        }
    }
    tempo_limite = time_us_64() / 1000 + tempo_ms;
}


//Função responsável por inicializar os componentes utilizados no código principal
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
	gpio_init(ButtonA);
    gpio_set_dir(ButtonA, GPIO_IN); 
    gpio_pull_up(ButtonA);
	gpio_init(ButtonB);
    gpio_set_dir(ButtonB, GPIO_IN); 
    gpio_pull_up(ButtonB);

	//Inicialização do ADC
	adc_init();
    adc_gpio_init(EixoY);
	adc_gpio_init(EixoX);
}

int main(){
	//Setup dos componentes que serão utilizados no código principal
	inicializacao();
	//Zerando a matriz de LED para garantir que nada esteja sendo
	npClear();
	//
	uint pos_atual1 = 1, pos_atual2y=1, pos_atual2x=1, menu=1, adc_y, adc_x, func=0, prod=0;
	uint flag_buttonA=0, flag_buttonB=0, opcao1=0, opcao2=0, opcao3=0;
	uint flag_coreMatrix=0, flag_wifi_embarcatech=0;

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

	//Zerando o display para garantir que nada esteja sendo exibido na tela OLED
    zeraDisplay(frame_area);

    while(true) {
		//inicialização do temporizador de 5 min para quebrar o while secundário e chamar a função de wifi
		iniciar_temporizador(300000);
		while(!tempo_acabou){
			if(menu==1){
				//Condicional da navegação no menu inicial
				if(flag_buttonA==0){
					if (pos_atual1 == 1) {
						zeraDisplay(frame_area);
						MenuA1(frame_area);
					} 
					else if (pos_atual1 == 2) {
						zeraDisplay(frame_area);
						MenuA2(frame_area);
					} 
					else if (pos_atual1 == 3) {
						zeraDisplay(frame_area);
						MenuA3(frame_area);
					}
				}

				//Condicional de pressionamento do botão A no menu inicial
				else if(flag_buttonA==1){
					flag_buttonA=0;
					pos_atual2x=1; pos_atual2y=1;
					menu++;
					zeraDisplay(frame_area);
					MenuB1(frame_area);
					if(pos_atual1==1){
						func=1;
					}
					else if(pos_atual1==2){
						func=2;
					}
					else if(pos_atual1==3){
						func=3;
					}
				}
			}

			else if(menu==2){
				//Condicional de navegação no segundo menu
				if(flag_buttonA==0 && flag_buttonB==0){
					if(pos_atual2y==1 && pos_atual2x==1){
						zeraDisplay(frame_area);
						MenuB1(frame_area);
					}
					else if(pos_atual2y==1 && pos_atual2x==2){
						zeraDisplay(frame_area);
						MenuB2(frame_area);
					}
					else if(pos_atual2y==1 && pos_atual2x==3){
						zeraDisplay(frame_area);
						MenuB3(frame_area);
					}
					else if(pos_atual2y==2 && pos_atual2x==1){
						zeraDisplay(frame_area);
						MenuB4(frame_area);
					}
					else if(pos_atual2y==2 && pos_atual2x==2){
						zeraDisplay(frame_area);
						MenuB5(frame_area);
					}
					else if(pos_atual2y==2 && pos_atual2x==3){
						zeraDisplay(frame_area);
						MenuB6(frame_area);
					}
				}
				//Condicional de pressionamento do botão A no segundo menu
				else if(flag_buttonA==1){
					if(pos_atual2y==1 && pos_atual2x==1){
						prod=0;
						CoreMatrix(func, prod, flag_coreMatrix);
					}
					else if(pos_atual2y==1 && pos_atual2x==2){
						prod=1;
						CoreMatrix(func, prod, flag_coreMatrix);
					}
					else if(pos_atual2y==1 && pos_atual2x==3){
						prod=2;
						CoreMatrix(func, prod, flag_coreMatrix);
						menu=1;
					}
					else if(pos_atual2y==2 && pos_atual2x==1){
						prod=3;
						CoreMatrix(func, prod, flag_coreMatrix);
					}
					else if(pos_atual2y==2 && pos_atual2x==2){
						prod=4;
						CoreMatrix(func, prod, flag_coreMatrix);
					}
					else if(pos_atual2y==2 && pos_atual2x==3){
						prod=5;
						CoreMatrix(func, prod, flag_coreMatrix);
					}
					MenuA1(frame_area);
					flag_coreMatrix=1;
					menu=1;
					pos_atual1 = 1; 
					pos_atual2y=1; pos_atual2x=1;
					func=0, prod=0;
					flag_buttonA=0; flag_buttonB=0;
				}
			//Condicional de pressionamento do botão B no segundo menu, para retornar do menu 2 para o menu 1
				else if(flag_buttonB==1){
					menu=1;
					zeraDisplay(frame_area);
					MenuA1(frame_area);
					flag_buttonB=0;
				}
			}	
			
			//do-while que coleta informação dos ADCs 1 e 0 do joystick, responsável pelo input do eixo X e Y
			do{
			adc_select_input(1);
			adc_x = adc_read();
			adc_select_input(0);
			adc_y = adc_read();
			if(menu==1){
				if (adc_y < 50) {
					if (pos_atual1 != 3) {
						pos_atual1++;
					}
				}
				else if (adc_y > 4050) {
					if (pos_atual1 != 1) {
						pos_atual1--;
					}
				}
				else if(gpio_get(ButtonA)==0){
					flag_buttonA=1;
				}
			}
			else if(menu==2){
				if(adc_x < 50){
					if(pos_atual2x != 1){
						pos_atual2x--;
					}
				}
				else if(adc_x > 4050){
					if(pos_atual2x != 3){
						pos_atual2x++;
					}
				}
				else if (adc_y < 50) {
					if (pos_atual2y != 2) {
						pos_atual2y++;
					}
				}
				else if (adc_y > 4050) {
					if (pos_atual2y != 1) {
						pos_atual2y--;
					}
				}
				else if(gpio_get(ButtonA)==0){
					flag_buttonA=1;
					pausa(500);
				}
				else if(gpio_get(ButtonB)==0){
					flag_buttonB=1;
					pausa(500);
				}
			}
			pausa(100);
			//Condicionais para a saída do do-while de navegação
			}while(adc_y>50 && adc_y<4050 && adc_x>50 &&
				 adc_x<4050 && flag_buttonA!=1 && flag_buttonB!=1
				  && !tempo_acabou);
		}
		//Chamada da função de wifi
		wifi_embarcatech(flag_wifi_embarcatech);
		flag_wifi_embarcatech=1;
	}
    return 0;
}