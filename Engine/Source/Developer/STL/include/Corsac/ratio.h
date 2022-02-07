/**
 * corsac::STL
 *
 * ratio.h
 *
 * Created by Falldot on 20.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_RATIO_H
#define CORSAC_STL_RATIO_H

#pragma once
/**
 * Описание (Falldot 20.11.2021)
 *
 * Реализует шаблон класса corsac::ratio который обеспечивает поддержку рациональной арифметики
 * во время компиляции. Каждый экземпляр этого шаблона точно представляет любое конечное
 * рациональное число, если его числитель Num и знаменатель Denom могут быть представлены как
 * константы времени компиляции типа intmax_t. Кроме того, Denom не может быть нулевым и не может
 * быть равным самому отрицательному значению. И числитель, и знаменатель автоматически
 * сокращаются до наименьших членов.
 *
 *      template <class R1, class R2> struct ratio_specialization
 *
 *      ratio_add	            R1 + R2
 *      ratio_divide	        R1 / R2
 *      ratio_equal	            R1 == R2
 *      ratio_greater	        R1 > R2
 *      ratio_greater_equal	    R1 >= R2
 *      ratio_less	            R1 < R2
 *      ratio_less_equal	    R1 <= R2
 *      ratio_multiply	        R1 * R2
 *      ratio_not_equal	        !(R1 == R2)
 *      ratio_subtract	        R1 - R2
 *
 */

#include "Corsac/Base/base.h"

namespace corsac
{
    // помощники по переполнению во время компиляции
#define CORSAC_RATIO_ABS(x)	((x) < 0 ? -(x) : (x))

    template <intmax_t X, intmax_t Y>
    struct AdditionOverFlow
    {
        static const bool c1 = (X <= 0 && 0 <= Y) || (Y < 0 && 0 < X);   // Истинно, если цифры имеют разные знаки.
        static const bool c2 = CORSAC_RATIO_ABS(Y) <= INTMAX_MAX - CORSAC_RATIO_ABS(X);
        static const bool value = c1 || c2;
    };

    template <intmax_t X, intmax_t Y>
    struct MultiplyOverFlow
    {
        static const bool value = (CORSAC_RATIO_ABS(X) <= (INTMAX_MAX / CORSAC_RATIO_ABS(Y)));
    };

    // ratio
    template <intmax_t N = 0, intmax_t D = 1>
    class ratio
    {
    public:
        static constexpr intmax_t num = N;
        static constexpr intmax_t den = D;
        using type = ratio<num, den>;
    };

    namespace internal
    {
        // gcd -- реализация на основе алгоритма евклида
        template <intmax_t X, intmax_t Y> struct gcd { static const intmax_t value = gcd<Y, X % Y>::value; };
        template <intmax_t X> struct gcd<X, 0>       { static const intmax_t value = X; };
        template <> struct gcd<0, 0>                 { static const intmax_t value = 1; };

        // lcm
        template<intmax_t X, intmax_t Y>
        struct lcm { static const intmax_t value = (X * (Y / gcd<X,Y>::value)); };

        // ct_add
        template <intmax_t X, intmax_t Y>
        struct ct_add
        {
            static_assert(AdditionOverFlow<X,Y>::value, "compile-time addition overflow");
            static const intmax_t value = X + Y;
        };

        // ct_sub
        template <intmax_t X, intmax_t Y>
        struct ct_sub
        {
            static_assert(AdditionOverFlow<X,-Y>::value, "compile-time addition overflow");
            static const intmax_t value = X - Y;
        };

        // ct_multi
        template <intmax_t X, intmax_t Y>
        struct ct_multi
        {
            static_assert(MultiplyOverFlow<X,Y>::value, "compile-time multiply overflow");
            static const intmax_t value = X * Y;
        };

        // ct_simplify
        template <class R1>
        struct ct_simplify
        {
            static const intmax_t divisor = internal::gcd<R1::num, R1::den>::value;
            static const intmax_t num = R1::num / divisor;
            static const intmax_t den = R1::den / divisor;

            using ratio_type = ratio<num, den>;
            using this_type  = ct_simplify<R1>;
        };

        template <intmax_t N1, intmax_t N2> intmax_t ct_add_v   = ct_add<N1, N2>::value;
        template <intmax_t N1, intmax_t N2> intmax_t ct_multi_v = ct_multi<N1, N2>::value;
        template <class R1, class R2> R2 ct_simplify_t          = ct_simplify<R1>::ratio_type;

