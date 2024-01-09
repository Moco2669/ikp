#pragma once
#include "Heap.h"
#include "Structs.h"



PointerNode_t* createNode(void* pointer, HeapNode_t* node);

bool hashPointer(int sizeOfArray, PointerNode_t** array, void* pointer, HeapNode_t* node);

PointerNode_t* getNodeFromPointer(int sizeOfArray, PointerNode_t** array, void* pointer);
