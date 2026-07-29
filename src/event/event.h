#pragma once

typedef enum {
  EVENT_SECOND,
  EVENT_ANIMATION,
  EVENT_BUTTON0,
  EVENT_BUTTON1,
  EVENT_BUTTON2,
  EVENT_BUTTON3
} event_t;

extern struct k_msgq event_msgq;
