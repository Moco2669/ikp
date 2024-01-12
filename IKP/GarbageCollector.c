#include "GarbageCollector.h"

GC* InitializeGC() {
	GC* gc = (GC*)malloc(sizeof(GC)); //inicijalizacija GC-ja
	if (gc == NULL) {
		return NULL;
	}

	PointerNode_t* hashMap[HASHMAPSIZE] = initializeMap(HASHMAPSIZE); //inicijalizacija mape
	if (hashMap == NULL) {
		return NULL;
	}
	for (int i = 0; i < HASHMAPSIZE; ++i) {
		hashMap[i] = NULL;
	}

	Heap* heap = initializeHeap(); //inicijalizacija hipa
	if (heap == NULL) {
		return NULL;
	}

	gc->mapSize = HASHMAPSIZE;
	gc->heap = heap;
	gc->mapPointer = hashMap;

	return gc;
}

void* GCMalloc(GC* gc, size_t size) {
	void* onoStoTrebaDaSeAlocira = malloc(size);
	if (onoStoTrebaDaSeAlocira == NULL) {
		return NULL;
	}

	HeapNode_t* newNode = addNodeToHeap(gc, size, onoStoTrebaDaSeAlocira);
	//ovde bismo pozvali markandsweep mozda ako je povratna vrednost null
	if (hashPointer(HASHMAPSIZE, gc->mapPointer, onoStoTrebaDaSeAlocira, newNode)) {
		return onoStoTrebaDaSeAlocira;
	}

	return NULL;
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

