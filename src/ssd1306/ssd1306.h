#pragma once

#include <stdint.h>

void ssd1306_init();

void ssd1306_flush(uint8_t *fb);

void ssd1306_display_on();

void ssd1306_display_off();

void ssd1306_color_turn(uint8_t i);

void ssd1306_set_constrast(uint8_t constrast);
