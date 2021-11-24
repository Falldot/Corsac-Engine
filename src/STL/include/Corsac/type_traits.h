/**
 * corsac::STL
 *
 * type_traits.h
 *
 * Created by Falldot on 01.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */

/**
 * Описание (Falldot 07.11.2021)
 *
 * Реализация здесь почти полностью основана на метапрограммировании шаблонов.
 * То есть, обозначает, использование функциональности шаблонов для определения
 * и преобразования типов на стадии компиляции вашей программы. API и алгоритмы
 * обновляеться с учетом последних версий библиотек corsac и Boost, а также в зависимости от
 * требований Corsac Engine. Мы здесь делаем сильный упор в возможности компилятора,
 * поэтому функциональность может меняться от одного компилятора к другому.
 *
 * Компиляторы:
 *      - g++       v10.3.0
 *      - clang++   v12.0.1
 *      - MSVC      v16.11.5
 *
 *  Trait                                            Description
 * ---------------------------------------------------------------------------------------------------
 * === Определители типов:
 *
 *    is_void<T>                                T преднадлежит типу void.
 *    is_null_pointer<T>                        Т принадлежит типу nullptr_t.
 *    is_integral<T>                            T принадлежит целому чесловому типу.
 *    is_floating_point<T>                      T принадлежит типу числа с плавающей точкой.
 *    is_array<T>                               T принадлежит типу массива.
 *    is_enum<T>                                T принадлежит типу перечисления.
 *    is_union<T>                               T принадлежит типу объединения.
 *    is_object<T>                              T принадлежит типу объекта.
 *    is_class<T>                               T принадлежит типу класса или структуры.
 *    is_function<T>                            T принадлежит типу функции.
 *
 *    is_signed<T>                              T являеться целочисленным типом со знаком.
 *    is_unsigned<T>                            T являеться целочисленным типом без знака.
 *
 *    is_pointer<T>                             T являеться указателем на тип. Включая указатель на функцию.
 *    is_member_object_pointer<T>               T являеться указателем на объект.
 *    is_member_function_pointer<T>             T являеться указателем на функцию.
 *    is_reference<T>                           T являеться ссылочным типом. Включая ссылку на функцию.
 *    is_rvalue_reference<T>                    T являеться ссылочным типом rvalue.
 *    is_lvalue_reference<T>                    T являеться ссылочным типом lvalue.
 *
 *    is_fundamental<T>                         T является фундаментальным типом (void, integral или floating).
 *    is_arithmetic<T>                          T является арифметическим типом (integral или floating).
 *    is_scalar<T>                              T является скалярным типом (arithmetic, enum, pointer, member_pointer)
 *    is_compound<T>                            T являеться вычесляемым типом (fundamental).
 *    is_member_pointer<T>                      T является указателем на экземпляр или экземпляр функции.
 *
 *    is_const<T>                               T определен как const.
 *    is_volatile<T>                            T определен как volatile.
 *
 *    is_trivial<T>                             T является тривиальным типом (scalar, trivially copyable, trivially constructible).
 *    is_trivially_copyable<T>                  Т являеться тривиально копируемым типом (полным типом или массивом с неизвестной границей).
 *    is_trivially_constructible<T>             Т владеет тривиальным конструктором (все аргументы полного типа, или/и void, или/и массивы с неизвестной границей).
 *    is_standard_layout<T>                     Т являеться типом стандартной компоновки (Standard Layout Type), которая не содержит специальных возможностей C++.
 *    is_pod<T>                                 T являеться pod (Plain Old Data) типом (scalar, trivially, is_standard_layout). Совместим с C.
 *
 *    is_empty<T>                               T являеться пустым классом или структурой.
 *    is_polymorphic<T>                         T являеться полиморфным классом или структурой (объявляет или наследует хотя бы одну виртуальную функцию).
 *    is_abstract<T>                            T являеться абстрактным классом или структурой (объявляет или наследует хотя бы одну чистую виртуальную функцию).
 *
 *    is_same<T, U>                             T эквивалентен U.
 *    is_base_of<T, U>                          T являеться производным U.
 *    is_convertible<T, U>                      Т возможно неявно преобразовать в U.
 *
 *    is_default_constructible<T>               T владеет конструктором по умолчанию
 *    is_constructible<T, Args...>              T владеет конструтором определяюший набор Args типов.
 *    is_nothrow_constructible<T, Args...>      T владеет конструктором определяюший набор Args типов не генерирующий исключения.
 *    is_trivially_default_constructible<T>     T владеет тривиальным конструктором по умолчанию.
 *    is_nothrow_default_constructible<T>       T владеет тривиальным конструктором по умолчанию не генерирующий исключения.
 *
 *    is_copy_constructible<T>                  T владеет конструктором для копирования.
 *    is_trivially_copy_constructible<T>        T владеет тревиальным конструктором для копирования.
 *    is_nothrow_copy_constructible<T>          T владеет не генерирующим исключения конструктором для копирования.
 *
 *    is_move_constructible<T>                  T владеет конструктором для переноса.
 *    is_trivially_move_constructible<T>        T владеет тревиальным конструктором для переноса.
 *    is_nothrow_move_constructible<T>          T владеет не генерирующим исключения конструктором для переноса.
 *
 *    is_assignable<T, U>                       T может быть присвоен из U.
 *    is_trivially_assignable<T, U>             T может быть, тривиально, присвоен из U.
 *    is_nothrow_assignable<T, U>               T может не генерируя исключения, быть присвоен из U.
 *    is_copy_assignable<T, U>                  T может быть присваиваемым при копировании из U.
 *    is_trivially_copy_assignable<T, U>        T может быть, тривиально, присваиваемым при копировании из U.
 *    is_nothrow_copy_assignable<T, U>          T может не генерируя исключения, быть присваиваемым при копировании из U.
 *    is_move_assignable<T, U>                  T может быть присваиваемым при перемещении из U.
 *    is_trivially_move_assignable<T, U>        T может быть, тривиально, присваиваемым при перемещении из U.
 *    is_nothrow_move_assignable<T, U>          T может не генерируя исключения, быть присваиваемым при перемещении из U.
 *
 *    is_destructible<T>                        T владеет диструктором.
 *    is_trivially_destructible<T>              T владеет тривиальным диструктором.
 *    is_nothrow_destructible<T>                T владеет диструктором не генерирующий исключения.
 *    has_virtual_destructor<T>                 T владеет вируальным диструктором.
 *
 *
 *    alignment_of<T>
 *      Целочисленное значение, представляющее количество байтов выравнивания объектов типа T;
 *      объект типа T может быть размещен по адресу, кратному его выравниванию.
 *
 *    rank<T>
 *      Целочисленное значение, представляющее ранг объектов типа T.
 *      Термин "ранг" здесь используется для описания количества измерений типа массива.
 *
 *    extent<T, i>
 *      Целочисленное значение, представляющее протяженность (размерность) i-й границы объектов типа T.
 *      Если тип T не является типом массива, имеет ранг меньше I или если I == 0 и T имеет тип массив
 *      неизвестной границы U, тогда значение должно быть равно нулю;
 *      в противном случае value будет оцениваться как количество элементов в i-й границе массива T.
 *      Термин «степень» здесь используется для описания количества элементов в типе массива.
 *
 * === Преобразователи типов:
 *
 *    remove_cv<T>                              T переопределяеться без const и volatile.
 *    remove_const<T>                           T переопределяеться без const.
 *    remove_volatile<T>                        T переопределяеться без volatile.
 *    add_cv<T>                                 T переопределяеться как const и volatile.
 *    add_const<T>                              T переопределяеться как const.
 *    add_volatile<T>                           T переопределяеться как volatile.
 *
 *    remove_reference<T>                       Возвращает тип на который ссылался Т.
 *    add_lvalue_reference<T>                   Возвращает lvalue Т.
 *    add_rvalue_reference<T>                   Возвращает rvalue Т.
 *
 *    remove_pointer<T>                         Возвращает тип на который указывал Т.
 *    add_pointer<T>                            Возвращает указатель на Т.
 *
 *    make_signed<T>                            Возвращет соответстующий Т, знаковый целочисленный тип.
 *    make_unsigned<T>                          Возвращет соответстующий Т, беззнаковый целочисленный тип.
 *
 *    remove_extent<T>                          Удаляет измерение из массива.
 *    remove_all_extents<T>                     Удаляет все измерение из массива.
 *
 *    aligned_storage<T, U>                     Возвращает статический массив Т размера, выровненный по U значению.
 *    aligned_union<T, U>                       Возвращает объединение Т размера, выровненный по U типу.
 *    decay<T>                                  Преобразование lvalue в rvalue, массива в указатель, функции в указатель.
 *    conditional<T, U, I>                      Возращает U, если выражение T true или I, если T false.
 *    common_type<T...>                         Определяет общий тип одного или нескольких типов.
 *    underlying_type<T>                        Создает базовый целочисленный тип для типа перечисления.
 *    result_of<T, Args...>                     Определяет возвращаемый тип вызываемого типа, который принимает заданные типы аргументов.
 *    identity<T>                               Просто устанавливает T как тип.
 *
 * === Служебный типы:
 *
 *    disable_if
 *    enable_if
 *    integral_constant
 *    bool_constant
 *    true_type
 *    false_type
 *
 * === Взаимствование с eastl:
 *
 *    is_aligned<T>                             Определяется как истина, если у типа требования к выравниванию выше, чем выравнивание по умолчанию, которое принимается равным 8.
 *    union_cast<sT, dT>(dV)                    Обеспечивает удобное для чтения приведение между типами, которые не связаны между собой, но имеют двоичную эквивалентность. Классический вариант использования - преобразование между представлениями битов float и int32_t.
 *    is_array_of_known_bounds<T>               Т являеться массивом с известными границами.
 *    is_array_of_unknown_bounds<T>             Т являеться массивом с неизвестными границами.
 *    add_reference<T>                          Возвращает ссылку на Т.
 *
 *    yes_type                                  Служебный тип.
 *    no_type                                   Служебный тип.
 *
 *    is_swappable                              <STL/utility.h>
 *    is_nothrow_swappable                      "
 */

