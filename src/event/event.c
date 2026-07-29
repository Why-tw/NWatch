#include "event.h"
#include <zephyr/kernel.h>

K_MSGQ_DEFINE(event_msgq, sizeof(event_t), 16, 4);
