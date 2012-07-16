global loader                               ; makes entry point visible to linker

extern kmain                                ; defined in kmain.cpp

; setting up the Multiboot header - see GRUB docs for details
MODULEALIGN equ 1<<0                        ; align loaded modules on page boundaries
MEMINFO     equ 1<<1                        ; provide memory map
FLAGS       equ MODULEALIGN | MEMINFO       ; the multiboot flag field
MAGIC       equ 0x1BADB002                  ; magic number to help bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)

section .text

; the multiboot header
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

STACKSIZE equ 0x4000                        ; 16k initial kernel stack space

loader:
    mov esp, stack + STACKSIZE              ; set up the stack

    push ebx                                ; multiboot info structure pointer passed as second parameter to kmain
    push eax                                ; multiboot magic number passed as first parameter to kmain
    call kmain

    cli
 .hang:
    hlt                                     ; halt machine if kernel returns
    jmp .hang

section .bss

align 4
stack: resb STACKSIZE                       ; reserver 16k stack on a doubleword boundary
