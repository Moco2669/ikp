#include "GarbageCollector.h"

GC* InitializeGC() {
	GC* gc = (GC*)malloc(sizeof(GC)); //inicijalizacija GC-ja
	if (gc == NULL) {
		return NULL;
	}

	PointerNode_t* hashMap[HASHMAPSIZE];
	hashMap[0] = initializeMap(HASHMAPSIZE); //inicijalizacija mape
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

HANDLE __stdcall GCCreateThread(GC* gc, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	HANDLE newThread = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	if (newThread!=0 && addHandleToList(gc->handleList, newThread)) {
		return newThread;
	}
	if (newThread != 0) {
		CloseHandle(newThread);
	}
	return NULL;
}

void* GCMalloc(GC* gc, size_t size) { //DODAJ POKAZIVAC NA TRED koji alocira da bismo znali cije je sta //ne nego samo listu tredova da bismo ih sve zaustavili, mani me od toj
	void* onoStoTrebaDaSeAlocira = malloc(size);
	if (onoStoTrebaDaSeAlocira == NULL) {
		return NULL;
	}

	HeapNode_t* newNode = addNodeToHeap(gc->heap, size, onoStoTrebaDaSeAlocira);
	if (newNode == NULL) {
		MarkAndSweep(gc, HASHMAPSIZE);
		return NULL; //takodje ovde trebaju free-ovi
	}
	if (hashPointer(HASHMAPSIZE, gc->mapPointer, onoStoTrebaDaSeAlocira, newNode)) {
		return onoStoTrebaDaSeAlocira;
	}

	//nedostaje bezbedno brisanje nodea i hashovanog pointera ako ne uspe da se hashuje

	return NULL;
}

void markAndSweep(GC* gc, unsigned sizeOfArray) {
	stopAllThreads(gc);
	printf("Threadovi stopirani\n");
	HandleNode_t* handleNode = gc->handleList->lastNode;
	while (handleNode->prev != NULL) {
		ScanThreadStack(gc, handleNode->handle);
		handleNode = handleNode->prev;
	}
	printf("Ciscenje\n");
	Sweep(gc);
	resumeAllThreads(gc);
}

void stopAllThreads(GC* gc) {
	HandleNode_t* handleNode = gc->handleList->lastNode;
	while (handleNode->prev != NULL) {
		SuspendThread(handleNode->handle);
		handleNode = handleNode->prev;
	}
}

void resumeAllThreads(GC* gc) {
	HandleNode_t* handleNode = gc->handleList->lastNode;
	while (handleNode->prev != NULL) {
		ResumeThread(handleNode->handle);
		handleNode = handleNode->prev;
	}
}

void Mark(HeapNode_t* heapNode) {
	heapNode->marked = true;
	//markChildren ovde nekako... kako??? NZM NAJACE BRE ALOOOOO
}

void Sweep(GC* gc) {
	HeapNode_t* hn = gc->heap->lastNode, *temp = NULL;

	while (hn != NULL) {
		if (!hn->marked) {
			printf("Dealocirali smo nesto, najace\n");
			dealloc(gc, hn);

			if (temp == NULL) {
				gc->heap->lastNode = hn->prev;
				free(hn);
				hn = gc->heap->lastNode;
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

void ScanThreadStack(GC* gc, HANDLE hThread) {
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(hThread, &context)) {
		for (LPVOID current = (LPVOID)context.Esp; current < (LPVOID)context.Ebp; current = (LPVOID)((char*)current + sizeof(LPVOID))) { //eventualno sizeof(1) u slucaju da je neki char bio pa poremetio redosled u steku
			PointerNode_t* nodeFromPointer = getNodeFromPointer(HASHMAPSIZE, gc->mapPointer, (void*)current);
			if (nodeFromPointer != NULL) {
				Mark(nodeFromPointer->node);
			}
		}
	}
}
