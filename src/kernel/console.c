#include "console.h"
#include "io.h"
#include "vargs.h"

#define COMPOSE_COLORS(f, b) (((b & 0xf) << 4) | (f & 0xf))
#define FRAMEBUFFER_OFFSET(x, y) (((_CHAR_WIDTH * y) + x) * 2)

enum {
    _CHAR_WIDTH = 80,
    _CHAR_HEIGHT = 25
};

static char* const framebuffer = (char*)0xB8000;
static console_color_t foreground = CONSOLE_COLOR_WHITE;
static console_color_t background = CONSOLE_COLOR_BLACK;
static size_t cursor_x = 0, cursor_y = 0;

static void _console_write_integer(int x, int base) {
    static char* characterLookup = "0123456789abcdef";
    
    int negative = 0;
    if (x < 0) {
        x = -x;
        negative = 1;
    }

    int length = 0;
    int temp = x;
    do {
        ++length;
    } while ((temp /= base));

    char buffer[length + 1];
    for (int i = length - 1; i >= 0; --i) {
        buffer[i] = characterLookup[x % base];
        x /= base;
    }
    buffer[length] = '\0';

    if (negative) {
        console_write_char('-');
    }

    console_write_str(buffer);
}

static void _console_set_cursor(int x, int y) {
    cursor_x = x, cursor_y = y;
    uint16_t cursorOffset = (cursor_y * 80) + cursor_x;
    outb(0x3D4, 14);                    // Inform VGA we are setting high cursor byte
    outb(0x3D5, cursorOffset >> 8);     // Set high cursor byte
    outb(0x3D4, 15);                    // Inform VGA we are setting low cursor byte
    outb(0x3D5, cursorOffset);          // Set low cursor byte;
}

void console_clear() {
    for (int i = 0; i < _CHAR_WIDTH * _CHAR_HEIGHT * 2; i += 2) {
        framebuffer[i] = ' ';
        framebuffer[i+1] = COMPOSE_COLORS(foreground, background); 
    }
}

void console_reset() {
    foreground = CONSOLE_COLOR_WHITE;
    background = CONSOLE_COLOR_BLACK;
    console_clear();
    _console_set_cursor(0, 0);
}

void console_set_foreground(console_color_t fg) {
    foreground = fg;
}

void console_set_background(console_color_t bg) {
    background = bg;
}

void console_write_char(char c) {
    size_t index = ((cursor_y * _CHAR_WIDTH) + cursor_x) * 2;

    if (c == '\n') {
        cursor_y += 1;
        cursor_x = 0;
    }
    else if (c == '\r') {
        cursor_x = 0;
    }
    else if (c == '\t') {
        cursor_x += 4;
    }
    else if (c == '\b') {
        if (cursor_x == 0) {
            if (cursor_y > 0) {
                cursor_y -= 1;
                cursor_x = _CHAR_WIDTH - 1;
            }
        }
        else {
            --cursor_x;
        }
    }
    else {
        framebuffer[index] = c;
        framebuffer[index + 1] = COMPOSE_COLORS(foreground, background);
        cursor_x += 1;
    }

    if (cursor_x >= _CHAR_WIDTH) {
        cursor_y += 1;
        cursor_x = 0;
    }

    // Scrolling
    if (cursor_y >= _CHAR_HEIGHT) {
        // Copy each line up one row
        for (size_t y = 1; y < _CHAR_HEIGHT; ++y) {
            for (size_t x = 0; x < _CHAR_WIDTH; ++x) {
                size_t index = ((y * _CHAR_WIDTH) + x) * 2;
                size_t previous = (((y - 1) * _CHAR_WIDTH) + x) * 2;
                framebuffer[previous] = framebuffer[index];
                framebuffer[previous + 1] = framebuffer[index + 1];
            }
        }

        // Clear the last row
        for (size_t x = 0, y = _CHAR_HEIGHT - 1; x < _CHAR_WIDTH; ++x) {
            size_t index = ((y * _CHAR_WIDTH) + x) * 2;
            framebuffer[index] = ' ';
            framebuffer[index + 1] = COMPOSE_COLORS(foreground, background);
        }

        cursor_y = _CHAR_HEIGHT - 1;
    }

    _console_set_cursor(cursor_x, cursor_y);
}

void console_write_str(char* str) {
    while (*str != '\0') {
        console_write_char(*str++);
    }
}

void console_printf(char* fmt, ...) {
    int vlSize = 0;
    char* fmtCopy = fmt;
    while (*fmtCopy != '\0') {
        if (*fmtCopy == '%') {
            char next = *(fmtCopy + 1);
            if (next == 'i' || next == 'x') {
                ++vlSize;
            }
        }
        ++fmtCopy;
    }

    va_list vl;
    va_start(vl, vlSize);
    while (*fmt != '\0') {
        char curr = *fmt;
        if (curr == '%') {
            char next = *(fmt + 1);
            if (next == 'i') {
                int i = va_arg(vl, int);
                _console_write_integer(i, 10);
                ++fmt;
            }
            else if (next == 'x') {
                int x = va_arg(vl, int);
                _console_write_integer(x, 0x10);
                ++fmt;
            }
            else {
                // False alarm
                console_write_char('%');
            }
        }
        else {
            console_write_char(curr);
        }

        ++fmt;
    }
    va_end(vl);
}

