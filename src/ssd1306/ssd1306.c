#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>

const struct device *spi = DEVICE_DT_GET(DT_NODELABEL(spi1));
static const struct gpio_dt_spec dc =
    GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), dc_gpio);

static const struct gpio_dt_spec cs = {.port =
                                           DEVICE_DT_GET(DT_NODELABEL(gpio0)),
                                       .pin = 24,
                                       .dt_flags = GPIO_ACTIVE_LOW};

static int ssd1306_write_cmd(uint8_t cmd) {
  gpio_pin_set_dt(&dc, 0);

  struct spi_buf tx_buf = {
      .buf = &cmd,
      .len = 1,
  };

  struct spi_buf_set tx = {
      .buffers = &tx_buf,
      .count = 1,
  };

  return spi_write(spi, &cfg, &tx);
}
