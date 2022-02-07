/**
 * corsac::STL
 *
 * internal/type_pod.h
 *
 * Created by Falldot on 01.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_TYPE_POD_H
#define CORSAC_STL_TYPE_POD_H

#pragma once

namespace corsac
{
    /**
     * is_empty
     *
     * is_empty<T>::value == true тогда и только тогда, когда T являеться пустым классом или структурой.
     * is_empty может применяться только к полным типам..
     *
     * is_empty нельзя использовать с типом union, до вызова is_union.
     */
    template <typename T>
    struct is_empty : public integral_constant<bool, __is_empty(T)>{};

    template <class T>
    constexpr bool is_empty_v = is_empty<T>::value;

    /**
    * is_pod
    *
    * is_pod<T>::value == true тогда и только тогда, когда переданный тип T:
    *    - is_scalar<T>::value == true, или
    *    - T - это класс или структура, у которых нет определяемого пользователем
    *      оператора или деструктора присваивания копии, и T не имеет нестатических элементов данных M,
    *       для которых is_pod <M> :: value == false, и элементов ссылочного типа, или
    *    - T - это тип массива объектов E, для которог is_pod<E>::value == true
    *
    * is_pod можно применять только к полным типам.
    *
    * Без помощи компилятора или пользователя is_pod не будет сообщать
    * что структура или класс является POD, но правильно сообщит,
    * что встроенные типы, такие как int, являются POD.
    */
    template <typename T>
    struct is_pod : public corsac::integral_constant<bool, __is_pod(T) || corsac::is_void<T>::value || corsac::is_scalar<T>::value>{};

    template <class T>
    constexpr bool is_pod_v = is_pod<T>::value;

    /**
    * is_standard_layout
    */
    template <typename T>
    struct is_standard_layout : public corsac::integral_constant<bool, __is_standard_layout(T) || corsac::is_void<T>::value || corsac::is_scalar<T>::value>{};

    template <class T>
    constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

    /**
    * is_abstract
    *
    * is_abstract<T>::value == true тогда и только тогда, когда T  является классом или структурой,
    * имеющей хотя бы одну чистую виртуальную функцию.
    *
    * is_abstract может применяться только к полным типам.
    */
    template <typename T>
    struct is_abstract : public integral_constant<bool, __is_abstract(T)>{};

    template <class T>
    constexpr bool is_abstract_v = is_abstract<T>::value;

    /**
    * is_trivially_copyable
    *
    * T  это тривиально копируемый тип (3.9). T должен быть полным типом
    * (возможно, cv-квалифицированным) void или массивом с неизвестной границей.
    *
    * 3.9, p3: для любого тривиально копируемого типаT,  если два указателя на T
    * указывают на разные объекты T obj1 и obj2,  где ни obj1 или ни
    * obj2 не являются подобъектами базового класса, если базовые байты, составляющие
    * obj1 копируются в obj2, obj2 впоследствии будет иметь то же значение,
    * что и obj1. Другими словами, вы можете использовать memcpy/memmove.
    */
    template <typename T>
    struct is_trivially_copyable { static const bool value = __is_trivially_copyable(T); };

    template <class T>
    constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

    /**
    * is_constructible
    *
    * См. Стандарт C++ 11, раздел 20.9.4.3, стр. 6.
    */
    template<typename T, typename... Args>
    struct is_constructible : public bool_constant<__is_constructible(T, Args...) > {};

    template <class T, class... Args>
    constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

    /**
    * is_trivially_constructible
    *
    * is_constructible<T, Args...>::value истинно, а определение переменной для
    * is_constructible, как определено ниже, не вызывает никаких
    * нетривиальных операций (3.9, 12). T и все типы в пакете параметров
    * Args должны быть полными типами (возможно, с квалификацией cv) void или
    * массивами с неизвестной границей.
    *
    * Примечание:
    * is_trivially_constructible  в C++ 11 звучит так же, как предстандартный признак типа
    * (который мы также здесь поддерживаем). Однако определение has_trivial_constructor никогда
    * не было формально стандартизировано, и поэтому мы не можем просто слепо приравнять их друг к другу.
    * Поскольку мы продвигаемся вперед с C++ 11 и отказываемся от старых свойств типов,
    * мы оставляем старые как есть, хотя и полагаемся на них в тех случаях,
    * когда нам кажется, что у нас нет хорошей альтернативы.
    */
    template <typename T, typename... Args>
    struct is_trivially_constructible
            : public corsac::integral_constant<bool, __is_trivially_constructible(T, Args...)> {};

    template <class T>
    constexpr bool is_trivially_constructible_v = is_trivially_constructible<T>::value;

    /**
    * is_trivial
    *
    * is_trivial<T>::value == true если T является скалярным типом,
    * тривиально копируемым классом с тривиальным конструктором по умолчанию или массивом такого класса типов,
    * возможно cv-квалифицированным), предоставляет значение константы члена, равное true.
    */
    template <typename T>
    struct is_trivial
            : public corsac::integral_constant<bool, corsac::is_trivially_copyable<T>::value && corsac::is_trivially_constructible<T>::value> {};

    template <class T>
    constexpr bool is_trivial_v = is_trivial<T>::value;

    /**
    * is_nothrow_constructible
    *
    * is_constructible<T, Args...>::value true и известно, что определение переменной для
    * is_constructible,  как определено ниже, не генерирует никаких исключений (5.3.7).
    * T и все типы в пакете параметров Args должны быть полными типами (возможно, с квалификацией cv)
    * void или массивами с неизвестной границей.
    */
    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper_noexcept_wrapper
    { static const bool value = noexcept(T(corsac::declval<Args>()...)); };

    template <bool, typename T, typename... Args>
    struct is_nothrow_constructible_helper;

    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper<true, T, Args...>
            : public corsac::integral_constant<bool, is_nothrow_constructible_helper_noexcept_wrapper<T, Args...>::value> {};

    template<typename T, typename Arg>
    struct is_nothrow_constructible_helper<true, T, Arg>
            : public corsac::integral_constant<bool, noexcept(T(corsac::declval<Arg>()))> {};

    template<typename T>
    struct is_nothrow_constructible_helper<true, T>
            : public corsac::integral_constant<bool, noexcept(T())> {};

    template <typename T, typename... Args>
    struct is_nothrow_constructible_helper<false, T, Args...>
            : public corsac::false_type {};

    template <typename T, typename... Args>
    struct is_nothrow_constructible
            : public corsac::is_nothrow_constructible_helper<corsac::is_constructible<T, Args...>::value, T, Args...> {};

    template <typename T, size_t N>
    struct is_nothrow_constructible<T[N]>
            : public corsac::is_nothrow_constructible_helper<corsac::is_constructible<T>::value, T> {};

    template <class T, typename... Args>
    constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

    /**
    * is_default_constructible
    *
    * is_constructible<T>::value is true.
    */
    template <typename T>
    struct is_default_constructible
            : public corsac::is_constructible<T> {};

    template <class T>
    constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

    /**
    * is_copy_constructible
    *
    * is_constructible<T, const T&>::value is true.
    */
    template <typename T>
    struct is_copy_constructible
            : public corsac::is_constructible<T, typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

    /**
    * is_trivially_copy_constructible
    *
    * is_trivially_constructible<T, const T&>::value is true.
    */
    template <typename T>
    struct is_trivially_copy_constructible
            : public corsac::is_trivially_constructible<T, typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

    /**
    * is_nothrow_copy_constructible
    *
    * is_nothrow_constructible<T, const T&>::value is true.
    */
    template <typename T>
    struct is_nothrow_copy_constructible
            : public is_nothrow_constructible<T, typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

    /**
    * is_move_constructible
    *
    * is_constructible<T, T&&>::value is true.
    */
    template <typename T>
    struct is_move_constructible
            : public corsac::is_constructible<T, typename corsac::add_rvalue_reference<T>::type> {};

    template <class T>
    constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

    /**
    * is_trivially_move_constructible
    *
    * is_trivially_constructible<T, T&&>::value is true.
    * T  должен быть полным типом, (возможно, cv-квалифицированным) void
    * или массивом с неизвестной границей.
    */
    template <typename T>
    struct is_trivially_move_constructible
            : public corsac::is_trivially_constructible<T, typename corsac::add_rvalue_reference<T>::type> {};

    template <class T>
    constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

    /**
    * is_assignable
    *
    * Выражение declval<T>() = declval<U>() правильно сформировано, когда оно рассматривается как неоцененный операнд
    * Проверка доступа выполняется, как если бы в контексте, не связанном с T и U.
    * Учитывается только действительность непосредственного контекста выражения присваивания.
    * Компиляция выражения может привести к побочным эффектам, таким как создание экземпляров
    * специализаций шаблонов классов и специализаций шаблонов функций, создание неявно определенных функций и т.д.
    * Такие побочные эффекты не относятся к «непосредственному контексту» и могут привести к неправильному формированию программы.
    *
    * Примечание:
    * эта черта типа имеет вводящее в заблуждение и противоречащее интуиции название. Он не указывает,
    * может ли экземпляр U быть назначен экземпляру T (например, t = u). Вместо этого он указывает,
    * можно ли выполнить присвоение после добавления ссылок rvalue к обоим, как в add_rvalue_reference<T>::type = add_rvalue_reference<U>::type.
    * Противоинтуитивным результатом этого является то, что is_assignable <int, int>::value == false.
    * Признак is_copy_assignable указывает, может ли тип быть назначен его собственному типу, хотя стандартного способа C++ определить,
    * можно ли присвоить произвольный тип другому типу, не существует.
    * http://stackoverflow.com/questions/19920213/why-is-stdis-assignable-counter-intuitive
    *
    * Примечание:
    * Истинное значение is_assignable не гарантирует, что выражение компилируемо, компилятор проверяет
    * только соответствие присвоения перед компиляцией. В частности, если у вас есть шаблон operator =
    * для класса, компилятор всегда будет говорить, что is_assignable истинно, независимо от того,
    * что тестируется в правой части выражения. На самом деле во время компиляции может оказаться,
    * что шаблонный оператор = не компилируется, потому что на практике он не принимает все возможные
    * типы для правой части выражения.
    *
    * Ожидаемые результаты:
    *     is_assignable<void, void>::value             == false
    *     is_assignable<int&, int>::value              == true
    *     is_assignable<int, int>::value               == false
    *     is_assignable<int, int&>::value              == false
    *     is_assignable<bool, bool>::value             == false
    *     is_assignable<int, float>::value             == false
    *     is_assignable<int[], int[]>::value           == false
    *     is_assignable<char*, int*>::value            == false
    *     is_assignable<char*, const char*>::value     == false
    *     is_assignable<const char*, char*>::value     == false
    *     is_assignable<PodA, PodB*>::value            == false
    *     is_assignable<Assignable, Assignable>::value == true
    *     is_assignable<Assignable, Unrelated>::value  == false
    */
    template<typename T, typename U>
    struct is_assignable_helper
    {
        template<typename, typename>
        static corsac::no_type is(...);

        template<typename T1, typename U1>
        static decltype(corsac::declval<T1>() = corsac::declval<U1>(), corsac::yes_type()) is(int);

        static const bool value = (sizeof(is<T, U>(0)) == sizeof(corsac::yes_type));
    };

    template<typename T, typename U>
    struct is_assignable :
            public corsac::integral_constant<bool, corsac::is_assignable_helper<T, U>::value> {};

    template <class T, class U>
    constexpr bool is_assignable_v = is_assignable<T, U>::value;

    /**
    * is_lvalue_assignable
    *
    * Это функция расширения corsac, которая похожа на is_assignable,
    * но работает для произвольных назначений, а не только для назначений rvalue.
    * Эта функция обеспечивает интуитивно понятный тест на возможность назначения,
    *  в отличие от is_assignable.
    *
    * Примечание: is_lvalue_assignable<T, T> === is_copy_assignable<T>
    */
    template <typename T, typename U>
    struct is_lvalue_assignable
            : public corsac::is_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_lvalue_reference<typename corsac::add_const<U>::type>::type> {};

    template <class T, class U>
    constexpr bool is_lvalue_assignable_v = is_lvalue_assignable<T, U>::value;

    /**
    * is_trivially_assignable
    *
    * is_assignable<T, U>::value is true и известно, что присвоение, как определено
    * is_assignable, не вызывает нетривиальных операций (3.9, 12).
    * T и U должны быть полными типами (возможно, с квалификацией cv)
    * void или массивами с неизвестной границей.
    */
    template <typename T, typename U>
    struct is_trivially_assignable
            : corsac::integral_constant<bool, __is_trivially_assignable(T, U)> {};

    template <class T, class U>
    constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;


    /**
    * is_nothrow_assignable
    *
    * is_assignable<T, U>::value is true  и известно, что присвоение
    * не генерирует никаких исключений (5.3.7). T и U должны быть полными типами
    *  (возможно, с квалификацией cv) void или массивами с неизвестной границей.
    */
