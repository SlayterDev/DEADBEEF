// kheap.h -- Brad Slayter

#include "kheap.h"

// end is defined in the linker script.
extern u32int end;
u32int placementAddress = (u32int)&end;

u32int kmalloc(u32int sz) {
    return kmalloc_int(sz, 0, 0);
}

u32int kmalloc_int(u32int sz, int align, u32int *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (align == 1 && (placementAddress & 0xFFFFF000) )
    {
        // Align the placement address;
        placementAddress &= 0xFFFFF000;
        placementAddress += 0x1000;
    }
    if (phys)
    {
        *phys = placementAddress;
    }
    u32int tmp = placementAddress;
    placementAddress += sz;
    return tmp;
}

u32int kmallocAp(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

u32int kmallocA(u32int sz)
{
    return kmalloc_int(sz, 1, 0);
}