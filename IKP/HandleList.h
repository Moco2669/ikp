#pragma once
#include "Structs.h"
#include <stdbool.h>

#ifndef HANDLELIST_H
#define HANDLELIST_H

HandleList_t* initializeHandleList();

bool addHandleToList(HandleList_t* list, HANDLE handle);

#endif // !HANDLELIST_H