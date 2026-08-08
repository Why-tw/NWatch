#pragma once

#include <stdbool.h>
#include <stdint.h>
#define MAX_OBJ 16

typedef struct {
  int8_t dx;
  int8_t dy;
} vec2;

typedef struct {
  uint8_t frame;
  uint8_t frame_count;
  vec2 *frames;
} animation_t;

typedef struct obj {
  bool visible;
  int16_t base_x;
  int16_t base_y;
  int16_t draw_x;
  int16_t draw_y;
  animation_t animation;
  void *data;

  void (*draw_cb)(uint8_t *fb, struct obj *self);
} obj_t;

typedef struct {
  obj_t *obj_arr[MAX_OBJ];
  uint8_t count;
} obj_manager_t;

obj_t obj_create(int16_t x, int16_t y,
                 void (*draw)(uint8_t *fb, struct obj *self), vec2 *slide,
                 uint8_t frame_count, void *data);

void obj_manager_init(obj_manager_t *obj_manager);

void obj_manager_add(obj_manager_t *obj_manager, obj_t *obj);

void obj_manager_update(uint8_t *fb, obj_manager_t *obj_manager,
                        bool *during_animation);

void obj_set_visible(obj_t *obj, bool visible);

void obj_animation_reset(obj_t *obj);
