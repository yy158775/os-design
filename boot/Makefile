OBJDIRS += boot

CC := gcc


LD	:= ld
LDFLAGS := -m elf_i386 

OBJCOPY = objcopy
OBJDUMP = objdump

BOOT_OBJS := $(OBJDIR)/boot/boot.o $(OBJDIR)/boot/main.o

$(OBJDIR)/boot/boot.o: boot/boot.S
	@echo + as $<
	@mkdir -p $(@D)
	$(CC) -nostdinc $(CFLAGS) -c -o $@ $< 

$(OBJDIR)/boot/main.o: boot/main.c
	@mkdir -p $(@D)
	$(CC) -nostdinc $(CFLAGS) -Os -c -o $@ $< 

$(OBJDIR)/boot/boot: $(BOOT_OBJS)
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7c00 -o $@.out $^
	$(OBJDUMP) -S $@.out > $@.asm
	$(OBJCOPY) -S -O binary -j .text $@.out $@
	perl boot/sign.pl $(OBJDIR)/boot/boot
	dd if=$(OBJDIR)/boot/boot of=bochs/hd60M.img bs=512 count=1 conv=notrunc

OBJ += $(OBJDIR)/boot/boot
bootobj += $(OBJDIR)/boot/boot
