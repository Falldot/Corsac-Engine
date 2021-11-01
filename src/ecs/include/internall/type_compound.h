// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_TYPE_COMPOUND_H
#define CORSAC_TYPE_COMPOUND_H

#pragma once

#include "internall/base/base.h"

namespace corsac
{
    /**
    * extent
    *
    * extent<T, I>::value это целочисленный тип, представляющий количество элементов
    * в 1-м измерении типа массива T.
    *
    * Для данного типа массива T[N], extent<T[N]>::value == N.
    * Для данного типа многомерного массива T[M][N], extent<T[M][N], 0>::value == N.
    * Для данного типа многомерного массива T[M][N], extent<T[M][N], 1>::value == M.
    * Для данного типа массива T и данное измерение I где I >= rank<T>::value, extent<T, I>::value == 0.
    * Для данного типа массива неизвестного extent T[], extent<T[], 0>::value == 0.
    * Для данного не-массива типа T и произвольного измерения I, extent<T, I>::value == 0.
    */
    template<typename T, unsigned N>
    struct extent_help : public corsac::integral_constant<size_t, 0> {};

    template<typename T, unsigned I>
    struct extent_help<T[I], 0> : public corsac::integral_constant<size_t, I> {};

    template<typename T, unsigned N, unsigned I>
    struct extent_help<T[I], N> : public corsac::extent_help<T, N - 1> { };

    template<typename T, unsigned N>
    struct extent_help<T[], N> : public corsac::extent_help<T, N - 1> {};

    template<typename T, unsigned N = 0> // extent использует беззнаковый тип вместо size_t.
    struct extent : public corsac::extent_help<T, N> { };

    template<typename T, unsigned N = 0>
        constexpr auto extent_v = extent<T, N>::value;


    /**
     * is_array
     *
     * is_array<T>::value == true тогда и только тогда, когда T преднадлежит типу массива,
     * включая неограниченные типы массивов.
     */
    template<typename T>
    struct is_array : public corsac::false_type {};

    template<typename T>
    struct is_array<T[]> : public corsac::true_type {};

    template<typename T, size_t N>
    struct is_array<T[N]> : public corsac::true_type {};

    template<typename T>
    constexpr bool is_array_v = is_array<T>::value;

    /**
    * is_array_of_known_bounds
    *
    * Не входит в стандарт C++ 11.
    * is_array_of_known_bounds<T>::value true если T является массивом и имеет известные границы
    * is_array_of_unknown_bounds<int[3]>::value == true,
    * когда, как is_array_of_unknown_bounds<int[]>::value = false.
    */
    template<typename T>
    struct is_array_of_known_bounds
            : public corsac::integral_constant<bool, corsac::extent<T>::value != 0> {};

    /**
    * is_array_of_known_bounds
    *
    * Не входит в стандарт C++ 11.
    * is_array_of_unknown_bounds<T>::value если T является массивом c неизвестными границами
    * is_array_of_unknown_bounds<int[3]>::value == false,
    * когда, как is_array_of_unknown_bounds<int[]>::value = true.
    */
    template<typename T>
    struct is_array_of_unknown_bounds
            : public corsac::integral_constant<bool, corsac::is_array<T>::value && (corsac::extent<T>::value == 0)> {};

