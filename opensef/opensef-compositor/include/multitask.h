/**
 * multitask.h - Overview Mode (Mission Control)
 */

#ifndef OSF_MULTITASK_H
#define OSF_MULTITASK_H

#include <stdbool.h>

struct osf_server;

struct osf_multitask {
  struct osf_server *server;
  bool active;
  float anim_progress;
  struct wlr_scene_tree *scene_tree;
  struct wlr_scene_rect *overlay;
};

void osf_multitask_init(struct osf_server *server);
void osf_multitask_toggle(struct osf_server *server);
void osf_multitask_update(struct osf_server *server);

#endif
