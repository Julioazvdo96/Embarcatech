// Retirado de https://github.com/BitDogLab/BitDogLab-C/blob/main/blink/blink.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LEDR 12
#define LEDG 11
#define LEDB 13

#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_C 22

void led_rgb_put(bool r, bool g, bool b) {
    gpio_put(LEDR, r);
    gpio_put(LEDG, g);
    gpio_put(LEDB, b);
}

int main() {
    int a=0;
    stdio_init_all();

    gpio_init(LEDR);
    gpio_init(LEDG);
    gpio_init(LEDB);
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_init(BUTTON_C);

    gpio_set_slew_rate(LEDR, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDG, GPIO_SLEW_RATE_SLOW);
    gpio_set_slew_rate(LEDB, GPIO_SLEW_RATE_SLOW);

    gpio_set_dir(LEDR, GPIO_OUT);
    gpio_set_dir(LEDG, GPIO_OUT);
    gpio_set_dir(LEDB, GPIO_OUT);
    
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_set_dir(BUTTON_C, GPIO_IN); 

    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    gpio_pull_up(BUTTON_C);


    while (true) {
        if(a==0){
            if(gpio_get(BUTTON_A)==0 && gpio_get(BUTTON_B)==0){
                led_rgb_put(true, true, true);
            }
            else if(gpio_get(BUTTON_A)==0){
                led_rgb_put(true, true, false);
            }
            else if(gpio_get(BUTTON_B)==0){
                led_rgb_put(true, false, true);
            }
            else if(gpio_get(BUTTON_C)==0){
                a++;
                led_rgb_put(true, true, true);
                sleep_ms(1000);
            }
            else{
                led_rgb_put(true, false, false);
            }
        }
        if(a==1){
            if(gpio_get(BUTTON_A)==0 && gpio_get(BUTTON_B)==0){
                led_rgb_put(true, true, true);
            }
            else if(gpio_get(BUTTON_A)==0){
                led_rgb_put(true, true, false);
            }
            else if(gpio_get(BUTTON_B)==0){
                led_rgb_put(false, true, true);
            }
            else if(gpio_get(BUTTON_C)==0){
                a++;
                led_rgb_put(true, true, true);
                sleep_ms(1000);
            }
            else{
                led_rgb_put(false, true, false);
            }
        }
        if(a==2){
            if(gpio_get(BUTTON_A)==0 && gpio_get(BUTTON_B)==0){
                led_rgb_put(true, true, true);
            }
            else if(gpio_get(BUTTON_A)==0){
                led_rgb_put(true, false, true);
            }
            else if(gpio_get(BUTTON_B)==0){
                led_rgb_put(false, true, true);
            }
            else if(gpio_get(BUTTON_C)==0){
                a=0;
                led_rgb_put(true, true, true);
                sleep_ms(1000);
            }
            else{
                led_rgb_put(false, false, true);
            }
        }
    sleep_ms(10);
    }
}