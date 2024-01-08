#pragma once
#include <stdbool.h>
#include <stddef.h>
#define HEAPSIZE 1024

typedef struct HeapNode HeapNode_t;

struct HeapNode {
	void* data;
	void* pointer;
	bool marked;
	HeapNode_t* prev;
	size_t size;
	int gen;
};

typedef struct Heap {
	HeapNode_t* lastNode;
	int size;
} Heap;

void initializeHeap(Heap* heap);

void* addNodeToHeap(Heap* heap, size_t size, void* dataPtr);

void dealloc(Heap* heap, HeapNode_t* node);

void removeNodeFromHeap(Heap* heap, void* ptr);