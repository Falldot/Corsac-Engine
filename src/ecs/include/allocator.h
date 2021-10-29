// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_ALLOCATOR_H
#define CORSAC_ALLOCATOR_H

#pragma once

#include <malloc.h>
#include <iostream>

#ifndef CORSAC_ALIGN_OF
	#define CORSAC_ALIGN_OF alignof
#endif

#if !defined(CORSAC_PLATFORM_PTR_SIZE)
    #define CORSAC_PLATFORM_PTR_SIZE 8
#endif

#if !defined(CORSAC_SYSTEM_ALLOCATOR_MIN_ALIGNMENT)
	#if defined(CORSAC_PLATFORM_MICROSOFT) || defined(CORSAC_PLATFORM_APPLE)
		#define CORSAC_SYSTEM_ALLOCATOR_MIN_ALIGNMENT 16
	#else
		#define CORSAC_SYSTEM_ALLOCATOR_MIN_ALIGNMENT (CORSAC_PLATFORM_PTR_SIZE * 2)
	#endif
#endif

namespace corsac
{
    class allocator
    {
    public:
        void* allocate(size_t size);
        void* allocate(size_t size, size_t alignment);

        void deallocate(void* point, size_t);
    };

} // namespace corsac


#endif // Header include guard