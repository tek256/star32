#ifndef STAR_INPUT_INCLUDED
#define STAR_INPUT_INCLUDED

#include <star32/star_types.h>
#include <astera/input.h>

#define STAR_MAX_GAMEPADS 4
#define STAR_MAX_BUTTONS  32
#define STAR_MAX_AXES     8 // these are your joysticks/joy axes

#define STAR_BUTTON_RPAD_DOWN  = 1
#define STAR_BUTTON_RPAD_LEFT  = 2
#define STAR_BUTTON_RPAD_RIGHT = 3
#define STAR_BUTTON_RPAD_UP    = 4
#define STAR_BUTTON_LPAD_DOWN  = 5
#define STAR_BUTTON_LPAD_LEFT  = 6
#define STAR_BUTTON_LPAD_RIGHT = 7
#define STAR_BUTTON_LPAD_UP    = 8
#define STAR_BUTTON_START      = 9
#define STAR_BUTTON_SELECT     = 10
#define STAR_BUTTON_L1         = 11
#define STAR_BUTTON_R1         = 12
#define STAR_BUTTON_L2         = 13
#define STAR_BUTTON_R2         = 14

typedef enum {
  STAR_EMPTY_GAMEPAD = 0,
  STAR_KEYBOARD,
  STAR_GAMEPAD,
  STAR_XBOX,
  STAR_PLAYSTATION,
} star_gamepad_type;

typedef enum {
  STAR_KEY = 0,
  STAR_GAMEPAD_BUTTON,
  STAR_GAMEPAD_AXIS,
  STAR_XBOX_BUTTON,
  STAR_XBOX_AXIS,
  STAR_PLAYSTATION_BUTTON,
  STAR_PLAYSTATION_AXIS,
} star_key_type;

typedef struct {
  star_key_type* src_types;
  u8*            src;
  u8*            dst;
  u16            count;
} star_keymap_t;

typedef struct {
  u8                buttons[STAR_MAX_BUTTONS];
  u8                last[STAR_MAX_BUTTONS];
  float             button_timers[STAR_MAX_BUTTONS];
  float             axes[STAR_MAX_AXES];
  float             axes_last[STAR_MAX_AXES];
  star_gamepad_type type;
  star_keymap_t     keymap;
} star_gamepad_t;

typedef struct {
  star_keymap_t  default_keymap;
  star_gamepad_t gamepads[4];
  i_ctx*         astera_ctx;
} star_input_ctx_t;

star_input_ctx_t* star_input_ctx_init(void);
void              star_input_ctx_exit(void);
void              star_input_ctx_update(float dt);

u8   star_input_assign_gamepad(u8 user, u8 joystick_id);
void star_input_close_gamepad(u8 user);

u8    star_input_button_down(u8 user, u8 button);
u8    star_input_button_up(u8 user, u8 button);
u8    star_input_button_clicked(u8 user, u8 button);
u8    star_input_button_released(u8 user, u8 button);
float star_input_button_timer(u8 user, u8 button);

#endif
