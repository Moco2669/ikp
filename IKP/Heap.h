#pragma once

#include "Structs.h"
#include "GarbageCollector.h"



void initializeHeap();

void* addNodeToHeap(size_t size, void* dataPtr);

void dealloc(HeapNode_t* node);

void removeNodeFromHeap(void* ptr);