/**
 * corsac::STL
 *
 * internal/integer_sequence.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_INTEGER_SEQUENCE_H
#define CORSAC_INTEGER_SEQUENCE_H

#pragma once

#include "type_traits.h"

namespace corsac
{
    // integer_sequence
    template <typename T, T... Ints>
    class integer_sequence
    {
    public:
        using value_type = T;
        static_assert(corsac::is_integral<T>::value, "corsac::integer_sequence can only be instantiated with an integral type");
        static constexpr size_t size() noexcept { return sizeof...(Ints); }
    };

    template <size_t N, typename IndexSeq>
    struct make_index_sequence_impl;

    template <size_t N, size_t... Is>
    struct make_index_sequence_impl<N, integer_sequence<size_t, Is...>>
    {
        typedef typename make_index_sequence_impl<N - 1, integer_sequence<size_t, N - 1, Is...>>::type type;
    };

    template <size_t... Is>
    struct make_index_sequence_impl<0, integer_sequence<size_t, Is...>>
    {
        typedef integer_sequence<size_t, Is...> type;
    };

    template <size_t... Is>
    using index_sequence = integer_sequence<size_t, Is...>;

    template <size_t N>
    using make_index_sequence = typename make_index_sequence_impl<N, integer_sequence<size_t>>::type;

    template <typename Target, typename Seq>
    struct integer_sequence_convert_impl;

    template <typename Target, size_t... Is>
    struct integer_sequence_convert_impl<Target, integer_sequence<size_t, Is...>>
    {
        using type = integer_sequence<Target, Is...>;
    };

    template <typename T, size_t N>
    struct make_integer_sequence_impl
    {
        using type = typename integer_sequence_convert_impl<T, make_index_sequence<N>>::type;
    };

    template <typename T, size_t N>
    using make_integer_sequence = typename make_integer_sequence_impl<T, N>::type;

    // Шаблон псевдонима помощника, который преобразует любой пакет параметров типа в последовательность индексов той же длины
    template<typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;

    namespace internal
    {

        template <typename T>
        struct integer_sequence_size_helper;

        template <typename T, T... Ints>
        struct integer_sequence_size_helper<corsac::integer_sequence<T, Ints...>> : public integral_constant<size_t, sizeof...(Ints)>
        {};

        template <typename T>
        struct integer_sequence_size : public integer_sequence_size_helper<corsac::remove_cv_t<T>>
        {};

        template <typename T>
        struct index_sequence_size : public integer_sequence_size_helper<corsac::remove_cv_t<T>>
        {};

        template <typename T>
        inline constexpr size_t integer_sequence_size_v = integer_sequence_size<T>::value;

        template <typename T>
        inline constexpr size_t index_sequence_size_v = index_sequence_size<T>::value;
    }
}

#endif //CORSAC_INTEGER_SEQUENCE_H
