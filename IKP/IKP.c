#include <stdio.h>
#include <Windows.h>
#include "Heap.h"


typedef struct Struktura_1 {
	int broj;
	int* dinamickiBroj;
	int broj[2];
} STRUKTURA_1;

typedef struct Struktura_2 {
	int broj;
	STRUKTURA_1 struktura1;
	STRUKTURA_1* dinamickaStruktura1;
} STRUKTURA_2;

DWORD WINAPI test_thread(LPVOID lpParam) {
	GC* gc = (GC*)lpParam;
	STRUKTURA_2* struktura2 = (STRUKTURA_2*)GCMalloc(gc, sizeof(STRUKTURA_2));
}

int main() {
	GC* gc = InitializeGC();

	return 0;
}