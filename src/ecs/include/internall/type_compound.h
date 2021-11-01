//
// internal/type_compund.h
//
// Created by Falldot on 01.11.2021.
// Copyright (c) 2021 Corsac. All rights reserved.
//
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
    * is_nothrow_convertible
    *
    * https://en.cppreference.com/w/cpp/types/is_convertible
    *
    * template<typename From, typename To>
    * struct is_explicitly_convertible
    *     : public is_constructible<To, From> {};
    */
    // TODO is_nothrow_convertible



    /**
    * is_explicitly_convertible
    *
    * This sometime-seen extension trait is the same as is_constructible
    * and so we don't define it.
    *
    * template<typename From, typename To>
    * struct is_explicitly_convertible
    *     : public is_constructible<To, From> {};
    */
    // TODO is_explicitly_convertible

    /**
    * is_union
    *
    * is_union<T>::value == true тогда и только тогда, когда T union type.
    *
    * Невозможно определить, является ли тип объединением, без помощи компилятора.
    * Пользователь может заставить что-то оцениваться как объединение через CORSAC_DECLARE_UNION.
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


    /**
    * is_class
    *
    * is_class<T>::value == true тогда и только тогда, когда T класс или структура
    * (а не тип union).
    *
    * Без специальной помощи компилятора невозможно различить объединения и классы.
    * В результате is_class ошибочно оценит значение true для типов объединения.
    */
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

    /**
    * is_polymorphic
    *
    * is_polymorphic<T>::value == true тогда и только тогда, когда T класс или структура
    * который объявляет или наследует виртуальную функцию.
    * is_polymorphic может применяться только к полным типам.
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
        struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)>{};
    #else
        template <typename T>
        struct is_polymorphic_imp1
        {
            using t = typename remove_cv<T>::type;

            struct helper_1 : public t
            {
                helper_1();
                ~helper_1() throw();
                char pad[64];
            };

            struct helper_2 : public t
            {
                helper_2();
                virtual ~helper_2() throw();
                char pad[64];
            };

            static const bool value = (sizeof(helper_1) == sizeof(helper_2));
        };

        template <typename T>
        struct is_polymorphic_imp2{ static const bool value = false; };

        template <bool is_class>
        struct is_polymorphic_selector{ template <typename T> struct rebind{ using type = is_polymorphic_imp2<T>; }; };

        template <>
        struct is_polymorphic_selector<true>{ template <typename T> struct rebind{ using type = is_polymorphic_imp1<T>; }; };

        template <typename T>
        struct is_polymorphic_value{
            using selector = is_polymorphic_selector<is_class<T>::value>;
            using binder = typename selector::template rebind<T>;
            using imp_type = typename binder::type;
            static const bool value = imp_type::value;
        };

        template <typename T>
        struct is_polymorphic : public integral_constant<bool, is_polymorphic_value<T>::value>{};
    #endif

    template<typename T>
        constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

    /**
    * is_object
    *
    * is_object<T>::value == тогда и только тогда, когда:
    *    is_reference<T>::value == false, и
    *    is_function<T>::value == false, и
    *    is_void<T>::value == false
    *
    * Стандарт C ++, раздел 3.9p9, гласит:
    * «Тип объекта - это (возможно, cv-квалифицированный) тип, который не является типом функции,
    * не ссылочным типом и не неполным (за исключением не полностью определенного типа объекта).
    */
    template <typename T>
    struct is_object : public integral_constant<bool,
            !is_reference<T>::value && !is_void<T>::value && !is_function<T>::value
    >{};

    template<typename T>
        constexpr bool is_object_v = is_object<T>::value;

    /**
    * is_scalar
    *
    * is_scalar<T>::value == true тогда и только тогда, когда:
    *    is_arithmetic<T>::value == true, или
    *    is_enum<T>::value == true, или
    *    is_pointer<T>::value == true, или
    *    is_member_pointer<T>::value == true, или
    *    is_null_pointer<T>::value == true
    *
    */
    template <typename T>
    struct is_scalar : public integral_constant<bool,
            is_arithmetic<T>::value || is_enum<T>::value || is_pointer<T>::value ||
            is_member_pointer<T>::value ||
            is_null_pointer<T>::value> {};

    template <typename T> struct is_scalar<T*>                : public true_type {};
    template <typename T> struct is_scalar<T* const>          : public true_type {};
    template <typename T> struct is_scalar<T* volatile>       : public true_type {};
    template <typename T> struct is_scalar<T* const volatile> : public true_type {};

    template<typename T>
        constexpr bool is_scalar_v = is_scalar<T>::value;

    /**
    * is_compound
    *
    * Соединение означает что угодно, но только не фундаментальное. См. Стандарт C ++, раздел 3.9.2.
    *
    * is_compound<T>::value == true тогда и только тогда, когда:
    *    is_fundamental<T>::value == false
    *
    * is_compound<T>::value == тогда и только тогда, когда:
    *    is_floating_point<T>::value == false, и
    *    is_integral<T>::value == false, и
    *    is_void<T>::value == false
    *
    */
    template <typename T>
    struct is_compound : public integral_constant<bool, !is_fundamental<T>::value>{};

        template<typename T>
            constexpr bool is_compound_v = is_compound<T>::value;

    /**
    * decay
    *
    * Преобразует тип T в его распавшийся эквивалент. Это означает преобразование lvalue в rvalue,
    * преобразование массива в указатель, преобразование функции в указатель и удаление констант и volatile.
    * Это преобразование типа, молчаливо применяемое компилятором ко всем аргументам функции при передаче по значению.
    */
    template<typename T>
    struct decay
    {
        typedef typename corsac::remove_reference<T>::type U;

        typedef typename corsac::conditional<
                corsac::is_array<U>::value,
                typename corsac::remove_extent<U>::type*,
                typename corsac::conditional<
                        corsac::is_function<U>::value,
                        typename corsac::add_pointer<U>::type,
                        typename corsac::remove_cv<U>::type
                >::type
        >::type type;
    };

    template<typename T>
        using decay_t = typename decay<T>::type;

    /**
    * common_type
    *
    * Определяет общий тип для всех типов T ..., то есть тип, в который все T ...
    * могут быть неявно преобразованы.
    *
    * Предполагается, что это будет специализировано пользователем для случаев, когда это полезно.
    * Пример специализации:
    *     template <typename Class1, typename Class2>
    *     struct common_type<MyClass1, MyClass2>{ typedef MyBaseClassB type; };
    *
    * Тип элемента typedef должен быть определен в соответствии с п. 20.9.7.6, p3.
    * Все типы в пакете параметров T должны быть полными или (возможно cv) недействительными.
    * Программа может специализировать этот признак, если хотя бы один параметр шаблона
    * в специализации является определяемым пользователем типом.
    * Примечание. Такие специализации необходимы, когда требуется только явное преобразование аргументов шаблона.
    */
    template<typename... T>
    struct common_type;

    template<typename T>
    struct common_type<T>
    { using type = decay_t<T>; };
    // Вопрос: следует ли использовать здесь T или decay_t<T>?
    // Стандарт C++ 11, в частности (20.9.7.6, p3), указывает, что он должен быть без распада, но libc ++ использует распад.

    template<typename T, typename U>
    struct common_type<T, U>
    {
        // Тип третичного выражения устанавливается компилятором как общий тип для двух типов результатов.
        using type = decay_t<decltype(true ? declval<T>() : declval<U>())>;
    };

    template<typename T, typename U, typename... V>
    struct common_type<T, U, V...>
    { using type = typename common_type<typename common_type<T, U>::type, V...>::type; };

    template <typename... T>
        using common_type_t = typename common_type<T...>::type;

    /**
    * is_final
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
            struct is_final : public integral_constant<bool, __is_final(T)> {};
    #else
        template <typename T>
        struct is_final : public false_type {};
    #endif

    template<typename T>
        constexpr bool is_final_v = is_final<T>::value;

    /**
    * is_aggregate
    *
    * https://en.cppreference.com/w/cpp/language/aggregate_initialization
    *
    * Агрегат может быть одного из следующих типов:
    * - array
    * - class (struct or union), который:
    *     - нет частных или защищенных нестатических элементов данных
    *     - нет конструкторов, предоставленных пользователем (разрешены явно заданные по умолчанию или удаленные конструкторы)
    *     - нет предоставленных пользователем, унаследованных или явных конструкторов
    *         - (разрешены явно заданные по умолчанию или удаленные конструкторы)
    *     - нет virtual, private, or protected (since C++17)  базовых классов
    *     - нет virtual functions
    *     - нет инициализаторов по умолчанию
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
        struct is_aggregate : public integral_constant<bool, __is_aggregate(T)> {};
    #else
        template <typename T>
        struct is_aggregate : public false_type {};
    #endif

    template <typename T>
        constexpr bool is_aggregate_v = is_aggregate<T>::value;

} // namespace corsac

#endif //CORSAC_TYPE_COMPOUND_H
