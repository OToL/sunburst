#pragma once

#define sbMalloc(size) sb::malloc(size)

#define sbMallocA(aize, alignment) sb::malloc(size, alignment)

#define sbMallocUsabeSize(ptr) sb::mallocUsableSize(ptr)

#define sbFree(ptr) sb::free(ptr)

#include <core/_impl/memory/malloc.h>
