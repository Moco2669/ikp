#pragma once
#include <stdbool.h>
#define HEAPSIZE 1024

typedef struct HeapNode {
	void* data;
	bool marked;
	HeapNode_t* next;
} HeapNode_t;

typedef struct Heap {
	HeapNode_t* firstNode;
} Heap;

void initializeHeap(Heap* heap);

void addNodeToHeap(Heap* heap, void* dataPtr);

void* allocate(Heap* heap, size_t size);