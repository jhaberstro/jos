#include "multiboot.h"
#include "console.h"


void kmain(uint32_t magic, struct multiboot_info* info) {
    //extern uint32_t magic;
    //extern void* mbd;

    if (magic != 0x2BADB002) {
        return;
    }

    console_set_background(CONSOLE_COLOR_BLUE);
    console_set_foreground(CONSOLE_COLOR_WHITE);
    console_clear();

}
