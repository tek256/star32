#include <star32/star_render.h>

static star_render_ctx_t* ctx = 0;

star_render_ctx_t* star_render_ctx_init() {}

void star_render_ctx_exit() {
  if (!ctx) {}
}
