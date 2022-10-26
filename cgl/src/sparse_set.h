// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#pragma once

// corsac_slice
#include "slice.h"

// panic
#include "panic.h"

#ifdef __cplusplus
extern "C" {
#endif

/***** PUBLIC *****/

/**
 * This struct sparse set.
 * @public
 * @param packed The packed array.
 * @param sparse The sparce array.
 */
typedef struct
{
    int* packed;
    int* sparse;
} corsac_sparse_set;

/**
 * This macros check has number in sparse set.
 * @public
 * @param sparse_set The sparse set.
 * @param x The check number.
 */
#define corsac_sparse_set_has(sparce_set, x) \
    (sparce_set->sparse[x] < corsac_slice_size(sparce_set->packed) && sparce_set->sparse[x] >= 0 && sparce_set->packed[sparce_set->sparse[x]] == x)

/**
 * This function create new sparse set.
 * @public
 * @return The new sparse set.
 */
corsac_sparse_set* corsac_sparse_set_new();

/**
 * This function insert number in sparse set.
 * @public
 * @param sparse_set The sparse set.
 * @param x The insert number.
 */
void corsac_sparse_set_add(corsac_sparse_set* sparse_set, int x);

/**
 * This function remove number from sparse set.
 * @public
 * @param sparse_set The sparse set.
 * @param x The remove number.
 */
void corsac_sparse_set_remove(corsac_sparse_set* sparse_set, int x);

/**
 * This function clear data sparse set.
 * @public
 * @param sparse_set The sparse set.
 */
void corsac_sparse_set_clear(corsac_sparse_set* sparse_set);

/**
 * This function free sparse set.
 * @public
 * @param sparse_set The sparse set.
 */
void corsac_sparse_set_free(corsac_sparse_set* sparse_set);

/***** PRIVATE *****/

#ifdef __cplusplus
}
#endif