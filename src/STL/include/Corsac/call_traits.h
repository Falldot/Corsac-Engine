/**
 * corsac::STL
 *
 * call_traits.h
 *
 * Created by Falldot on 25.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_CALL_TRAITS_H
#define CORSAC_CALL_TRAITS_H

#pragma once
/**
 * Описание (Falldot 26.11.2021)
 *
 * Дизайн call_traits здесь очень похож на тот, который можно найти в библиотеках
 * метапрограммирования шаблонов, таких как Boost, GCC и Metrowerks, учитывая, что эти
 * библиотеки установили этот интерфейс как стандарт для решения этой проблемы.
 * Кроме того, они описаны в различных книгах по теме метапрограммирования шаблонов,
 * таких как «Современный дизайн C++».
 *
 * Цель call_traits заключается в том, чтобы гарантировать, что такие проблемы,
 * как «ссылки на ссылки», никогда не возникают, и что параметры передаются
 * наиболее эффективным способом. В каждом случае, если ваша существующая практика
 * заключается в использовании типа, определенного слева, замените его call_traits определенным типом справа.
 * Примеры:
 *      call_traits<T>::value_type      -> T        (возврат по значению)
 *      call_traits<T>::reference       -> T&       (возвращаемое значение)
 *      call_traits<T>::const_reference -> const T& (возвращаемое значение)
 *      call_traits<T>::param_type      -> const T& (параметр функции)
 */

#include "Corsac/type_traits.h"

namespace corsac
{
    template <typename T, bool small_>
    struct ct_imp2 { using param_type = const T&; };

    template <typename T>
    struct ct_imp2<T, true> { using param_type = const T; };

    template <typename T, bool isp, bool b1>
    struct ct_imp { using param_type = const T&; };

    template <typename T, bool isp>
    struct ct_imp<T, isp, true> { using param_type = typename ct_imp2<T, sizeof(T) <= sizeof(void*)>::param_type; };

    template <typename T, bool b1>
    struct ct_imp<T, true, b1> { using param_type = T const; };

    template <typename T>
    struct call_traits
    {
    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using param_type = typename ct_imp<T, is_pointer<T>::value, is_arithmetic<T>::value>::param_type ;
    };

    template <typename T>
    struct call_traits<T&>
    {
        using value_type = T&;
        using reference = T&;
        using const_reference = const T&;
        using param_type = T&;
    };

    template <typename T, size_t N>
    struct call_traits<T [N]>
    {
    private:
        typedef T array_type[N];

    public:
        using value_type = const T*;
        using reference = array_type&;
        using const_reference = const array_type&;
        using param_type = const T* const;
    };

    template <typename T, size_t N>
    struct call_traits<const T [N]>
    {
    private:
        typedef const T array_type[N];

    public:
        using value_type = const T*;
        using reference = array_type&;
        using const_reference = const array_type&;
        using param_type = const T* const;
    };
}

#endif //CORSAC_CALL_TRAITS_H
