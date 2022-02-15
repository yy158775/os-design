#include <inc/mmu.h>
#include <inc/user.h>
#include <inc/memlayout.h>

struct Taskstate ts0,ts1;

void
ts_init(void)
{
    ts0.ts_ss0 = GD_KD;
    ts0.ts_esp0 = STACKTOP;
    ts0.ts_iomb = sizeof(struct Taskstate);
}