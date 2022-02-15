#include <inc/clock.h>
#include <inc/trap.h>
#include <inc/console.h>
#include <inc/user.h>
#include <inc/env.h>
#include <inc/ts.h>
#include <inc/mmu.h>
#include <inc/pagedir.h>
#include <inc/desc.h>
#include <inc/memlayout.h>

extern pde_t entry_pgdir[];
struct Trapframe tf1,tf2;

void init() {
    cga_init();

    lcr3(entry_pgdir);
    
    uint32_t cr0;
    cr0 = rcr0();
    // cr0 |= CR0_PE|CR0_PG|CR0_AM|CR0_WP|CR0_NE|CR0_MP;
    //这里不知道有什么问题 call不动当时
    cr0 |= CR0_PG;
    // cr0 &= ~(CR0_TS|CR0_EM);
    lcr0(cr0);
    env_init(); //初始化
    
    ts_init(); //ts 进入中断要用的
    //ts0初始化

    trap_init();
    pic_init(); //初始化中断
    
    // asm("sti;");
    //进入用户模式了    
    // memset(&tf, 0, sizeof(struct Trapframe));
    
    char *p = (char*)&tf1;
    char *q = (char*)&tf2;
    int m = sizeof(struct Trapframe);
    while(m > 0) {
        *p = 0;
        *q = 0;
        p++;q++;
        m--;
    }

    
    tf1.tf_ds = GD_UD | 3;
	tf1.tf_es = GD_UD | 3;
	tf1.tf_ss = GD_UD | 3;
	tf1.tf_esp = STACKTOP;
	tf1.tf_cs = GD_UT | 3;
    tf1.tf_eflags |= (FL_IF | 0x2);
    tf1.tf_eip = pIS19;

    tf2.tf_ds = GD_UD | 3;
    tf2.tf_es = GD_UD | 3;
    tf2.tf_ss = GD_UD | 3;
    tf2.tf_esp = STACKTOP;
    tf2.tf_cs = GD_UT | 3;
    tf2.tf_eflags |= (FL_IF | 0x2);
    tf2.tf_eip = pHUST;

    env_pop_tf(&tf1);
}