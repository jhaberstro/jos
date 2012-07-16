global _gdt_flush
extern _gdt_ptr;

section .text

_gdt_flush:
    lgdt [_gdt_ptr]
    
    mov ax, 0x10        ; data segment is 3rd gdt_entry which has offset of 16 bytes (0x10)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; implicitly sets the cs register. Use offset of 8 b/c code segment is second gdt_entry, which has offset of 8 bytes

.flush:
    ret
