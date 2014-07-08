// timer.c -- Brad Slayter

#include "timer.h"
#include "isr.h"
#include "monitor.h"

u32int tick = 0;

static void timerCallback(registers_t regs) {
	tick++;
	monitorWrite("Tick: ");
	monitorWriteDec(tick);
	monitorWrite("\n");
}

void initTimer(u32int frequency) {
	registerInteruptHandler(IRQ0, &timerCallback);

	u32int divisor = 1193180 / frequency;

	outb(0x43, 0x36);

	u8int l = (u8int)(divisor & 0xFF);
	u8int h = (u8int)((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}