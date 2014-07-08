// paging.h -- Brad Slayter

#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct page {
	u32int present : 1;
	u32int rw : 1;
	u32int user : 1;
	u32int accessed : 1;
	u32int dirty : 1;
	u32int unused : 7;
	u32int frame : 20;
} page_t;

typedef struct pageTable {
	page_t pages[1024];
} pageTable_t;

typedef struct pageDirectory {
	/* array of page table ptrs */
	pageTable_t *tables[1024];

	// physical location of ptrs above for CR3
	u32int tablePhysical[1024];

	// physical address of tablePhysical
	u32int physicalAddr;
} pageDirectory_t;

void switchPageDirectory(pageDirectory_t *new);
page_t *getPage(u32int address, int make, pageDirectory_t *dir);
void pageFault(registers_t regs);
void initialisePaging();

#endif