// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_TYPE_TRAITS_H
#define CORSAC_TYPE_TRAITS_H

#pragma once

namespace corsac
{
    /**
     * integral_constant
     *
     * Это базовый класс для различных класификаторов типа, как
     * определено в C ++ 11. По сути, это служебный базовый
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
     * yes_type / no_type
     *
     * Они используются как утилита для отличия двух типов.
     */
    using   yes_type = char ;               // sizeof(yes_type) == 1
    struct  no_type { char padding[8]; };   // sizeof(no_type)  != 1

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
     * type_and
     *
     * Это служебный класс для создания признаков составного типа.
     */
    template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false>
    struct type_or;

    template <bool b1, bool b2, bool b3, bool b4, bool b5>
    struct type_or { static const bool value = true; };

    template <>
    struct type_or<false, false, false, false, false> { static const bool value = false; };

    ///////////////////////////////////////////////////////////////////////
    // type_not
    //
    // Это служебный класс для создания признаков составного типа.
    //
    template <bool b>
    struct type_not{ static const bool value = true; };

    template <>
    struct type_not<true>{ static const bool value = false; };

    /**
     * is_reference
     *
     * is_reference<T>::value == true тогда и только тогда, когда T является
     * ссылочным типом (ссылка l-значения или ссылка r-значения).
     * В эту категорию входят ссылки на типы функций.
     */
    template <typename T> struct is_reference      : public corsac::false_type{};
    template <typename T> struct is_reference<T&>  : public corsac::true_type{};
    template <typename T> struct is_reference<T&&> : public corsac::true_type{};

    template<typename T>
    constexpr bool is_reference_v = is_reference<T>::value;

    ///////////////////////////////////////////////////////////////////////
    // is_same
    //
    // Для двух (возможно, идентичных) типов T и U, is_same<T, U>::value == true
    // тогда и только тогда, когда T и U относятся к одному типу.
    //
    ///////////////////////////////////////////////////////////////////////

    template <typename T, typename U>
    struct is_same : public corsac::false_type { };

    template <typename T>
    struct is_same<T, T> : public corsac::true_type { };

    /**
     * remove_reference
     *
     * Свойство преобразования remove_reference удаляет верхний уровень
     * косвенного обращения по ссылке (при наличии) из типа, к которому он применяется.
     * Для типа T, remove_reference<T &>::type эквивалентен.
     */
    template <typename T> struct remove_reference     { using type = T; };
    template <typename T> struct remove_reference<T&> { using type = T; };
    template <typename T> struct remove_reference<T&&>{ using type = T; };

    template<typename T>
        using remove_reference_t = typename remove_reference<T>::type;

    /**
    * remove_volatile
    *
    * Удаляет volatile из типа.
    *
    * remove_volatile преобразователь типа, который удаляет volatile
    * верхнего уровня (при наличии) из типа, к которому оно применимо.
    * Для типа T, remove_volatile<T volatile>::type эквивалентен.
    * Например, remove_volatile<char*>::type эквивалентен типу char* a,
    * remove_volatile<volatile char*>::type эквивалентен volatile char*.
    * В последнем примере, квалификатор const изменяет char, без *,
    * и поэтому не находится на верхнем уровне.
    */
    template <typename T>           struct remove_volatile                { using type = T;   };
    template <typename T>           struct remove_volatile<volatile T>    { using type = T;   };
    template <typename T>           struct remove_volatile<volatile T[]>  { typedef T type[]; };
    template <typename T, size_t N>  struct remove_volatile<volatile T[N]> { typedef T type[N];};

    /**
    * remove_const
    *
    * Удаляет const из типа.
    *
    * remove_const преобразователь типа, который удаляет const
    * верхнего уровня (при наличии) из типа, к которому оно применимо.
    * Для типа T, remove_const<T const>::type эквивалентен.
    * Например, remove_const<char*>::type эквивалентен типу char* a,
    * remove_const<const char*>::type эквивалентен const char*.
    * В последнем примере, квалификатор const изменяет char, без *,
    * и поэтому не находится на верхнем уровне.
    */
   template <typename T>           struct remove_const             { using type = T;    };
   template <typename T>           struct remove_const<const T>    { using type = T;    };
   template <typename T>           struct remove_const<const T[]>  { typedef T type[];  };
   template <typename T, size_t N>  struct remove_const<const T[N]> { typedef T type[N]; };

     /**
     * remove_cv
     *
     * Удаляет const и volatile из типа.
     *
     * remove_cv преобразователь типа, который удаляет const (и/или) volatile
     * верхнего уровня (при наличии) из типа, к которому оно применимо.
     * Для типа T, remove_cv<T const volatile>::type эквивалентен.
     * Например, remove_cv<char* volatile>::type эквивалентен типу char*,
     * когда как remove_cv<const char*>::type эквивалентен типу const char*.
     * В последнем примере, квалификатор const изменяет char, без *,
     * и поэтому не находится на верхнем уровне.
     *
     */
    template <typename T>
    struct remove_cv { using type = typename corsac::remove_volatile<typename corsac::remove_const<T>::type>::type; };


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


    ///////////////////////////////////////////////////////////////////////
    // add_rvalue_reference
    //
    // Rules (8.3.2 p6):
    //      void + &&  -> void
    //      T    + &&  -> T&&
    //      T&   + &&  -> T&
    //      T&&  + &&  -> T&&
    ///////////////////////////////////////////////////////////////////////

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
    * Преобразует любой тип T в ссылочный тип, что позволяет
    * использовать функции-члены в выражениях decltype без указания
    * конструкторов. Он не используется вне выражений decltype. По
    * дизайну реализации нет, так как она никогда не выполняется, а
    * используется только в выражениях decltype. В разделе 20.2.4
    * стандарта C ++ 11 указано, что мы должны объявить это.
    */
    template <typename T>
    typename corsac::add_rvalue_reference<T>::type declval() noexcept;

    /**
     * conditional
     *
     * Предоставляет тип typedef члена, который определяется как T,
     * если B истинно во время компиляции, или как F, если B имеет значение false.
     */
    template<bool B, typename T, typename F>
    struct conditional { using type = T; };

    template <typename T, typename F>
    struct conditional<false, T, F> { using type = F; };

    template <bool B, class T, class F>
    using conditional_t = typename conditional<B, T, F>::type;


    /**
    * Функция приведения множественных аргументов функции в указаннй тип
    * @public
    * @param args множество аргументов, например (Args&&... args)
    * @return T
    */
    template <typename T>
    constexpr T&& forward(typename corsac::remove_reference<T>::type& args) noexcept
    {
        return static_cast<T&&>(args);
    }

} // namespace corsac


#endif // Header include guard