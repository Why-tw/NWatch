#include "gfx.h"
#include "input.h"
#include "rtc.h"
#include "ssd1306.h"
#include "timer.h"
#include <event.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static uint8_t fb[1024];

int main() {
  buttons_init();
  rtc_init();
  ssd1306_init();
  rtc_fps_set(5);
  struct k_poll_event events[] = {
      K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_MSGQ_DATA_AVAILABLE,
                                      K_POLL_MODE_NOTIFY_ONLY, &event_msgq, 0),
  };
  watch_time_t watch_time;
  set_timer(&watch_time, 20, 25, 0);

  while (1) {
    k_poll(events, 1, K_FOREVER);
    event_t e;
    while (k_msgq_get(&event_msgq, &e, K_NO_WAIT) == 0) {
      if (e == EVENT_SECOND) {
        timer_add_one_second(&watch_time);
        gfx_clear(fb);
        char buf[12];
        snprintk(buf, 12, "%02d:%02d:%02d", watch_time.hour, watch_time.minute,
                 watch_time.second);
        gfx_print(fb, 5, 25, buf, 2, 5);
      }
      if (e == EVENT_ANIMATION) {
      }
      ssd1306_flush(fb);
    }
  }
}
