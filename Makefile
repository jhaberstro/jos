CC		= clang
CFLAGS	= -Wall -Wextra -pedantic -m32 -O0 -std=c99 -finline-functions -fno-stack-protector -nostdinc -ffreestanding -Wno-unused-function -Wno-unused-parameter -g -ccc-host-triple i586-linux-elf
LD		= cross/bin/i586-elf-ld

OBJFILES = src/kernel/loader.o src/kernel/kmain.o src/kernel/io.o src/kernel/console.o src/kernel/gdt.o src/kernel/gdt_flush.o

all: kernel.img

qemu: kernel.bin
	qemu -kernel kernel.bin

.s.o:
	nasm -f elf -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

kernel.bin: $(OBJFILES)
	$(LD) -T src/kernel/linker.ld -o $@ $^

kernel.img: kernel.bin
	dd if=/dev/zero of=src/pad bs=1 count=750
	cat grub-0.97-i386-pc/boot/grub/stage1 grub-0.97-i386-pc/boot/grub/stage2 src/pad $< > $@
	$(RM) src/pad

clean:
	$(RM) $(OBJFILES) kernel.bin kernel.img

install:
	$(RM) $(OBJFILES) kernel.bin
