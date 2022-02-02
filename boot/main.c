#include <inc/belf.h>
#include <inc/x86.h>

#define SECTSIZE	512
#define ELFHDR		((Elf32_Ehdr*)0x10000) // scratch space
#define ELF_MAGIC   "\x7FELF"	/* "\x7FELF" in little endian */

void 
bootmain(void)
{	
    Elf32_Phdr *ph, *eph;
	int i;
   
	// read 1st page off disk
	readseg((uint32_t)ELFHDR, SECTSIZE*8, 0);

	// is this a valid ELF?
	for(int i = 0;i < 4;i++) {
        if(ELFHDR->e_ident[i] != ELF_MAGIC[i])
            goto bad;
    }

    // load each program segment (ignores ph flags)
	ph = (Elf32_Phdr *) ((uint8_t *) ELFHDR + ELFHDR->e_phoff);
	eph = ph + ELFHDR->e_phnum;
	for (; ph < eph; ph++) {
		// p_pa is the load address of this segment (as well
		// as the physical address)
		readseg(ph->p_paddr, ph->p_memsz, ph->p_offset);  //物理地址要小心了
		for (i = 0; i < ph->p_memsz - ph->p_filesz; i++) {
			*((char *) ph->p_paddr + ph->p_filesz + i) = 0;
		}
	}

	// call the entry point from the ELF header
	// note: does not return!
	((void (*)(void)) (ELFHDR->e_entry))();

bad:
	outw(0x8A00, 0x8A00);
	outw(0x8A00, 0x8E00);
	while (1)
		/* do nothing */;
}

void
readseg(uint32_t pa, uint32_t count, uint32_t offset)
{
    uint32_t end_pa;

	end_pa = pa + count;

	// round down to sector boundary
	pa &= ~(SECTSIZE - 1);

	// translate from bytes to sectors, and kernel starts at sector 1
	offset = (offset / SECTSIZE) + 1;

	// If this is too slow, we could read lots of sectors at a time.
	// We'd write more to memory than asked, but it doesn't matter --
	// we load in increasing order.
	while (pa < end_pa) {
		// Since we haven't enabled paging yet and we're using
		// an identity segment mapping (see boot.S), we can
		// use physical addresses directly.  This won't be the
		// case once JOS enables the MMU.
		readsect((uint8_t*) pa, offset);
		pa += SECTSIZE;
		offset++;
	}
}

void 
readsect(uint8_t* dst,uint32_t offset)
{
    // wait for disk to be ready
	waitdisk();

	outb(0x1F2, 1);		// count = 1
	outb(0x1F3, offset);
	outb(0x1F4, offset >> 8);
	outb(0x1F5, offset >> 16);
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);	// cmd 0x20 - read sectors

	// wait for disk to be ready
	waitdisk();

	// read a sector
	insl(0x1F0, dst, SECTSIZE/4);
}

void
waitdisk(void)
{
	// wait for disk reaady
	while ((inb(0x1F7) & 0xC0) != 0x40)
		/* do nothing */;
}