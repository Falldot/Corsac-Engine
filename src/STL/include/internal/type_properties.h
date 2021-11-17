/**
 * corsac::STL
 *
 * internal/type_fundamental.h
 *
 * Created by Falldot on 04.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_TYPE_PROPERTIES_H
#define CORSAC_TYPE_PROPERTIES_H

#pragma once

namespace corsac
{
    /**
    * underlying_type
    *
    * Определяет тип typedef члена типа, который является базовым типом для перечисления T.
    * Для реализации требуется явная поддержка компилятора.
    */
    template <typename T>
    struct underlying_type{ using type = __underlying_type(T); };

    template <typename T>
    using underlying_type_t = typename underlying_type<T>::type;

    /**
    * to_underlying
    *
    * Приведение значения перечисления к его базовому типу.
    *
    * Пример:
    *       enum class MyEnum : uint8_t { Value = 0; }
    *       auto x = MyEnum::Value;
    *       std::cout << to_underlying(x) эквивалент std::cout << static_cast<uint8_t>(x);
    */
    template<class T>
        constexpr underlying_type_t<T> to_underlying(T value) noexcept
    {
        return static_cast<underlying_type_t<T>>(value);
    }

    /**
    * has_unique_object_representations
    *
    * Если T имеет значение TriviallyCopyable и любые два объекта типа T с одинаковым значением
    * имеют одинаковое представление объекта, значение равно true.
    * Для любого другого типа значение false.
    */
    template <typename T>
    struct has_unique_object_representations
        : public integral_constant<bool, __has_unique_object_representations(remove_cv_t<remove_all_extents_t<T>>)>
    {};

    template <class T>
            constexpr auto has_unique_object_representations_v = has_unique_object_representations<T>::value;

    /**
    * is_signed
    *
    * is_signed<T>::value == true тогда и только тогда, когда T:
    *    [const] [volatile] char (maybe)
    *    [const] [volatile] signed char
    *    [const] [volatile] short
    *    [const] [volatile] int
    *    [const] [volatile] long
    *    [const] [volatile] long long
    *    [const] [volatile] float
    *    [const] [volatile] double
    *    [const] [volatile] long double
    *
    * Используется, чтобы определить, является ли целочисленный тип подписанным или беззнаковым.
    */
    template <typename T> struct is_signed_helper : public false_type{};

    template <> struct is_signed_helper<signed char>      : public true_type{};
    template <> struct is_signed_helper<signed short>     : public true_type{};
    template <> struct is_signed_helper<signed int>       : public true_type{};
    template <> struct is_signed_helper<signed long>      : public true_type{};
    template <> struct is_signed_helper<signed long long> : public true_type{};
    template <> struct is_signed_helper<float>            : public true_type{};
    template <> struct is_signed_helper<double>           : public true_type{};
    template <> struct is_signed_helper<long double>      : public true_type{};
    #if (CHAR_MAX == SCHAR_MAX)
        template <> struct is_signed_helper<char>         : public true_type{};
    #endif
    #ifndef CORSAC_WCHAR_T_NON_NATIVE
        #if defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 32767)) // GCC определяет __WCHAR_MAX__ для большинства платформ.
            template <> struct is_signed_helper<wchar_t>  : public true_type{};
        #endif
    #endif

    template <typename T>
    struct is_signed : public corsac::is_signed_helper<typename corsac::remove_cv<T>::type>{};

    template <class T>
		constexpr bool is_signed_v = is_signed<T>::value;

    /**
    * is_unsigned
    *
    * is_unsigned<T>::value == true тогда и только тогда, когда T:
    *    [const] [volatile] char (maybe)
    *    [const] [volatile] unsigned char
    *    [const] [volatile] unsigned short
    *    [const] [volatile] unsigned int
    *    [const] [volatile] unsigned long
    *    [const] [volatile] unsigned long long
    *
    * Используется, чтобы определить, является ли целочисленный тип подписанным или беззнаковым.
    */
    template <typename T> struct is_unsigned_helper : public false_type{};

    template <> struct is_unsigned_helper<unsigned char>      : public true_type{};
    template <> struct is_unsigned_helper<unsigned short>     : public true_type{};
    template <> struct is_unsigned_helper<unsigned int>       : public true_type{};
    template <> struct is_unsigned_helper<unsigned long>      : public true_type{};
    template <> struct is_unsigned_helper<unsigned long long> : public true_type{};
    #if (CHAR_MAX == UCHAR_MAX)
        template <> struct is_unsigned_helper<char>           : public true_type{};
    #endif
    #ifndef CORSAC_WCHAR_T_NON_NATIVE // Если wchar_t - это собственный тип, а не просто определение существующего типа ...
        #if defined(_MSC_VER) || (defined(__WCHAR_MAX__) && ((__WCHAR_MAX__ == 4294967295U) || (__WCHAR_MAX__ == 65535))) // GCC определяет __WCHAR_MAX__ для большинства платформ.
            template <> struct is_unsigned_helper<wchar_t> : public true_type{};
        #endif
    #endif

    template <typename T>
    struct is_unsigned : public corsac::is_unsigned_helper<typename corsac::remove_cv<T>::type>{};

    template <class T>
		constexpr bool is_unsigned_v = is_unsigned<T>::value;

    /**
    * alignment_of
    *
    * alignment_of<T>::value является целым значением, представляющим в байтах
    * выравнивание памяти объектов типа T.
    *
    * alignment_of может применяться только к полным типам.
    */
    template <typename T>
    struct alignment_of_value{ static const size_t value = alignof(T); };

    template <typename T>
    struct alignment_of : public integral_constant<size_t, alignment_of_value<T>::value>{};

    template <class T>
		constexpr size_t alignment_of_v = alignment_of<T>::value;

    /**
    * is_aligned
    *
    * Определяется как истина, если для типа требуются более высокие требования к выравниванию,
    * чем выравнивание по умолчанию, которое принимается равным 8.
    * Это позволяет выполнять выделение и размещение специализированных объектов для таких типов.
    */
    template <typename T>
    struct is_aligned_value{ static const bool value = (alignof(T) > 8); };

    template <typename T>
    struct is_aligned : public integral_constant<bool, is_aligned_value<T>::value>{};

    template <class T>
        constexpr size_t is_aligned_v = is_aligned<T>::value;

    /**
    * rank
    *
    * rank<T>::value это целое значение, представляющее количество измерений, которыми обладает тип массива.
    * Например, для многомерного массива типа T[M][N], std::tr1::rank<T[M][N]>::value == 2.
    * Для данного типа не-массива T, std::tr1::rank<T>::value == 0.
    */
    template<typename T>
    struct rank : public corsac::integral_constant<size_t, 0> {};

    template<typename T>
    struct rank<T[]> : public corsac::integral_constant<size_t, rank<T>::value + 1> {};

    template<typename T, size_t N>
    struct rank<T[N]> : public corsac::integral_constant<size_t, rank<T>::value + 1> {};

    template <class T>
        constexpr auto rank_v = rank<T>::value;

    /**
    * is_base_of
    *
    * Для двух (возможно, идентичных) типов Base and Derived, is_base_of<Base, Derived>::value == true
    * тогда и только тогда, когда Base является прямым или косвенным базовым классом Derived,
    * или Base и Derived относятся к одному типу.
    *
    * is_base_of может применяться только к полным типам.
    */
    template <typename Base, typename Derived>
    struct is_base_of : public corsac::integral_constant<bool, __is_base_of(Base, Derived) || corsac::is_same<Base, Derived>::value>{};

    template <typename Base, typename Derived>
        inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

    /**
    * is_lvalue_reference
    */
    template<typename T> struct is_lvalue_reference     : public corsac::false_type {};
    template<typename T> struct is_lvalue_reference<T&> : public corsac::true_type {};

    template<typename T>
        constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    /**
    * is_rvalue_reference
    */
    template <typename T> struct is_rvalue_reference      : public corsac::false_type {};
    template <typename T> struct is_rvalue_reference<T&&> : public corsac::true_type {};

    template<typename T>
        constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    /**
    * result_of
    */
    template<typename> struct result_of;

    template<typename F, typename... ArgTypes>
    struct result_of<F(ArgTypes...)>
    { using type = decltype(corsac::declval<F>()(corsac::declval<ArgTypes>()...)); };

    template <typename T>
        using result_of_t = typename result_of<T>::type;

    /**
    * has_equality
    *
    * Определяет, можно ли проверить равенство указанного типа.
    */
    template <typename, typename = corsac::void_t<>>
    struct has_equality : corsac::false_type {};

    template <typename T>
    struct has_equality<T, corsac::void_t<decltype(corsac::declval<T>() == corsac::declval<T>())>> : corsac::true_type {};

    template <class T>
        constexpr auto has_equality_v = has_equality<T>::value;

    namespace internal
    {
        /**
        * is_complete_type
        *
        * Определяет, является ли указанный тип полным.
        *
        * Предупреждение: будьте осторожны при использовании is_complete_type, поскольку значение фиксируется при первом создании экземпляра.
        *
        * Учтите следующее:
        *      struct Foo;
        *      is_complete_type_v<Foo> // false
        *      struct Foo {};
        *      is_complete_type_v<Foo> // still false
        */
        template<typename T, typename = void>
        struct is_complete_type : public false_type {};

        template<typename T>
        struct is_complete_type<T, corsac::void_t<decltype(sizeof(T) != 0)>> : public true_type {};

        template<>
        struct is_complete_type<const volatile void> : public false_type {};
        template<>
        struct is_complete_type<const void> : public false_type {};
        template<>
        struct is_complete_type<volatile void> : public false_type {};
        template<>
        struct is_complete_type<void> : public false_type {};

        template<typename T>
        struct is_complete_type<T, corsac::enable_if_t<corsac::is_function_v<T>>> : public true_type {};

        template <typename T>
            inline constexpr bool is_complete_type_v = is_complete_type<T, void>::value;
    }

}

#endif //CORSAC_TYPE_PROPERTIES_H
