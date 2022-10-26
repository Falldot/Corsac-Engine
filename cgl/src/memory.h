// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT

// malloc, realloc, free
#include <malloc.h>

#include "panic.h"

#define corsac_malloc(size) \
    ({                                                      \
        void* data = malloc(size);                          \
        if (!data) corsac_panic("Falled memory allocate!"); \
        data;                                               \
    })

#define corsac_realloc(src, size) \
    ({                                                          \
        void* dst = realloc(src, size);                         \
        if (!data) corsac_panic("Falled memory reallocate!");   \
        dst;                                                    \
    })
