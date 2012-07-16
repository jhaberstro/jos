#include "multiboot.h"
#include "console.h"
#include "gdt.h"


void kmain(uint32_t magic, struct multiboot_info* info) {
    if (magic != 0x2BADB002) {
        return;
    }

    gdt_init();

    console_set_background(CONSOLE_COLOR_BLUE);
    console_clear();
}
