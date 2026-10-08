// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layer_names {
  _BASE_L,
  _MEDIA_L,
  _RGB_L,
};

// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   2

enum keycodes {
  KC_LAYER_GO_UP = QK_USER,
  KC_LAYER_GO_DOWN,
  KC_LOGO,
  KC_SC_UP,
  KC_SC_DOWN
};

static bool logo_rendered = false;

#ifdef OLED_ENABLE
// Starting level for OLED screen brightness
static uint8_t oled_level = 255;
const uint8_t oled_level_step = 15;
#endif

// const char PROGMEM layer_names[2][20] = {
//   "Base Layer",
//   "RGB Layer"
// };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE_L] = LAYOUT(
        KC_HOME, LSFT(KC_HOME), LCTL(KC_C),  LCTL(KC_Z),
        KC_END,  LSFT(KC_END),  LCTL(KC_V),  KC_LOGO
    ),
    [_MEDIA_L] = LAYOUT(
        KC_MRWD,  KC_MUTE, KC_NO,   KC_MFFD,
        KC_MPRV,  KC_MSTP, KC_MPLY, KC_MNXT
    ),
    [_RGB_L] = LAYOUT(
        UG_TOGG, UG_NEXT, KC_NO,   KC_NO,
        KC_NO,   UG_PREV, KC_NO,   KC_NO
    )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE_L] = {
        ENCODER_CCW_CW(MS_WHLD, MS_WHLU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN),
        ENCODER_CCW_CW(KC_LEFT, KC_RIGHT), ENCODER_CCW_CW(KC_UP, KC_DOWN),   ENCODER_CCW_CW(KC_LAYER_GO_DOWN, KC_LAYER_GO_UP)
    },
    [_MEDIA_L] = {
        ENCODER_CCW_CW(UG_VALD, UG_VALU),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN),
        ENCODER_CCW_CW(KC_RIGHT, KC_LEFT), ENCODER_CCW_CW(KC_DOWN, KC_UP),   ENCODER_CCW_CW(KC_LAYER_GO_DOWN, KC_LAYER_GO_UP)
    },
    [_RGB_L] = {
        ENCODER_CCW_CW(UG_HUED, UG_HUEU),  ENCODER_CCW_CW(UG_VALD, UG_VALU), ENCODER_CCW_CW(UG_SATD, UG_SATU),
        ENCODER_CCW_CW(KC_SC_DOWN, KC_SC_UP),  ENCODER_CCW_CW(UG_PREV, UG_NEXT), ENCODER_CCW_CW(KC_LAYER_GO_DOWN, KC_LAYER_GO_UP)
    }
};
#endif

