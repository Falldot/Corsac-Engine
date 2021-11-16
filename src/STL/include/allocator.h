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

#include <new>

void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line);
void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line);

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
        void *allocate(size_t n, int flags = 0);
        void *allocate(size_t n, size_t alignment, size_t offset, int flags = 0);
        void deallocate(void *p, size_t n);
    };

    inline void* allocator::allocate(size_t n, int flags)
    {
        return ::new(static_cast<char*>(0), flags, 0, static_cast<char*>(0), 0) char[n];
    }

    inline void* allocator::allocate(size_t n, size_t alignment, size_t offset, int flags)
    {
        return ::new(alignment, offset, static_cast<char*>(0), flags, 0, static_cast<char*>(0), 0) char[n];
    }

    inline void allocator::deallocate(void* p, size_t)
    {
        delete[] static_cast<char*>(p);
    }

    inline bool operator==(const allocator&, const allocator&)
    {
        return true;
    }

    inline bool operator!=(const allocator&, const allocator&)
    {
        return false;
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
