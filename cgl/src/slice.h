// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#pragma once

// memcpy
#include <string.h>

// panic
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CORSAC_SLICE_DEFAULT_CAPACITY 4

#define corsac_slice(T) \
    ((T*)_corsac_slice(sizeof(T), CORSAC_SLICE_DEFAULT_CAPACITY))

#define corsac_slice_make(T, length) \
    ((T*)_corsac_slice(sizeof(T), length))

void* _corsac_slice(
    size_t size,
    size_t length);

#define corsac_slice_create(T, array) \
    ((T*)_corsac_slice_create(sizeof(T), sizeof(array), array))

#define corsac_slice_args(T, ...) \
    ({T arr[] = {__VA_ARGS__}; (T*)corsac_slice_create(T, arr);})

void* _corsac_slice_create(
    size_t el_size,
    size_t arr_size,
    void* array);

#define corsac_slice_get(slice, T, index) \
    ((T*)_corsac_slice_get(slice, sizeof(T), index))

void* _corsac_slice_get(
    void* slice,
    size_t el_size,
    size_t index);

#define corsac_slice_push(slice, T, ...) \
	(*((T*)_corsac_slice_push(slice, sizeof(T))) = ((T)__VA_ARGS__))

void* _corsac_slice_push(
    void* slice,
    size_t el_size);

#define corsac_slice_pop(slice, T) \
    ((T*)_corsac_slice_pop(slice, sizeof(T)))

void* _corsac_slice_pop(
    void* slice,
    size_t el_size);

#define corsac_slice_shrink(slice) \
    (_corsac_slice_reserve(slice, corsac_slice_size(slice)))

void _corsac_slice_reserve(
    void* slice,
    size_t length);

#define corsac_slice_at(slice, size_step) \
    (void*)(((size_t)(slice)) + ((size_t)(size_step)))

#define corsac_slice_clear(slice) \
    _corsac_slice_set_size(slice, 0)

#define corsac_slice_free(slice) \
    (free(&((size_t*)(slice))[-2]))

#define corsac_slice_capacity(slice) \
    ((size_t*)(slice))[-1]

#define corsac_slice_size(slice) \
    ((size_t*)(slice))[-2]

#define _corsac_slice_set_capacity(slice, size) \
    ((size_t*)(slice))[-1] = size

#define _corsac_slice_set_size(slice, size) \
    ((size_t*)(slice))[-2] = size

#ifdef __cplusplus
}
#endif