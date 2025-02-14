#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>

#define EixoX 27
#define EixoY 26

#define LEDB 12
#define LEDG 11
#define LEDR 13

void led_rgb_put(bool r, bool g, bool b) {
    gpio_put(LEDR, r);
    gpio_put(LEDG, g);
    gpio_put(LEDB, b);
}

int main(){
    uint adc_x, adc_y;

    stdio_init_all();
    adc_init();

    gpio_init(LEDR);
    gpio_init(LEDG);
    gpio_init(LEDB);
    
    adc_gpio_init(EixoX);
    adc_gpio_init(EixoY);

    gpio_set_slew_rate(LEDR, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDG, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDB, GPIO_SLEW_RATE_SLOW);

    gpio_set_dir(LEDR, GPIO_OUT);
    gpio_set_dir(LEDG, GPIO_OUT);
    gpio_set_dir(LEDB, GPIO_OUT);

    while(true){
        adc_select_input(1); //o ADC1 está localizado no GP27, eixo X, com essa função
        //estamos selecionando qual adc vai dar a informação no adc_read()
        adc_x = adc_read();

        adc_select_input(0);
        adc_y = adc_read();

        printf("X = %d Y = %d \n", adc_x, adc_y);


        if(adc_x>50 && adc_x<420){
            if(adc_y>3500 && adc_y<4070){
                led_rgb_put(1,0,1);
            }
            else if(adc_y>30 && adc_y<400){
                led_rgb_put(1,1,0);
            }
        }
        else if(adc_x>3400 && adc_x<4070){
            if(adc_y>3500&& adc_y<4070){
                led_rgb_put(0,1,1);
            }
            if(adc_y>30 && adc_y<400){
                led_rgb_put(0,1,1);
            }
        }
        else if(adc_y < 30){
            led_rgb_put(0,1,0);
        }
        else if(adc_y > 4070){
            led_rgb_put(0,0,1);
        }
        else if(adc_x <30){
            led_rgb_put(1,0,0);
        }
        else if(adc_x > 4070){
            led_rgb_put(1,1,1);
        }
        else{
            led_rgb_put(0,0,0);
        }
        sleep_ms(50);
    }

}