#ifndef CORSAC_TYPE_TRAITS_H
#define CORSAC_TYPE_TRAITS_H

#pragma once

#include "internal/config.h"
#include <stddef.h> // Требуется для использования size_t

namespace corsac
{
   /**
     * integral_constant
     *
     * Это базовый класс для различных класификаторов типа, как
     * определено в C++ 11. По сути, это служебный базовый
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
     * bool_constant
     *
     * Это удобный помощник для часто используемого интегрального_константа <bool, value>.
     */
    template <bool B>
        struct bool_constant : public integral_constant<bool, B> {};

    /**
     * yes_type / no_type
     *
     * Они используются как утилита для различения двух целей.
     */
    using           yes_type = char;               // sizeof(yes_type) == 1
    struct          no_type { char padding[8]; };  // sizeof(no_type)  != 1

    /**
     * unused
     *
     * Используется внутри для обозначения специального аргумента шаблона, который означает,
     * что это неиспользуемый аргумент.
     */
    struct unused { };

    /**
     * argument_sink
     *
     * Используется как тип, который строится из чего угодно.
     *
     * Для компиляторов, которые поддерживают вариативные шаблоны,
     * мы предоставляем альтернативный параметр argument_sink,
     * который обеспечивает перегрузку конструктора вариативного пакета аргументов по ссылке.
     * Это позволяет избежать проблем с выравниванием объектов, которые не соблюдаются в компиляторах Microsoft.
     * Видно в предварительном просмотре VS2015. В общем, поскольку аргументы потребляются и игнорируются,
     * потреблять ссылки дешевле, чем передавать по значению, что влечет за собой затраты на построение.
     */
    struct argument_sink
    {
        template<typename... Args>
        argument_sink(Args&&...) {}
    };

