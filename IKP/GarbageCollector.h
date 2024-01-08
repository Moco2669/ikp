#pragma once
#include "Heap.h"
#include "PointerToNodeMap.h"

typedef struct GarbageCollector {
	Heap* heap;
	PointerNode_t* mapPointer;
} GC;

GC* InitializeGC(Heap* heap, PointerNode_t* map);

void Mark(GC* collector, int sizeOfArray, PointerNode_t** array, void* pointer);

void Sweep(GC* collector);