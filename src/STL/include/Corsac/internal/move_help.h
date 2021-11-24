/**
 * corsac::STL
 *
 * internal/move_help.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_MOVE_HELP_H
#define CORSAC_MOVE_HELP_H

#pragma once

#include "Corsac/internal/base/base.h"

#include "Corsac/internal/config.h"
#include "Corsac/type_traits.h"

namespace corsac
{
    /**
    * forward
    *
    * пересылает аргумент другой функции точно так же, как он был передан вызывающей функции.
    * Не путать с перемещением, это специально для передачи шаблонных типов аргументов другой функции.
    * move специально предназначен для того, чтобы сделать тип ссылочным (т. е. подвижным) типом.
    *
    * Пример использования:
    *     template <class T>
    *     void WrapperFunction(T&& arg)
    *         { foo(corsac::forward<T>(arg)); }
    *
    *     template <class... Args>
    *     void WrapperFunction(Args&&... args)
    *         { foo(corsac::forward<Args>(args)...); }
    */
    template <typename T>
    constexpr T&& forward(typename corsac::remove_reference<T>::type& x) noexcept
    {
        return static_cast<T&&>(x);
    }

    template <typename T>
    constexpr T&& forward(typename corsac::remove_reference<T>::type&& x) noexcept
    {
        static_assert(!corsac::is_lvalue_reference<T>::value, "forward T isn't lvalue reference");
        return static_cast<T&&>(x);
    }

    /**
    * move
    *
    * move получает ссылку на свой аргумент rvalue и преобразует ее в значение x.
    * По определению возвращает: static_cast<typename remove_reference<T>::type&&>(t).
    * Основное использование этого - передать тип move функции, которая принимает T&&,
    * и, таким образом, выбрать эту функцию вместо (например) функции, которая принимает T или T&.
    */
    template <typename T>
    constexpr typename corsac::remove_reference<T>::type&& move(T&& x) noexcept
    {
        return static_cast<typename corsac::remove_reference<T>::type&&>(x);
    }

    /**
    * move_if_noexcept
    *
    * Возвращает T&&, если построение перемещения T не вызывает исключений. Вместо этого возвращает const T&,
    * если конструкция перемещения T вызывает исключения или не имеет доступного конструктора копирования.
    * Цель этого состоит в том, чтобы использовать автоматическое использование конструкции копирования
    * вместо конструкции перемещения, когда перемещение может вызвать исключение.
    */
    #if CORSAC_EXCEPTIONS_ENABLED
        template <typename T>
        constexpr typename corsac::conditional<!corsac::is_nothrow_move_constructible<T>::value &&
                                                        corsac::is_copy_constructible<T>::value, const T&, T&&>::type
        move_if_noexcept(T& x) noexcept
        {
            return corsac::move(x);
        }
    #else
        template <typename T>
        constexpr T&& move_if_noexcept(T& x) noexcept
        {
            return corsac::move(x);
        }
    #endif
}

#endif //CORSAC_MOVE_HELP_H
