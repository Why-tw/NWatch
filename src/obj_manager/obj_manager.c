#include "obj_manager.h"
#include <stdbool.h>
#include <zephyr/sys/printk.h>

obj_t obj_create(int16_t x, int16_t y,
                 void (*draw)(uint8_t *fb, struct obj *self), vec2 *slide,
                 uint8_t frame_count, void *data) {
  animation_t animation = {
      .frame = 0, .frame_count = frame_count, .frames = slide};
  obj_t _obj = {.visible = 1,
                .base_x = x,
                .base_y = y,
                .draw_x = x,
                .draw_y = y,
                .animation = animation,
                .data = data,
                .draw_cb = draw};
  return _obj;
}

void obj_manager_init(obj_manager_t *obj_manager) { obj_manager->count = 0; }

void obj_manager_add(obj_manager_t *obj_manager, obj_t *obj) {
  printk("ADD: obj=%p draw_cb=%p frame=%d/%d\n", obj, obj->draw_cb,
         obj->animation.frame, obj->animation.frame_count);
  if (obj_manager->count >= MAX_OBJ)
    return;
  obj_manager->obj_arr[obj_manager->count] = obj;
  obj_manager->count++;
}

void obj_manager_update(uint8_t *fb, obj_manager_t *obj_manager,
                        bool *during_animation) {
  bool animation_playing = false;
  for (int i = 0; i < obj_manager->count; i++) {
    obj_t *current_obj = obj_manager->obj_arr[i];
    if (current_obj->visible == 0)
      continue;
    printk("UPDATE: obj=%p draw_cb=%p frame=%d/%d pos=(%d, %d)\n", current_obj,
           current_obj->draw_cb, current_obj->animation.frame,
           current_obj->animation.frame_count, current_obj->draw_x,
           current_obj->draw_y);
    current_obj->draw_cb(fb, current_obj);
    if (current_obj->animation.frame < current_obj->animation.frame_count) {
      animation_playing = true;
      current_obj->draw_x +=
          current_obj->animation.frames[current_obj->animation.frame].dx;
      current_obj->draw_y +=
          current_obj->animation.frames[current_obj->animation.frame].dy;
      current_obj->animation.frame++;
    }
  }
  *during_animation = animation_playing;
}

void obj_set_visible(obj_t *obj, bool visible) { obj->visible = visible; }

void obj_animation_reset(obj_t *obj) {
  obj->animation.frame = 0;
  obj->draw_x = obj->base_x;
  obj->draw_y = obj->base_y;
}
