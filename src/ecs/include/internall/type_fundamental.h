// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_TYPE_FUNDAMENTAL_H
#define CORSAC_TYPE_FUNDAMENTAL_H

#pragma once

#include "internall/base/base.h"

namespace corsac
{
    /*
     * is_void
     *
     * is_void<T>::value == true тогда и только тогда, когда T преднадлежить одному из типов:
     *      [const][volatile] void
     */
    template <typename T> struct is_void : public false_type{};

    template <> struct is_void<void>                        : public true_type{};
    template <> struct is_void<void const>                  : public true_type{};
    template <> struct is_void<void volatile>               : public true_type{};
    template <> struct is_void<void const volatile>         : public true_type{};

    template <class T>
    constexpr bool is_void_v = is_void<T>::value;

    /**
    * has_void_arg
    *
    * утилита, которая определяет, является ли какой-либо из заданных аргументов шаблона недействительным.
    */
    // TODO(rparolin):  refactor with fold expressions when C++17 compilers are widely available.
    template <typename ...Args>
    struct has_void_arg;

    template <>
    struct has_void_arg<>
           : public corsac::false_type {};

    template <typename A0, typename ...Args>
    struct has_void_arg<A0, Args...>
       { static const bool value = (corsac::is_void<A0>::value || corsac::has_void_arg<Args...>::value); };

    /**
     * is_null_pointer
     *
     * Признак типа C +14. Относится только к nullptr_t, а не к NULL(0).
     *      corsac::is_null_pointer<nullptr>::value == true
     *      corsac::is_null_pointer<std::nullptr_t>::value == true
     *      corsac::is_null_pointer<void*>::value == false
     *      corsac::is_null_pointer<NULL>::value == [cannot compile]
     */
    // TODO: Проверить утверждение от eastl: "VC++'s handling of decltype(nullptr) is broken."
    #if !defined(_MSC_VER)
        template <typename T>
        struct is_null_pointer : public corsac::is_same<typename corsac::remove_cv<T>::type, decltype(nullptr)> {};
    #else
    template <typename T>
        struct is_null_pointer : public corsac::is_same<typename corsac::remove_cv<T>::type, std::nullptr_t> {};
    #endif

    template <class T>
    constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    /**
    * is_integral
    *
    * is_integral<T>::value == тогда и только тогда, когда T преднадлежит одному из:
    *    [const] [volatile] bool
    *    [const] [volatile] char
    *    [const] [volatile] signed char
    *    [const] [volatile] unsigned char
    *    [const] [volatile] wchar_t
    *    [const] [volatile] short
    *    [const] [volatile] int
    *    [const] [volatile] long
    *    [const] [volatile] long long
    *    [const] [volatile] unsigned short
    *    [const] [volatile] unsigned int
    *    [const] [volatile] unsigned long
    *    [const] [volatile] unsigned long long
    *
    */
    template <typename T> struct is_integral_helper             : public false_type{};

    template <> struct is_integral_helper<unsigned char>        : public true_type{};
    template <> struct is_integral_helper<unsigned short>       : public true_type{};
    template <> struct is_integral_helper<unsigned int>         : public true_type{};
    template <> struct is_integral_helper<unsigned long>        : public true_type{};
    template <> struct is_integral_helper<unsigned long long>   : public true_type{};
    template <> struct is_integral_helper<signed char>          : public true_type{};
    template <> struct is_integral_helper<signed short>         : public true_type{};
    template <> struct is_integral_helper<signed int>           : public true_type{};
    template <> struct is_integral_helper<signed long>          : public true_type{};
    template <> struct is_integral_helper<signed long long>     : public true_type{};
    template <> struct is_integral_helper<bool>                 : public true_type{};
    template <> struct is_integral_helper<char>                 : public true_type{};
    template <> struct is_integral_helper<wchar_t>              : public true_type{};

    template <typename T>
    struct is_integral : public corsac::is_integral_helper<typename corsac::remove_cv<T>::type>{};

    template <class T>
    constexpr bool is_integral_v = is_integral<T>::value;

    /**
    * is_floating_point
    *
    * is_floating_point<T>::value == true тогда и только тогда, когда T преднадлежит одному из:
    *    [const] [volatile] float
    *    [const] [volatile] double
    *    [const] [volatile] long double
    */
    template <typename T> struct is_floating_point_helper    : public false_type{};

    template <> struct is_floating_point_helper<float>       : public true_type{};
    template <> struct is_floating_point_helper<double>      : public true_type{};
    template <> struct is_floating_point_helper<long double> : public true_type{};

    template <typename T>
    struct is_floating_point : public corsac::is_floating_point_helper<typename corsac::remove_cv<T>::type>{};

    template <class T>
    constexpr bool is_floating_point_v = is_floating_point<T>::value;

    /**
    * is_arithmetic
    *
    * is_arithmetic<T>::value == true тогда и только тогда, когда:
    *    is_floating_point<T>::value == true, or
    *    is_integral<T>::value == true
    */
    template <typename T>
    struct is_arithmetic
            : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value> {};

    template<typename T>
    constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

    /**
    * is_fundamental
    *
    * is_fundamental<T>::value == true тогда и только тогда, когда:
    *    is_floating_point<T>::value == true, or
    *    is_integral<T>::value == true, or
    *    is_void<T>::value == true
    *    is_null_pointer<T>::value == true
    */
    template <typename T>
    struct is_fundamental
            : public bool_constant<is_void_v<T> || is_integral_v<T> || is_floating_point_v<T> || is_null_pointer_v<T>> {};

    template<typename T>
    constexpr bool is_fundamental_v = is_fundamental<T>::value;

    /**
    * is_hat_type
    *
    * is_hat_type<T>::value == тогда и только тогда, когда:
    *   базовый тип - это тип C++/CX '^', например: Foo^
    *	означает, что тип выделяется в куче и подсчитывается количество ссылок
    */
    template <typename T> struct is_hat_type_helper : public false_type {};

    template <typename T>
    struct is_hat_type : public corsac::is_hat_type_helper<T> {};

    template<typename T>
    constexpr bool is_hat_type_v = is_hat_type<T>::value;

}

#endif //CORSAC_TYPE_FUNDAMENTAL_H
