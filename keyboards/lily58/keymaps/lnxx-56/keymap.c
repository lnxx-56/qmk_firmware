#include <stdint.h>
#include QMK_KEYBOARD_H

// for keyboard-level data sync:
#define SPLIT_TRANSACTION_IDS_KB KEYBOARD_SYNC_A, KEYBOARD_SYNC_B

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

uint8_t volume_level = 0;
uint8_t cpu_usage = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_GRV, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_MINS, KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC, KC_RBRC, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LALT, KC_LGUI, MO(1), KC_SPC, KC_ENT, MO(2), KC_BSPC, KC_RGUI),
	[1] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_VOLD, KC_MUTE, KC_MPLY, KC_TRNS, KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_GRV, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_TILD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(3), KC_DEL, KC_TRNS),
	[2] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_GRV, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_INS, KC_HOME, KC_UP, KC_END, KC_NO, KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_NO, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_TRNS, KC_PLUS, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_TRNS, KC_TRNS, MO(3), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),
	[3] = LAYOUT(KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

void render_usage(void) {
    // Convert the CPU usage to a string and display it
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "CPU: %d%%", cpu_usage);
    oled_write(buffer, false);

    // Calculate the starting Y value based on the CPU usage
    uint8_t y_start = 128 - (uint8_t)(cpu_usage * 0.98);  // Scaling factor for display height (128)
    for (uint8_t y = y_start; y < 128; y++) {
        for (uint8_t x = 0; x < 32; x++) {
            oled_write_pixel(x, y, true);
        }
    }
    oled_render();  // Render the updated buffer to the screen
}

void render_volume(void) {
    // Convert the Volume to a string and display it
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "VOL: %d%%", volume_level);
    oled_write(buffer, false);

    // Calculate the starting Y value based on the CPU usage
    uint8_t y_start = 128 - (uint8_t)(volume_level * 0.98);  // Scaling factor for display height (128)
    for (uint8_t y = y_start; y < 128; y++) {
        for (uint8_t x = 0; x < 32; x++) {
            oled_write_pixel(x, y, true);
        }
    }
    oled_render();  // Render the updated buffer to the screen
}

// This runs every time there is any oled action
bool oled_task_user(void) {
  if (!is_keyboard_master()) {
    render_volume();
    // oled_write(read_logo(), false);
  }
  return false;
}

// Time for some RAW HID
// The first data should be the cpu usage value, the second on should be the volume level: 34&65 -> cpu: 34% & vol: 65%
void raw_hid_receive(uint8_t *data, uint8_t length) {
    // size_t length = sizeof(data) / sizeof(data[0]);
    bool is_cpu_buffered = false;

    // make the global values 0
    cpu_usage = 0;
    volume_level = 0;

    for (uint8_t i = 0; i < length; i++) {
        if (data[i] == '&') {
            is_cpu_buffered = true;
            continue;
        }

        if (!is_cpu_buffered) {
            cpu_usage += data[i];
        }
        else {
            volume_level += data[i];
        }
    }

    oled_clear();
    render_usage();
}
#endif // OLED_ENABLE
