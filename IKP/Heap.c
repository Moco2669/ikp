#include <stdlib.h>
#include "Heap.h"

Heap* initializeHeap() {
	Heap* heap = (Heap*)malloc(sizeof(Heap));
	if (heap == NULL) {
		return NULL;
	}

	heap->lastNode = NULL;
	heap->size = HEAPSIZE;

	return heap;
}

HeapNode_t* addNodeToHeap(Heap* heap, size_t size, void* data) {
	//simulirani heap koji ima velicinu u bajtima
	int temp = heap->size - size;
	if (temp < 0) {
		//ovde treba da se pozove GC
		return NULL;
	}

	HeapNode_t* node = (HeapNode_t*)malloc(sizeof(HeapNode_t));
	if (node == NULL) {
		return NULL;
	}
	
	//cuva pokazivace i na referencu i na heap
	//na koji hip brate trenutno smo u hipu alo
	node->data = data;
	//node->pointer = dataPtr;
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
	//komentar iznad je netacan sada vracamo pokazivac na heapNode jer treba da ubacimo u mapu pa ono
	return node;
}

size_t dealloc(GC* gc, HeapNode_t* node) { //vidi da l je druga generacija blabalablabalbal
	gc->heap->size += node->size;
	size_t oslobodjeno = node->size;
	free(node->data);
	return oslobodjeno;
}
/*
void removeNodeFromHeap(void* ptr) {
	HeapNode_t* hn = heap->lastNode, * temp = NULL;

	while (hn != NULL) {
		if (hn->pointer == ptr) {
			if (temp == NULL) {
				heap->lastNode = hn->prev;
			}
			else {
				temp->prev = hn->prev;
			}

			dealloc(hn);
			break;
		}

		temp = hn;
		hn = hn->prev;
	}

	free(hn);
}*/