// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#pragma once

#include <stdio.h>
#include <stdlib.h>

/***** METHODS *****/

#define corsac_panic(M, ...) \
    {                                                                                       \
        fprintf(stderr, "\n[ERROR] (%s:%d) " M "\n\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        exit(1);                                                                            \
    }
