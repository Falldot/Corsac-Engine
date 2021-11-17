// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_SLICE_H
#define CORSAC_SLICE_H

#pragma once

#include "internal/config.h"
#include "allocator.h"
#include "type_traits.h"
#include "iterator.h"
#include "algorithm.h"
#include "initializer_list.h"
#include "memory.h"

#include <new>
#include <stddef.h>
#if CORSAC_EXCEPTIONS_ENABLED
    #include <stdexcept> // std::out_of_range, std::length_error.
#endif

namespace corsac
{
    /// VectorBase
    ///
    /// The reason we have a VectorBase class is that it makes exception handling
    /// simpler to implement because memory allocation is implemented entirely
    /// in this class. If a user creates a vector which needs to allocate
    /// memory in the constructor, VectorBase handles it. If an exception is thrown
    /// by the allocator then the exception throw jumps back to the user code and
    /// no try/catch code need be written in the vector or VectorBase constructor.
    /// If an exception is thrown in the vector (not VectorBase) constructor, the
    /// destructor for VectorBase will be called automatically (and free the allocated
    /// memory) before the execution jumps back to the user code.
    /// However, if the vector class were to handle both allocation and initialization
    /// then it would have no choice but to implement an explicit try/catch statement
    /// for all pathways that allocate memory. This increases code size and decreases
    /// performance and makes the code a little harder read and maintain.
    ///
    /// The C++ standard (15.2 paragraph 2) states:
    ///    "An object that is partially constructed or partially destroyed will
    ///     have destructors executed for all its fully constructed subobjects,
    ///     that is, for subobjects for which the constructor has been completed
    ///     execution and the destructor has not yet begun execution."
    ///
    /// The C++ standard (15.3 paragraph 11) states:
    ///    "The fully constructed base classes and members of an object shall
    ///     be destroyed before entering the handler of a function-try-block
    ///     of a constructor or destructor for that block."
    ///
    template <typename T, typename Allocator>
    struct VectorBase
    {
        using allocator_type = Allocator;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        static const size_type npos     = (size_type)-1;      // 'npos' означает недопустимую позицию или просто непозицию.
        static const size_type kMaxSize = (size_type)-2;      // -1 зарезервировано для npos. Также немного выгодно, что kMaxSize имеет значение меньше -1, поскольку это помогает нам справиться с потенциальными проблемами переноса целых чисел.

    protected:
        T*                                          mpBegin;
        T*                                          mpEnd;
        corsac::compressed_pair<T*, allocator_type>  mCapacityAllocator;

        T*& internalCapacityPtr() EA_NOEXCEPT { return mCapacityAllocator.first(); }
        T* const& internalCapacityPtr() const EA_NOEXCEPT { return mCapacityAllocator.first(); }
        allocator_type&  internalAllocator() EA_NOEXCEPT { return mCapacityAllocator.second(); }
        const allocator_type&  internalAllocator() const EA_NOEXCEPT { return mCapacityAllocator.second(); }

    public:
        VectorBase();
        VectorBase(const allocator_type& allocator);
        VectorBase(size_type n, const allocator_type& allocator);

        ~VectorBase();

        const allocator_type& get_allocator() const EA_NOEXCEPT;
        allocator_type&       get_allocator() EA_NOEXCEPT;
        void                  set_allocator(const allocator_type& allocator);

    protected:
        T*        DoAllocate(size_type n);
        void      DoFree(T* p, size_type n);
        size_type GetNewCapacity(size_type currentCapacity);

    }; // VectorBase
}

#endif

