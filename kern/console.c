#include <inc/types.h>
#include <inc/x86.h>

static unsigned addr_6845;
static uint16_t *crt_buf;
static uint16_t crt_pos;

#define MONO_BASE	0x3B4
#define MONO_BUF	0xB0000
#define CGA_BASE	0x3D4
#define CGA_BUF		0xB8000

void
cga_init(void)
{
	volatile uint16_t *cp;
	uint16_t was;
	unsigned pos;

	cp = (uint16_t*) (CGA_BUF);
	was = *cp;
	*cp = (uint16_t) 0xA55A;
	if (*cp != 0xA55A) {
		cp = (uint16_t*) (MONO_BUF);
		addr_6845 = MONO_BASE;
	} else {
		*cp = was;
		addr_6845 = CGA_BASE;
	}

	/* Extract cursor location */
    //确定鼠标光标的位置
	outb(addr_6845, 14);
	pos = inb(addr_6845 + 1) << 8;
	outb(addr_6845, 15);
	pos |= inb(addr_6845 + 1);

	crt_buf = (uint16_t*) cp;
    //屏幕输出的缓存地址
	crt_pos = pos;
    //位置指针
}
//主动通过汇编指令与设备进行交互，不是中断

void putstr(char *str) {
    int c = 0;
    
    int i = 0;
    while(str[i] != '\0') {
        c = 0;
        c |= 0x0700;
        c |= str[i];
        crt_buf[i] = c;
        i++;
    }
}