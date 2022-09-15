#ifndef STAR_RENDER_INCLUDED
#define STAR_RENDER_INCLUDED

#include <astera/render.h>

typedef struct {
  r_ctx astera_ctx;
} star_render_ctx_t;

typedef struct {
  r_sprite astera_sprite;
} star_sprite_t;

star_render_ctx_t* star_render_ctx_init();
void               star_render_ctx_exit();

void star_render_ctx_clear();
void star_render_ctx_draw();
void star_render_ctx_swap();

void star_render_draw();

#endif
