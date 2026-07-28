#include "ssd1306.h"
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

static const struct i2c_dt_spec i2c_ssd1306 =
    I2C_DT_SPEC_GET(DT_NODELABEL(ssd1306));

static int ssd1306_cmd(uint8_t cmd) {
  uint8_t buf[2];
  buf[0] = 0x00;
  buf[1] = cmd;
  int ret = i2c_write_dt(&i2c_ssd1306, buf, sizeof(buf));
  return ret;
}

static int ssd1306_data(uint8_t *data, size_t len) {
  uint8_t buf[129];
  buf[0] = 0x40;
  memcpy(&buf[1], data, len);
  int ret = i2c_write_dt(&i2c_ssd1306, buf, len + 1);
  // printk("ret = %d\n", ret);
  return ret;
}

static int ssd1306_cmds(const uint8_t *cmds, size_t len) {
  uint8_t buf[len + 1];
  buf[0] = 0x00;
  memcpy(&buf[1], cmds, len);
  int ret = i2c_write_dt(&i2c_ssd1306, buf, len + 1);
  // printk("ret = %d\n", ret);
  return ret;
}

void ssd1306_init() {
  static const uint8_t init_cmds[] = {0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00,
                                      0x40, 0x8D, 0x14, 0x20, 0x02, 0xA1, 0xC8,
                                      0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 0xDB,
                                      0x40, 0xA4, 0xA6, 0xAF};
  ssd1306_cmds(init_cmds, sizeof(init_cmds));
}

void ssd1306_flush(uint8_t *fb) {
  for (int page = 0; page < 8; page++) {
    ssd1306_cmd(0xB0 + page);
    ssd1306_cmd(0x00);
    ssd1306_cmd(0x10);
    ssd1306_data(&fb[page * 128], 128);
  }
}

void ssd1306_display_on() { ssd1306_cmd(0xAF); }

void ssd1306_display_off() { ssd1306_cmd(0xAE); }

void ssd1306_color_turn(uint8_t i) {
  if (i)
    ssd1306_cmd(0xA7);
  else
    ssd1306_cmd(0xA6);
}

void ssd1306_set_contrast(uint8_t contrast) {
  uint8_t contrast_cmds[] = {0x81, contrast};
  ssd1306_cmds(contrast_cmds, sizeof(contrast_cmds));
}
