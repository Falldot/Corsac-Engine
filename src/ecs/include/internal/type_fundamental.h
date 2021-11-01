#ifndef CORSAC_TYPE_FUNDAMENTAL_H
#define CORSAC_TYPE_FUNDAMENTAL_H

#pragma once

#include "type_traits.h"
#include "type_compound.h"

namespace corsac
{
    /**
     * is_void
     *
     * is_void<T>::value == true тогда и только тогда когда соответствует типу:
     * [const][volatile] void
     */
    template <typename T> struct is_void : public false_type{};

    template <> struct is_void<void>                : public true_type{};
    template <> struct is_void<void const>          : public true_type{};
    template <> struct is_void<void volatile>       : public true_type{};
    template <> struct is_void<void const volatile> : public true_type{};

    /**
    * is_function
    *
    * is_function<T>::value == true тогда и только тогда когда соответсвует function type.
    * Тип функции здесь не включает тип функции-члена.
    *
    */
    template <typename>
    struct is_function
            : public corsac::false_type {};

    template <typename ReturnValue, typename... ArgPack>
    struct is_function<ReturnValue (ArgPack...)>
            : public corsac::true_type {};

    template <typename ReturnValue, typename... ArgPack>
    struct is_function<ReturnValue (ArgPack..., ...)>
            : public corsac::true_type {};


    ///////////////////////////////////////////////////////////////////////
    // is_convertible
    //
    // Given two (possible identical) types From and To, is_convertible<From, To>::value == true
    // if and only if an lvalue of type From can be implicitly converted to type To,
    // or is_void<To>::value == true
    //
    // An instance of the type predicate holds true if the expression To to = from;, where from is an object of type From, is well-formed.
    //
    // is_convertible may only be applied to complete types.
    // Type To may not be an abstract type.
    // If the conversion is ambiguous, the program is ill-formed.
    // If either or both of From and To are class types, and the conversion would invoke
    // non-public member functions of either From or To (such as a private constructor of To,
    // or a private conversion operator of From), the program is ill-formed.
    //
    // Note that without compiler help, both is_convertible and is_base
    // can produce compiler errors if the conversion is ambiguous.
    // Example:
    //    struct A {};
    //    struct B : A {};
    //    struct C : A {};
    //    struct D : B, C {};
    //    is_convertible<D*, A*>::value; // Generates compiler error.
    ///////////////////////////////////////////////////////////////////////

    template<typename From, typename To, bool = corsac::is_void<From>::value || corsac::is_function<To>::value || corsac::is_array<To>::value >
    struct is_convertible_helper
    { static const bool value = corsac::is_void<To>::value; };

    template<typename From, typename To>
    class is_convertible_helper<From, To, false>
    {
        template<typename To1>
        static void ToFunction(To1);

        template<typename /*From1*/, typename /*To1*/>
        static corsac::no_type is(...);

        template<typename From1, typename To1>
        static decltype(ToFunction<To1>(corsac::declval<From1>()), corsac::yes_type()) is(int);

    public:
        static const bool value = sizeof(is<From, To>(0)) == 1;
    };

    template<typename From, typename To>
    struct is_convertible
            : public integral_constant<bool, is_convertible_helper<From, To>::value> {};


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

    /**
     * is_floating_point
     *
     * iis_floating_point<T>::value == true если T принадлежит одному их типов:
     *      [const] [volatile] float
     *      [const] [volatile] double
     *      [const] [volatile] long double
     */
    template <typename T> struct is_floating_point_helper    : public false_type{};

    template <> struct is_floating_point_helper<float>       : public true_type{};
    template <> struct is_floating_point_helper<double>      : public true_type{};
    template <> struct is_floating_point_helper<long double> : public true_type{};

    template <typename T>
    struct is_floating_point : public corsac::is_floating_point_helper<typename corsac::remove_cv<T>::type>{};

    /**
     * is_arithmetic
     *
     * is_arithmetic<T>::value == true если принадлежит:
     *      is_floating_point<T>::value == true, or
     *      is_integral<T>::value == true
     */
    template <typename T>
    struct is_arithmetic
            : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value>{};

    ///////////////////////////////////////////////////////////////////////
    // is_enum
    //
    // is_enum<T>::value == true if and only if T is an enumeration type.
    //
    ///////////////////////////////////////////////////////////////////////

    struct int_convertible{ int_convertible(int); };

    template <bool is_arithmetic_or_reference>
    struct is_enum_helper { template <typename T> struct nest : public is_convertible<T, int_convertible>{}; };

    template <>
    struct is_enum_helper<true> { template <typename T> struct nest : public false_type {}; };

    template <typename T>
    struct is_enum_helper2
    {
        using selector  = type_or<is_arithmetic<T>::value, is_reference<T>::value, is_class<T>::value>;
        using helper_t  = is_enum_helper<selector::value>;
        using ref_t     = typename add_reference<T>::type;
        using result    = typename helper_t::template nest<ref_t>;
    };

    template <typename T>
    struct is_enum : public integral_constant<bool, is_enum_helper2<T>::result::value>{};

    template <> struct is_enum<void> : public false_type {};
    template <> struct is_enum<void const> : public false_type {};
    template <> struct is_enum<void volatile> : public false_type {};
    template <> struct is_enum<void const volatile> : public false_type {};

} // namespace corsac

#endif //CORSAC_TYPE_FUNDAMENTAL_H
