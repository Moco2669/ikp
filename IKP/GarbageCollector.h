#pragma once
#include "Heap.h"
#include "PointerToNodeMap.h"
#include "HandleList.h"
#include "Structs.h"

#ifndef GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_H

GC* InitializeGC();

void DeinitializeGC(GC* gc);

HANDLE WINAPI GCCreateThread(GC* gc, LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);

void* GCMalloc(GC* gc, size_t size);

size_t markAndSweep(GC* gc, unsigned sizeOfArray);

void stopAllThreads(GC* gc);

void resumeAllThreads(GC* gc);

void Mark(HeapNode_t* heapNode);

size_t Sweep(GC* gc);

void ScanThreadStack(GC* gc, HANDLE hThread);

#endif