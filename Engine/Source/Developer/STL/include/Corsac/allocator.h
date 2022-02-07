/**
 * corsac::STL
 *
 * assert.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_ALLOCATOR_H
#define CORSAC_STL_ALLOCATOR_H

#pragma once

#include "Corsac/STL/config.h"

namespace corsac
{
    class allocator
    {
    public:
        explicit allocator(const char *pName = CORSAC_NAME_VAL(CORSAC_DEFAULT_ALLOCATOR_NAME));

        allocator(const allocator &alloc);

        allocator(const allocator &x, const char *pName);

        allocator &operator=(const allocator &alloc);

        void *allocate(size_t n, int flags = 0);

        void *allocate(size_t n, size_t alignment, size_t offset, int flags = 0);

        void deallocate(void *p, size_t n);

        [[nodiscard]] const char *get_name() const;

        void set_name(const char *pName);

    protected:
        #if CORSAC_NAME_ENABLED
                const char* mpName; // Имя отладки, используемое для отслеживания памяти.
        #endif
    };
}

#if !CORSAC_DLL
    // Если вы создаете обычную библиотеку, а не собираете CORSAC как DLL ...
    // Ожидается, что приложение определит следующие
    // версии оператора new для приложения. Либо это, либо
    // пользователю необходимо переопределить реализацию класса распределителя.
    void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
    void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
#endif

namespace corsac
{
    allocator  gDefaultAllocator;
    allocator* gpDefaultAllocator = &gDefaultAllocator;

    allocator* GetDefaultAllocator()
    {
        return gpDefaultAllocator;
    }

    allocator* SetDefaultAllocator(allocator* pAllocator)
    {
        allocator* const pPrevAllocator = gpDefaultAllocator;
        gpDefaultAllocator = pAllocator;
        return pPrevAllocator;
    }

    inline allocator::allocator(const char* CORSAC_NAME(pName))
    {
        #if CORSAC_NAME_ENABLED
                mpName = pName ? pName : CORSAC_DEFAULT_ALLOCATOR_NAME;
        #endif
    }

    inline allocator::allocator(const allocator& CORSAC_NAME(alloc))
    {
        #if CORSAC_NAME_ENABLED
                mpName = alloc.mpName;
        #endif
    }

    inline allocator::allocator(const allocator&, const char* CORSAC_NAME(pName))
    {
        #if CORSAC_NAME_ENABLED
                mpName = pName ? pName : CORSAC_DEFAULT_ALLOCATOR_NAME;
        #endif
    }

    inline allocator& allocator::operator=(const allocator& CORSAC_NAME(alloc))
    {
        #if CORSAC_NAME_ENABLED
                mpName = alloc.mpName;
        #endif
                return *this;
    }

    inline const char* allocator::get_name() const
    {
        #if CORSAC_NAME_ENABLED
                return mpName;
        #else
                return CORSAC_DEFAULT_ALLOCATOR_NAME;
        #endif
    }


    inline void allocator::set_name(const char* CORSAC_NAME(pName))
    {
        #if CORSAC_NAME_ENABLED
                mpName = pName;
        #endif
    }

    inline void* allocator::allocate(size_t n, int flags)
    {
        #if CORSAC_NAME_ENABLED
            #define pName mpName
        #else
            #define pName CORSAC_DEFAULT_ALLOCATOR_NAME
        #endif

        #if (CORSAC_DEBUG_PARAMS_LEVEL <= 0)
                return ::new(0, flags, 0, 0, 0) char[n];
        #elif (CORSAC_DEBUG_PARAMS_LEVEL == 1)
                return ::new(pName, flags, 0, 0, 0) char[n];
        #else
                return ::new(pName, flags, 0, __FILE__, __LINE__) char[n];
        #endif
    }


    inline void* allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        #if (CORSAC_DEBUG_PARAMS_LEVEL <= 0)
                return ::new(alignment, offset, 0, flags, 0, 0, 0) char[n];
        #elif (CORSAC_DEBUG_PARAMS_LEVEL == 1)
                return ::new(alignment, offset, pName, flags, 0, 0, 0) char[n];
        #else
                return ::new(alignment, offset, pName, flags, 0, __FILE__, __LINE__) char[n];
        #endif

        #undef pName
    }

    inline void allocator::deallocate(void* p, size_t)
    {
        delete[]static_cast<char*>(p);
    }

    inline bool operator==(const allocator&, const allocator&)
    {
        return true;
    }

    inline bool operator!=(const allocator&, const allocator&)
    {
        return false;
    }
}

namespace corsac
{
    template <typename Allocator>
    inline Allocator* get_default_allocator(const Allocator*)
    {
        return NULL; // По умолчанию мы возвращаем NULL; пользователь должен сделать специализацию этой функции, чтобы обеспечить собственную реализацию.
    }

    inline CORSAC_DEFAULT_ALLOCATOR_TYPE* get_default_allocator(const CORSAC_DEFAULT_ALLOCATOR_TYPE*)
    {
        return CORSAC_DEFAULT_ALLOCATOR();
    }

    inline void* free_memory(size_t n, void* pBuffer, void* /*pContext*/)
    {
        CORSAC_DEFAULT_ALLOCATOR_TYPE* const pAllocator = CORSAC_DEFAULT_ALLOCATOR();

        if (pBuffer) // Если освободить ...
        {
            CORSAC_Free(*pAllocator, pBuffer, n);
            return nullptr;  // Возвращаемое значение бессмысленно.
        }
        else
        {
            return CORSAC_ALLOC(*pAllocator, n);
        }
    }

    /**
    * allocate_memory
    *
    * Это диспетчерская функция выделения памяти.
    */
    template <typename Allocator>
    inline void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset)
    {
        void *result;
        if (alignment <= CORSAC_ALLOCATOR_MIN_ALIGNMENT)
        {
            result = CORSAC_ALLOC(a, n);
            CORSAC_ASSERT((reinterpret_cast<size_t>(result)& ~(alignment - 1)) == reinterpret_cast<size_t>(result));
        }
        else
        {
            result = CORSAC_ALLOC_ALIGNED(a, n, alignment, alignmentOffset);
            auto resultMinusOffset = static_cast<char*>(result) - alignmentOffset;
            CORSAC_UNUSED(resultMinusOffset);
            CORSAC_ASSERT((reinterpret_cast<size_t>(resultMinusOffset)& ~(alignment - 1)) == reinterpret_cast<size_t>(resultMinusOffset));
        }
        return result;
    }
}

#endif //CORSAC_STL_ALLOCATOR_H