#if defined(CORSAC_COMPILER_MSVC) || defined(CORSAC_COMPILER_CLANG)
    template <typename T, typename U>
        struct is_nothrow_assignable
            : corsac::integral_constant<bool, __is_nothrow_assignable(T, U)> {};
#else
    template <bool, typename T, typename U>
    struct is_nothrow_assignable_helper;

    template <typename T, typename U>
    struct is_nothrow_assignable_helper<false, T, U>
            : public false_type {};

    template <typename T, typename U>
    struct is_nothrow_assignable_helper<true, T, U> // Установите значение true, если присвоение (то же, что и is_assignable) не может создать исключение.
            : public corsac::integral_constant<bool, noexcept(corsac::declval<T>() = corsac::declval<U>()) >
    {};

    template <typename T, typename U>
    struct is_nothrow_assignable
            : public corsac::is_nothrow_assignable_helper<corsac::is_assignable<T, U>::value, T, U>
    {};
#endif

    template <class T, class U>
    constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

    /**
    * is_copy_assignable
    *
    * is_assignable<T&, const T&>::value is true.  T должен быть полным типом
    * (возможно, cv -qualified) void или массивом с неизвестной границей.
    *
    * Это (а не is_assignable) признак типа, который вы используете, чтобы определить,
    * можете ли вы выполнять произвольное присвоение. is_assignable сообщает,
    * можете ли вы выполнять присвоение специально для rvalue.
    * http://stackoverflow.com/a/19921030/725009
    */
    template <typename T>
    struct is_copy_assignable
            : public corsac::is_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;


    /**
    * is_trivially_copy_assignable
    *
    * is_trivially_assignable<T&, const T&>::value is true. T должен быть полным типом
    * (возможно, cv-квалифицированным) void или массивом с неизвестной границей.
    */
    template <typename T>
    struct is_trivially_copy_assignable
            : public corsac::is_trivially_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

    /**
    * is_nothrow_copy_assignable
    */
    template <typename T>
    struct is_nothrow_copy_assignable
            : public corsac::is_nothrow_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_lvalue_reference<typename corsac::add_const<T>::type>::type> {};

    template <class T>
    constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;


    /**
    * is_move_assignable
    *
    * is_assignable<T&, T&&>::value is true. T  должен быть полным типом
    *  (возможно, cv -qualified) void или массивом с неизвестной границей.
    */
    template <typename T>
    struct is_move_assignable
            : public corsac::is_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_rvalue_reference<T>::type> {};

    template <class T>
    constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

    /**
    * is_trivially_move_assignable
    *
    * is_trivially_-assignable<T&, T&&>::value is true. T должен быть полным типом
    * (возможно, cv-квалифицированным) void или массивом с неизвестной границей.
    */
    template <typename T>
    struct is_trivially_move_assignable
            : public corsac::is_trivially_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_rvalue_reference<T>::type> {};

    template <class T>
    constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

    /**
    * is_nothrow_move_assignable
    */
    template <typename T>
    struct is_nothrow_move_assignable
            : public corsac::is_nothrow_assignable<typename corsac::add_lvalue_reference<T>::type,
                    typename corsac::add_rvalue_reference<T>::type> {};

    template <class T>
    constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

    /**
    * is_destructible
    *
    * Для полного типа T и данного шаблона
    *     template <class U>
    *     struct test { U u; };
    * test<T>::~test() не удаляется (C++11 "= delete").
    * T должен быть полным типом (возможно, cv-квалифицированным) void или массивом с неизвестной границей.
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
            struct is_destructible
                : integral_constant<bool, __is_destructible(T)> {};
    #else
    template <typename U>
    struct destructible_test_helper{ U u; };

    template <typename>
    corsac::false_type destructible_test_function(...);

    template <typename T, typename U = decltype(corsac::declval<corsac::destructible_test_helper<T> >().~destructible_test_helper<T>())>
    corsac::true_type destructible_test_function(int);

    // Исключите эти типы из числа разрушаемых.
    template <typename T, bool = corsac::is_array_of_unknown_bounds<T>::value ||
                                 corsac::is_void<T>::value                    ||
                                 corsac::is_function<T>::value                ||
                                 corsac::is_abstract<T>::value>
    struct is_destructible_helper
            : public corsac::type_identity<decltype(corsac::destructible_test_function<T>(0))>::type {};
    // Необходимо обернуть decltype идентификатором, потому что в противном случае некоторым компиляторам не нравится использование простого decltype.

    template <typename T>
    struct is_destructible_helper<T, true>
            : public corsac::false_type {};

    template <typename T>
    struct is_destructible
            : public is_destructible_helper<T> {};
#endif

    template <class T>
    constexpr bool is_destructible_v = is_destructible<T>::value;

    /**
    * is_trivially_destructible
    *
    * is_destructible<T>::value is true а указанный деструктор известен как
    * тривиальный. T должен быть полным типом (возможно, cv-квалифицированным
    * void или массивом с неизвестной границей.
    *
    * Деструктор тривиален, если он не предоставлен пользователем и если:
    *    - деструктор не виртуальный,
    *    - все прямые базовые классы этого класса имеют тривиальные деструкторы, и
    *    - для всех нестатических членов данных своего класса, которые относятся к типу класс
    *      (или его массиву), каждый такой класс имеет тривиальный деструктор.
    */
    #if defined(CORSAC_COMPILER_MSVC)
        template <typename T>
        struct is_trivially_destructible
            : integral_constant<bool, __is_trivially_destructible(T)> {};
    #else
    // Нельзя использовать только __has_trivial_destructor(T) потому что некоторые компиляторы придают ему немного другое значение
    // и просто не работают, например __has_trivial_destructor VC++, который говорит false для основных типов.
    template <typename T>
    struct is_trivially_destructible
            : public integral_constant<bool, corsac::is_destructible<T>::value && ((__has_trivial_destructor(T) && !corsac::is_hat_type<T>::value)|| corsac::is_scalar<typename corsac::remove_all_extents<T>::type>::value)> {};
    #endif
    template <class T>
    constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

    /**
    * is_nothrow_destructible
    *
    * is_destructible<T>::value is true и известно, что указанный деструктор
    * не генерирует никаких исключений (5.3.7).  T должен быть полным типом
    * (возможно, cv-квалифицированным) void или массивом с неизвестной границей.
    */
