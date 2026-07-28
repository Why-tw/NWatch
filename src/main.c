#include "gfx.h"
#include "ssd1306.h"
#include "zephyr/kernel.h"
#include <stdint.h>

static uint8_t fb[1024];

int main() {
  ssd1306_init();
  gfx_draw_line(fb, 128, 64, 0, 0);
  ssd1306_flush(fb);
}
