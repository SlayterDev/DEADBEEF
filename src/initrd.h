// initrd.h -- Brad Slayter

#ifndef INITRD_H
#define INITRD_H

#include "common.h"
#include "fs.h"
#include "kheap.h"

typedef struct {
	u32int nfiles; // number of files in ramdisk
} initrdHeader_t;

typedef struct {
	u8int magic;
	char name[64];
	u32int offset;
	u32int length;
} initrdFileHeader_t;

fsNode_t *initializeInitrd(u32int location);

#endif