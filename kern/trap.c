#include <inc/mmu.h>
#include <inc/user.h>
#include <inc/desc.h>
#include <inc/trap.h>

/* Interrupt descriptor table.  (Must be built at run time because
 * shifted function addresses can't be represented in relocation records.)
 */
struct Gatedesc idt[256] = { { 0 } };

extern struct Taskstate ts0,ts1;

struct Pseudodesc idt_pd = {
	sizeof(idt) - 1, (uint32_t) idt
};

int now = 0;

extern struct Trapframe tf1,tf2;

void timer_handler();

void 
trap_init(void) 
{
    extern struct Segdesc gdt[];
    

    SETGATE(idt[IRQ_OFFSET + IRQ_TIMER],    0, GD_KT, timer_handler, 0);

	gdt[GD_TSS0 >> 3] = SEG16(STS_T32A,(uint32_t)(&ts0),sizeof(struct Taskstate)-1,0);
    gdt[GD_TSS0 >> 3].sd_s = 0;
    
    // gdt[GD_TSS1 >> 3] = SEG16(STS_T32A,(uint32_t)(&ts1),sizeof(struct Taskstate)-1,0);
    // gdt[GD_TSS1 >> 3].sd_s = 0;

    ltr(GD_TSS0);
    lidt(&idt_pd);
}

void timer_handler()
{
    if(now == 0) {
        now = 1;
        env_pop_tf(&tf2);
    } else {
        now = 0;
        env_pop_tf(&tf1);
    }
}

void env_pop_tf(struct Trapframe *tf)
{
	asm volatile(
		"\tmovl %0,%%esp\n"
		"\tpopal\n"
		"\tpopl %%es\n"
		"\tpopl %%ds\n"
		"\taddl $0x8,%%esp\n" /* skip tf_trapno and tf_errcode */
		"\tiret\n"
		: : "g" (tf) : "memory");
}