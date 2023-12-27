#pragma once

typedef struct HeapNode {
	void* data;
	bool marked;
	HeapNode_t* next;
} HeapNode_t;