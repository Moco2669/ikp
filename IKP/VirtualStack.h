#pragma once
#include <stdlib.h>
#include "Structs.h"

#ifndef VIRTUALSTACK_H
#define VIRTUALSTACK_H

void* addItemsToStack(VirtualStack_t* virtualStack, void* pointer);

void findAndRemovePointer(VirtualStack_t* virtualStack, void* pointer);

#endif