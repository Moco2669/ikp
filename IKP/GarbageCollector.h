#pragma once
#include "Heap.h"
#include "PointerToNodeMap.h"
#include "Structs.h"


GC* InitializeGC();

void* GCMalloc(GC* gc, size_t size);

void Mark(int sizeOfArray, PointerNode_t** array, void* pointer);

void Sweep();