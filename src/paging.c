// paging.c -- Brad Slayter

#include "paging.h"
#include "kheap.h"
#include "monitor.h"

// The current page directory;
pageDirectory_t *kernelDirectory = 0;
pageDirectory_t *currentDirectory = 0;

u32int *frames;
u32int nFrames;

extern u32int placementAddress;

extern heap_t *kheap;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void setFrame(u32int frameAddr) {
	u32int frame = frameAddr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clearFrame(u32int frameAddr) {
	u32int frame = frameAddr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

static u32int testFrame(u32int frameAddr) {
	u32int frame = frameAddr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

static u32int firstFrame() {
	u32int i, j;
	for (i = 0; i < INDEX_FROM_BIT(nFrames); i++) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; j++) {
				u32int toTest = 0x1 << j;
				if (!(frames[i]&toTest)) {
					return i * 4 * 8 + j;
				}
			}
		}
	}
}

void allocFrame(page_t *page, int isKernel, int isWriteable) {
	if (page->frame != 0) {
		return;
	} else {
		u32int idx = firstFrame();
		if (idx == (u32int)-1) {
			PANIC("No Free Frames!");
		}
		setFrame(idx*0x1000);
		page->present = 1;
		page->rw = (isWriteable) ? 1 : 0;
		page->user = (isKernel) ? 0 : 1;
		page->frame = idx;
	}
}

void freeFrame(page_t *page) {
	u32int frame;
	if (!(frame = page->frame)) {
		return;
	} else {
		clearFrame(frame);
		page->frame = 0x0;
	}
}

void initialisePaging() {
	//kernelDirectory = 0;

	u32int memEndPage = 0x10000000;

	nFrames = memEndPage / 0x1000;
	frames = (u32int *)kmalloc(INDEX_FROM_BIT(nFrames));
	memset(frames, 0, INDEX_FROM_BIT(nFrames));

	kernelDirectory = (pageDirectory_t *)kmallocA(sizeof(pageDirectory_t));
	memset(kernelDirectory, 0, sizeof(pageDirectory_t));
	currentDirectory = kernelDirectory;

	int i = 0;
	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000) {
		getPage(i, 1, kernelDirectory);
	}

	i = 0;
	while (i < placementAddress + 0x1000) {
		allocFrame(getPage(i, 1, kernelDirectory), 0, 0);
		i += 0x1000;
	}

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
		allocFrame(getPage(i, 1, kernelDirectory), 0, 0);

	registerInteruptHandler(14, pageFault);

	switchPageDirectory(kernelDirectory);

	kheap = createHeap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switchPageDirectory(pageDirectory_t *dir) {
	currentDirectory = dir;
	asm volatile("mov %0, %%cr3" :: "r"(&dir->tablePhysical));
	u32int cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000; // enable paging!
	asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

page_t *getPage(u32int address, int make, pageDirectory_t *dir) {
	address /= 0x1000;

	u32int tableIdx = address / 1024;
	if (dir->tables[tableIdx]) {
		return &dir->tables[tableIdx]->pages[address%1024];
	} else if (make) {
		u32int tmp;
		dir->tables[tableIdx] = (pageTable_t *)kmallocAp(sizeof(pageTable_t), &tmp);
		memset(dir->tables[tableIdx], 0, 0x1000);
		dir->tablePhysical[tableIdx] = tmp | 0x7;
		return &dir->tables[tableIdx]->pages[address%1024];
	} else {
		return 0;
	}
}

void pageFault(registers_t regs) {
	u32int faultingAddress;
	asm volatile("mov %%cr2, %0" : "=r"(faultingAddress));

	int present = !(regs.errCode & 0x1);
	int rw = regs.errCode & 0x2;
	int us = regs.errCode & 0x4;
	int reserved = regs.errCode & 0x8;
	int id = regs.errCode & 0x10;

	monitorWrite("Page Fault! ( ");
	if (present) { monitorWrite("present "); }
	if (rw) { monitorWrite("readOnly "); }
	if (us) { monitorWrite("userMode"); }
	if (reserved) { monitorWrite("reserved "); }
	monitorWrite(") at ");
	monitorWriteHex(faultingAddress);
	monitorWrite("\n");
	PANIC("Page fault");
}