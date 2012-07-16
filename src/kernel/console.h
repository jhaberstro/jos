#ifndef KERNEL_CONSOLE_H
#define KERNEL_CONSOLE_H

#include "types.h"

enum {
    CONSOLE_COLOR_BLACK           = 0x0,
    CONSOLE_COLOR_BLUE            = 0x1,
    CONSOLE_COLOR_GREEN           = 0x2,
    CONSOLE_COLOR_CYAN            = 0x3,
    CONSOLE_COLOR_RED             = 0x4,
    CONSOLE_COLOR_MAGENTA         = 0x5,
    CONSOLE_COLOR_BROWN           = 0x6,
    CONSOLE_COLOR_WHITE           = 0x7,
    CONSOLE_COLOR_GRAY            = 0x8,
    CONSOLE_COLOR_LIGHTBLUE       = 0x9,
    CONSOLE_COLOR_LIGHTGREEN      = 0xA,
    CONSOLE_COLOR_LIGHTCYAN       = 0xB,
    CONSOLE_COLOR_LIGHTRED        = 0xC,
    CONSOLE_COLOR_LIGHTMAGENTA    = 0xD,
    CONSOLE_COLOR_YELLOW          = 0xE,
    CONSOLE_COLOR_BRIGHTWHITE     = 0xF
};
typedef uint32_t console_color_t;

void console_clear();

void console_reset();

void console_set_foreground(console_color_t foreground);

void console_set_background(console_color_t background);

void console_write_char(char c);

void console_write_str(char* str);

void console_printf(char* fmt, ...);

#endif // KERNEL_CONSOLE_H
