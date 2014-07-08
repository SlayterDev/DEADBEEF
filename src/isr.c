// isr.c -- Brad Slayter

#include "common.h"
#include "isr.h"
#include "monitor.h"

isr_t interruptHandlers[256];

void registerInteruptHandler(u8int n, isr_t handler) {
	interruptHandlers[n] = handler;
	monitorWrite("Interrupt ");
	monitorWriteDec(n);
	monitorWrite(" Registered\n");
}

void isrHandler(registers_t regs) {
	isr_t handler = interruptHandlers[regs.intNo];
	handler(regs);
	/*monitorWrite("recieved interrupt: ");
	monitorWriteDec(regs.intNo);
	monitorPut('\n');*/
}

void irqHandler(registers_t regs) {
	if (regs.intNo >= 40) {
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);

	if (interruptHandlers[regs.intNo] != 0) {
		isr_t handler = interruptHandlers[regs.intNo];
		handler(regs);
	}
}