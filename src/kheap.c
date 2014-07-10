// kheap.h -- Brad Slayter

#include "kheap.h"
#include "paging.h"

// end is defined in the linker script.
extern u32int end;
u32int placementAddress = (u32int)&end;
extern pageDirectory_t *kernelDirectory;
heap_t *kheap = 0;

u32int kmalloc(u32int sz) {
    return kmalloc_int(sz, 0, 0);
}

u32int kmalloc_int(u32int sz, int align, u32int *phys)
{
    // This will eventually call malloc() on the kernel heap.
    // For now, though, we just assign memory at placement_address
    // and increment it by sz. Even when we've coded our kernel
    // heap, this will be useful for use before the heap is initialised.
    if (kheap != 0) {
    	void *addr = alloc(sz, (u8int)align, kheap);
    	if (phys != 0) {
    		page_t *page = getPage((u32int)addr, 0, kernelDirectory);
    		*phys = page->frame * 0x1000 + (u32int)addr&0xFFF;
    	}
    	return (u32int)addr;
    } else {
    	if (align == 1 && (placementAddress & 0xFFFFF000)) {
    		placementAddress &= 0xFFFFF000;
    		placementAddress += 0x1000;
    	}

    	if (phys) {
    		*phys = placementAddress;
    	}
    	u32int tmp = placementAddress;
    	placementAddress += sz;
    	return tmp;
    }
}

u32int kmallocAp(u32int sz, u32int *phys)
{
    return kmalloc_int(sz, 1, phys);
}

u32int kmallocA(u32int sz)
{
    return kmalloc_int(sz, 1, 0);
}

void kfree(void *p) {
	free(p, kheap);
}

static s32int findSmallestHole(u32int size, u8int pageAlign, heap_t *heap) {
	u32int i = 0;
	while (i < heap->index.size) {
		header_t *header = (header_t *)lookupOrderedArray(i, &heap->index);

		if (pageAlign > 0) {
			// page align the starting point of the header
			u32int location = (u32int)header;
			s32int offset = 0;
			if ((location + sizeof(header_t)) & 0xFFFFF000 != 0)
				offset = 0x1000 - (location + sizeof(header_t)) % 0x1000;

			s32int holeSize = (s32int)header->size - offset;
			// can we fit now?
			if (holeSize >= (s32int)size)
				break;
		} else if (header->size >= size) {
			break;
		}

		i++;
	}

	if (i == heap->index.size)
		return -1;
	else
		return i;
}

static s8int header_t_lessThan(void *a, void *b) {
	return (((header_t *)a)->size < ((header_t *)b)->size) ? 1 : 0;
}

heap_t *createHeap(u32int start, u32int endAddr, u32int max, u8int supervisor, u8int readOnly) {
	heap_t *heap = (heap_t *)kmalloc(sizeof(heap_t));

	// make sure things are alligned
	ASSERT(start%0x1000 == 0);
	ASSERT(endAddr%0x1000 == 0);

	heap->index = placeOrderedArray((void *)start, HEAP_INDEX_SIZE, &header_t_lessThan);

	start += sizeof(type_t)*HEAP_INDEX_SIZE;

	if (start & 0xFFFFF000 != 0) {
		start &= 0xFFFFF000;
		start += 0x1000;
	}

	heap->startAddress = start;
	heap->endAddress = endAddr;
	heap->maxAddress = max;
	heap->supervisor = supervisor;
	heap->readOnly = readOnly;

	header_t *hole = (header_t *)start;
	hole->size = endAddr - start;
	hole->magic = HEAP_MAGIC;
	hole->isHole = 1;
	insertOrderedArray((void *)hole, &heap->index);

	return heap;
}

static void expand(u32int newSize, heap_t *heap) {
	// sanity check
	ASSERT(newSize > heap->endAddress - heap->startAddress);

	if (newSize & 0xFFFFF000 != 0) {
		newSize &= 0xFFFFF000;
		newSize += 0x1000;
	}

	ASSERT(heap->startAddress + newSize <= heap->maxAddress);

	u32int oldSize = heap->endAddress - heap->startAddress;
	u32int i = oldSize;
	while (i < newSize) {
		allocFrame(getPage(heap->startAddress + i, 1, kernelDirectory), (heap->supervisor) ? 1 : 0, (heap->readOnly) ? 0 : 1);
		i += 0x1000;
	}
	heap->endAddress;
}

static u32int contract(u32int newSize, heap_t *heap) {
	// sanity check
	ASSERT(newSize < heap->endAddress - heap->startAddress);

	if (newSize & 0x1000) {
		newSize &= 0x1000;
		newSize += 0x1000;
	}

	if (newSize < HEAP_MIN_SIZE)
		newSize = HEAP_MIN_SIZE;
	u32int oldSize = heap->endAddress - heap->startAddress;
	u32int i = oldSize - 0x1000;
	while (newSize < i) {
		freeFrame(getPage(heap->startAddress + i, 0, kernelDirectory));
		i -= 0x1000;
	}
	heap->endAddress = heap->startAddress + newSize;
	return newSize;
}

