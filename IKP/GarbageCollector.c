#include "GarbageCollector.h"
#include "Structs.h"

GC* InitializeGC() {
	GC* gc = (GC*)malloc(sizeof(GC));
	if (gc == NULL) {
		return NULL;
	}

	PointerNode_t** hashMap = initializeMap(HASHMAPSIZE);
	if (hashMap == NULL) {
		free(gc);
		return NULL;
	}
	for (int i = 0; i < HASHMAPSIZE; ++i) {
		hashMap[i] = NULL;
	}

	Heap* heap = initializeHeap();
	if (heap == NULL) {
		free(gc);
		free(hashMap);
		return NULL;
	}

	HandleList_t* handleList = initializeHandleList();
	if (handleList == NULL) {
		free(gc);
		free(hashMap);
		free(heap);
		return NULL;
	}

	VirtualStack_t* virtualStack = initializeStack();
	if (virtualStack == NULL) {
		free(gc);
		free(hashMap);
		free(heap);
		free(handleList);
		return NULL;
	}

	gc->mapSize = HASHMAPSIZE;
	gc->heap = heap;
	gc->mapPointer = hashMap;
	gc->handleList = handleList;
	gc->virtualStack = virtualStack;

	for (int i = 0; i < HASHMAPSIZE; ++i) {
		printf("%p\n", gc->mapPointer[i]);
	}

	return gc;
}

void DeinitializeGC(GC* gc) {
	DeinitializeHeap(gc->heap);
	DeinitializeMap(gc->mapPointer, HASHMAPSIZE);
	free(gc);
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
		for (int i = 0; i < HASHMAPSIZE; ++i) {
			printf("%p\n", gc->mapPointer[i]);
		}
		if (addItemsToStack(gc->virtualStack, onoStoTrebaDaSeAlocira) != NULL) {
			return onoStoTrebaDaSeAlocira;
		} // TODO: pocistiti hashpointer i ko zna sta sve
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
		/*
		ScanThreadStack(gc, handleNode->handle);
		ScanThreadStack2(gc, handleNode->handle);*/
		scanVirtualStack(gc);
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

void Mark(GC* gc, HeapNode_t* heapNode) {
	printf("Markiramo %p\n", heapNode);
	if (heapNode->marked == true) return;
	heapNode->marked = true;
	size_t remainingData = heapNode->size - sizeof(void*); //npr ako je velicina data-e 5 (int+char) mi cemo uzeti prva 4 bajta data-e, proveriti, i onda nam ostaje samo 1 bajt da se proveri
	for (size_t dataSize = 0; dataSize <= remainingData; ++dataSize) {
		PointerNode_t* nodeFromPointer = getNodeFromPointer(HASHMAPSIZE, gc->mapPointer, (void*)((char*)heapNode->data + dataSize)); //ukoliko je ovo UNUTAR data-e isto pokazivac, vratice se node koji ce da pokazuje na node na koji pokazuje pokazivac UNUTAR data-e
		if (nodeFromPointer != NULL) {
			Mark(gc, nodeFromPointer->node); //ijao rekurzija
		}
	}
}

size_t Sweep(GC* gc) { // treba da se odmarkiraju markirani, mada i ne mora jer druga generacija najace
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
			void* dataOnStack = *(void**)(current);
			printf("Skeniranje 1: %p\n", dataOnStack);
			PointerNode_t* nodeFromPointer = getNodeFromPointer(gc->mapSize, gc->mapPointer, dataOnStack);
			if (nodeFromPointer != NULL) {
				Mark(gc, nodeFromPointer->node);
			}
		}
	}
} //nene C JE SUPER JEZIK KAD DOBRO OTKUCAS SVE RADI KAKO TREBA AHA JESTE JESTE TACNO U POTPUNOSTI SE SLAZEM NEMA STA JA PAMETNIJEGA JEZIKA NISAM VIDIJO
/*
void ScanAndMarkStack(GC* gc) {
	MEMORY_BASIC_INFORMATION mbi;
	SYSTEM_INFO si;
	GetSystemInfo(&si); // Get system information to find the page size

	// Start from the address of 'mbi', which is on the stack
	void* stackAddr = &mbi;
	while (VirtualQuery(stackAddr, &mbi, sizeof(mbi))) {
		// Check if this memory block is a stack region
		if (mbi.State == MEM_COMMIT && (mbi.Protect & PAGE_READWRITE)) {
			void* stackBottom = mbi.BaseAddress;
			void* stackTop = (char*)stackBottom + mbi.RegionSize;

			// Ensure we're aligned to the size of a pointer
			stackBottom = (void*)(((uintptr_t)stackBottom + sizeof(void*) - 1) & ~(sizeof(void*) - 1));

			// Check every pointer-sized location within the stack for pointers to mark
			for (void** location = (void**)stackBottom; location < (void**)stackTop; location++) {
				void* potentialPtr = *location;
				// Check if it's a pointer we need to mark
				PointerNode_t* nodeFromPointer = getNodeFromPointer(gc->mapSize, gc->mapPointer, potentialPtr);
				if (nodeFromPointer != NULL) {
					Mark(gc, nodeFromPointer->node);
				}

			}
		}

		// Move to the next memory block
		stackAddr = (char*)mbi.BaseAddress + mbi.RegionSize;
		if (stackAddr >= si.lpMaximumApplicationAddress) {
			break; // We have reached the end of the stack
		}
	}
}*/

void ScanThreadStack2(GC* gc, HANDLE hThread) {
	PVOID StackBase;
	PVOID StackLimit;
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	NT_TIB tib;
	if (GetThreadContext(hThread, &context)) {
		StackBase = context.Ebp;
		StackLimit = context.Esp;
	}
	else {
		return;
	}

	for (PBYTE stackPtr = StackLimit; stackPtr < StackBase; ++stackPtr) {
		PVOID potentialPointer = *(PVOID*)stackPtr;
		printf("Skeniranje 2: %d\n", (void*)potentialPointer);
		PointerNode_t* nodeFromPointer = getNodeFromPointer(HASHMAPSIZE, gc->mapPointer, potentialPointer);
		if (nodeFromPointer != NULL) {
			Mark(gc, nodeFromPointer->node);
		}
	}
}

VirtualStack_t* initializeStack() {
	VirtualStack_t* stack = (VirtualStack_t*)malloc(sizeof(VirtualStack_t));
	if (stack == NULL) {
		return NULL;
	}
	stack->lastPointer = NULL;
	return stack;
}

void removeItemsFromStack(GC* gc, void* pointer) {
	findAndRemovePointer(gc->virtualStack, pointer);
}

void scanVirtualStack(GC* gc) {
	PointerOnStack_t* pos = gc->virtualStack->lastPointer;
	while (pos != NULL) {
		PointerNode_t* nodeFromPointer = getNodeFromPointer(gc->mapSize, gc->mapPointer, pos->pointerFromStack);
		if (nodeFromPointer != NULL) {
			Mark(gc, nodeFromPointer->node);
		}
		pos = pos->prev;
	}
}