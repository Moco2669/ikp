#include "GarbageCollector.h"

GC* InitializeGC(Heap* heap, PointerNode_t* map) {
	GC* collector = (GC*)malloc(sizeof(GC));

	if (collector == NULL) {
		return NULL;
	}

	collector->heap = heap;
	collector->mapPointer = map;

	return collector;
}

void Mark(GC* collector, int sizeOfArray, PointerNode_t** array, void* pointer) {
	PointerNode_t* pn = getNodeFromPointer(sizeOfArray, array, pointer);
	HeapNode_t* hn = collector->heap->lastNode;
	
	while (hn != NULL) {
		if (hn->pointer == pn->data) {
			hn->marked = true;
			break;
		}

		hn = hn->prev;
	}
}

void Sweep(GC* collector) {
	HeapNode_t* hn = collector->heap->lastNode, *temp = NULL;

	while (hn != NULL) {
		if (!hn->marked) {
			if (temp == NULL) {
				removeNodeFromHeap(collector->heap, hn);
				collector->heap->lastNode = hn->prev;
				free(hn);
				hn = collector->heap->lastNode;
			}
			else {
				temp->prev = hn->prev;
				free(hn);
				hn = temp->prev;
			}
		}
		else {
			temp = hn;
			hn = hn->prev;
		}
	}
}