/**
 * corsac::STL
 *
 * internal/type_fundamental.h
 *
 * Created by Falldot on 04.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_TYPE_TRANSFORMATION_H
#define CORSAC_STL_TYPE_TRANSFORMATION_H

#pragma once

#include "Corsac/STL/config.h"

#include <limits.h>

namespace corsac
{
    /**
      * add_const
      *
      * Добавьте const к типу.
      *
      * Для данного типа T, add_const<T>::type эквивалентен T
      * const if is_const<T>::value == false, 1и
      *    - is_void<T>::value == true, или
      *    - is_object<T>::value == true.
      *
      * В противном случае add_const<T>::type эквивалентно T.
      */
    template <typename T, bool = corsac::is_const<T>::value || corsac::is_reference<T>::value || corsac::is_function<T>::value>
    struct add_const_helper
    { using type = T; };

    template <typename T>
    struct add_const_helper<T, false>
    { using type = const T; };

    template <typename T>
    struct  add_const
    { using type = typename corsac::add_const_helper<T>::type; };

    template <typename T>
    using add_const_t = typename add_const<T>::type;

    /**
    * add_volatile
    *
    * Add volatile to a type.
    *
    * Для данного типа T, add_volatile<T>::type эквивалентно T volatile
    * if is_volatile<T>::value == false, и
    *   - is_void<T>::value == true, или
    *   - is_object<T>::value == true.
    *
    * В противном случае add_volatile<T>::type эквивалентно T.
    */
    template <typename T, bool = corsac::is_volatile<T>::value || corsac::is_reference<T>::value || corsac::is_function<T>::value>
    struct add_volatile_helper
    { using type = T; };

    template <typename T>
    struct add_volatile_helper<T, false>
    { using type = volatile T; };

    template <typename T> struct add_volatile
    { using type = typename corsac::add_volatile_helper<T>::type; };

    template <class T> using add_volatile_t = typename add_volatile<T>::type;

    /**
    * add_cv
    *
    * Признак преобразования add_cv добавляет квалификацию const и volatile к типу,
    * к которому он применяется. Для данного типа T
    * add_volatile<T>::type эквивалентно add_const<add_volatile<T>::type>::type.
    *
    */
    template<typename T>
    struct add_cv
    {
        using type = typename add_const<typename add_volatile<T>::type>::type;
    };

    template <class T> using add_cv_t = typename add_cv<T>::type;

    /**
    * make_signed
    *
    * Используется для преобразования целочисленного типа в его знаковый эквивалент, если еще не был.
    * T должен быть целочисленным типом или перечислением (возможно, const и или volatile),
    * но не типом bool;
    *
    * Пользователь может определить свои собственные переопределения make_signed для своих типов,
    * сделав специализацию шаблона, как показано ниже, и добавив ее в код пользователя.
    */
    namespace internal
    {
        template <typename T, bool = corsac::is_enum_v<T> || corsac::is_integral_v<T>>
        struct make_signed_helper_0
        {
            struct char_helper
            {
                using type = signed char;
            };

            struct short_helper
            {
                using type = signed short;
            };

            struct int_helper
            {
                using type = signed int;
            };

            struct long_helper
            {
                using type = signed long;
            };

            struct longlong_helper
            {
                using type = signed long long;
            };

            struct int128_helper
            {
                #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG))
                      using type = __int128_t;
                #endif
            };

            struct no_type_helper
            {};

            typedef typename
            corsac::conditional<sizeof(T) <= sizeof(signed char), char_helper,
                    corsac::conditional_t<sizeof(T) <= sizeof(signed short), short_helper,
                            corsac::conditional_t<sizeof(T) <= sizeof(signed int), int_helper,
                                    corsac::conditional_t<sizeof(T) <= sizeof(signed long), long_helper,
                                            corsac::conditional_t<sizeof(T) <= sizeof(signed long long), longlong_helper,
                                                #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG))
                                                     corsac::conditional_t<sizeof(T) <= sizeof(__int128_t), int128_helper,
                                                                    no_type_helper
                                                                >
                                                #else
                                                    no_type_helper
                                                #endif
                                            >
                                    >
                            >
                    >
            >::type type;
        };

        template <typename T>
        struct make_signed_helper_0<T, false>
        {
            struct no_type_helper
            {};
            using type = no_type_helper;
        };

        template <typename T>
        struct make_signed_helper_1
        {
            using type = typename T::type;
        };

        template <typename T>
        struct make_signed_helper
        {
            using type = typename corsac::internal::make_signed_helper_1<typename corsac::internal::make_signed_helper_0<T>::type>::type;
        };

    } // namespace internal

    template <typename T>
    struct make_signed
    {
        using type = typename corsac::internal::make_signed_helper<T>::type;
    };

    template <> struct make_signed<bool> {};
    template <> struct make_signed<signed char>              { using type = signed char;        };
    template <> struct make_signed<unsigned char>            { using type = signed char;        };
    template <> struct make_signed<signed short>             { using type = signed short;       };
    template <> struct make_signed<unsigned short>           { using type = signed short;       };
    template <> struct make_signed<signed int>               { using type = signed int;         };
    template <> struct make_signed<unsigned int>             { using type = signed int;         };
    template <> struct make_signed<signed long>              { using type = signed long;        };
    template <> struct make_signed<unsigned long>            { using type = signed long;        };
    template <> struct make_signed<signed long long>         { using type = signed long long;   };
    template <> struct make_signed<unsigned long long>       { using type = signed long long;   };
    #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG))
        template <> struct make_signed<__int128_t>           { typedef __int128_t			  type; };
            template <> struct make_signed<__uint128_t>          { typedef __int128_t			  type; };
    #endif

        // Если char беззнаковый, мы конвертируем char в подписанный char. Однако, если char подписан, make_signed возвращает сам char, а не подписанный char.
    #if (defined(CHAR_MAX) && defined(UCHAR_MAX) && (CHAR_MAX == UCHAR_MAX))
        template <> struct make_signed<char> { using type = signed char; };
    #endif

    template <typename T>
    struct make_signed<const T>
    {
        using type = corsac::add_const_t<typename corsac::make_signed<T>::type>;
    };

    template <typename T>
    struct make_signed<volatile T>
    {
        using type = corsac::add_volatile_t<typename corsac::make_signed<T>::type>;
    };

    template <typename T>
    struct make_signed<const volatile T>
    {
        using type = corsac::add_cv_t<typename corsac::make_signed<T>::type>;
    };

    template <typename T>
    using make_signed_t = typename make_signed<T>::type;

    /**
    * make_unsigned
    *
    * Используется для преобразования целочисленного типа в его беззнаковый эквивалент, если это еще не сделано.
    * T должен быть целочисленным типом или перечислением (возможно, const и или volatile), но не типом bool;
    *
    * Пользователь может определить свои собственные переопределения make_unsigned для своих типов,
    * сделав специализацию шаблона, как показано ниже, и добавив ее в код пользователя.
    */
    namespace internal
    {
        template <typename T, bool = corsac::is_enum<T>::value || corsac::is_integral<T>::value>
        struct make_unsigned_helper_0
        {
            struct char_helper
            {
                typedef unsigned char type;
            };

            struct short_helper
            {
                typedef unsigned short type;
            };

            struct int_helper
            {
                typedef unsigned int type;
            };

            struct long_helper
            {
                typedef unsigned long type;
            };

            struct longlong_helper
            {
                typedef unsigned long long type;
            };

            struct int128_helper
            {
                #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG))
                    typedef __uint128_t type;
                #endif
            };

            struct no_type_helper
            {};


            typedef typename
            corsac::conditional<sizeof(T) <= sizeof(unsigned char), char_helper,
                    corsac::conditional_t<sizeof(T) <= sizeof(unsigned short), short_helper,
                            corsac::conditional_t<sizeof(T) <= sizeof(unsigned int), int_helper,
                                    corsac::conditional_t<sizeof(T) <= sizeof(unsigned long), long_helper,
                                            corsac::conditional_t<sizeof(T) <= sizeof(unsigned long long), longlong_helper,
                                                #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) && defined(CORSAC_COMPILER_CLANG))
                                                    corsac::conditional_t<sizeof(T) <= sizeof(__uint128_t), int128_helper,
                                                                            no_type_helper
                                                                        >
                                                #else
                                                    no_type_helper
                                                #endif
                                            >
                                    >
                            >
                    >
            >::type type;
        };

        template <typename T>
        struct make_unsigned_helper_0<T, false>
        {
            struct no_type_helper
            {};

            typedef no_type_helper type;
        };

        template <typename T>
        struct make_unsigned_helper_1
        {
            using type = typename T::type;
        };

        template <typename T>
        struct make_unsigned_helper
        {
            using type = typename corsac::internal::make_unsigned_helper_1<typename corsac::internal::make_unsigned_helper_0<T>::type>::type;
        };

    } // namespace internal

    template <typename T>
    struct make_unsigned
    {
        using type = typename corsac::internal::make_unsigned_helper<T>::type;
    };

    template <> struct make_unsigned<bool> {};
    template <> struct make_unsigned<signed char>            { using type = unsigned char;      };
    template <> struct make_unsigned<unsigned char>          { using type = unsigned char;      };
    template <> struct make_unsigned<signed short>           { using type = unsigned short;     };
    template <> struct make_unsigned<unsigned short>         { using type = unsigned short;     };
    template <> struct make_unsigned<signed int>             { using type = unsigned int;       };
    template <> struct make_unsigned<unsigned int>           { using type = unsigned int;       };
    template <> struct make_unsigned<signed long>            { using type = unsigned long;      };
    template <> struct make_unsigned<unsigned long>          { using type = unsigned long;      };
    template <> struct make_unsigned<signed long long>       { using type = unsigned long long; };
    template <> struct make_unsigned<unsigned long long>     { using type = unsigned long long; };
    #if CORSAC_INT128_SUPPORTED && (defined(CORSAC_COMPILER_GCC) || defined(CORSAC_COMPILER_CLANG))
        template <> struct make_unsigned<__int128_t>         { using type = __uint128_t; };
            template <> struct make_unsigned<__uint128_t>        { using type = __uint128_t; };
    #endif

        // Если char подписан, мы конвертируем char в unsigned char. Однако, если char беззнаковый, make_unsigned возвращает сам char, а не unsigned char.
    #if (CHAR_MIN < 0)
        template <> struct make_unsigned<char> { typedef unsigned char type; };
    #endif

    #if defined(CORSAC_CHAR8_UNIQUE) && CORSAC_CHAR8_UNIQUE
        template <> struct make_unsigned<char8_t> { typedef unsigned char type; };
    #endif

    template <typename T>
    struct make_unsigned<const T>
    {
        using type = corsac::add_const_t<typename corsac::make_unsigned<T>::type>;
    };

    template <typename T>
    struct make_unsigned<volatile T>
    {
        using type = corsac::add_volatile_t<typename corsac::make_unsigned<T>::type>;
    };

    template <typename T>
    struct make_unsigned<const volatile T>
    {
        using type = corsac::add_cv_t<typename corsac::make_unsigned<T>::type>;
    };

    template <typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    /**
    * remove_pointer
    *
    * Удалить указатель из типа.
    *
    * Свойство преобразования remove_pointer удаляет косвенное указание верхнего уровня по указателю
    * (если есть) от типа, к которому он применяется. Указатели на элементы не затрагиваются.
    * Для данного типа T remove_pointer<T>::type эквивалентен T.
    */
    template<typename T> struct remove_pointer                    { using type = T; };
    template<typename T> struct remove_pointer<T*>                { using type = T; };
    template<typename T> struct remove_pointer<T* const>          { using type = T; };
    template<typename T> struct remove_pointer<T* volatile>       { using type = T; };
    template<typename T> struct remove_pointer<T* const volatile> { using type = T; };

    template <class T>
    using remove_pointer_t = typename remove_pointer<T>::type;

    /**
    * add_pointer
    *
    * Добавить указатель на тип.
     *
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

    ///////////////////////////////////////////////////////////////////////
    // aligned_storage
    //
    // Признак преобразования align_storage предоставляет тип,
    // который соответствующим образом выровнен для хранения объекта,
    // размер которого не превышает длину и чье выравнивание является делителем
    // выравнивания. При использовании align_storage длина должна быть ненулевой,
    // а выравнивание должно >= alignment_of<T>::value для некоторого типа T.
    // Мы требуем, чтобы значение выравнивания было степенью двойки.
    //
    // Пример использования:
    //     aligned_storage<sizeof(Widget), alignment_of(Widget)>::type widget;
    //     Widget* pWidget = new(&widget) Widget;
    //
    //     aligned_storage<sizeof(Widget), 64>::type widgetAlignedTo64;
    //     Widget* pWidget = new(&widgetAlignedTo64) Widget;
    //
    //     aligned_storage<sizeof(Widget), alignment_of(Widget)>::type widgetArray[37];
    //     Widget* pWidgetArray = new(widgetArray) Widget[37];
    ///////////////////////////////////////////////////////////////////////

    template<size_t N, size_t Align = alignof(double)>
    struct aligned_storage
    {
        using type = struct {
            alignas(Align) unsigned char mCharData[N];
        };
    };

    template <size_t N, size_t Align = alignof(double)>
    using aligned_storage_t = typename aligned_storage<N, Align>::type;

    /**
    * aligned_union
    *
    * Тип typedef члена должен быть типом POD, подходящим для использования в качестве
    * неинициализированного хранилища для любого объекта, тип которого указан в Типах;
    * его размер должен быть не менее Len. Статический член alignment_value должен быть интегральной константой
    * типа std :: size_t, значение которой является самым строгим выравниванием из всех типов, перечисленных в Типах.
    * Обратите внимание, что результирующий тип не является объединением C/C++,
    * просто блоком памяти (типа pod), который можно использовать для нового размещения фактического объединения типов CC ++.
    * Фактический union, который вы объявляете, может быть union, не относящимся к POD.
    *
    * Пример:
    *     union MyUnion {
    *         char  c;
    *         int   i;
    *         float f;
    *
    *         MyUnion(float fValue) : f(fValue) {}
    *     };
    *
    *     aligned_union<sizeof(MyUnion), char, int, float>::type myUnionStorage;
    *     MyUnion* pMyUnion = new(&myUnionStorage) MyUnion(21.4f);
    *     pMyUnion->i = 37;
    */
    template <size_t minSize, typename Type0, typename ...TypeN>
    struct aligned_union
    {
        static const size_t size            = corsac::static_max<minSize, sizeof(Type0), sizeof(TypeN)...>::value;
        static const size_t alignment_value = corsac::static_max<alignof(Type0), alignof(TypeN)...>::value;

        using type = typename corsac::aligned_storage<size, alignment_value>::type;
    };

    template <size_t minSize, typename... TypeN>
    using aligned_union_t = typename aligned_union<minSize, TypeN...>::type;

    /**
    * union_cast
    *
    * Безопасное преобразование между несвязанными типами, имеющими двоичную эквивалентность.
    * Этот подход требуется для строго соответствующих компиляторов C++,
    * поскольку прямое использование преобразования C или C++ между несвязанными типами
    * чревато возможностью неопределенного поведения во время выполнения из-за псевдонима типов.
    * Типы Source и Dest должны быть типами POD из-за использования объединения в версиях C++ до C++ 11.
    * C++ 11 ослабляет определение POD, так что он позволяет классам с тривиальными конструкторами по умолчанию,
    * тогда как в предыдущих версиях этого не было, поэтому будьте осторожны при написании переносимого кода.
    *
    * Пример использования:
    *    float f32 = 1.234f;
    *    uint32_t n32 = union_cast<uint32_t>(f32);
    *
    * Пример возможного неправильного использования:
    * Следующее действительно только в том случае, если вы задаете псевдоним значения указателя,
    * а не того, на что он указывает. В большинстве случаев пользователь имеет в виду последнее,
    * что не совсем возможно.
    *    Widget* pWidget = CreateWidget();
    *    Foo*    pFoo    = union_cast<Foo*>(pWidget);
    */
    template <typename DestType, typename SourceType>
    DestType union_cast(SourceType sourceValue)
    {
        static_assert((sizeof(DestType) == sizeof(SourceType)) && (alignof(DestType) == alignof(SourceType)));
        union {
            SourceType sourceValue;
            DestType   destValue;
        } u;
        u.sourceValue = sourceValue;

        return u.destValue;
    }

    /**
    * void_t
    *
    * Отображает последовательность любых типов в void.
    * Этот служебный класс используется в метапрограммировании шаблонов для упрощения механизмов
    * отражения во время компиляции, требуемых стандартной библиотекой.
    *
    * Пример:
    *    template <typename T, typename = void>
    *    struct is_iterable : false_type {};
    *
    *    template <typename T>
    *    struct is_iterable<T, void_t<decltype(declval<T>().begin()),
    *                                 decltype(declval<T>().end())>> : true_type {};
    */
    template <class...>
    using void_t = void;
}

#endif //CORSAC_STL_TYPE_TRANSFORMATION_H
