#include <stdio.h>
#include "Heap.h"

int main() {
	initializeHeap();
	int* node1 = (int*)addNodeToHeap(128 * sizeof(int), &node1);
	if (node1 != NULL) {
		printf("Ima memorije\n");
	}
	else {
		printf("Nema memorije\n");
	}

	int* node2 = (int*)addNodeToHeap(128 * sizeof(int), &node2);
	if (node2 != NULL) {
		printf("Ima memorije\n");
	}
	else {
		printf("Nema memorije\n");
	}

	removeNodeFromHeap(&node1);

	int* node3 = (int*)addNodeToHeap(128 * sizeof(int), &node3);
	if (node3 != NULL) {
		printf("Ima memorije\n");
	}
	else {
		printf("Nema memorije\n");
	}

	int* node4 = (int*)addNodeToHeap(128 * sizeof(int), &node4);
	if (node4 != NULL) {
		printf("Ima memorije\n");
	}
	else {
		printf("Nema memorije\n");
	}

	return 0;
}