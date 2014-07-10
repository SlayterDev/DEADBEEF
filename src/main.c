// main.c -- Brad Slayter

#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"
#include "kb.h"

int main(struct multiboot *mbootPtr) {
	// Init code here
	monitorClear();
	initDescriptorTables();

	asm volatile("sti");

	initialisePaging();
	installKeyboard();

	monitorPut('>');

	

	return 0xDEADBEEF;
}