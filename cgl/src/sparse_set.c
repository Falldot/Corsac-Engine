// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#include "sparse_set.h"

/**
 * This function create new sparse set.
 * @public
 * @return The new sparse set.
 */
corsac_sparse_set* corsac_sparse_set_new()
{
    corsac_sparse_set* set = malloc(sizeof(corsac_sparse_set));
    if (set == NULL) corsac_panic("Falled memory allocate!");

    set->packed = corsac_slice(int);
    set->sparse = corsac_slice(int);

    return set;
}

/**
 * This function insert number in sparse set.
 * @public
 * @param sparse_set The sparse set.
 * @param x The insert number.
 */
void corsac_sparse_set_add(corsac_sparse_set* sparse_set, int x)
{
    if (x >= corsac_slice_capacity(sparse_set->sparse)) _corsac_slice_reserve(sparse_set->sparse, x * 2);

    if (corsac_sparse_set_has(sparse_set, x))
        corsac_panic("This number already exist!")

    sparse_set->sparse[x] = corsac_slice_size(sparse_set->packed);
    corsac_slice_push(sparse_set->packed, int, x);
}

/**
 * This function remove number from sparse set.
 * @public
 * @param sparse_set The sparse set.
 * @param x The remove number.
 */
void corsac_sparse_set_remove(corsac_sparse_set* sparse_set, int x)
{
    if (!corsac_sparse_set_has(sparse_set, x))
        corsac_panic("This number does not exist!")

    int index = sparse_set->sparse[x];
    int* swap = corsac_slice_pop(sparse_set->packed, int);

    if (*swap != x)
    {
        sparse_set->packed[index] = *swap;
        sparse_set->sparse[*swap] = index;
    }
}

/**
 * This function clear data sparse set.
 * @public
 * @param sparse_set The sparse set.
 */
void corsac_sparse_set_clear(corsac_sparse_set* sparse_set)
{
    corsac_slice_clear(sparse_set->packed);
    corsac_slice_clear(sparse_set->sparse);
}

/**
 * This function free sparse set.
 * @public
 * @param sparse_set The sparse set.
 */
void corsac_sparse_set_free(corsac_sparse_set* sparse_set)
{
    corsac_slice_free(sparse_set->packed);
    corsac_slice_free(sparse_set->sparse);
    free(sparse_set);
}