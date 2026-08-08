#include "gfx.h"
#include "input.h"
#include "obj_manager.h"
#include "rtc.h"
#include "timer.h"
#include "ui.h"
#include <event.h>
#include <stdint.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

static uint8_t fb[1024];
static struct display_buffer_descriptor desc = {
    .buf_size = sizeof(fb),
    .width = 128,
    .height = 64,
    .pitch = 128,
};
static const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

static vec2 slide[20] = {{40, 0}, {10, 0}, {10, 0}, {4, 0}, {3, 0},
                         {2, 0},  {1, 0},  {1, 0},  {1, 0}, {0, 0},
                         {1, 0},  {0, 0},  {0, 0},  {0, 0}, {1, 0},
                         {0, 0},  {0, 0},  {0, 0},  {0, 0}, {0, 0}};

static vec2 back_slide[20] = {{-40, 0}, {-10, 0}, {-10, 0}, {-4, 0}, {-3, 0},
                              {-2, 0},  {-1, 0},  {-1, 0},  {-1, 0}, {0, 0},
                              {-1, 0},  {0, 0},   {0, 0},   {0, 0},  {-1, 0},
                              {0, 0},   {0, 0},   {0, 0},   {0, 0},  {0, 0}};

int main() {
  bool during_animation = false;
  bool need_render = false;
  obj_manager_t obj_manager;
  obj_manager_init(&obj_manager);
  buttons_init();
  rtc_init();
  bool visible = true;
  struct k_poll_event events[] = {
      K_POLL_EVENT_STATIC_INITIALIZER(K_POLL_TYPE_MSGQ_DATA_AVAILABLE,
                                      K_POLL_MODE_NOTIFY_ONLY, &event_msgq, 0),
  };
  watch_time_t watch_time;

  set_timer(&watch_time, 5, 10, 0);
  obj_t clock_obj =
      obj_create(-68, 25, draw_clock_ui_cb, slide, 20, &watch_time);
  obj_t test_obj = obj_create(100, 0, draw_test_ui_fb, back_slide, 20, NULL);
  obj_manager_add(&obj_manager, &clock_obj);
  obj_manager_add(&obj_manager, &test_obj);

  while (1) {
    k_poll(events, 1, K_FOREVER);
    event_t e;
    while (k_msgq_get(&event_msgq, &e, K_NO_WAIT) == 0) {
      if (e == EVENT_SECOND) {
        timer_add_one_second(&watch_time);
      }
      if (e == EVENT_ANIMATION) {
      }
      if (e == EVENT_BUTTON0) {
        obj_animation_reset(&test_obj);
        obj_animation_reset(&clock_obj);
      }
      if (e == EVENT_BUTTON1) {
        visible = !visible;
        obj_set_visible(&test_obj, visible);
      }
      // update screen
      switch (e) {
      case EVENT_SECOND:
        if (!during_animation)
          need_render = true;

        break;

      case EVENT_ANIMATION:
        need_render = true;
        break;

      case EVENT_BUTTON0:
        obj_animation_reset(&clock_obj);
        need_render = true;
        break;
      }
      if (need_render) {
        gfx_clear(fb);
        obj_manager_update(fb, &obj_manager, &during_animation);
        display_write(display, 0, 0, &desc, fb);
        if (during_animation)
          rtc_fps_set(20);
        else
          rtc_fps_off();
      }
    }
  }
}
