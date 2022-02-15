OBJDIR := obj

OBJDIRS :=
OBJ :=

TOP = .

CFLAGS := -fno-builtin -MD -static -fno-pie -Wall -m32 -std=gnu99 -gstabs

CFLAGS += -I$(TOP)

kernelobj :=
bootobj := 
include boot/Makefile
include kern/Makefile

boot : $(bootobj)
	@echo "make boot"

kernel : $(kernelobj)
	@echo "make kernel"

all : $(OBJ)
	@echo "make boot and kernel"

clean :
	rm $(OBJDIR)/boot/*.o
	rm $(OBJDIR)/boot/boot
	rm $(OBJDIR)/boot/boot.out
	rm $(OBJDIR)/kern/*.o
	rm $(OBJDIR)/kern/kernel
