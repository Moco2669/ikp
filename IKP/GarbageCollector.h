#pragma once
#include "Heap.h"
#include "PointerToNodeMap.h"
#include "Structs.h"


GC* InitializeGC(PointerNode_t* map);

void Mark(int sizeOfArray, PointerNode_t** array, void* pointer);

void Sweep();