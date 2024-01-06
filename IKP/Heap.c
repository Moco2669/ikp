#include <stdlib.h>
#include "Heap.h"

void initializeHeap(Heap* heap) {
	heap->firstNode = NULL;
	heap->size = HEAPSIZE;
}

void* addNodeToHeap(Heap* heap, size_t size, void* dataPtr) {
	if (heap->size - size < 0) {
		return NULL;
	}

	HeapNode_t* node = (HeapNode_t*)malloc(sizeof(HeapNode_t));
	void* data = (void*)malloc(sizeof(size));
	
	node->data = data;
	node->pointer = dataPtr;
	node->marked = false;
	node->last = heap->firstNode;
	
	heap->size -= size;
	heap->firstNode = node;

	return data;
}