// kheap.h -- Brad Slayter

#ifndef KHEAP_H
#define KHEAP_H

#include "common.h"

u32int kmalloc_int(u32int sz, int align, u32int *phys);
u32int kmallocAp(u32int sz, u32int *phys);
u32int kmalloc(u32int sz);
u32int kmallocA(u32int sz);

#endif