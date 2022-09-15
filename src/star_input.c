#include <star32/star_input.h>
#include <star32/star_debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static star_input_ctx_t* ctx = 0;

// Search thru the keymap to get the correct output
static u8 star_input_get_dst(u8 user, u8 src_value) {
  if (user >= STAR_MAX_GAMEPADS) {
    return src_value;
  }

  star_gamepad_t* gamepad = &ctx->gamepads[user];
  star_keymap_t   keymap  = gamepad->keymap;
  for (u8 i = 0; i < keymap.count; ++i) {
    if (keymap.src[i] == src_value) {
      return keymap.dst[i];
    }
  }

  return src_value;
}

static u8 star_input_is_valid_for_gamepad(star_gamepad_type type,
                                          star_key_type     key_type) {
  switch (type) {
    case STAR_EMPTY_GAMEPAD:
      return 0;
    case STAR_KEYBOARD:
      return key_type == STAR_KEY;
    case STAR_GAMEPAD:
      return key_type == STAR_GAMEPAD_BUTTON || key_type == STAR_GAMEPAD_AXIS;
    case STAR_XBOX:
      return key_type == STAR_XBOX_BUTTON || key_type == STAR_XBOX_AXIS;
    case STAR_PLAYSTATION:
      return key_type == STAR_PLAYSTATION_BUTTON ||
             key_type == STAR_PLAYSTATION_AXIS;
    default:
      star_warning_out("Invalid type checked for key validity: %i\n", type);
      return 0;
  }
}

static void star_input_load_default_keymap() {
  if (!ctx)
    return;

  // TODO this whole thing lol
}

star_input_ctx_t* star_input_ctx_init() {
  // Don't overwrite any existing input contexts
  if (ctx) {
    return 0;
  }

  ctx = (star_input_ctx_t*)calloc(1, sizeof(star_input_ctx_t));
  star_input_load_default_keymap();
}

void star_input_ctx_exit() {
  // Don't destroy nothing!
  if (!ctx) {
    return;
  }
}

void star_input_ctx_update(float dt) {
  if (!ctx) {
    star_warning_out("No input context present at time of updating!\n");
    return;
  }

  for (u8 i = 0; i < STAR_MAX_GAMEPADS; ++i) {
    star_gamepad_t* gamepad = &ctx->gamepads[i];
    // If a valid controller (all require user pointer)

    if (gamepad->type != STAR_EMPTY_GAMEPAD) {
      memcpy(gamepad->last, gamepad->buttons, sizeof(u8) * STAR_MAX_BUTTONS);
      memcpy(gamepad->axes_last, gamepad->axes,
             sizeof(float) * STAR_MAX_BUTTONS);

      for (u8 j = 0; j < STAR_MAX_AXES; ++j) {
        if (gamepad->axes[j] != 0.0f) {
          gamepad->button_timers[j] += dt;
        } else {
          gamepad->button_timers[j] = 0.0f;
        }
      }
    }
  }
}

static star_gamepad_type star_input_get_gamepad_type(const char* guid) {
  return STAR_GAMEPAD;
}

// Let me check glfw docs real quick
u8 star_input_assign_gamepad(u8 user, u8 joystick_id) {
  const char*       guid = glfwGetJoystickGUID(joystick_id);
  star_gamepad_type type = STAR_EMPTY_GAMEPAD;

  if (guid) {
    type = star_input_get_gamepad_type(guid);
  } else {
    return 0;
  }

  if (type == STAR_EMPTY_GAMEPAD) {
    star_warning_out("Unsupported controller type: %s\n", guid);
    return 0;
  }

  star_gamepad_t* gamepad = &ctx->gamepads[user];

  glfwSetJoystickUserPointer(joystick_id, gamepad);

  // Assign keymap specific to controller type
  star_keymap_t src_map        = ctx->default_keymap;
  star_keymap_t new_map        = (star_keymap_t){0};
  u8            relevant_count = 0;

  for (int i = 0; i < src_map.count; ++i) {
    if (star_input_is_valid_for_gamepad(type, src_map.src_types[i])) {
      ++relevant_count;
    }
  }

  new_map.src_types =
      (star_key_type*)calloc(relevant_count, sizeof(star_gamepad_type));
  new_map.dst = (u8*)calloc(relevant_count, sizeof(u8));
  new_map.src = (u8*)calloc(relevant_count, sizeof(u8));

  for (int i = 0; i < src_map.count; ++i) {
    if (star_input_is_valid_for_gamepad(type, src_map.src_types[i])) {
      new_map.src[new_map.count]       = src_map.src[i];
      new_map.dst[new_map.count]       = src_map.dst[i];
      new_map.src_types[new_map.count] = src_map.src_types[i];
      ++new_map.count;
    }
  }

  gamepad->keymap = new_map;

  return 1;
}

void star_input_close_joystick(u8 user) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return;
  }

  // close out keymap
  star_keymap_t* keymap = &ctx->gamepads[user].keymap;
  free(keymap->src);
  free(keymap->src_types);
  free(keymap->dst);

  ctx->gamepads[user].keymap = (star_keymap_t){0};
  ctx->gamepads[user].type   = STAR_EMPTY_GAMEPAD;
  memset(ctx->gamepads[user].buttons, 0, sizeof(u8) * STAR_MAX_BUTTONS);
  memset(ctx->gamepads[user].last, 0, sizeof(u8) * STAR_MAX_BUTTONS);
  memset(ctx->gamepads[user].axes, 0, sizeof(float) * STAR_MAX_AXES);
  memset(ctx->gamepads[user].axes_last, 0, sizeof(float) * STAR_MAX_AXES);
  memset(ctx->gamepads[user].button_timers, 0, sizeof(float) * STAR_MAX_AXES);
}

u8 star_input_button_down(u8 user, u8 button) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return 0;
  }

  return ctx->gamepads[user].buttons[button];
}

u8 star_input_button_up(u8 user, u8 button) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return 0;
  }

  return ctx->gamepads[user].buttons[button] == 0;
}

u8 star_input_button_clicked(u8 user, u8 button) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return 0;
  }

  return ctx->gamepads[user].buttons[button] == 1 &&
         ctx->gamepads[user].last[button] == 0;
}

u8 star_input_button_released(u8 user, u8 button) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return 0;
  }

  return ctx->gamepads[user].buttons[button] == 0 &&
         ctx->gamepads[user].last[button] == 1;
}

float star_input_button_timer(u8 user, u8 button) {
  if (user > STAR_MAX_GAMEPADS) {
    star_warning_out("No users [%i] exist past: %i\n", user, STAR_MAX_GAMEPADS);
    return 0;
  }

  return ctx->gamepads[user].button_timers[button];
}

