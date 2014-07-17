// kheap.h -- Brad Slayter
// A basic memory sub-system for out OS

#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"
#include "orderedArray.h"

u32int kmalloc_int(u32int sz, int align, u32int *phys);
u32int kmallocAp(u32int sz, u32int *phys);
u32int kmalloc(u32int sz);
u32int kmallocA(u32int sz);

#define KHEAP_START 	   0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define HEAP_INDEX_SIZE	   0x20000
#define HEAP_MAGIC		   0xDEADBEEF
#define HEAP_MIN_SIZE	   0x70000

typedef struct {
	u32int magic;
	u8int isHole;
	u32int size;
} header_t;

typedef struct {
	u32int magic;
	header_t *header;
} footer_t;

typedef struct {
	orderedArray_t index;
	u32int startAddress;
	u32int endAddress;
	u32int maxAddress;
	u8int supervisor;
	u8int readOnly;
} heap_t;

heap_t *createHeap(u32int start, u32int end, u32int max, u8int supervisor, u8int readonly);
void *alloc(u32int size, u8int pageAlign, heap_t *heap);
void free(void *p, heap_t *heap);
void kfree(void *p);

#endif