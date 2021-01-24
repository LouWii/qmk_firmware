/* Copyright 2019 LouWii
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include <print.h>
#    include <LUFA/Drivers/USB/USB.h>
#    include "midi.h"
#    include "qmk_midi.h"

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BL  0
#define _UP  1
#define _BKL 2
#define _LGTRM 3
#define _LGTRM2 4
#define _LSEL  31 // Make sure this layer is the very last one

const int layers_count = 5;

const char PROGMEM layer_names[5][20] = {
  "Base Layer",
  "Up Layer",
  "Backlight Layer",
  "Lightroom 1",
  "Lightroom 2"
};

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL,
  MIDI_SEND_CLOCK,
  MIDI_SEND_TICK,
  MIDI_SEND_START,
  MIDI_SEND_CONTINUE,
  MIDI_SEND_STOP,
  MIDI_SEND_ACTIVESENSE
};

static uint8_t layer_pre_select;
static uint16_t rotary_click_timer;
// MidiDevice midi_device;

int16_t lt_contrast = 100;

#define LUP_TG TG(_UP)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BL] = LAYOUT( /* Base */
    KC_A, KC_B, KC_C, KC_D,
    KC_1, KC_2, KC_3, KC_4
  ),
  [_UP] = LAYOUT(
    KC_1, KC_2, KC_3, KC_4,
    KC_A, KC_B, KC_C, KC_D
  ),
  [_BKL] = LAYOUT(
    RGB_TOG,        RGB_MOD,  KC_NO, KC_NO,
    RGB_MODE_PLAIN, RGB_RMOD, KC_NO, KC_NO
  ),
  [_LSEL] = LAYOUT(
    TO(_BL), TO(_UP), TO(_BKL), TO(_BL),
    TO(_BL), TO(_UP), TO(_BKL), TO(_BL)
  ),
  [_LGTRM] = LAYOUT(
    MI_C, MI_Cs, MI_D,  MI_Ds,
    MI_E, MI_F,  MI_Fs, MI_G
  ),
  [_LGTRM2] = LAYOUT(
    MI_C, MI_Cs, MI_D,  MI_Ds,
    MI_E, MI_F,  MI_Fs, MI_G
  )
};

void matrix_init_user(void) {
  // Rotary encoder 1 switch (used for switching layers)
  setPinInputHigh(D5);
  // Set pre-select layer, used when user wants to change layer
  layer_pre_select = biton32(layer_state);
  //print("matrix init user done");
}

void cc_callback(MidiDevice * device, uint8_t chan, uint8_t num, uint8_t val) {
  //sending it back on the next channel
  //midi_send_cc(device, (chan + 1) % 16, num, val);
  dprintf("CC callback received on channel %d, num %d, value %d\n", chan, num, val);
}

void catch_all_callback(MidiDevice * device, uint16_t count, uint8_t byte0, uint8_t byte1, uint8_t byte2) {
  //dprint("Catch all callback midi\n");
}

void pitchbend_callback(MidiDevice * device, uint8_t chan, uint8_t num, uint8_t val) {
  dprintf("Pitchbend callback channel %d, num %d, value %d\n", chan, num, val);
}

void keyboard_post_init_user(void) {
  // Debug setup
  debug_enable = true;
  //debug_matrix = true;
  debug_keyboard = true;
  //debug_mouse=true;

  // Midi device init
  // This currently triggers an infinite loop switch 1 trigger
  // midi_device_init(&midi_device);
//   midi_device_set_send_func(&midi_device, usb_send_func);
//     midi_device_set_pre_input_process_func(&midi_device, usb_get_midi);
//     midi_register_fallthrough_callback(&midi_device, fallthrough_callback);
//     midi_register_cc_callback(&midi_device, cc_callback);

  midi_register_cc_callback(&midi_device, cc_callback);

  // midi_register_catchall_callback(&midi_device, catch_all_callback);

  midi_register_pitchbend_callback(&midi_device, pitchbend_callback);

  //print("keyboard post init user done");
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QMKBEST:
      if (record->event.pressed) {
        // when keycode QMKBEST is pressed
        SEND_STRING("QMK is the best thing ever!");
      } else {
        // when keycode QMKBEST is released
      }
      break;
    case QMKURL:
      if (record->event.pressed) {
        // when keycode QMKURL is pressed
        SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
      } else {
        // when keycode QMKURL is released
      }
      break;
    // case MIDI_SEND_START:
    //   if (record->event.pressed) {
    //     midi_send_start(midi_device);
    //   }
    //   break;
    // case MIDI_SEND_STOP:
    //   if (record->event.pressed) {
    //     midi_send_stop(midi_device);
    //   }
    //   break;
    // case MIDI_SEND_CONTINUE:
    //   if (record->event.pressed) {
    //     midi_send_continue(midi_device);
    //   }
    //   break;
    // case MIDI_SEND_ACTIVESENSE:
    //   if (record->event.pressed) {
    //     midi_send_activesense(midi_device);
    //   }
    //   break;

  }
  return true;
}

void display_layer_pre_select(void) {
  // oled_set_cursor(0, 1);
  // oled_write_P(PSTR("> "), false);
  // oled_write_ln_P(layer_names[layer_pre_select], false);
  dprintf("Display layer pre select %d\n", layer_pre_select);
}

