# louwiipad

Louwii-Pad is a custom-built macropad with 8 keys, 6 encoders, RGB underglow and an OLED screen.

* Keyboard Maintainer: [Louwii](https://github.com/Louwii)
* Hardware Supported: Louwii-Pad PCB (based on ATmega32u4)
* Hardware Availability: not available for purchase but the hardware is [open source](https://github.com/LouWii/louwii-pad)

Make example for this keyboard (after setting up your build environment):

    make louwiipad:default

Or

    qmk compile -kb louwiipad -km DEFAULT

Flashing example for this keyboard:

    make louwiipad:default:flash

When making changes to any of the keyboard files, run the linter to make sure everything's clean:

    qmk lint -kb louwiipad

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Double press the button marked *sw1*
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available

## Features & Docs

Various things I used while writing the firmware for Louwii Pad.

* [Keycodes](https://docs.qmk.fm/keycodes)
* [Encoder](https://docs.qmk.fm/features/encoders); [Encoder json config](https://docs.qmk.fm/reference_info_json#encoder)
* [RGBLight docs](https://docs.qmk.fm/features/rgblight); [RGBLight json config](https://docs.qmk.fm/reference_info_json#rgblight) (underglow)
* [QMK Logo Editor](https://joric.github.io/qle/)

## Debugging

Some handy code to help with debugging issues.

Enable console in `rules.mk`.

```
CONSOLE_ENABLE = yes
```

With console enabled, you can use the _HID Console_ in _QMK Toolbox_ to see console logs.

To log anything:

```
print("This is the best macropad ever!");
dprintf("I want to output a number: %d \n", 1337);
```

This code creates custom keycodes to store data and output it later on when requested.

```
enum custom_keycodes {
    STORE_SETUPS = SAFE_RANGE,
    PRINT_SETUPS,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case STORE_SETUPS:
            if (record->event.pressed) {
                store_setups_in_eeprom(); //store data about USB setup packets in EEPROM
            }
            return false;
        case PRINT_SETUPS:
            if (record->event.pressed) {
                print_stored_setups();
            }
            return false;
        default:
            return true; // Process all other keycodes normally
    }
}
```