    /**
    * is_member_function_pointer
    *
    * is_member_function_pointer<T>::value == true тогда и только тогда, когда T
    * является указателем на тип функции-члена.
    *
    * Мы обнаруживаем функции-члены с аргументами от 0 до N
    * При необходимости мы можем расширить это за дополнительные аргументы.
    */
    template <typename T> struct is_mem_fun_pointer_value : public false_type{};

    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)()> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() const> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() volatile> : public true_type{};
    template <typename R, typename T> struct is_mem_fun_pointer_value<R (T::*)() const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0)> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) const> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0> struct is_mem_fun_pointer_value<R (T::*)(Arg0) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const volatile> : public true_type{};

    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) volatile> : public true_type{};
    template <typename R, typename T, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7> struct is_mem_fun_pointer_value<R (T::*)(Arg0, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const volatile> : public true_type{};

    template <typename T>
    struct is_member_function_pointer : public integral_constant<bool, is_mem_fun_pointer_value<T>::value>{};

    template<typename T>
        constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    /**
    * is_member_pointer
    *
    * is_member_pointer<T>::value == true тогда и только тогда, когда:
    *    is_member_object_pointer<T>::value == true, или
    *    is_member_function_pointer<T>::value == true
    *
    */
    template <typename T>
    struct is_member_pointer
            : public corsac::integral_constant<bool, corsac::is_member_function_pointer<T>::value>{};

    template <typename T, typename U>
    struct is_member_pointer<U T::*>
            : public corsac::true_type{};

    template<typename T>
    constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    /**
    * is_member_object_pointer
    *
    * is_member_object_pointer<T>::value == true тогда и только тогда, когда T:
    * указатель на тип элемента данных.
    *
    */

    template<typename T>
    struct is_member_object_pointer : public corsac::integral_constant<bool,
            corsac::is_member_pointer<T>::value &&
            !corsac::is_member_function_pointer<T>::value
    > {};
    template<typename T>
        constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

    /**
    * is_pointer
    *
    * is_pointer<T>::value == true true тогда и только тогда, когда T это тип указателя.
    * В эту категорию входят типы указателей на функции,
    * но не указатели на типы членов.
    *
    */

    template <typename T> struct is_pointer_helper : public false_type{};

    template <typename T> struct is_pointer_helper<T*>                : public true_type{};
    template <typename T> struct is_pointer_helper<T* const>          : public true_type{};
    template <typename T> struct is_pointer_helper<T* volatile>       : public true_type{};
    template <typename T> struct is_pointer_helper<T* const volatile> : public true_type{};

    template <typename T>
    struct is_pointer_value : public type_and<is_pointer_helper<T>::value, type_not<is_member_pointer<T>::value>::value> {};

    template <typename T>
    struct is_pointer : public integral_constant<bool, is_pointer_value<T>::value>{};

    template<typename T>
        constexpr bool is_pointer_v = is_pointer<T>::value;

    /**
    * is_union
    *
    * is_union<T>::value == true тогда и только тогда, когда T union type.
    *
    * Невозможно определить, является ли тип объединением, без помощи компилятора.
    * Пользователь может заставить что-то оцениваться как объединение через CORSAC_DECLARE_UNION.
    *
    */
    #if defined(CORSAC_COMPILER_MSVC)
            template <typename T>
            struct is_union : public integral_constant<bool, __is_union(T)>{};
    #else
        template <typename T> struct is_union : public false_type{};
    #endif

    #define CORSAC_DECLARE_UNION(T) namespace corsac{ template <> struct is_union<T> : public true_type{}; template <> struct is_union<const T> : public true_type{}; }

    template<typename T>
         constexpr bool is_union_v = is_union<T>::value;


    ///////////////////////////////////////////////////////////////////////
    // is_class
    //
    // is_class<T>::value == true if and only if T is a class or struct
    // type (and not a union type).
    //
    // Without specific compiler help, it is not possible to
    // distinguish between unions and classes. As a result, is_class
    // will erroneously evaluate to true for union types.
    ///////////////////////////////////////////////////////////////////////
    #if defined(CORSAC_COMPILER_MSVC)
		template <typename T>
		struct is_class : public integral_constant<bool, __is_class(T)>{};
    #else
        template <typename U> static yes_type is_class_helper(void (U::*)());
        template <typename U> static no_type  is_class_helper(...);

        template <typename T>
        struct is_class : public integral_constant<bool,
                sizeof(is_class_helper<T>(0)) == sizeof(yes_type) && !is_union<T>::value
        >{};
    #endif

    template<typename T>
    constexpr bool is_class_v = is_class<T>::value;


    /**
    * is_convertible
    *
    * Учитывая два (возможных идентичных) типа From и To, is_convertible<From, To>::value == true
    * тогда и только тогда, когда lvalue типа From может быть неявно преобразовано в тип To,
    * или is_void<To>::value == true
    *
    * Экземпляр предиката типа имеет значение true, если выражение To = from; где from - объект типа From, правильно сформировано.
    *
    * is_convertible может применяться только к полным типам.
    * Тип To не может быть абстрактным типом.
    * Если преобразование неоднозначно, программа сформирована неправильно.
    * Если один или оба From и To являются типами классов и преобразование вызовет закрытые функции-члены From или To
    * (например, частный конструктор To или частный оператор преобразования From), программа плохо работает.
    *
    * Обратите внимание, что без помощи компилятора как is_convertible,
    * так и is_base могут вызывать ошибки компилятора, если преобразование неоднозначно.
    * Пример:
    *    struct A {};
    *    struct B : A {};
    *    struct C : A {};
    *    struct D : B, C {};
    *    is_convertible<D*, A*>::value; // Генерирует ошибку компилятора.
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename From, typename To>
        struct is_convertible : public integral_constant<bool, __is_convertible_to(From, To)>{};
    #else
        template<typename From, typename To, bool = corsac::is_void<From>::value || corsac::is_function<To>::value || corsac::is_array<To>::value >
        struct is_convertible_helper // Все может быть аннулировано. Ничто не может быть преобразовано в функцию или массив.
        { static const bool value = corsac::is_void<To>::value; };

        template<typename From, typename To>
        class is_convertible_helper<From, To, false>
        {
            template<typename To1>
            static void ToFunction(To1); // Мы пытаемся вызвать эту функцию с экземпляром From. Допустимо, если From можно преобразовать в To.

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
    #endif

    template<typename From, typename To>
    constexpr bool is_convertible_v = is_convertible<From, To>::value;

    /**
    * is_enum
    *
    * is_enum<T>::value == true тогда и только тогда, когда T преднадлежит перечисляемому типу.
    *
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
                    struct is_enum : public integral_constant<bool, __is_enum(T)>{};
    #else
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

        template <> struct is_enum<void>                : public false_type {};
        template <> struct is_enum<void const>          : public false_type {};
        template <> struct is_enum<void volatile>       : public false_type {};
        template <> struct is_enum<void const volatile> : public false_type {};
    #endif

    template<typename T>
    constexpr bool is_enum_v = is_enum<T>::value;

    #define CORSAC_DECLARE_ENUM(T) namespace corsac{ template <> struct is_enum<T> : public true_type{}; template <> struct is_enum<const T> : public true_type{}; }
}

#endif //CORSAC_TYPE_COMPOUND_H
