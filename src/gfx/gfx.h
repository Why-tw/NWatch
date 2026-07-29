#pragma once
#include <stdint.h>

void gfx_draw_pixel(uint8_t *fb, int x, int y);

void gfx_draw_rect(uint8_t *fb, int x1, int y1, int x2, int y2);
void gfx_clear(uint8_t *fb);
void gfx_draw_font(uint8_t *fb, int x, int y, int ch, int scale);
void gfx_print(uint8_t *fb, int x, int y, char str[], uint8_t scale,
               uint8_t space);
void gfx_draw_line(uint8_t *fb, int x1, int y1, int x2, int y2);
