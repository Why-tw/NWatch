#include "timer.h"
#include <stdint.h>

int set_timer(watch_time_t *watch_time, uint8_t hr, uint8_t min, uint8_t sec) {
  if (hr >= 24 || min >= 60 || sec >= 60)
    return 1;
  watch_time->hour = hr;
  watch_time->minute = min;
  watch_time->second = sec;
  return 0;
}

void timer_add_one_second(watch_time_t *watch_time) {
  if (watch_time->second >= 59) {
    watch_time->second = 0;
    if (watch_time->minute >= 59) {
      watch_time->hour++;
      watch_time->minute = 0;
    } else {
      watch_time->minute++;
    }
  } else {
    watch_time->second++;
  }
}
