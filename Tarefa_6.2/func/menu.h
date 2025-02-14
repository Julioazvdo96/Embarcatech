#ifndef MENU_H
#define MENU_H

#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include <string.h>

void zeraDisplay(struct render_area frame_area);  // Declaração da função zeraDisplay

void Menu1(struct render_area frame_area);       // Declaração da função Menu1
void Menu2(struct render_area frame_area);       // Declaração da função Menu1
void Menu3(struct render_area frame_area);       // Declaração da função Menu1
void Menu4(struct render_area frame_area);       // Declaração da função Menu1


#endif 