#ifndef MENU_H
#define MENU_H

#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include <string.h>

void zeraDisplay(struct render_area frame_area);  // Declaração da função zeraDisplay

void MenuA1(struct render_area frame_area);       // Declaração da função MenuA1
void MenuA2(struct render_area frame_area);       // Declaração da função MenuA2
void MenuA3(struct render_area frame_area);       // Declaração da função MenuA3

void MenuB1(struct render_area frame_area);       // Declaração da função MenuB1
void MenuB2(struct render_area frame_area);       // Declaração da função MenuB2
void MenuB3(struct render_area frame_area);       // Declaração da função MenuB3
void MenuB4(struct render_area frame_area);       // Declaração da função MenuB4
void MenuB5(struct render_area frame_area);       // Declaração da função MenuB5
void MenuB6(struct render_area frame_area);       // Declaração da função MenuB6

#endif 