#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdbool.h>
#include <stddef.h>

#define POINTERTONODEMAP_H
#define HEAPSIZE 1024

typedef struct HeapNode HeapNode_t;

struct HeapNode {
	void* data;
	void* pointer;
	bool marked;
	HeapNode_t* prev;
	size_t size;
	int gen;
};

typedef struct Heap {
	HeapNode_t* lastNode;
	int size;
} Heap;

typedef struct PointerToNodeEntry {
	void* data;
	HeapNode_t* node;
	struct PointerNode_t* next;
} PointerNode_t;

typedef struct GarbageCollector {
	Heap* heap;
	PointerNode_t* mapPointer;
	unsigned mapSize;
} GC;

static Heap* heap;
static GC* collector;