#include "spi.h"

#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>

static const struct device *spi_dev = DEVICE_DT_GET(DT_NODELABEL(spi1));
static const struct spi_config spi_cfg = {
    .frequency = 8000000,
    .operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB,
    .slave = 0,
};
int drv_spi_init(void) {
  if (!device_is_ready(spi_dev)) {
    return -1;
  }

  return 0;
}
int drv_spi_write(const uint8_t *data, size_t len) {
  struct spi_buf buf = {
      .buf = (void *)data,
      .len = len,
  };

  struct spi_buf_set tx = {
      .buffers = &buf,
      .count = 1,
  };

  return spi_write(spi_dev, &spi_cfg, &tx);
}
