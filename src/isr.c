// isr.c -- Brad Slayter

#include "common.h"
#include "isr.h"
#include "monitor.h"

void isrHandler(registers_t regs) {
	monitorWrite("recieved interrupt: ");
	monitorWriteDec(regs.intNo);
	monitorPut('\n');
}