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

#include "../include/qwerty.h"
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <u8string.h>

#define UTF8_CODEPOINT_4B 0xF0
#define UTF8_CODEPOINT_3B 0xE0
#define UTF8_CODEPOINT_2B 0xC0

#if defined(__unix__)
struct termios stored_settings;
#elif defined(_WIN32) || defined(__CYGWIN__)
DWORD stored_mode = 0;
#endif

#if defined(__unix__)
int oldf;
struct termios newt, oldt;
#elif defined(_WIN32) || defined(__CYGWIN__)
HANDLE stdin_handle = GetStdHandle (STD_INPUT_HANDLE);
DWORD old_mode = 0;
#endif

static void
_enable_echo_mode (void)
{
#if defined(__unix__)
  tcsetattr (0, TCSANOW, &stored_settings);
#elif defined(_WIN32) || defined(__CYGWIN__)
  SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), stored_mode);
#endif
}

static void
_disable_echo_mode (void)
{
#if defined(__unix__)
  struct termios noecho_settings;

  noecho_settings = stored_settings;
  noecho_settings.c_lflag &= (unsigned int)(~ICANON & ~ECHO);
  noecho_settings.c_cc[VTIME] = 0;
  noecho_settings.c_cc[VMIN] = 1;

  tcsetattr (0, TCSANOW, &noecho_settings);
#elif defined(_WIN32) || defined(__CYGWIN__)
  SetConsoleMode (GetStdHandle (STD_INPUT_HANDLE),
                  (DWORD)(~ENABLE_LINE_INPUT));
#endif
}

void
qw_initialize (void)
{
#if defined(_WIN32) || defined(__CYGWIN__)
  DWORD mode = 0;
#endif

#if defined(__unix__)
  tcgetattr (0, &stored_settings);
#elif defined(_WIN32) || defined(__CYGWIN__)
  HANDLE stdin_handle = GetStdHandle (STD_INPUT_HANDLE);
  DWORD old_mode = 0;

  SetConsoleOutputCP (CP_UTF8);

  SetConsoleCP (CP_UTF8);

  GetConsoleMode (GetStdHandle (STD_OUTPUT_HANDLE), &mode);

  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  mode |= DISABLE_NEWLINE_AUTO_RETURN;

  SetConsoleMode (GetStdHandle (STD_OUTPUT_HANDLE), mode);
  GetConsoleMode (GetStdHandle (STD_INPUT_HANDLE), &stored_mode);
#endif

#if defined(__unix__)
  tcgetattr (0, &oldt);
  newt = oldt;
  newt.c_lflag &= (unsigned int)(~ICANON & ~ECHO);
  tcsetattr (0, TCSANOW, &newt);
  oldf = fcntl (0, F_SETFL, 0);
  fcntl (0, F_SETFL, oldf | O_NONBLOCK);
#elif defined(_WIN32) || defined(__CYGWIN__)
  GetConsoleMode (stdin_handle, &old_mode);

  SetConsoleMode (stdin_handle,
                  old_mode
                      & (uint64_t) ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#endif

  _disable_echo_mode ();
}

static int
__read_char (void)
{
#if defined(__unix__)
  return getchar ();
#elif defined(_WIN32) || defined(__CYGWIN__)
  HANDLE stdin_handle = GetStdHandle (STD_INPUT_HANDLE);
  INPUT_RECORD input_rec;
  DWORD unread_records = 0;
  DWORD unread_events = 0;
  KEY_EVENT_RECORD *key_event;
  BOOL success;
  int key = EOF;

  memset (&input_rec, 0, sizeof (INPUT_RECORD));

  while (TRUE)
    {
      success = GetNumberOfConsoleInputEvents (stdin_handle, &unread_events);

      if (!success || !unread_events)
        break;

      success
          = ReadConsoleInput (stdin_handle, &input_rec, 1, &unread_records);

      if (!success || !unread_records)
        break;

      key_event = &input_rec.Event.KeyEvent;

      if (input_rec.EventType != KEY_EVENT || !key_event->bKeyDown)
        continue;

      key = key_event->uChar.AsciiChar;

      /* For QW_KEY_RETURN. */
      if (key == '\r')
        key = '\n';

      break;
    }

  return key;
#endif
}

static qw_key_t
_get_utf8_key (int key)
{
  size_t count = 0;
  qw_key_t retkey;

  memset (retkey.unicode_char, 0, U8CHAR_LEN);

  for (; key != EOF && count < U8CHAR_LEN; ++count)
    {
      retkey.unicode_char[count] = (char)key;
      key = __read_char ();
    }

  retkey.key = QW_KEY_UNICODE;

  return retkey;
}

static qw_key_t
_get_key (int key)
{
  unsigned int tmp_key = 0;
  qw_key_t retkey;
  
  while (key != EOF)
    {
      tmp_key = (tmp_key * (key > 100 ? 1000 : 100)) + (unsigned int)key;
      key = __read_char ();
    }

  key = (int)tmp_key;

  retkey.key = (qw_keycode_t)key;

  return retkey;
}

qw_key_t
qw_get_key (void)
{
  qw_key_t retkey = { 0, { 0 } };
  int key = 0;

  key = __read_char ();

  if (key == EOF)
    return retkey;

  if ((UTF8_CODEPOINT_4B & key) == UTF8_CODEPOINT_4B
      || (UTF8_CODEPOINT_3B & key) == UTF8_CODEPOINT_3B
      || (UTF8_CODEPOINT_2B & key) == UTF8_CODEPOINT_2B)
    return _get_utf8_key (key);

  return _get_key (key);
}

void
qw_end (void)
{
#if defined(__unix__)
  tcsetattr (0, TCSANOW, &oldt);
  fcntl (0, F_SETFL, oldf);
#elif defined(_WIN32) || defined(__CYGWIN__)
  SetConsoleMode (stdin_handle, old_mode);
#endif

  _enable_echo_mode ();
}
