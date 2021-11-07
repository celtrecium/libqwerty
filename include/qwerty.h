/*
 * This file is part of libqwerty.
 *
 * libqwerty is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libqwerty is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libqwerty. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef QWERTY_H
#define QWERTY_H

#include <stdbool.h>
#include <stdlib.h>
#include <u8string.h>

typedef enum qw_keycode
  {
    QW_KEY_UP            = 0x4427d,
    QW_KEY_LEFT          = 0x44280,
    QW_KEY_DOWN          = 0x4427e,
    QW_KEY_RIGHT         = 0x4427f,
    QW_KEY_PGUP          = 0x10a389e6,
    QW_KEY_PGDOWN        = 0x10a38dce,
    QW_KEY_HOME          = 0x10a37a46,
    QW_KEY_END           = 0x10a385fe,
    QW_KEY_INSERT        = 0x10a37e2e,
    QW_KEY_DELETE        = 0x10a38216,
    QW_KEY_F1            = 0x43ddc,
    QW_KEY_F2            = 0x43ddd,
    QW_KEY_F3            = 0x43dde,
    QW_KEY_F4            = 0x43ddf,
    QW_KEY_F5            = 0x7fdc61a6,
    QW_KEY_F6            = 0x7fdc6976,
    QW_KEY_F7            = 0x7fdc6d5e,
    QW_KEY_F8            = 0x7fdc7146,
    QW_KEY_F9            = 0x7fddd4be,
    QW_KEY_F10           = 0x7fddd8a6,
    QW_KEY_F11           = 0x7fdde076,
    QW_KEY_F12           = 0x7fdde45e,
    QW_KEY_UNICODE       = 0x554e4943, /* codl_get_key returns QW_KEY_UNICODE if key is unicode*/ 
    QW_KEY_UC_A          = 0x41, /* Uppercase keys */
    QW_KEY_UC_B          = 0x42,
    QW_KEY_UC_C          = 0x43,
    QW_KEY_UC_D          = 0x44,
    QW_KEY_UC_E          = 0x45,
    QW_KEY_UC_F          = 0x46,
    QW_KEY_UC_G          = 0x47,
    QW_KEY_UC_H          = 0x48,
    QW_KEY_UC_I          = 0x49,
    QW_KEY_UC_J          = 0x4a,
    QW_KEY_UC_K          = 0x4b,
    QW_KEY_UC_L          = 0x4c,
    QW_KEY_UC_M          = 0x4d,
    QW_KEY_UC_N          = 0x4e,
    QW_KEY_UC_O          = 0x4f,
    QW_KEY_UC_P          = 0x50,
    QW_KEY_UC_Q          = 0x51,
    QW_KEY_UC_R          = 0x52,
    QW_KEY_UC_S          = 0x53,
    QW_KEY_UC_T          = 0x54,
    QW_KEY_UC_U          = 0x55,
    QW_KEY_UC_V          = 0x56,
    QW_KEY_UC_W          = 0x57,
    QW_KEY_UC_X          = 0x58,
    QW_KEY_UC_Y          = 0x59,
    QW_KEY_UC_Z          = 0x5a,
    QW_KEY_LC_A          = 0x61, /* Lowercase keys */
    QW_KEY_LC_B          = 0x62,
    QW_KEY_LC_C          = 0x63,
    QW_KEY_LC_D          = 0x64,
    QW_KEY_LC_E          = 0x65,
    QW_KEY_LC_F          = 0x66,
    QW_KEY_LC_G          = 0x67,
    QW_KEY_LC_H          = 0x68,
    QW_KEY_LC_I          = 0x69,
    QW_KEY_LC_J          = 0x6a,
    QW_KEY_LC_K          = 0x6b,
    QW_KEY_LC_L          = 0x6c,
    QW_KEY_LC_M          = 0x6d,
    QW_KEY_LC_N          = 0x6e,
    QW_KEY_LC_O          = 0x6f,
    QW_KEY_LC_P          = 0x70,
    QW_KEY_LC_Q          = 0x71,
    QW_KEY_LC_R          = 0x72,
    QW_KEY_LC_S          = 0x73,
    QW_KEY_LC_T          = 0x74,
    QW_KEY_LC_U          = 0x75,
    QW_KEY_LC_V          = 0x76,
    QW_KEY_LC_W          = 0x77,
    QW_KEY_LC_X          = 0x78,
    QW_KEY_LC_Y          = 0x79,
    QW_KEY_LC_Z          = 0x7a,
    QW_KEY_0             = 0x30,
    QW_KEY_1             = 0x31,
    QW_KEY_2             = 0x32,
    QW_KEY_3             = 0x33,
    QW_KEY_4             = 0x34,
    QW_KEY_5             = 0x35,
    QW_KEY_6             = 0x36,
    QW_KEY_7             = 0x37,
    QW_KEY_8             = 0x38,
    QW_KEY_9             = 0x39,
    QW_KEY_SPACE         = 0x20, /*   */
    QW_KEY_EXCLAM        = 0x21, /* ! */
    QW_KEY_AT            = 0x40, /* @ */
    QW_KEY_QUOTE         = 0x22, /* " */
    QW_KEY_APOSTROPHE    = 0x27, /* ' */
    QW_KEY_SHARP         = 0x23, /* # */
    QW_KEY_DOLLAR        = 0x24, /* $ */
    QW_KEY_SEMICOLON     = 0x3b, /* ; */
    QW_KEY_PERCENT       = 0x25, /* % */
    QW_KEY_CARET         = 0x5e, /* ^ */
    QW_KEY_COLON         = 0x3a, /* : */
    QW_KEY_DOT           = 0x2e, /* . */
    QW_KEY_COMMA         = 0x2c, /* , */
    QW_KEY_PIPE          = 0x7c, /* | */
    QW_KEY_BACKSLASH     = 0x5c, /* \ */
    QW_KEY_SLASH         = 0x2f, /* / */
    QW_KEY_TILDE         = 0x7e, /* ~ */
    QW_KEY_GRAVE_ACCENT  = 0x60, /* ` */
    QW_KEY_L_SQ_BRACKET  = 0x5b, /* [ */
    QW_KEY_R_SQ_BRACKET  = 0x5d, /* ] */
    QW_KEY_L_FIG_BRACKET = 0x7b, /* { */
    QW_KEY_R_FIG_BRACKET = 0x7d, /* } */
    QW_KEY_L_BRACKET     = 0x28, /* ( */
    QW_KEY_R_BRACKET     = 0x29, /* ) */
    QW_KEY_LESS          = 0x3c, /* < */
    QW_KEY_MORE          = 0x3e, /* > */
    QW_KEY_QUESTION_MARK = 0x3f, /* ? */
    QW_KEY_ASTERISK      = 0x2a, /* * */
    QW_KEY_MINUS         = 0x2d, /* - */
    QW_KEY_PLUS          = 0x2b, /* + */
    QW_KEY_EQUAL         = 0x3d, /* = */
    QW_KEY_AMPERSAND     = 0x26, /* & */
    QW_KEY_LOW_LINE      = 0x5f, /* _ */
    QW_KEY_ESC           = 0x1b, /* Escape symbol     \e */
    QW_KEY_TAB           = 0x09, /* Tabulation symbol \t */
    QW_KEY_BACKSPACE     = 0x7f, /* Backspace            */
    QW_KEY_RETURN        = 0x0A  /* Enter, Return,    \n */
} qw_keycode_t;

typedef struct qw_key
{
  qw_keycode_t key;
  u8char_t unicode_char;
} qw_key_t;

void qw_initialize (void);
qw_key_t qw_get_key(void);
void qw_end (void);

#endif  /* QWERTY_H */
