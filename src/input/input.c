#include "event.h"
#include "zephyr/kernel.h"
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

static const struct gpio_dt_spec btn0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec btn2 = GPIO_DT_SPEC_GET(DT_ALIAS(sw2), gpios);
static const struct gpio_dt_spec btn3 = GPIO_DT_SPEC_GET(DT_ALIAS(sw3), gpios);

static struct gpio_callback btn0_cb;
static struct gpio_callback btn1_cb;
static struct gpio_callback btn2_cb;
static struct gpio_callback btn3_cb;

static void btn0_handler(const struct device *port, struct gpio_callback *cb,
                         gpio_port_pins_t pins) {
  event_t e = EVENT_BUTTON0;
  k_msgq_put(&event_msgq, &e, K_NO_WAIT);
}
static void btn1_handler(const struct device *port, struct gpio_callback *cb,
                         gpio_port_pins_t pins) {
  event_t e = EVENT_BUTTON1;
  k_msgq_put(&event_msgq, &e, K_NO_WAIT);
}
static void btn2_handler(const struct device *port, struct gpio_callback *cb,
                         gpio_port_pins_t pins) {
  event_t e = EVENT_BUTTON2;
  k_msgq_put(&event_msgq, &e, K_NO_WAIT);
}
static void btn3_handler(const struct device *port, struct gpio_callback *cb,
                         gpio_port_pins_t pins) {
  event_t e = EVENT_BUTTON3;
  k_msgq_put(&event_msgq, &e, K_NO_WAIT);
}

int buttons_init() {
  if (!gpio_is_ready_dt(&btn0) || !gpio_is_ready_dt(&btn1) ||
      !gpio_is_ready_dt(&btn2) || !gpio_is_ready_dt(&btn3))
    return 1;

  gpio_pin_configure_dt(&btn0, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&btn0, GPIO_INT_EDGE_TO_ACTIVE);
  gpio_init_callback(&btn0_cb, btn0_handler, BIT(btn0.pin));
  gpio_add_callback_dt(&btn0, &btn0_cb);

  gpio_pin_configure_dt(&btn1, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&btn1, GPIO_INT_EDGE_TO_ACTIVE);
  gpio_init_callback(&btn1_cb, btn1_handler, BIT(btn1.pin));
  gpio_add_callback_dt(&btn1, &btn1_cb);

  gpio_pin_configure_dt(&btn2, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&btn2, GPIO_INT_EDGE_TO_ACTIVE);
  gpio_init_callback(&btn2_cb, btn2_handler, BIT(btn2.pin));
  gpio_add_callback_dt(&btn2, &btn2_cb);

  gpio_pin_configure_dt(&btn3, GPIO_INPUT);
  gpio_pin_interrupt_configure_dt(&btn3, GPIO_INT_EDGE_TO_ACTIVE);
  gpio_init_callback(&btn3_cb, btn3_handler, BIT(btn3.pin));
  gpio_add_callback_dt(&btn3, &btn3_cb);

  return 0;
}
