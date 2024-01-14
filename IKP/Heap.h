#pragma once
#include "Structs.h"
#include "GarbageCollector.h"

#ifndef HEAP_H
#define HEAP_H

Heap* initializeHeap();

HeapNode_t* addNodeToHeap(Heap* heap, size_t size, void* dataPtr);

void dealloc(GC* gc, HeapNode_t* node);

//void removeNodeFromHeap(void* ptr);

#endif