    /**
     * type_select
     *
     * Это используется для объявления типа из одного из двух вариантов типа.
     * Результат зависит от типа условия.
     * Это имеет определенные применения в метапрограммировании шаблонов.
     *
     * Пример:
     *    using ChosenType = type_select_t<is_integral_v<SomeType>, ChoiceAType, ChoiceBType>;
     */
    template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
    struct type_select { using type = ConditionIsTrueType; };

    template <typename ConditionIsTrueType, class ConditionIsFalseType>
    struct type_select<false, ConditionIsTrueType, ConditionIsFalseType> { using type = ConditionIsFalseType; };

    template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
        using type_select_t = typename type_select<bCondition, ConditionIsTrueType, ConditionIsFalseType>::type;

    /**
     * first_type_select
     *
     * Подобно type_select, но в одностороннем порядке выбирает первый тип.
     */
    template <typename T, typename = corsac::unused, typename = corsac::unused>
    struct first_type_select { using type = T; };

    /**
     * type_and
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true>
    struct type_and;

    template <bool b1, bool b2, bool b3, bool b4, bool b5>
    struct type_and{ static const bool value = false; };

    template <>
    struct type_and<true, true, true, true, true>{ static const bool value = true; };

    /**
     * type_equal
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <int b1, int b2>
    struct type_equal{ static const bool value = (b1 == b2); };

    /**
     * type_not_equal
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <int b1, int b2>
    struct type_not_equal{ static const bool value = (b1 != b2); };

    /**
     * type_not
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <bool b>
    struct type_not{ static const bool value = true; };

    template <>
    struct type_not<true>{ static const bool value = false; };

    /**
     * type_or
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false>
    struct type_or;

    template <bool b1, bool b2, bool b3, bool b4, bool b5>
    struct type_or { static const bool value = true; };

    template <>
    struct type_or<false, false, false, false, false> { static const bool value = false; };

    /**
     * enable_if, disable_if
     *
     * template <bool B, typename T = void> struct enable_if;
     * template <bool B, typename T = void> struct disable_if;
     */
    template<bool B, typename T = void>
    struct enable_if {};

