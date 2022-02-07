/**
 * corsac::STL
 *
 * initializer_list.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_INITIALIZER_LIST_H
#define CORSAC_INITIALIZER_LIST_H

#pragma once
/**
 * Описание (Falldot 20.11.2021)
 *
 * Реализация статического массива элементов, в котором каждый элемент имеет указанный тип.
 */
#include "Corsac/STL/config.h"
#include "Corsac/Base/have.h"

#if defined(CORSAC_HAVE_INITIALIZER_LIST) // Если компилятор может генерировать вызовы std::initializer_list...

// Тип initializer_list должен быть объявлен в пространстве имен std,
	// так как это пространство имен компилятор использует при генерации кода для его использования.
	#include <initializer_list>

#else
namespace std
{
    template<class E>
    class initializer_list
    {
    public:
        using value_type        = E;
        using reference         = const E&;
        using const_reference   = const E&;
        using size_type         = size_t;
        using iterator          = const E*; // Должен быть константой, поскольку initializer_list (и его mpArray) является неизменяемым временным объектом.
        using const_iterator    = const E*;

    private:
        iterator  mpArray;
        size_type mArraySize;

        // Этот конструктор является частным, но компилятор C++ может вызывать его в соответствии со стандартом C++ 11.
        initializer_list(const_iterator pArray, size_type arraySize)
                : mpArray(pArray), mArraySize(arraySize) {}

    public:
        initializer_list() noexcept
                : mpArray(NULL), mArraySize(0) {}

        size_type      size()  const noexcept { return mArraySize; }
        const_iterator begin() const noexcept { return mpArray; }            // Должен быть const_iterator, поскольку initializer_list (и его mpArray) является неизменяемым временным объектом.
        const_iterator end()   const noexcept { return mpArray + mArraySize; }
    };


    template<class T>
    const T* begin(std::initializer_list<T> ilist) noexcept
    {
        return ilist.begin();
    }

    template<class T>
    const T* end(std::initializer_list<T> ilist) noexcept
    {
        return ilist.end();
    }
}
#endif

#endif //CORSAC_INITIALIZER_LIST_H
