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

	removeItemsFromStack(gc, s2);
}

DWORD WINAPI test_thread(LPVOID lpParam) {
	GC* gc = (GC*)lpParam;
	STRUKTURA_2* struktura2 = (STRUKTURA_2*)GCMalloc(gc, sizeof(STRUKTURA_2));
	printf("Pokazivac sa steka koji treba da se markira: %p\n", struktura2);
	alocirajIIzgubiPointer(gc);
	struktura2->dinamickaStruktura1 = (STRUKTURA_1*)GCMalloc(gc, sizeof(STRUKTURA_1));
	struktura2->dinamickaStruktura1->broj = 2;
	printf("%d\n", struktura2->dinamickaStruktura1->broj);

	removeItemsFromStack(gc, struktura2);
	removeItemsFromStack(gc, struktura2->dinamickaStruktura1);
}

int main() {
	GC* gc = InitializeGC();

	//HANDLE test_tred = CreateThread(NULL, 0, test_thread, NULL, 0, NULL);
	HANDLE tred1 = GCCreateThread(gc, NULL, 0, test_thread, gc, 0, NULL);

	int slovo = getchar();

	CloseHandle(tred1);

	DeinitializeGC(gc);

	return 0;
}