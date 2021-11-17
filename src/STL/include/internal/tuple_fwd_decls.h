/**
 * corsac::STL
 *
 * internal/integer_sequence.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_TUPLE_FWD_DECLS_H
#define CORSAC_TUPLE_FWD_DECLS_H

#pragma once

namespace corsac
{
    template <typename... T>
    class tuple;

    template <typename Tuple>
    class tuple_size;

    template <size_t I, typename Tuple>
    class tuple_element;

    template <size_t I, typename Tuple>
    using tuple_element_t = typename tuple_element<I, Tuple>::type;

    // const typename для tuple_element_t, когда кортеж или TupleImpl сам по себе не может быть const
    template <size_t I, typename Tuple>
    using const_tuple_element_t = typename conditional<
            corsac::is_lvalue_reference<tuple_element_t<I, Tuple>>::value,
            corsac::add_lvalue_reference_t<const corsac::remove_reference_t<tuple_element_t<I, Tuple>>>,
            const tuple_element_t<I, Tuple>
    >::type;

    template <size_t I, typename... Ts_>
    tuple_element_t<I, tuple<Ts_...>>& get(tuple<Ts_...>& t);

    template <size_t I, typename... Ts_>
    const_tuple_element_t<I, tuple<Ts_...>>& get(const tuple<Ts_...>& t);

    template <size_t I, typename... Ts_>
    tuple_element_t<I, tuple<Ts_...>>&& get(tuple<Ts_...>&& t);

    template <typename T, typename... ts_>
    T& get(tuple<ts_...>& t);

    template <typename T, typename... ts_>
    const T& get(const tuple<ts_...>& t);

    template <typename T, typename... ts_>
    T&& get(tuple<ts_...>&& t);
}

#endif //CORSAC_TUPLE_FWD_DECLS_H
