#pragma once
#include "Heap.h"
#include "Structs.h"
#include <stdbool.h>

#ifndef POINTERTONODEMAP_H
#define POINTERTONODEMAP_H

PointerNode_t* createNode(void* pointer, HeapNode_t* node);

bool hashPointer(unsigned sizeOfArray, PointerNode_t** array, void* pointer, HeapNode_t* node);

PointerNode_t* getNodeFromPointer(unsigned sizeOfArray, PointerNode_t** array, void* pointer);

PointerNode_t** initializeMap(unsigned sizeOfArray);

void DeinitializeMap(PointerNode_t** map, unsigned sizeOfArray);

#endif