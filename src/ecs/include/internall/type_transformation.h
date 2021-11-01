//
// internal/type_transformation.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
#ifndef CORSAC_ENGINE_TYPE_TRANSFORMATION_H
#define CORSAC_ENGINE_TYPE_TRANSFORMATION_H

namespace corsac
{
    /**
    * add_pointer
    *
    * Добавить указатель на тип.
    * Предоставляет тип typedef члена, который является типом T.
    * Если T является ссылочным типом, то type является указателем на тип.
    */
    template<class T>
    struct add_pointer { typedef typename corsac::remove_reference<T>::type* type; };

    template <class T>
        using add_pointer_t = typename add_pointer<T>::type;

    /**
    * remove_extent
    *
    * Удаляет измерение из массива.
    * Для данного типа, не являющегося массивом T, remove_extent<T>::type эквивалентно T.
    * Для данного типа массива T[N], remove_extent<T[N]>::type эквивалентно T.
    * Для данного типа массива const T[N], remove_extent<const T[N]>::type эквивалентно const T.
    * Например, для многомерного массива типа T[M][N], remove_extent<T[M][N]>::type эквивалентно T[N].
    */
    template<class T>           struct remove_extent       { using type = T; };
    template<class T>           struct remove_extent<T[]>  { using type = T; };
    template<class T, size_t N> struct remove_extent<T[N]> { using type = T; };

    template <typename T>
    using remove_extent_t = typename remove_extent<T>::type;
}

#endif //CORSAC_ENGINE_TYPE_TRANSFORMATION_H
