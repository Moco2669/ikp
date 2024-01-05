#include <stdlib.h>
#include "Heap.h"

void initializeHeap(Heap* heap) {
	heap->firstNode = NULL;
}

void addNodeToHeap(Heap* heap, void* dataPtr) {
	HeapNode_t* node = (HeapNode_t*)malloc(sizeof(HeapNode_t));
	
	node->data = dataPtr;
	node->marked = false;
	node->next = heap->firstNode;

	heap->firstNode = node;
}

void* allocate(Heap* heap, size_t size) {
	void* data = malloc(size);
	
	if (data == NULL) {
		return NULL;
	}

	addNodeToHeap(heap, data);
	return data;
}