void encoder_update_user(uint8_t index, bool clockwise) {
  dprintf("Encoder %d changed\n", index);
  if (biton32(layer_state) == _LSEL) {
    if (clockwise) {
      // Pre-select layer++
      // oled_write_P(PSTR("Layer++\n"), false);
      layer_pre_select = (layer_pre_select + 1) % layers_count;
    } else {
      // Pre-select layer--
      // oled_write_P(PSTR("Layer--\n"), false);
      if (layer_pre_select == 0) {
        layer_pre_select = (layers_count - 1);
      } else {
        layer_pre_select--;
      }
    }
    display_layer_pre_select();
  } else {
    if (index == 0) { /* First encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 0, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 0, 0x14, 126);
        }
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 6, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 6, 0x14, 126);
        }
      } else if (biton32(layer_state) == _BL) {
        if (clockwise) {
          tap_code(KC_PGDN);
        } else {
          tap_code(KC_PGUP);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_increase_val();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_decrease_val();
        }
      }
    } else if (index == 1) { /* Second encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 1, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 1, 0x14, 126);
        }
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 7, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 7, 0x14, 126);
        }
      } else if (biton32(layer_state) == _BL) {
        if (clockwise) {
          tap_code(KC_AUDIO_VOL_DOWN);
        } else {
          tap_code(KC_AUDIO_VOL_UP);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_increase_hue();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_decrease_hue();
        }
      }
    } else if (index == 2) { /* Third encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 2, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 2, 0x14, 126);
        }
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 8, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 8, 0x14, 126);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_increase_sat();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_decrease_sat();
        }
      }
    } else if (index == 3) { /* Fourth encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 3, 0x14, 2);
          if (lt_contrast < 201) {
            lt_contrast += 1;
          }
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 3, 0x14, 126);
          if (lt_contrast > -1) {
            lt_contrast -= 1;
          }
        }
        //midi_send_pitchbend(&midi_device, 0, lt_contrast);
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 9, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 9, 0x14, 126);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_increase_val();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_decrease_val();
        }
      }
    } else if (index == 4) { /* Fifth encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 4, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 4, 0x14, 126);
        }
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 10, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 10, 0x14, 126);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_increase_val();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_decrease_val();
        }
      }
    } else if (index == 5) { /* Sixth encoder */
      if (biton32(layer_state) == _LGTRM) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 5, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 5, 0x14, 126);
        }
      } else if (biton32(layer_state) == _LGTRM2) {
        if (clockwise) {
          // midi_send_noteon(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 11, 0x14, 2);
        } else {
          // midi_send_noteoff(midi_device, 1, 1, 1);
          midi_send_cc(&midi_device, 11, 0x14, 126);
        }
      } else {
        if (clockwise) {
          // tap_code(KC_PGDN);
          // tap_code(RGB_VAI);
          // rgblight_step();
        } else {
          // tap_code(KC_PGUP);
          // tap_code(RGB_VAD);
          // rgblight_step_reverse();
        }
      }
    }
  }
}

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
  // Host Keyboard Layer Status
  // oled_write_P(PSTR("Layer: "), false);
  // switch (get_highest_layer(layer_state)) {
  //   case _BL:
  //     oled_write_P(PSTR("Default\n"), false);
  //     break;
  //   case _UP:
  //     oled_write_P(PSTR("FN\n"), false);
  //     break;
  //   default:
  //     // Or use the write_ln shortcut over adding '\n' to the end of your string
  //     oled_write_ln_P(PSTR("Undefined"), false);
  // }

  // Host Keyboard LED Status
  // uint8_t led_usb_state = host_keyboard_leds();
  // oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
  // oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
  // oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}
#endif

void matrix_scan_user(void) {
  if (!readPin(D5)) {

    // "Debounce"
    if (rotary_click_timer == 0 || timer_elapsed(rotary_click_timer) > 500) {
      if (biton32(layer_state) != _LSEL) {
        layer_clear();
        layer_on(_LSEL);
      } else {
        layer_clear();
        layer_on(layer_pre_select);
      }

      rotary_click_timer = timer_read();
    }

    // SEND_STRING("UP");
    // layer_clear();
    // layer_on(_LSEL);
    // Or use set_single_persistent_default_layer instead?
  }

  // #ifdef RGBLIGHT_ENABLE

  // static uint8_t old_layer = 255;
  // uint8_t new_layer = biton32(layer_state);

  // if (old_layer != new_layer) {
  //   switch (new_layer) {
  //     case _PS:
  //       // #31C5F0
  //       rgblight_setrgb(49, 197, 240);
  //     break;
  //     case _AI:
  //       // #FF8011
  //       rgblight_setrgb(255, 128, 17);
  //     break;
  //     case _PR:
  //       // #E788FF
  //       rgblight_setrgb(231, 136, 255);
  //     break;
  //     case _XD:
  //       // #FF2BC2
  //       rgblight_setrgb(255, 43, 194);
  //     break;
  //   }
  //   old_layer = new_layer;
  // }
  // #endif
}

void led_set_user(uint8_t usb_led) {

}

layer_state_t layer_state_set_user(layer_state_t state) {
  // runs every time that the layer get changed
dprintf("Layer changed to %d\n", get_highest_layer(state));
  // oled_clear();
  // switch (get_highest_layer(state)) {
  //   case _LSEL:
  //     // rgblight_setrgb (0x00,  0x00, 0xFF);
  //     oled_write_ln_P(PSTR("Select Layer:"), false);
  //     display_layer_pre_select();
  //     break;
  //   default: //  for any other layers, or the default layer
  //     // rgblight_setrgb (0x00,  0xFF, 0xFF);
  //     oled_write_P(layer_names[get_highest_layer(state)], false);
  //     break;
  // }
  return state;
}
