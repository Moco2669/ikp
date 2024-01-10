#include "GarbageCollector.h"

#define HASHMAPSIZE 1024

GC* InitializeGC() {
	collector = (GC*)malloc(sizeof(GC));

	if (collector == NULL) {
		return NULL;
	}

	collector->mapSize = HASHMAPSIZE;
	
	PointerNode_t* hashMap[HASHMAPSIZE] = initializeMap(HASHMAPSIZE);
	if (hashMap == NULL) {
		return NULL;
	}

	for (int i = 0; i < HASHMAPSIZE; ++i) {
		hashMap[i] = NULL;
	}

	collector->heap = heap;
	collector->mapPointer = hashMap;

	return collector;
}

void Mark(int sizeOfArray, PointerNode_t** array, void* pointer) {
	PointerNode_t* pn = getNodeFromPointer(sizeOfArray, array, pointer);
	HeapNode_t* hn = collector->heap->lastNode;
	
	while (hn != NULL) {
		if (hn->gen == 1) {
			if (hn->pointer == pn->data) {
				hn->marked = true;
				break;
			}
		}

		hn = hn->prev;
	}
}

void Sweep() {
	HeapNode_t* hn = collector->heap->lastNode, *temp = NULL;

	while (hn != NULL) {
		if (!hn->marked) {
			dealloc(collector->heap, hn);

			if (temp == NULL) {
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
			hn->gen = 2;
			temp = hn;
			hn = hn->prev;
		}
	}
}

void ScanThreadStack(HANDLE hThread) {
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(hThread, &context)) {
		for (LPVOID current = (LPVOID)context.Esp; current < (LPVOID)context.Ebp; current = (LPVOID)((char*)current + sizeof(LPVOID))) {
			
		}
	}
}