        // ratio_add
        template <class R1, class R2>
        struct ratio_add
        {
            using type = typename ct_simplify
                    <
                            typename ratio
                                    <
                                            ct_add
                                                    <
                                                            ct_multi<R1::num, R2::den>::value,
                                                            ct_multi<R2::num, R1::den>::value
                                                    >::value,
                                            ct_multi<R1::den, R2::den>::value
                                    >::type
                    >::ratio_type;
        };

        // ratio_subtract
        template <class R1, class R2>
        struct ratio_subtract
        {
            using type = typename ct_simplify
                    <
                            typename ratio
                                    <
                                            ct_sub
                                                    <
                                                            ct_multi<R1::num, R2::den>::value,
                                                            ct_multi<R2::num, R1::den>::value
                                                    >::value,
                                            ct_multi<R1::den, R2::den>::value
                                    >::type
                    >::ratio_type;
        };

        // ratio_multiply
        template <class R1, class R2>
        struct ratio_multiply
        {
            using type = typename ct_simplify
                    <
                            typename ratio
                                    <
                                            ct_multi<R1::num, R2::num>::value,
                                            ct_multi<R1::den, R2::den>::value
                                    >::type
                    >::ratio_type;
        };

        // ratio_divide
        template <class R1, class R2>
        struct ratio_divide
        {
            using type =typename ct_simplify
                    <
                            typename ratio
                                    <
                                            ct_multi<R1::num, R2::den>::value,
                                            ct_multi<R1::den, R2::num>::value
                                    >::type
                    >::ratio_type;
        };

        // ratio_equal
        template <class R1, class R2>
        struct ratio_equal
        {
            using sr1_t = ct_simplify<R1>;
            using sr2_t = ct_simplify<R2>;

            static const bool value = (sr1_t::num == sr2_t::num) && (sr1_t::den == sr2_t::den);
        };

        // ratio_less
        template <class R1, class R2>
        struct ratio_less
        {
            static const bool value = (R1::num * R2::den) < (R2::num * R1::den);
        };
    } // namespace internal

    // ratio arithmetic
    template <class R1, class R2> using ratio_add      = typename internal::ratio_add<R1, R2>::type;
    template <class R1, class R2> using ratio_subtract = typename internal::ratio_subtract<R1, R2>::type;
    template <class R1, class R2> using ratio_multiply = typename internal::ratio_multiply<R1, R2>::type;
    template <class R1, class R2> using ratio_divide   = typename internal::ratio_divide<R1, R2>::type;

    // ratio comparison
    template <class R1, class R2> struct ratio_equal         : public integral_constant<bool, internal::ratio_equal<R1, R2>::value> {};
    template <class R1, class R2> struct ratio_not_equal     : public integral_constant<bool, !ratio_equal<R1, R2>::value> {};
    template <class R1, class R2> struct ratio_less          : public integral_constant<bool, internal::ratio_less<R1, R2>::value> {};
    template <class R1, class R2> struct ratio_less_equal    : public integral_constant<bool, !ratio_less<R2, R1>::value> {};
    template <class R1, class R2> struct ratio_greater       : public integral_constant<bool, ratio_less<R2, R1>::value> {};
    template <class R1, class R2> struct ratio_greater_equal : public integral_constant<bool, !ratio_less<R1, R2>::value> {};

    // using yocto = ratio<1, 1000000000000000000000000> ;  // не поддерживается, слишком большой для intmax_t
    // using zepto = ratio<1, 1000000000000000000000   > ;  // не поддерживается, слишком большой для intmax_t
    using atto  = ratio<1, 1000000000000000000      > ;
    using femto = ratio<1, 1000000000000000         > ;
    using pico  = ratio<1, 1000000000000            > ;
    using nano  = ratio<1, 1000000000               > ;
    using micro = ratio<1, 1000000                  > ;
    using milli = ratio<1, 1000                     > ;
    using centi = ratio<1, 100                      > ;
    using deci  = ratio<1, 10                       > ;
    using deca  = ratio<10, 1                       > ;
    using hecto = ratio<100, 1                      > ;
    using kilo  = ratio<1000, 1                     > ;
    using mega  = ratio<1000000, 1                  > ;
    using giga  = ratio<1000000000, 1               > ;
    using tera  = ratio<1000000000000, 1            > ;
    using peta  = ratio<1000000000000000, 1         > ;
    using exa   = ratio<1000000000000000000, 1      > ;
    // using zetta = ratio<1000000000000000000000, 1   > ;  // не поддерживается, слишком большой для intmax_t
    // using yotta = ratio<1000000000000000000000000, 1> ;  // не поддерживается, слишком большой для intmax_t
}

#endif //CORSAC_STL_RATIO_H
