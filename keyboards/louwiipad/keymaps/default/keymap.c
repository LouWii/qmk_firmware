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

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BL  0
#define _UP  1
#define _BKL 2

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  QMKBEST = SAFE_RANGE,
  QMKURL
};

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
};

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
  }
  return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {

  if (index == 0) { /* First encoder */
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_increase_val();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_decrease_val();
    }
  } else if (index == 1) { /* Second encoder */  
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_increase_hue();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_decrease_hue();
    }
  } else if (index == 2) { /* Third encoder */
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_increase_sat();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_decrease_sat();
    }
  } else if (index == 3) { /* Fourth encoder */
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_increase_val();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_decrease_val();
    }
  } else if (index == 4) { /* Fifth encoder */
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_increase_val();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_decrease_val();
    }
  } else if (index == 5) { /* Sixth encoder */
    if (clockwise) {
      // tap_code(KC_PGDN);
      // tap_code(RGB_VAI);
      rgblight_step();
    } else {
      // tap_code(KC_PGUP);
      // tap_code(RGB_VAD);
      rgblight_step_reverse();
    }
  }
}

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer: "), false);
  switch (get_highest_layer(layer_state)) {
    case _BL:
      oled_write_P(PSTR("Default\n"), false);
      break;
    case _UP:
      oled_write_P(PSTR("FN\n"), false);
      break;
    default:
      // Or use the write_ln shortcut over adding '\n' to the end of your string
      oled_write_ln_P(PSTR("Undefined"), false);
  }

  // Host Keyboard LED Status
  // uint8_t led_usb_state = host_keyboard_leds();
  // oled_write_P(led_usb_state & (1<<USB_LED_NUM_LOCK) ? PSTR("NUMLCK ") : PSTR("       "), false);
  // oled_write_P(led_usb_state & (1<<USB_LED_CAPS_LOCK) ? PSTR("CAPLCK ") : PSTR("       "), false);
  // oled_write_P(led_usb_state & (1<<USB_LED_SCROLL_LOCK) ? PSTR("SCRLCK ") : PSTR("       "), false);
}
#endif

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}