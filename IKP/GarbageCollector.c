#include "GarbageCollector.h"

GC* InitializeGC() {
	GC* gc = (GC*)malloc(sizeof(GC));
	if (gc == NULL) {
		return NULL;
	}

	PointerNode_t* hashMap[HASHMAPSIZE];
	hashMap[0] = initializeMap(HASHMAPSIZE);
	if (hashMap == NULL) {
		return NULL;
	}
	for (int i = 0; i < HASHMAPSIZE; ++i) {
		hashMap[i] = NULL;
	}

	Heap* heap = initializeHeap();
	if (heap == NULL) {
		return NULL;
	}

	HandleList_t* handleList = initializeHandleList();
	if (handleList == NULL) {
		return NULL;
	}

	gc->mapSize = HASHMAPSIZE;
	gc->heap = heap;
	gc->mapPointer = hashMap;
	gc->handleList = handleList;

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
	return 0;
}

void* GCMalloc(GC* gc, size_t size) { //DODAJ POKAZIVAC NA TRED koji alocira da bismo znali cije je sta //ne nego samo listu tredova da bismo ih sve zaustavili, mani me od toj
	void* onoStoTrebaDaSeAlocira = malloc(size);
	if (onoStoTrebaDaSeAlocira == NULL) {
		return NULL;
	}

	HeapNode_t* newNode = addNodeToHeap(gc->heap, size, onoStoTrebaDaSeAlocira);
	if (newNode == NULL) {
		size_t napravljenoMesta = markAndSweep(gc, HASHMAPSIZE);
		if (napravljenoMesta + gc->heap->size >= size) {
			newNode = addNodeToHeap(gc->heap, size, onoStoTrebaDaSeAlocira);
			if (newNode == NULL) { //msm da ovde treba i onaj gore newNode da se obrise i dealocira
				free(onoStoTrebaDaSeAlocira);
				return NULL;
			}
		}
		else {
			free(onoStoTrebaDaSeAlocira);
			return NULL; //takodje ovde trebaju free-ovi // msm da vise ne trebaju
		}
	}
	if (hashPointer(HASHMAPSIZE, gc->mapPointer, onoStoTrebaDaSeAlocira, newNode)) {
		printf("Alocirali smo memoriju na %p\n", onoStoTrebaDaSeAlocira);
		return onoStoTrebaDaSeAlocira;
	}

	//nedostaje bezbedno brisanje nodea i hashovanog pointera ako ne uspe da se hashuje
	free(onoStoTrebaDaSeAlocira);
	return NULL;
}

size_t markAndSweep(GC* gc, unsigned sizeOfArray) {
	stopAllThreads(gc);
	printf("Threadovi stopirani\n");
	HandleNode_t* handleNode = gc->handleList->lastNode;
	do {
		ScanThreadStack(gc, handleNode->handle);
		handleNode = handleNode->prev;
	} while (handleNode != NULL);
	printf("Ciscenje\n");
	size_t napravljenoMesta = Sweep(gc);
	resumeAllThreads(gc);
	return napravljenoMesta;
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

size_t Sweep(GC* gc) { // treba da se odmarkiraju markirani
	HeapNode_t* hn = gc->heap->lastNode, *temp = NULL;
	size_t napravljenoMesta = 0;
	while (hn != NULL) {
		if (!hn->marked) {
			printf("Dealocirali smo %p\n", hn->data);
			napravljenoMesta += dealloc(gc, hn);

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
	return napravljenoMesta;
}

void ScanThreadStack(GC* gc, HANDLE hThread) {
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(hThread, &context)) {
		for (LPVOID current = (LPVOID)context.Esp; current < (LPVOID)context.Ebp; current = (LPVOID)((char*)current + 1)) { //eventualno sizeof(1) u slucaju da je neki char bio pa poremetio redosled u steku
			PointerNode_t* nodeFromPointer = getNodeFromPointer(HASHMAPSIZE, gc->mapPointer, (void*)current);
			if (nodeFromPointer != NULL) {
				Mark(nodeFromPointer->node);
			}
		}
	}
}
