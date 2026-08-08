#include "ui.h"
#include "gfx.h"
#include "obj_manager.h"
#include "timer.h"
#include <stdint.h>
#include <stdio.h>

static void draw_clock_ui(uint8_t *fb, int16_t x, int16_t y,
                          watch_time_t *watch_time) {
  char buf[12];
  snprintf(buf, 12, "%02d:%02d:%02d", watch_time->hour, watch_time->minute,
           watch_time->second);
  gfx_print(fb, x, y, buf, 2, 5);
}

void draw_clock_ui_cb(uint8_t *fb, obj_t *obj) {
  draw_clock_ui(fb, obj->draw_x, obj->draw_y, (watch_time_t *)obj->data);
}

static void draw_test_ui(uint8_t *fb, int16_t x, int16_t y) {
  gfx_print(fb, x, y, "NWATCH", 2, 5);
  gfx_print(fb, x + 7, y + 48, "ESP32", 2, 5);
}

void draw_test_ui_fb(uint8_t *fb, obj_t *obj) {
  draw_test_ui(fb, obj->draw_x, obj->draw_y);
}
