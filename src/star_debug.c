#include <star32/star_debug.h>

star_debug_ctx_t* star_debug_ctx_init();
void              star_debug_ctx_exit();

void star_debug_enable_logs(uint8_t enabled);

void star_debug_out(const char* fmt, ...);
void star_error_out(const char* fmt, ...);