void *alloc(u32int size, u8int pageAlign, heap_t *heap) {
	u32int newSize = size + sizeof(header_t) + sizeof(footer_t);
	s32int i = findSmallestHole(newSize, pageAlign, heap);

	if (i == -1) { // No suitable hole
		u32int oldLength = heap->endAddress - heap->startAddress;
		u32int oldEndAddress = heap->endAddress;

		expand(oldLength + newSize, heap);
		u32int newLength = heap->endAddress - heap->startAddress;

		i = 0;

		u32int idx = -1; u32int value = 0x0;
		while (i < heap->index.size) {
			u32int tmp = (u32int)lookupOrderedArray(i, &heap->index);
			if (tmp > value) {
				value = tmp;
				idx = i;
			}
			i++;
		}

		if (idx == -1) {
			header_t *header = (header_t *)oldEndAddress;
			header->magic = HEAP_MAGIC;
			header->size = newLength - oldLength;
			header->isHole = 1;
			footer_t *footer = (footer_t *)(oldEndAddress + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
			insertOrderedArray((void *)header, &heap->index);
		} else {
			header_t *header = lookupOrderedArray(idx, &heap->index);
			header->size += newLength - oldLength;
			// rewrite footer
			footer_t *footer = (footer_t *)((u32int)header + header->size - sizeof(footer_t));
			footer->header = header;
			footer->magic = HEAP_MAGIC;
		}

		return alloc(size, pageAlign, heap);
	}

	header_t *origHoleHeader = (header_t *)lookupOrderedArray(i, &heap->index);
	u32int origHolePos = (u32int)origHoleHeader;
	u32int origHoleSize = origHoleHeader->size;

	if (origHoleSize - newSize < sizeof(header_t) + sizeof(footer_t)) {
		size += origHoleSize - newSize;
		newSize = origHoleSize;
	}

	if (pageAlign && origHolePos & 0xFFFFF000) {
		u32int newLocation = origHolePos + 0x1000 - (origHolePos & 0xFFF) - sizeof(header_t);
		header_t *holeHeader = (header_t *)origHolePos;
		holeHeader->size = 0x1000 - (origHolePos & 0xFFF) - sizeof(header_t);
		holeHeader->magic = HEAP_MAGIC;
		holeHeader->isHole = 1;
		footer_t *holeFooter = (footer_t *)((u32int)newLocation - sizeof(footer_t));
		holeFooter->magic = HEAP_MAGIC;
		holeFooter->header = holeHeader;
		origHolePos = newLocation;
		origHoleSize = origHoleSize - holeHeader->size;
	} else {
		removeOrderedArray(i, &heap->index);
	}

	header_t *blockHeader = (header_t *)origHolePos;
	blockHeader->magic = HEAP_MAGIC;
	blockHeader->isHole = 0;
	blockHeader->size = newSize;
	footer_t *blockFooter = (footer_t *)(origHolePos + sizeof(header_t) + size);
	blockFooter->magic = HEAP_MAGIC;
	blockFooter->header = blockHeader;

	if (origHoleSize - newSize > 0) {
		header_t *holeHeader = (header_t *)(origHolePos + sizeof(header_t) + size + sizeof(footer_t));
		holeHeader->magic = HEAP_MAGIC;
		holeHeader->isHole = 1;
		holeHeader->size = origHoleSize - newSize;
		footer_t *holeFooter = (footer_t *)((u32int)holeHeader + origHoleSize - newSize - sizeof(footer_t));
		if ((u32int)holeFooter < heap->endAddress) {
			holeFooter->magic = HEAP_MAGIC;
			holeFooter->header = holeHeader;
		}

		insertOrderedArray((void *)holeHeader, &heap->index);
	}

	return (void *)((u32int)blockHeader + sizeof(header_t));
}

void free(void *p, heap_t *heap) {
	if (p == 0)
		return;

	header_t *header = (header_t *)((u32int)p - sizeof(header_t));
	footer_t *footer = (footer_t *)((u32int)header + header->size - sizeof(footer_t));

	// sanity check
	ASSERT(header->magic == HEAP_MAGIC);
	ASSERT(footer->magic == HEAP_MAGIC);

	header->isHole = 1;

	char doAdd = 1;

	// unify left
	footer_t *testFooter = (footer_t *)((u32int)header - sizeof(footer_t));
	if (testFooter->magic == HEAP_MAGIC && testFooter->header->isHole == 1) {
		u32int cacheSize = header->size;
		header = testFooter->header;
		header->size += cacheSize;
		doAdd = 0;
	}

	// unify right
	header_t *testHeader = (header_t *)((u32int)footer + sizeof(header_t));
	if (testHeader->magic == HEAP_MAGIC && testHeader->isHole == 1) {
		header->size += testHeader->size;
		testFooter = (footer_t *)((u32int)testHeader + testHeader->size - sizeof(footer_t));

		footer = testFooter;
		u32int i = 0;
		while ((i < heap->index.size) && (lookupOrderedArray(i, &heap->index) != (void *)testHeader))
			i++;

		ASSERT(i < heap->index.size);

		removeOrderedArray(i, &heap->index);
	}

	// if the footer is at the end, we can contract
	if ((u32int)footer + sizeof(footer_t) == heap->endAddress) {
		u32int oldLength = heap->endAddress - heap->startAddress;
		u32int newLength = contract((u32int)header - heap->startAddress, heap);

		if (header->size - (oldLength - newLength) > 0) {
			// we still exist
			header->size -= oldLength - newLength;
			footer = (footer_t *)((u32int)header + header->size - sizeof(footer_t));
			footer->magic = HEAP_MAGIC;
			footer->header = header;
		} else {
			u32int i = 0;
			while ((i < heap->index.size) && (lookupOrderedArray(i, &heap->index) != (void *)testHeader)) {
				i++;
			}

			if (i < heap->index.size)
				removeOrderedArray(i, &heap->index);
		}
	}

	if (doAdd == 1)
		insertOrderedArray((void *)header, &heap->index);
}