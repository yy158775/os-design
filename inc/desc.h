#ifndef JOS_INC_DESC_H
#define JOS_INC_DESC_H

static inline void
lidt(void *p)
{
	asm volatile("lidt (%0)" : : "r" (p));
} //中断表

static inline void
lgdt(void *p)
{
	asm volatile("lgdt (%0)" : : "r" (p));
} //全局描述符

static inline void
lldt(uint16_t sel)
{
	asm volatile("lldt %0" : : "r" (sel));
}

static inline void
ltr(uint16_t sel)
{
	asm volatile("ltr %0" : : "r" (sel));
} //TSS的选择子

static inline void
lcr0(uint32_t val)
{
	asm volatile("movl %0,%%cr0" : : "r" (val));
} 

static inline void
lcr3(uint32_t val)
{
	asm volatile("movl %0,%%cr3" : : "r" (val));
}

static inline uint32_t
rcr0(void)
{
	uint32_t val;
	asm volatile("movl %%cr0,%0" : "=r" (val));
	return val;
}

#endif