    template <typename T>
    struct enable_if<true, T> { using type = T; };

    template <bool B, class T = void>
		using enable_if_t = typename enable_if<B, T>::type;

    template<bool B, typename T = void>
    struct disable_if {};

    template <typename T>
    struct disable_if<false, T> { using type = T; };

    template <bool B, class T = void>
		using disable_if_t = typename disable_if<B, T>::type;

    /**
     * conditional
     *
     * Предоставляет тип using члена, который определяется как T если B true
     * во время компиляции, или как F если B false.
     */
    template<bool B, typename T, typename F>
    struct conditional { using type = T; };

    template <typename T, typename F>
    struct conditional<false, T, F> { using type = F; };

    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;

    /**
     * conjunction
     *
     * Это стандартный служебный класс C++ 17, который выполняет короткое замыкание
     * логического И над последовательностью признаков типа.
     */
    template <class...>
    struct conjunction : corsac::true_type {};

    template <class B>
    struct conjunction<B> : B {};

    template <class B, class... Bn>
    struct conjunction<B, Bn...> : conditional<bool(B::value), conjunction<Bn...>, B>::type {};

    template <typename... Bn>
        inline constexpr bool conjunction_v = conjunction<Bn...>::value;

    /**
     * disjunction
     *
     * Это стандартный служебный класс C++ 17, который выполняет короткое замыкание
     * логического ИЛИ над последовательностью признаков типа.
     */
    template <class...>
    struct disjunction : corsac::false_type {};

    template <class B>
    struct disjunction<B> : B {};

    template <class B, class... Bn>
    struct disjunction<B, Bn...> : conditional<bool(B::value), B, disjunction<Bn...>>::type {};

    template <typename... B>
        inline constexpr bool disjunction_v = disjunction<B...>::value;

    /**
     * negation
     *
     * Это стандартный служебный класс C++ 17, который выполняет
     * логическое НЕ для одного признака типа.
     */
    template <class B>
    struct negation : corsac::bool_constant<!bool(B::value)> {};

    template <typename B>
        inline constexpr bool negation_v = negation<B>::value;

    /**
     * type_identity
     *
     * Обычно это делается для работы с невыявленными контекстами шаблонов
     * См. Стандарт C++ 11, 14.8.2.5 p5.
     * Также: http://cppquiz.org/quiz/question/109?result=CE&answer=&did_answer=Answer
     *
     * https://en.cppreference.com/w/cpp/types/type_identity
     */
    template <typename T>
    struct type_identity { using type = T; };

    template <typename T>
        using type_identity_t = typename type_identity<T>::type;

    /**
     * is_same
     *
     * Для двух (возможно, идентичных) типов T и U, is_same<T, U>::value == true
     * тогда и только тогда, когда T и U относятся к одному типу.
     */
    template <typename T, typename U>
    struct is_same : public corsac::false_type { };

    template <typename T>
    struct is_same<T, T> : public corsac::true_type { };

    template <class T, class U>
        constexpr bool is_same_v = is_same<T, U>::value;

