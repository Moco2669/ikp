#include <stdlib.h>
#include "Heap.h"

void initializeHeap(Heap* heap) {
	heap->firstNode = NULL;
	heap->size = HEAPSIZE;
}

void* addNodeToHeap(Heap* heap, size_t size, void* dataPtr) {
	//simulirani heap koji ima velicinu u bajtima
	if (heap->size - size < 0) {
		return NULL;
	}

	HeapNode_t* node = (HeapNode_t*)malloc(sizeof(HeapNode_t));
	void* data = (void*)malloc(sizeof(size));
	
	//cuva pokazivace i na referencu i na heap
	node->data = data;
	node->pointer = dataPtr;
	node->marked = false;
	node->last = heap->firstNode;
	
	heap->size -= size;
	heap->firstNode = node;

	//vraca pokazivac na heap koji moze da se kastuje u potrebni tip podataka kao kod pravog malloca
	return data;
}