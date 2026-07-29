#include "event.h"
#include "mdk/nrf52/nrf52840/nrf52840.h"
#include "zephyr/toolchain.h"
#include <hal/nrf_rtc.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#define RTC_FREQ 32768U

static volatile uint32_t frame_interval_ticks;

static void RTC2_IRQHandler(const void *arg) {
  ARG_UNUSED(arg);
  if (nrf_rtc_event_check(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0)) {
    nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0);
    event_t e = EVENT_SECOND;
    k_msgq_put(&event_msgq, &e, K_NO_WAIT);
    uint32_t cc = nrf_rtc_cc_get(NRF_RTC2, 0);
    nrf_rtc_cc_set(NRF_RTC2, 0, cc + RTC_FREQ);
  }
  if (nrf_rtc_event_check(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1)) {
    nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1);
    event_t e = EVENT_ANIMATION;
    k_msgq_put(&event_msgq, &e, K_NO_WAIT);
    uint32_t cc = nrf_rtc_cc_get(NRF_RTC2, 1);
    nrf_rtc_cc_set(NRF_RTC2, 1, cc + frame_interval_ticks);
  }
}

void rtc_init(void) {
  nrf_rtc_task_trigger(NRF_RTC2, NRF_RTC_TASK_STOP);
  nrf_rtc_task_trigger(NRF_RTC2, NRF_RTC_TASK_CLEAR);
  nrf_rtc_prescaler_set(NRF_RTC2, 0);
  nrf_rtc_cc_set(NRF_RTC2, 0, RTC_FREQ);
  nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_0);
  nrf_rtc_int_enable(NRF_RTC2, NRF_RTC_INT_COMPARE0_MASK);
  IRQ_CONNECT(RTC2_IRQn, 1, RTC2_IRQHandler, NULL, 0);
  irq_enable(RTC2_IRQn);
  nrf_rtc_task_trigger(NRF_RTC2, NRF_RTC_TASK_START);
}

void rtc_fps_set(uint32_t fps) {
  if (fps == 0 || fps >= RTC_FREQ)
    return;
  frame_interval_ticks = RTC_FREQ / fps;
  uint32_t now = nrf_rtc_counter_get(NRF_RTC2);
  nrf_rtc_cc_set(NRF_RTC2, 1, now + frame_interval_ticks);
  nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1);
  nrf_rtc_int_enable(NRF_RTC2, NRF_RTC_INT_COMPARE_1_MASK);
}

void rtc_fps_off(void) {
  nrf_rtc_event_clear(NRF_RTC2, NRF_RTC_EVENT_COMPARE_1);
  nrf_rtc_int_disable(NRF_RTC2, NRF_RTC_INT_COMPARE1_MASK);
  NVIC_ClearPendingIRQ(RTC2_IRQn);
}
