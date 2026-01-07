/**
 * tiling.h - Hyprland-style Tiling Engine
 */

#ifndef OSF_TILING_H
#define OSF_TILING_H

#include "server.h"
#include <stdbool.h>
#include <stdint.h>

struct osf_tiling_config {
  int inner_gap;
  int outer_gap;
  int border_width;
  uint32_t active_border_color;
  uint32_t inactive_border_color;
};

void osf_tiling_init(struct osf_server *server);
void osf_tiling_update(struct osf_server *server);
void osf_tiling_arrange(struct osf_server *server, struct osf_output *output);
void osf_view_update_borders(struct osf_view *view, bool active);

#endif
