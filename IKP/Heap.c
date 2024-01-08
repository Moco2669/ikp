#include <stdlib.h>
#include "Heap.h"

void initializeHeap(Heap* heap) {
	heap->lastNode = NULL;
	heap->size = HEAPSIZE;
}

void* addNodeToHeap(Heap* heap, size_t size, void* dataPtr) {
	//simulirani heap koji ima velicinu u bajtima
	int temp = heap->size - size;
	if (temp < 0) {
		//ovde treba da se pozove GC
		return NULL;
	}

	HeapNode_t* node = (HeapNode_t*)malloc(sizeof(HeapNode_t));
	void* data = (void*)malloc(sizeof(size));
	
	//cuva pokazivace i na referencu i na heap
	node->data = data;
	node->pointer = dataPtr;
	node->marked = false;
	node->prev = heap->lastNode;
	node->size = size;

	if (size <= 10240) {
		node->gen = 1;
		heap->size -= size;
	}
	else {
		node->gen = 0;
	}
	
	heap->lastNode = node;

	//vraca pokazivac na heap koji moze da se kastuje u potrebni tip podataka kao kod pravog malloca
	return data;
}

void dealloc(Heap* heap, HeapNode_t* node) {
	heap->size += node->size;
	free(node->data);
}

void removeNodeFromHeap(Heap* heap, void* ptr) {
	HeapNode_t* hn = heap->lastNode, * temp = NULL;

	while (hn != NULL) {
		if (hn->pointer == ptr) {
			if (temp == NULL) {
				heap->lastNode = hn->prev;
			}
			else {
				temp->prev = hn->prev;
			}

			dealloc(heap, hn);
			break;
		}

		temp = hn;
		hn = hn->prev;
	}

	free(hn);
}