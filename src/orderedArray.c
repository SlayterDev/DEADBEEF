// orderedArray.c -- Brad Slayter

#include "orderedArray.h"
#include "kheap.h"

s8int standardLessThanPredicate(type_t a, type_t b) {
	return (a < b) ? 1 : 0;
}

orderedArray_t createOrderedArray(u32int maxSize, lessThanPredicate_t lessThan) {
	orderedArray_t toRet;
	toRet.array = (void *)kmalloc(maxSize * sizeof(type_t));
	memset(toRet.array, 0, maxSize*sizeof(type_t));
	toRet.size = 0;
	toRet.maxSize = maxSize;
	toRet.lessThan = lessThan;
	return toRet;
}

orderedArray_t placeOrderedArray(void *addr, u32int maxSize, lessThanPredicate_t lessThan) {
	orderedArray_t toRet;
	toRet.array = (type_t *)addr;
	memset(toRet.array, 0, maxSize*sizeof(type_t));
	toRet.size = 0;
	toRet.maxSize = maxSize;
	toRet.lessThan = lessThan;
	return toRet;
}

void destroyOrderedArray(orderedArray_t *array) {
	//kfree(array->array);
}

void insertOrderedArray(type_t item, orderedArray_t *array) {
	ASSERT(array->lessThan);
	u32int iterator = 0;
	while (iterator < array->size && array->lessThan(array->array[iterator], item)) {
		iterator++;
	}

	if (iterator == array->size) {
		array->array[array->size++] = item;
	} else {
		type_t tmp = array->array[iterator];
		array->array[iterator] = item;
		while (iterator < array->size) {
			iterator++;
			type_t tmp2 = array->array[iterator];
			array->array[iterator] = tmp;
			tmp = tmp2;
		}
		array->size++;
	}
}

type_t lookupOrderedArray(u32int i, orderedArray_t *array) {
	ASSERT(i < array->size);
	return array->array[i];
}

void removeOrderedArray(u32int i, orderedArray_t *array) {
	while (i < array->size) {
		array->array[i] = array->array[i+1];
		i++;
	}
	array->size--;
}