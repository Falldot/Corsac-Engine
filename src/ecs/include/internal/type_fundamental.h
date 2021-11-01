#ifndef CORSAC_TYPE_FUNDAMENTAL_H
#define CORSAC_TYPE_FUNDAMENTAL_H

#pragma once

#include "type_traits.h"

namespace corsac
{
    /**
     * integral_constant
     *
     * Это базовый класс для различных класификаторов типа, как
     * определено в C ++ 11. По сути, это служебный базовый
     * класс для определения свойств как констант
     * класса (value), так и типов (type).
     */
    template <typename T, T v>
    struct integral_constant
    {
        static constexpr T value    = v;
        using value_type            = T ;
        using type                  = integral_constant<T, v>;

        constexpr explicit operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    /**
     * true_type / false_type
     *
     * Это типы используемые при реализации type_traits.
     * Другие целочисленные типы констант могут быть определены,
     * например, основанные на int.
     */
    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    /**
     * is_integral
     *
     * is_integral<T>::value == true если T принадлежит одному их типов:
     */
    template <typename T> struct is_integral_helper : public false_type{};

    template <> struct is_integral_helper<unsigned char>      : public true_type{};
    template <> struct is_integral_helper<unsigned short>     : public true_type{};
    template <> struct is_integral_helper<unsigned int>       : public true_type{};
    template <> struct is_integral_helper<unsigned long>      : public true_type{};
    template <> struct is_integral_helper<unsigned long long> : public true_type{};

    template <> struct is_integral_helper<signed char>        : public true_type{};
    template <> struct is_integral_helper<signed short>       : public true_type{};
    template <> struct is_integral_helper<signed int>         : public true_type{};
    template <> struct is_integral_helper<signed long>        : public true_type{};
    template <> struct is_integral_helper<signed long long>   : public true_type{};

    template <> struct is_integral_helper<bool>               : public true_type{};
    template <> struct is_integral_helper<char>               : public true_type{};

    template <> struct is_integral_helper<wchar_t>        : public true_type{};

    template <typename T>
    struct is_integral : public corsac::is_integral_helper<typename corsac::remove_cv<T>::type>{};

} // namespace corsac

#endif //CORSAC_TYPE_FUNDAMENTAL_H
