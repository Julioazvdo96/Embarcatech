#include "joy.h"


// Inicialização das variáveis globais
uint16_t led_b_level = 100;
uint16_t led_r_level = 100;
uint slice_led_b, slice_led_r;

// Função para configurar o joystick
void setup_joystick() {
    adc_init();
    adc_gpio_init(VRX);
    adc_gpio_init(VRY);

    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
}

// Função para configurar o PWM de um LED
void setup_pwm_led(uint led, uint *slice, uint16_t level) {
    gpio_set_function(led, GPIO_FUNC_PWM);
    *slice = pwm_gpio_to_slice_num(led);
    pwm_set_clkdiv(*slice, DIVIDER_PWM);
    pwm_set_wrap(*slice, PERIOD1);
    pwm_set_gpio_level(led, level);
    pwm_set_enabled(*slice, true);
}

// Função de configuração geral
void setup_joy() {
    stdio_init_all();
    setup_joystick();
    setup_pwm_led(LED_B, &slice_led_b, led_b_level);
    setup_pwm_led(LED_R, &slice_led_r, led_r_level);
}

// Função para ler os valores dos eixos do joystick
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value) {
    adc_select_input(ADC_CHANNEL_0);
    sleep_us(2);
    *vrx_value = adc_read();

    adc_select_input(ADC_CHANNEL_1);
    sleep_us(2);
    *vry_value = adc_read();
}

// Loop principal do joystick
void joystick_loop(uint opcao1) {
    uint16_t vrx_value, vry_value;
    if(opcao1==0){
        setup_joy();
    }

    while (1) {
        joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
        // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
        pwm_set_gpio_level(LED_B, vrx_value); // Ajusta o brilho do LED azul com o valor do eixo X
        pwm_set_gpio_level(LED_R, vry_value); // Ajusta o brilho do LED vermelho com o valor do eixo Y
        
        // Pequeno delay antes da próxima leitura
        if(gpio_get(Button)==0){
            pwm_set_gpio_level(LED_B, 0);
            pwm_set_gpio_level(LED_R, 0);
            sleep_ms(500);
            break;
        }
        sleep_ms(100); // Espera 100 ms antes de repetir o ciclo
    }
}
