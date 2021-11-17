/**
 * corsac::STL
 *
 * allocator.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_ALLOCATOR_H
#define CORSAC_ALLOCATOR_H

#pragma once

#include "internal/config.h"

namespace corsac {
    /**
    * alloc_flags
    *
    * Определяет флаги allocate.
    */
    enum class alloc_flags {
        MEM_TEMP = 0, // Недостаток памяти, не обязательно временный.
        MEM_PERM = 1  // Большая память для вещей, которые не выгружаются.
    };

    /**
    * allocator
    *
    * Обратите внимание на то, что в этом классе распределителя нет шаблонов для какого-либо
    * типа, а вместо этого он просто выделяет блоки памяти так же,
    * как функции malloc и free в C. Его можно рассматривать как аналог C++ std::allocator <char>.
    *
    * C++11's std::allocator (20.6.9)  нет конструктора перемещения или оператора присваивания.
    * Возможно, это связано с тем, что std::allocator'ы связаны с типами, а не с экземплярами.
    * Потенциальная неэквивалентность экземпляров C++ std::allocator была источником некоторых
    * признанных проблем проектирования. Мы не реализуем поддержку построения или назначения
    * перемещения в corsac::allocator, но пользователи могут определять свои собственные
    * распределители, у которых есть функции перемещения, и контейнеры corsac
    * совместимы с такими распределителями (т.е. ничего неожиданного не произойдет).
    */
    class allocator {
    public:
        explicit allocator(const char *pName = CORSAC_NAME_VAL(CORSAC_ALLOCATOR_DEFAULT_NAME));

        allocator(const allocator &x);

        allocator(const allocator &x, const char *pName);

        allocator &operator=(const allocator &x);

        void *allocate(size_t n, int flags = 0);

        void *allocate(size_t n, size_t alignment, size_t offset, int flags = 0);

        void deallocate(void *p, size_t n);

        const char *get_name() const;

        void set_name(const char *pName);

        protected:
        #if CORSAC_NAME_ENABLED
                const char* mpName; // Имя отладки, используемое для отслеживания памяти.
        #endif
        };
} // namespace corsac

// Если пользователь не заявил, что он где-то определил другую реализацию распределителя ...
#ifndef CORSAC_USER_DEFINED_ALLOCATOR

    #include <new>
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
        CORSAC_API allocator  gDefaultAllocator;
        CORSAC_API allocator* gpDefaultAllocator = &gDefaultAllocator;

        CORSAC_API allocator* GetDefaultAllocator()
        {
            return gpDefaultAllocator;
        }

        CORSAC_API allocator* SetDefaultAllocator(allocator* pAllocator)
        {
            allocator* const pPrevAllocator = gpDefaultAllocator;
            gpDefaultAllocator = pAllocator;
            return pPrevAllocator;
        }

        inline allocator::allocator(const char* CORSAC_NAME(pName))
        {
            #if CORSAC_NAME_ENABLED
                mpName = pName ? pName : CORSAC_ALLOCATOR_DEFAULT_NAME;
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
                mpName = pName ? pName : CORSAC_ALLOCATOR_DEFAULT_NAME;
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
                    return CORSAC_ALLOCATOR_DEFAULT_NAME;
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
                #define pName CORSAC_ALLOCATOR_DEFAULT_NAME
            #endif

            #if CORSAC_DLL
                return allocate(n, CORSAC_SYSTEM_ALLOCATOR_MIN_ALIGNMENT, 0, flags);
            #elif (CORSAC_DEBUGPARAMS_LEVEL <= 0)
                return ::new(0, flags, 0, 0, 0) char[n];
            #elif (CORSAC_DEBUGPARAMS_LEVEL == 1)
                return ::new(pName, flags, 0, 0, 0) char[n];
                    #else
                return ::new(pName, flags, 0, __FILE__, __LINE__) char[n];
            #endif
        }


        inline void* allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
        {
            #if CORSAC_DLL
                // В настоящее время у нас нет поддержки для реализации флагов при использовании
                // новой функции оператора библиотеки времени выполнения C. Пользователь может использовать
                // SetDefaultAllocator для переопределения распределителя по умолчанию.
                CORSAC_UNUSED(offset); CORSAC_UNUSED(flags);

                size_t adjustedAlignment = (alignment > CORSAC_PLATFORM_PTR_SIZE) ? alignment : CORSAC_PLATFORM_PTR_SIZE;

                void* p = new char[n + adjustedAlignment + CORSAC_PLATFORM_PTR_SIZE];
                void* pPlusPointerSize = (void*)((uintptr_t)p + CORSAC_PLATFORM_PTR_SIZE);
                void* pAligned = (void*)(((uintptr_t)pPlusPointerSize + adjustedAlignment - 1) & ~(adjustedAlignment - 1));

                void** pStoredPtr = (void**)pAligned - 1;
                CORSAC_ASSERT(pStoredPtr >= p);
                *(pStoredPtr) = p;

                CORSAC_ASSERT(((size_t)pAligned & ~(alignment - 1)) == (size_t)pAligned);

                return pAligned;
            #elif (CORSAC_DEBUGPARAMS_LEVEL <= 0)
                return ::new(alignment, offset, 0, flags, 0, 0, 0) char[n];
            #elif (CORSAC_DEBUGPARAMS_LEVEL == 1)
                return ::new(alignment, offset, pName, flags, 0, 0, 0) char[n];
            #else
                return ::new(alignment, offset, pName, flags, 0, __FILE__, __LINE__) char[n];
            #endif

            #undef pName
        }

        inline void allocator::deallocate(void* p, size_t)
        {
            #if CORSAC_DLL
                if (p != nullptr)
                {
                    void* pOriginalAllocation = *((void**)p - 1);
                    delete[]static_cast<char*>(pOriginalAllocation);
                }
            #else
                delete[]static_cast<char*>(p);
            #endif
        }

        inline bool operator==(const allocator&, const allocator&)
        {
            return true;
        }

        inline bool operator!=(const allocator&, const allocator&)
        {
            return false;
        }
    } // namespace corsac

