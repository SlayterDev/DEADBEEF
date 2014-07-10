// orderedArray.h -- Brad Slayter

#ifndef ORDERED_ARRAY_H
#define ORDERED_ARRAY_H

#include "common.h"

typedef void* type_t;

typedef s8int (*lessThanPredicate_t)(type_t, type_t);
typedef struct {
	type_t *array;
	u32int size;
	u32int maxSize;
	lessThanPredicate_t lessThan;
} orderedArray_t;

s8int standardLessThanPredicate(type_t a, type_t b);

orderedArray_t createOrderedArray(u32int maxSize, lessThanPredicate_t lessThan);
orderedArray_t placeOrderedArray(void *addr, u32int maxSize, lessThanPredicate_t lessThan);

void destroyOrderedArray(orderedArray_t *array);

void insertOrderedArray(type_t item, orderedArray_t *array);

type_t lookupOrderedArray(u32int i, orderedArray_t *array);

void removeOrderedArray(u32int i, orderedArray_t *array);

#endif