    /**
     * is_const
     *
     * is_const<T>::value == true тогда и только тогда, когда T имеет квалификацию const.
     *
     */
    template <typename T> struct is_const_value                    : public corsac::false_type{};
    template <typename T> struct is_const_value<const T *>          : public corsac::true_type{};
    template <typename T> struct is_const_value<const volatile T *> : public corsac::true_type{};

    template <typename T> struct is_const : public corsac::is_const_value<T *>{};
    // Обратите внимание, что T - это const, а не ссылка на T. Итак, is_const - false. См. Раздел 8.3.2p1 стандарта C++.
    template <typename T> struct is_const<T&> : public corsac::false_type{};

    template <class T>
        constexpr bool is_const_v = is_const<T>::value;

    /**
     * is_volatile
     *
     * is_volatile<T>::value == true тогда и только тогда, когда T имеет квалификацию volatile.
     *
     */
    template <typename T> struct is_volatile_value                    : public corsac::false_type{};
    template <typename T> struct is_volatile_value<volatile T *>       : public corsac::true_type{};
    template <typename T> struct is_volatile_value<const volatile T *> : public corsac::true_type{};

    template <typename T> struct is_volatile : public corsac::is_volatile_value<T *>{};
    template <typename T> struct is_volatile<T&> : public corsac::false_type{}; // Обратите внимание, что T является изменчивым, а не ссылкой на T. Таким образом, is_const является ложным. См. Раздел 8.3.2p1 стандарта C++.

    template <class T>
        constexpr bool is_volatile_v = is_volatile<T>::value;

    /**
     * is_reference
     *
     * is_reference<T>::value == true тогда и только тогда, когда T являеться ссылочным типом (Ссылка l-значения или ссылка r-значения).
     * В эту категорию входят ссылки на типы функций.
     */
    template <typename T> struct is_reference      : public corsac::false_type{};
    template <typename T> struct is_reference<T&>  : public corsac::true_type{};
    template <typename T> struct is_reference<T&&> : public corsac::true_type{};

    template<typename T>
    constexpr bool is_reference_v = is_reference<T>::value;

    /**
     * is_function
     *
     * is_function<T>::value == true  тогда и только тогда, когда T преднадлежит типу функции.
     * Тип функции здесь не включает тип функции-члена.
     */
    template <typename>
    struct is_function
            : public corsac::false_type {};

    #if CORSAC_PLATFORM_PTR_SIZE == 4 && defined(CORSAC_PLATFORM_MICROSOFT)
        // __cdecl specialization
        template <typename ReturnValue, typename... ArgPack>
        struct is_function<ReturnValue __cdecl (ArgPack...)>
            : public corsac::true_type {};

        template <typename ReturnValue, typename... ArgPack>
        struct is_function<ReturnValue __cdecl (ArgPack..., ...)> // Второй многоточие обрабатывает случай функции, которая принимает многоточие, например printf.
            : public corsac::true_type {};

        // __stdcall specialization
        template <typename ReturnValue, typename... ArgPack>
        struct is_function<ReturnValue __stdcall (ArgPack...)>
            : public corsac::true_type {};

    #else
        template <typename ReturnValue, typename... ArgPack>
        struct is_function<ReturnValue (ArgPack...)>
                : public corsac::true_type {};

        template <typename ReturnValue, typename... ArgPack>
        struct is_function<ReturnValue (ArgPack..., ...)> // Второй многоточие обрабатывает случай функции, которая принимает многоточие, например printf.
                : public corsac::true_type {};
    #endif

        template<typename T>
        constexpr bool is_function_v = is_function<T>::value;


    /**
     * remove_const
     *
     * Удаляет const из типа.
     *
     * remove_const преобразователь типа, который удаляет const
     * верхнего уровня (при наличии) из типа, к которому оно применимо.
     * Для типа T, remove_const<T const>::type эквивалентен.
     * Например, remove_const<char *>::type эквивалентен типу char * a,
     * remove_const<const char *>::type эквивалентен const char *.
     * В последнем примере, квалификатор const изменяет char, без  *,
     * и поэтому не находится на верхнем уровне.
     */
    template <typename T>           struct remove_const             { using type = T;    };
    template <typename T>           struct remove_const<const T>    { using type = T;    };
    template <typename T>           struct remove_const<const T[]>  { typedef T type[];  };
    template <typename T, size_t N>  struct remove_const<const T[N]> { typedef T type[N]; };

