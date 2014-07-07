// descriptorTables.c -- Brad Slayter

#include "common.h"
#include "descriptorTables.h"
#include "monitor.h"

extern void gdtFlush(u32int);
extern void idtFlush(u32int);

static void initIdt();
static void idtSetGate(u8int, u32int, u16int, u8int);

static void initGdt();
static void gdtSetGate(s32int, u32int, u32int, u8int, u8int);

gdtEntry_t  gdtEntries[5];
gdtPtr_t	gdtPtr;
idtEntry_t	idtEntries[256];
idtPtr_t	idtPtr;

void initDescriptorTables() {
	monitorWrite("initGDT\n");
	initGdt();
	monitorWrite("initIdt\n");
	initIdt();
}

static void initGdt() {
	gdtPtr.limit = (sizeof(gdtEntry_t) * 5) - 1;
	gdtPtr.base  = (u32int)&gdtEntries;

	gdtSetGate(0, 0, 0, 0, 0);
	gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdtSetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdtSetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	gdtFlush((u32int)&gdtPtr);
}

static void gdtSetGate(s32int num, u32int base, u32int limit, u8int access, u8int gran) {
	gdtEntries[num].baseLow		= (base & 0xFFFF);
	gdtEntries[num].baseMiddle  = (base >> 16) & 0xFF;
	gdtEntries[num].baseHigh	= (base >> 24) & 0xFF;

	gdtEntries[num].limitLow	= (limit & 0xFFFF);
	gdtEntries[num].granularity = (limit >> 16) & 0x0F;

	gdtEntries[num].granularity |= gran & 0xF0;
	gdtEntries[num].access		= access;
}

static void initIdt() {
	idtPtr.limit = sizeof(idtEntry_t) * 256 - 1;
	idtPtr.base  = (u32int)&idtEntries;

	memset(&idtEntries, 0, sizeof(idtEntry_t)*256);

	idtSetGate(0, (u32int)isr0, 0x08, 0x8E);
	idtSetGate(1, (u32int)isr1, 0x08, 0x8E);
	idtSetGate(2, (u32int)isr2, 0x08, 0x8E);
	idtSetGate(3, (u32int)isr3, 0x08, 0x8E);
	idtSetGate(4, (u32int)isr4, 0x08, 0x8E);
	idtSetGate(5, (u32int)isr5, 0x08, 0x8E);
	idtSetGate(6, (u32int)isr6, 0x08, 0x8E);
	idtSetGate(7, (u32int)isr7, 0x08, 0x8E);
	idtSetGate(8, (u32int)isr8, 0x08, 0x8E);
	idtSetGate(9, (u32int)isr9, 0x08, 0x8E);
	idtSetGate(10, (u32int)isr10, 0x08, 0x8E);
	idtSetGate(11, (u32int)isr11, 0x08, 0x8E);
	idtSetGate(12, (u32int)isr12, 0x08, 0x8E);
	idtSetGate(13, (u32int)isr13, 0x08, 0x8E);
	idtSetGate(14, (u32int)isr14, 0x08, 0x8E);
	idtSetGate(15, (u32int)isr15, 0x08, 0x8E);
	idtSetGate(16, (u32int)isr16, 0x08, 0x8E);
	idtSetGate(17, (u32int)isr17, 0x08, 0x8E);
	idtSetGate(18, (u32int)isr18, 0x08, 0x8E);
	idtSetGate(19, (u32int)isr19, 0x08, 0x8E);
	idtSetGate(20, (u32int)isr20, 0x08, 0x8E);
	idtSetGate(21, (u32int)isr21, 0x08, 0x8E);
	idtSetGate(22, (u32int)isr22, 0x08, 0x8E);
	idtSetGate(23, (u32int)isr23, 0x08, 0x8E);
	idtSetGate(24, (u32int)isr24, 0x08, 0x8E);
	idtSetGate(25, (u32int)isr25, 0x08, 0x8E);
	idtSetGate(26, (u32int)isr26, 0x08, 0x8E);
	idtSetGate(27, (u32int)isr27, 0x08, 0x8E);
	idtSetGate(28, (u32int)isr28, 0x08, 0x8E);
	idtSetGate(29, (u32int)isr29, 0x08, 0x8E);
	idtSetGate(30, (u32int)isr30, 0x08, 0x8E);
	idtSetGate(31, (u32int)isr31, 0x08, 0x8E);

	idtFlush((u32int)&idtPtr);
}

static void idtSetGate(u8int num, u32int base, u16int sel, u8int flags) {
	idtEntries[num].baseLo = base & 0xFFFF;
	idtEntries[num].baseHi = (base >> 16) & 0xFFFF;

	idtEntries[num].sel	   = sel;
	idtEntries[num].always0 = 0;

	idtEntries[num].flags  = flags /* | 0x60*/;
}