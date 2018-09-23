#pragma once

#include <core/platform.h>

namespace sb {

void * malloc(usize size);

void * malloc(usize size, usize alignment);

usize mallocUsableSize(void * memPtr);

void free(void * memPtr);

} // namespace sb
