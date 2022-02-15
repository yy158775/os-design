OBJDIRS += kern

OBJDUMP := objdump

KERN_LDFLAGS := -m elf_i386 -nostdlib -e init -Ttext 0x200000

KERN_OBJFILE := 	$(OBJDIR)/kern/init.o \
				$(OBJDIR)/kern/clock.o \
				$(OBJDIR)/kern/console.o \
				$(OBJDIR)/kern/env.o \
				$(OBJDIR)/kern/trap.o \
				$(OBJDIR)/kern/ts.o \
				$(OBJDIR)/kern/user.o

# How to build kernel object files
$(OBJDIR)/kern/%.o: kern/%.c
	@echo + cc $<
	@mkdir -p $(@D)
	$(CC) -nostdinc $(CFLAGS) -c -o $@ $<

# How to build the kernel itself
$(OBJDIR)/kern/kernel: $(KERN_OBJFILE)
	@echo + ld $@
	$(LD) -o $@ $(KERN_LDFLAGS) $^
	$(OBJDUMP) -S $@ > $@.asm
	dd if=$(OBJDIR)/kern/kernel of=bochs/hd60M.img bs=512 seek=1 count=8000 conv=notrunc

OBJ += $(OBJDIR)/kern/kernel
kernelobj += $(OBJDIR)/kern/kernel
