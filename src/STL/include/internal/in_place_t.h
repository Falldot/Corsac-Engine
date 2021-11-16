/**
 * corsac::STL
 *
 * internal/integer_sequence.h
 *
 * Created by Falldot on 09.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_IN_PLACE_T_H
#define CORSAC_IN_PLACE_T_H

#pragma once

namespace corsac
{
    namespace internal
    {
        struct in_place_tag {};
        template <class> struct in_place_type_tag {};
        template <size_t> struct in_place_index_tag {};
    }

    struct in_place_tag
    {
        in_place_tag() = delete;

    private:
        explicit in_place_tag(internal::in_place_tag) {}
        friend inline in_place_tag internal_ConstructInPlaceTag();
    };

    // внутренняя фабричная функция для in_place_tag
    inline in_place_tag internal_ConstructInPlaceTag() { return in_place_tag(internal::in_place_tag{}); }

    using in_place_t = in_place_tag(&)(internal::in_place_tag);

    template <class T>
    using in_place_type_t = in_place_tag(&)(internal::in_place_type_tag<T>);

    template <size_t N>
    using in_place_index_t = in_place_tag(&)(internal::in_place_index_tag<N>);

    inline in_place_tag in_place(internal::in_place_tag) { return internal_ConstructInPlaceTag(); }

    template <class T>
    inline in_place_tag in_place(internal::in_place_type_tag<T>) { return internal_ConstructInPlaceTag(); }

    template <std::size_t I>
    inline in_place_tag in_place(internal::in_place_index_tag<I>) { return internal_ConstructInPlaceTag(); }

} // namespace corsac

#endif //CORSAC_IN_PLACE_T_H
