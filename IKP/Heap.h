#pragma once

#include "Structs.h"
#include "GarbageCollector.h"


Heap* initializeHeap();

HeapNode_t* addNodeToHeap(Heap* heap, size_t size, void* dataPtr);

void dealloc(HeapNode_t* node);

void removeNodeFromHeap(void* ptr);