// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#include "slice.h"

void* _corsac_slice(
    size_t el_size,
    size_t lenght)
{
    size_t* slice = corsac_malloc(
        lenght * el_size
        + sizeof(size_t) * 2);

    slice[1] = lenght;
    slice[0] = 0;

    return (void*)&slice[2];
}

void* _corsac_slice_create(
    size_t el_size,
    size_t arr_size,
    void* array)
{
    void* slice = _corsac_slice(arr_size, 1);
    memcpy(slice, array, arr_size);
    return slice;
}

void _corsac_slice_reserve(
    void* slice,
    size_t lenght)
{
    if (!slice) corsac_panic("Invalid operation!");
    size_t* data = corsac_realloc(
        &((size_t *)slice)[-2],
        lenght * sizeof(*slice)
        + (sizeof(size_t) * 2));

    data[1] = lenght;

    slice = (void*)&data[2];
}

void* _corsac_slice_get(
    void* slice,
    size_t el_size,
    size_t index)
{
    if (!slice) corsac_panic("Invalid operation!");
    if (index > corsac_slice_size(slice))
        corsac_panic("Invalid index!");

    return corsac_slice_at(slice, el_size*index);
}

void* _corsac_slice_pop(
    void* slice,
    size_t el_size)
{
    if (!slice) corsac_panic("Invalid operation!");
    if (corsac_slice_size(slice) <= 0)
        corsac_panic("Invalid operation!");

    _corsac_slice_set_size(slice, corsac_slice_size(slice) - 1);
    return corsac_slice_at(slice, el_size*(corsac_slice_size(slice)));
}

void* _corsac_slice_push(
    void* slice,
    size_t el_size)
{
    if (!slice) corsac_panic("Invalid operation!");
    if (corsac_slice_capacity(slice) <= corsac_slice_size(slice))
         _corsac_slice_reserve(slice, corsac_slice_capacity(slice) * 2);

    _corsac_slice_set_size(slice, corsac_slice_size(slice) + 1);
    return corsac_slice_at(slice, el_size*(corsac_slice_size(slice)-1));
}