    template<typename T>
        using remove_const_t = typename remove_const<T>::type;

    /**
     * remove_volatile
     *
     * Удаляет volatile из типа.
     *
     * remove_volatile преобразователь типа, который удаляет volatile
     * верхнего уровня (при наличии) из типа, к которому оно применимо.
     * Для типа T, remove_volatile<T volatile>::type эквивалентен.
     * Например, remove_volatile<char *>::type эквивалентен типу char * a,
     * remove_volatile<volatile char *>::type эквивалентен volatile char *.
     * В последнем примере, квалификатор const изменяет char, без  *,
     * и поэтому не находится на верхнем уровне.
     */
    template <typename T>               struct remove_volatile                { using type = T;   };
    template <typename T>               struct remove_volatile<volatile T>    { using type = T;   };
    template <typename T>               struct remove_volatile<volatile T[]>  { typedef T type[]; };
    template <typename T, size_t N>     struct remove_volatile<volatile T[N]> { typedef T type[N];};

    template<typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    /**
     * remove_cv
     *
     * Удаляет const и volatile из типа.
     *
     * remove_cv преобразователь типа, который удаляет const (и/или) volatile
     * верхнего уровня (при наличии) из типа, к которому оно применимо.
     * Для типа T, remove_cv<T const volatile>::type эквивалентен.
     * Например, remove_cv<char * volatile>::type эквивалентен типу char *,
     * когда как remove_cv<const char *>::type эквивалентен типу const char *.
     * В последнем примере, квалификатор const изменяет char, без  *,
     * и поэтому не находится на верхнем уровне.
     *
     */
    template <typename T>
    struct remove_cv { using type = typename corsac::remove_volatile<typename corsac::remove_const<T>::type>::type; };

    template<typename T>
        using remove_cv_t = typename remove_cv<T>::type;

    /**
     * add_reference
     *
     * Добавьте ссылку на тип.
     *
     * Признак преобразования add_reference добавляет уровень косвенности путем ссылки на тип,
     * к которому он применяется. Для данного типа T add_reference<T>::type эквивалентен T&
     * если is_lvalue_reference<T>::value == false, и T в противном случае.
     *
     * Примечение: из-за правил сворачивания ссылок, если вы предоставите ссылку на r-значение, такую ​​как T&&, она свернется до T&.
     */
    template <typename T> struct add_reference_impl      { using type = T&; };
    template <typename T> struct add_reference_impl<T&>  { using type = T&; };
    template <>           struct add_reference_impl<void>{ using type = void; };

    template <typename T> struct add_reference { using type = typename add_reference_impl<T>::type; };

    /**
     * remove_reference
     *
     * Удаляет обращение по ссылки из типа.
     *
     * remove_reference преобразователь типа, который удаляет верхний уровень
     * обращения по ссылки (при наличии) из типа, к которому оно применимо.
     * Для типа T, remove_reference<T&>::type эквивалентен.
     */
    template <typename T> struct remove_reference     { using type = T; };
    template <typename T> struct remove_reference<T&> { using type = T; };
    template <typename T> struct remove_reference<T&&>{ using type = T; };

    template<typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    /**
     * remove_cvref
     *
     * Удаляет const и volatile из ссылочного типа.
     *
     * Признак преобразования remove_cvref удаляет квалификацию const и или volatile верхнего уровня
     * (если есть) из ссылочного типа, к которому она применяется.
     * Для данного типа remove_cvref<T& const volatile>::type эквивалентен T.
     * Например, remove_cv<int& volatile>::type эквивалентен int.
     */
    template <typename T>
    struct remove_cvref { using type = typename corsac::remove_volatile<typename corsac::remove_const<typename corsac::remove_reference<T>::type>::type>::type; };

    template<typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    /**
     * add_lvalue_reference
     *
     * Стандарт C++ 11, раздел 20.9.7.2
     * Если T называет тип объекта или функции, тогда тип typedef члена должен называть T&;
     * в противном случае, если T называет тип «ссылка rvalue на T1», тогда тип typedef члена должен называть T1 &;
     * в противном случае тип должен называться T.
     *
     * Примечание (8.3.2 p6):
     *      void + &  -> void
     *      T    + &  -> T&
     *      T&   + &  -> T&
     *      T&&  + &  -> T&
     */
    template <typename T> struct add_lvalue_reference                      { using type = T&;                  };
    template <typename T> struct add_lvalue_reference<T&>                  { using type = T&;                  };
    template <>           struct add_lvalue_reference<void>                { using type = void;                };
    template <>           struct add_lvalue_reference<const void>          { using type = const void;          };
    template <>           struct add_lvalue_reference<volatile void>       { using type = volatile void;       };
    template <>           struct add_lvalue_reference<const volatile void> { using type = const volatile void; };

