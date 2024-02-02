#include "VirtualStack.h"

void* addItemsToStack(VirtualStack_t* virtualStack, void* pointer) {
	EnterCriticalSection(&virtualStack->lock);
	PointerOnStack_t* pos = (PointerOnStack_t*)malloc(sizeof(PointerOnStack_t));
	if (pos == NULL) {
		return NULL;
	}
	pos->pointerFromStack = pointer;
	pos->prev = virtualStack->lastPointer;
	virtualStack->lastPointer = pos;
	LeaveCriticalSection(&virtualStack->lock);
	return pointer;
}

void findAndRemovePointer(VirtualStack_t* virtualStack, void* pointer) {
	PointerOnStack_t *pos = virtualStack->lastPointer, *prev = NULL;
	while (pos != NULL) {
		if (pos->pointerFromStack == pointer) {
			if (prev == NULL) {
				virtualStack->lastPointer = pos->prev;
				free(pos);
				pos = virtualStack->lastPointer;
			}
			else {
				prev->prev = pos->prev;
				free(pos);
				pos = prev->prev; 
			}
			return;
		}
		prev = pos;
		pos = pos->prev;
	}
}