static void render_logo(void) {
    #ifdef OLED_ENABLE
    static const char PROGMEM raw_logo[] = {
        0,  0,  0,  0,  0,128,128,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224, 96,240,240,248,248,248,240,224,224,192,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,128,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,128,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,128,128,  0,  0,  0,  0,192,224,224,240,240,248,  0,192,224,224,240,240,248,  0,  0,  0,  0,128,192, 96, 96, 96, 96, 96,192,128,  0,
        0,  2,  2,  7,  7, 15, 15,255,255,247,255,250,254,  0,  0,  0,  0,  0,  0,224,192,192,128,  0,  0,  0,254,250,255,247,255,239, 31, 15,  7,  7,  2,  3,  1,  3,  7,  7, 15, 15,255,239,255,255,250,254,  0,254,250,255,247,255,255,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,192,255,223,255,255,126,254,  0,254,254,255,255,255,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,254,254,  0,255,255,247,255,251,255,  0,255,255,247,255,251,255,  0,  0,  0,  0,143,207,102,102, 54,182,246,119, 55,  0, 
        0,  0,  0,  0,  0,  0,  0,255,255,251,255,253,255,  0,128,192,192,224,224,255,239,255,255,191,255,  0,255,255,255,255,255,255,240,224,192,192,128,128,  0,128,192,192,224,224,255,239,255,255,191,255,  0,255,255,255,255,255,255,240,248,248,252,252,254,126,127, 63, 27, 31, 23,255,247,255,255,253,255,  0,255,253,255,251,255,255,240,248,248,252,252,254,126,254,252,248,248,240,255,239,255,255,191,255,  0,255,255,223,255,191,255,  0,255,255,223,255,191,255,  0,  0,  0,  0,193,227, 54, 54,236,205, 15, 14, 12,  0, 
        0,  0,  0,  0,  0,  0,  0,  7, 15, 13, 31, 30, 63, 63, 63, 31, 15, 15,  7,  7,  3,  3,  1,  0,  0,  0,  0,  0,  1,  1,  3,  3,  7, 15, 13, 31, 30, 63, 63, 63, 31, 15, 15,  7,  7,  3,  3,  1,  0,  0,  0,  0,  0,  1,  1,  3,  3,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  7,  3,  3,  1,  0,  0,  0,  0,  0,  1,  1,  3,  3,  7,  3,  1,  1,  0,  0,  0,  0,  1,  1,  3,  3,  7,  3,  3,  1,  0,  0,  0,  7, 15, 13, 31, 30, 63,  0,  7, 15, 13, 31, 30, 63,  0,  0,  0,  0,  3,  3,  3,  3,  3,  3,  3,  3,  3,  0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
    logo_rendered = true;
    #endif
}

static void clear_logo(void) {
    #ifdef OLED_ENABLE
    if (logo_rendered == true) {
        oled_clear();
        logo_rendered = false;
    }
    #endif
}

void process_layer_cycle(bool is_up) {
    uint8_t current_layer = get_highest_layer(layer_state);

    // Check if we are within the range, if not quit
    if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return;
    }

    uint8_t next_layer = 0;

    if (is_up == true) {
        next_layer = current_layer + 1;
    } else {
        next_layer = current_layer - 1;
    }

    if (next_layer > LAYER_CYCLE_END || next_layer < LAYER_CYCLE_START) {
        return;
    }
    layer_move(next_layer);
}

void process_screen_brightness_cycle(bool is_up) {
    uint8_t new_level = oled_level;
    if (is_up == true) {
        if (255 - oled_level < oled_level_step) {
            new_level = 255;
        } else {
            new_level += oled_level_step;
        }
    } else {
        if (oled_level < oled_level_step) {
            new_level = 0;
        } else {
            new_level -= oled_level_step;
        }
    }
    oled_set_brightness(new_level);
    oled_level = new_level;

    // Display brightness level on screen
    char buf[24];
    // %u to insert uint var, -3 to add left padding to compensate between 1 char int and 3 chars int
    snprintf(buf, sizeof(buf), "Brightness: %-3u    ", oled_level);
    oled_set_cursor(0, 0);
    oled_write(buf, false);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    switch (keycode) {
        case KC_LAYER_GO_UP:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }
            process_layer_cycle(true);
            return false;
    
        case KC_LAYER_GO_DOWN:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }
            process_layer_cycle(false);
            return false;

        case KC_LOGO:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }

            if (logo_rendered == true) {
                clear_logo();
            } else {
                render_logo();
            }
            return false;

        case KC_SC_UP:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }
            process_screen_brightness_cycle(true);
            return false;

        case KC_SC_DOWN:
            // Our logic will happen on presses, nothing is done on releases
            if (!record->event.pressed) {
                // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
                return false;
            }
            process_screen_brightness_cycle(false);
            return false;

        // Process other keycodes normally
        default:
            return true;
    }
    return true;
}

// runs every time that the layer get changed
layer_state_t layer_state_set_user(layer_state_t state) {
    #ifdef OLED_ENABLE
    clear_logo();
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(state)) {
        case _BASE_L:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _MEDIA_L:
            oled_write_P(PSTR("Media\n"), false);
            break;
        case _RGB_L:
            oled_write_P(PSTR("RGB & Light\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }
    #endif

    return state;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // Set the brightness on init
    oled_set_brightness(oled_level);

    return rotation;
}

bool oled_task_user(void) {
    // time in ms since the keyboard booted
    if (timer_elapsed32(0) < 3000) {
        render_logo();
        return false; // false = skip the keyboard-level OLED drawing
    }
    return true;
}
#endif