#if defined(CORSAC_COMPILER_MSVC)
    template <typename T>
    struct is_nothrow_destructible
        : integral_constant<bool, __is_nothrow_destructible(T)> {};
#else
    template <typename T>
    struct is_nothrow_destructible_helper_noexcept_wrapper
    { static const bool value = noexcept(corsac::declval<T&>().~T()); };

    template <typename T, bool>
    struct is_nothrow_destructible_helper;

    template <typename T>
    struct is_nothrow_destructible_helper<T, false>
            : public corsac::false_type {};

    template <typename T>
    struct is_nothrow_destructible_helper<T, true>     // Если выражение T::~T является выражением noexcept, то оно не выполняется.
            : public corsac::integral_constant<bool, is_nothrow_destructible_helper_noexcept_wrapper<T>::value > {};

    template <typename T>
    struct is_nothrow_destructible                      // Тип должен быть по крайней мере разрушаемым, прежде чем его нельзя будет уничтожить.
            : public corsac::is_nothrow_destructible_helper<T, corsac::is_destructible<T>::value> {};

    template <typename T, size_t N>                     // Массив не является разрушаемым, если его тип элемента не является разрушаемым.
    struct is_nothrow_destructible<T[N]>                // На заметку: замените это путем remove_all_extents.
            : public corsac::is_nothrow_destructible<T> {};

    template <typename T>
    struct is_nothrow_destructible<T&>                  // Ссылочный тип не может выбросить во время разрушения. Это просто ссылка.
            : public corsac::true_type {};

    template <typename T>
    struct is_nothrow_destructible<T&&>                 // Ссылочный тип rvalue не может генерироваться во время уничтожения.
            : public corsac::true_type {};
#endif

    template <class T>
    constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

    /**
    * is_nothrow_default_constructible
    */
    template <typename T>
    struct is_nothrow_default_constructible
            : public corsac::is_nothrow_constructible<T> {};

    template <class T>
    constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

    /**
    * is_nothrow_move_constructible
    */
    template <typename T>
    struct is_nothrow_move_constructible
            : public corsac::is_nothrow_constructible<T, typename corsac::add_rvalue_reference<T>::type> {};
    template <class T>
    constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;
}


#endif //CORSAC_TYPE_POD_H
