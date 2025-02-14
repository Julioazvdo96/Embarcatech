#include "led_rgb.h"


void setup_led(uint led_level) {
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(LED);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIVIDER_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PERIOD);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(LED, led_level);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
    gpio_init(Button);
    gpio_set_dir(Button, GPIO_IN); 
    gpio_pull_up(Button);
    stdio_init_all(); // Inicializa o sistema padrão de I/O
}

int LED_RGB(uint opcao3) {
    

    uint up_down = 1; // Variável para controlar se o nível do LED aumenta ou diminui
    uint led_level = 100; // Nível inicial do PWM (duty cycle)
    
    if(opcao3==0){
        setup_led(led_level);      // Configura o PWM
    }
    while (true){
        pwm_set_gpio_level(LED, led_level); // Define o nível atual do PWM (duty cycle)
        sleep_ms(1000);                     // Atraso de 1 segundo
        if(gpio_get(Button)==0){
			pwm_set_gpio_level(LED, 0);
			sleep_ms(1000);
			return 0;
		}
		else if (up_down)
        {
            led_level += LED_STEP; // Incrementa o nível do LED
            if (led_level >= PERIOD)
                up_down = 0; // Muda direção para diminuir quando atingir o período máximo
        }
		else
        {
            led_level -= LED_STEP; // Decrementa o nível do LED
            if (led_level <= LED_STEP)
                up_down = 1; // Muda direção para aumentar quando atingir o mínimo
        }
    }
}
