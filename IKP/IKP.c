#include <stdio.h>
#include <Windows.h>
#include "Heap.h"


typedef struct Struktura_1 {
	int broj;
	int* dinamickiBroj;
	int broj2[2];
} STRUKTURA_1;

typedef struct Struktura_2 {
	int broj;
	STRUKTURA_1 struktura1;
	STRUKTURA_1* dinamickaStruktura1;
} STRUKTURA_2;

void alocirajIIzgubiPointer(GC* gc) {
	STRUKTURA_2* s2 = (STRUKTURA_2*)GCMalloc(gc, sizeof(STRUKTURA_2));
	printf("Alocirali smo nesto sto bi trebalo da bude pokupljeno u sledecem potezu\n");
}

DWORD WINAPI test_thread(LPVOID lpParam) {
	GC* gc = (GC*)lpParam;
	STRUKTURA_2* struktura2 = (STRUKTURA_2*)GCMalloc(gc, sizeof(STRUKTURA_2));
	alocirajIIzgubiPointer(gc);
	struktura2->dinamickaStruktura1 = (STRUKTURA_1*)GCMalloc(gc, sizeof(STRUKTURA_1));
	struktura2->dinamickaStruktura1->broj = 2;
	printf("%d\n", struktura2->dinamickaStruktura1->broj);
}

int main() {
	GC* gc = InitializeGC();

	//HANDLE test_tred = CreateThread(NULL, 0, test_thread, NULL, 0, NULL);
	HANDLE tred1 = GCCreateThread(gc, NULL, 0, test_thread, gc, 0, NULL);
	HANDLE tred2 = GCCreateThread(gc, NULL, 0, test_thread, gc, 0, NULL);

	int slovo = getchar();

	CloseHandle(tred1);
	CloseHandle(tred2);

	return 0;
}