    template <typename T>
        using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    /**
     * add_rvalue_reference
     *
     * Стандарт C++ 11, раздел 20.9.7.2 Если T называет тип объекта или функции, тогда тип typedef члена
     * должен быть как T&&; в противном случае тип должен быть T.
     *
     * Примечание:
     *      Это правило отражает семантику сворачивания ссылок (8.3.2).
     *      Например, когда тип T называет тип T1& add_rvalue_reference<T>::type не является ссылкой rvalue.
     *       void + &&  -> void
     *       T    + &&  -> T&&
     *       T&   + &&  -> T&
     *       T&&  + &&  -> T&&
     */
    template <typename T> struct add_rvalue_reference                      { using type = T&&;                 };
    template <typename T> struct add_rvalue_reference<T&>                  { using type = T&;                  };
    template <>           struct add_rvalue_reference<void>                { using type = void;                };
    template <>           struct add_rvalue_reference<const void>          { using type = const void;          };
    template <>           struct add_rvalue_reference<volatile void>       { using type = volatile void;       };
    template <>           struct add_rvalue_reference<const volatile void> { using type = const volatile void; };

    template <typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    /**
     * declval
     *
     * declval обычно находится в <utility.h>, но он нужен для свойств типа, и он включает в себя служебную программу.
     *
     *  Преобразует любой тип T в ссылочный тип, что позволяет использовать функции-члены в выражениях decltype без указания конструкторов.
     *  Он не используется вне выражений decltype. По дизайну реализации нет, так как она никогда не выполняется,
     *  а используется только в выражениях decltype.
     *  В разделе 20.2.4 стандарта C++ 11 указано, что мы должны объявить это
     *  http://en.cppreference.com/w/cpp/utility/declval
     */
    template <typename T>
    typename corsac::add_rvalue_reference<T>::type declval() noexcept;

    /**
     * static_min / static_max
     *
     * Они в первую очередь полезны в шаблонном коде для метапрограммирования.
     * В настоящее время мы ограничены размером size_t,
     * поскольку C + не позволяет использовать общие параметры шаблона.
     * При необходимости мы можем расширить поддерживаемые типы,
     * включив в них дополнительные целые числа.
     *
     * Отсутствует в стандарте C++.
     *
     * Пример использования:
     *     Printf("%zu", static_max<3, 7, 1, 5>::value);
     *     prints "7"
     */
    template <size_t I0, size_t ...in>
    struct static_min;

    template <size_t I0>
    struct static_min<I0>
    { static const size_t value = I0; };

    template <size_t I0, size_t I1, size_t ...in>
    struct static_min<I0, I1, in...>
    { static const size_t value = ((I0 <= I1) ? static_min<I0, in...>::value : static_min<I1, in...>::value); };

    template <size_t I0, size_t ...in>
    struct static_max;

    template <size_t I0>
    struct static_max<I0>
    { static const size_t value = I0; };

    template <size_t I0, size_t I1, size_t ...in>
    struct static_max<I0, I1, in...>
    { static const size_t value = ((I0 >= I1) ? static_max<I0, in...>::value : static_max<I1, in...>::value); };

   /**
     * Этот класс перечисления полезен для определения того, является ли система прямым или прямым порядком байтов.
     * Смешанный или средний порядок байтов здесь не моделируется, как описано в спецификации C++ 20.
     */
    enum class endian
    {
        #ifdef CORSAC_SYSTEM_LITTLE_ENDIAN
                little = 1,
                big = 0,
                native = little
        #else
                little = 0,
                big = 1,
                native = big
        #endif
    };

}

#include <internal/type_fundamental.h>
#include <internal/type_transformation.h>
#include <internal/type_properties.h>
#include <internal/type_compound.h>
#include <internal/type_pod.h>

#endif //CORSAC_TYPE_FUNDAMENTAL_H