#endif // CORSAC_USER_DEFINED_ALLOCATOR

namespace corsac
{
    template <typename Allocator>
    inline Allocator* get_default_allocator(const Allocator*)
    {
        return NULL; // По умолчанию мы возвращаем NULL; пользователь должен сделать специализацию этой функции, чтобы обеспечить собственную реализацию.
    }

    inline CORSAC_AllocatorType* get_default_allocator(const CORSAC_AllocatorType*)
    {
        // Для встроенного распределителя CORSAC_AllocatorType у нас уже есть функция для возврата экземпляра распределителя по умолчанию, поэтому мы предоставляем ее.
        return CORSAC_AllocatorDefault();
    }

    inline void* default_allocfreemethod(size_t n, void* pBuffer, void* /*pContext*/)
    {
        CORSAC_AllocatorType* const pAllocator = CORSAC_AllocatorDefault();

        if(pBuffer) // Если освободить ...
        {
            CORSAC_Free(*pAllocator, pBuffer, n);
            return NULL;  // Возвращаемое значение бессмысленно.
        }
        else // распределение
            return CORSAC_Alloc(*pAllocator, n);
    }

    /**
    * allocate_memory
    *
    * Это диспетчерская функция выделения памяти.
    * Todo: Сделать согласованные и несогласованные специализации.
    *        Обратите внимание, что для этого нам нужно будет использовать класс со статической
    *        функцией вместо отдельной функции, как показано ниже.
    */
    template <typename Allocator>
    inline void* allocate_memory(Allocator& a, size_t n, size_t alignment, size_t alignmentOffset)
    {
        void *result;
        if (alignment <= CORSAC_ALLOCATOR_MIN_ALIGNMENT)
        {
            result = CORSAC_Alloc(a, n)
            CORSAC_ASSERT((reinterpret_cast<size_t>(result)& ~(alignment - 1)) == reinterpret_cast<size_t>(result));
        }
        else
        {
            result = CORSAC_AllocAligned(a, n, alignment, alignmentOffset);
            auto resultMinusOffset = static_cast<char*>(result) - alignmentOffset;
            CORSAC_UNUSED(resultMinusOffset)
            CORSAC_ASSERT((reinterpret_cast<size_t>(resultMinusOffset)& ~(alignment - 1)) == reinterpret_cast<size_t>(resultMinusOffset));
        }
        return result;
    }
}

#endif //CORSAC_ALLOCATOR_H
