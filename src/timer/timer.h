#pragma once
#include <stdint.h>

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} watch_time_t;

int set_timer(watch_time_t *watch_time, uint8_t hr, uint8_t min, uint8_t sec);

void timer_add_one_second(watch_time_t *watch_time);
