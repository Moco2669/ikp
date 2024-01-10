#include "PointerToNodeMap.h"


bool hashPointer(int sizeOfArray, PointerNode_t** array, void* pointer, HeapNode_t* node) {
	unsigned indexOfNode = (unsigned)pointer % sizeOfArray;
	PointerNode_t* current = array[indexOfNode];
	if (current == NULL) {
		PointerNode_t* newNode = createNode(pointer, node);
		if (newNode == NULL) return false;
		array[indexOfNode] = newNode;
		return true;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	PointerNode_t* newNode = createNode(pointer, node);
	if (newNode == NULL) return false;
	current->next = newNode;
	return true;
} //mozda bi ovde mogo neki bool ili cak da se opet vrati pokazivac na pointernode nzm iskr videcemo kasnije

PointerNode_t* createNode(void* pointer, HeapNode_t* node) {
	PointerNode_t* newNode = (PointerNode_t*)malloc(sizeof(PointerNode_t));
	if (newNode == NULL) {
		return NULL;
	}
	newNode->data = pointer;
	newNode->node = node;
	newNode->next = NULL;
	return newNode;
}

PointerNode_t* getNodeFromPointer(int sizeOfArray, PointerNode_t **array, void* pointer) {
	PointerNode_t* current = array[(unsigned)pointer % sizeOfArray];
	while (current != NULL) {
		if (current->data == pointer) {
			return current;
		} current = current->next;
	}
	return NULL;
}

PointerNode_t* initializeMap(int sizeOfArray) {
	PointerNode_t* map = (PointerNode_t*)malloc(sizeof(PointerNode_t*) * sizeOfArray);
	return map;
}