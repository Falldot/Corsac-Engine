// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_ALLOCATOR_H
#define CORSAC_ALLOCATOR_H

#pragma once

#include <malloc.h>

#ifndef corsac_allocator_type
	#define corsac_allocator_type corsac::malloc_allocator
#endif

#ifndef corsac_alloc
	#define corsac_alloc(allocator, size, alignment, offset) (allocator).allocate((size), (alignment), (offset))
#endif

#ifndef corsac_free
	#define corsac_free(allocator, point, size) (allocator).deallocate(static_cast<void*>(point), (size))
#endif

namespace corsac
{
    class malloc_allocator
    {
    public:

        void* allocate(size_t size);
        void* allocate(size_t size, size_t alignment, size_t offset);

        void deallocate(void* point, size_t);
    };

    void* malloc_allocator::allocate(size_t size)
    {
        return malloc(size);
    }

    void* malloc_allocator::allocate(size_t size, size_t alignment, size_t offset)
    {
        return malloc(size + alignment + offset);
    }

    void malloc_allocator::deallocate(void* point, size_t)
    {
        if (point != nullptr)
        {
            free(point);
        }
    }

    template <typename Allocator>
    inline void* allocate_memory(Allocator& allocator, size_t value, size_t alignment, size_t offset)
    {
        return corsac_alloc(allocator, value, alignment, offset);
    }

    template<class T>
    void destruct(T* p)
    {
        p->~T();
    }

    template<class T, typename Iterator>
    void destruct(Iterator first, Iterator last)
    {
        for(; first != last; ++first)
            (*first).~T();
    }

} // namespace corsac

//Slicee Benchmark         1343    1.511000
//Vector Benchmark         701     1.001000

//Slicee Benchmark         1228    1.001000
//Vector Benchmark         721     1.001000

#endif // Header include guard