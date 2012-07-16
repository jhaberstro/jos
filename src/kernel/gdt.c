#include "gdt.h"
#include "types.h"

#define GDT_FLAG(GRANULARITY, OPERAND_SIZE) (((GRANULARITY & 0x1) << 3) | ((OPERAND_SIZE & 0x1) << 2))
#define GDT_ACCESS(PRESENT, DPL, DT, TYPE)  (((PRESENT & 0x1) << 7) | ((DPL & 0x3) << 5) | ((DT & 0x1) << 4) | (TYPE & 0xf))

struct gdt_ptr_t {
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed));
typedef struct gdt_ptr_t gdt_ptr_t;

struct gdt_entry_t {
    uint16_t low_limit;
    uint16_t low_base_addr;
    uint8_t  mid_base_addr;
    uint8_t  access;
    uint8_t  high_limit : 4;
    uint8_t  flags : 4;
    uint8_t  high_base_addr;
} __attribute__ ((packed));
typedef struct gdt_entry_t gdt_entry_t;

gdt_entry_t _gdt_entries[5];
gdt_ptr_t _gdt_ptr;

extern void _gdt_flush();

static void _gdt_set_entry(gdt_entry_t* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->low_limit      = limit & 0xffff;
    entry->low_base_addr  = base & 0xffff;
    entry->mid_base_addr  = (base >> 16) & 0xff;
    entry->access         = access;
    entry->high_limit     = (limit >> 16) & 0xf;
    entry->flags          = flags;
    entry->high_base_addr = (base >> 24) & 0xff;
}

void gdt_init() {
    _gdt_ptr.limit = sizeof(_gdt_entries) - 1;
    _gdt_ptr.base = (uint32_t)&_gdt_entries;

    uint8_t flags  = GDT_FLAG(1, 1);
    uint32_t base   = 0x0;
    uint32_t limit = 0xffffffff;

    // null segment
    _gdt_set_entry(&_gdt_entries[0], 0, 0, 0, 0);

    // code segment
    _gdt_set_entry(&_gdt_entries[1], base, limit, GDT_ACCESS(1, 0, 1, 0xa), flags);

    // data segment
    _gdt_set_entry(&_gdt_entries[2], base, limit, GDT_ACCESS(1, 0, 1, 0x2), flags);

    // user mode code segment
    _gdt_set_entry(&_gdt_entries[3], base, limit, GDT_ACCESS(1, 3, 1, 0xa), flags);

    // user mode data segment
    _gdt_set_entry(&_gdt_entries[4], base, limit, GDT_ACCESS(1, 3, 1, 0x2), flags);

    // let the processor know about the gdt we've setup
    _gdt_flush();
}
