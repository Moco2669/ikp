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

void Mark(GC* collector, int sizeOfArray, PointerNode_t** array) {
	while (collector->heap->firstNode->next != NULL) {
		if(collector->heap->firstNode->data)
	}
	
}