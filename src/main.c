// main.c -- Brad Slayter

#include "common.h"
#include "monitor.h"
#include "descriptorTables.h"
#include "timer.h"
#include "paging.h"
#include "kheap.h"

int main(struct multiboot *mbootPtr) {
	// Init code here
	monitorClear();
	initDescriptorTables();

	u32int a = kmalloc(8);

	initialisePaging();

	u32int b = kmalloc(8);
	u32int c = kmalloc(8);

	monitorWrite("a: ");
	monitorWriteHex(a);
	monitorWrite(", b: ");
	monitorWriteHex(b);
	monitorWrite("\nc: ");
	monitorWriteHex(c);

	kfree(c);
	kfree(b);
	u32int d = kmalloc(12);
	monitorWrite(", d: ");
	monitorWriteHex(d);

	

	return 0xDEADBEEF;
}