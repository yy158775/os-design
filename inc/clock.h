#ifndef PRO_INC_CLOCK_H
#define PRO_INC_CLOCK_H


#define MAX_IRQS	16	// Number of IRQs

// I/O Addresses of the two 8259A programmable interrupt controllers
#define IO_PIC1		0x20	// Master (IRQs 0-7)
#define IO_PIC2		0xA0	// Slave (IRQs 8-15)

#define IRQ_SLAVE	2	// IRQ at which slave connects to master

void pic_init(void);

#endif
