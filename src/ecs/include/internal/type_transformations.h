//
// Created by surve on 31.10.2021.
//

#ifndef CORSAC_TYPE_TRANSFORMATIONS_H
#define CORSAC_TYPE_TRANSFORMATIONS_H

#pragma once

namespace corsac
{
    /**
     * remove_all_extents
     *
     * Признак преобразования remove_all_extents удаляет все измерения из массива.
     * Для данного типа массива T[N] remove_all_extents T[N]>::type эквивалентен T.
     * Для данного типа массива T[N], remove_all_extents<T[N]>::type эквивалентен T.
     * Для данного типа массива const T[N], remove_all_extents<const T[N]>::type эквивалентен const T.
     * Например, для многомерного массива типа T[M][N], remove_all_extents<T[M][N]>::type эквивалентен T.
     */
    template<typename T>           struct remove_all_extents       { using type = T; };
    template<typename T, size_t N> struct remove_all_extents<T[N]> { using type = typename corsac::remove_all_extents<T>::type; };
    template<typename T>           struct remove_all_extents<T[]>  { using type = typename corsac::remove_all_extents<T>::type; };

    template <typename T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

}

#endif //CORSAC_TYPE_TRANSFORMATIONS_H
