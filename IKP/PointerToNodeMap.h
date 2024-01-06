#pragma once
#include "Heap.h"
#include <stdlib.h>
#include <stdbool.h>

#ifndef POINTERTONODEMAP_H
#define POINTERTONODEMAP_H

typedef struct PointerToNodeEntry {
	void* data;
	HeapNode_t* node;
	struct PointerNode_t* next;
} PointerNode_t;

PointerNode_t* createNode(void* pointer, HeapNode_t* node);

bool hashPointer(int sizeOfArray, PointerNode_t** array, void* pointer, HeapNode_t* node);

PointerNode_t* getNodeFromPointer(int sizeOfArray, PointerNode_t** array, void* pointer);

#endif // !POINTERTONODEMAP_H