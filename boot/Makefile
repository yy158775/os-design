CC := gcc
CFLAGS := -fno-builtin -MD -static -fno-pie -Wall -m32 -std=gnu99 -gstabs

LD	:= ld
LDFLAGS := -m elf_i386

boot.o: boot.S
	$(CC) -nostdinc $(KERN_CFLAGS) -c -o $@ $< -I..

main.o: main.c
	$(CC) -nostdinc $(KERN_CFLAGS) -Os -c -o $@ $< -I..

boot: boot.o main.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7c00 -o $@.out $^