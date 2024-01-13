#pragma once
#include "Heap.h"
#include "PointerToNodeMap.h"
#include "HandleList.h"
#include "Structs.h"


GC* InitializeGC();

HANDLE WINAPI GCCreateThread(GC* gc, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);

void* GCMalloc(GC* gc, size_t size);

void Mark(int sizeOfArray, PointerNode_t** array, void* pointer);

void Sweep();