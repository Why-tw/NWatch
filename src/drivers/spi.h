#pragma once

#include <stddef.h>
#include <stdint.h>

int drv_spi_init(void);
int drv_spi_write(const uint8_t *data, size_t len);
