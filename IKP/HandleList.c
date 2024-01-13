#include "HandleList.h"
#include "Structs.h"
#include <stdbool.h>
#include <Windows.h>

bool addHandleToList(HandleList_t* list, HANDLE handle) {
	HandleNode_t* handleNode = (HandleNode_t*)malloc(sizeof(HandleNode_t));
	if (handleNode == NULL) {
		return false;
	}

	handleNode->handle = handle;
	handleNode->prev = list->lastNode;

	list->lastNode = handleNode;
	return true;
}