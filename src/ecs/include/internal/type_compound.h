#ifndef CORSAC_TYPE_COMPOUND_H
#define CORSAC_TYPE_COMPOUND_H

#pragma once

#include "type_traits.h"
#include "type_fundamental.h"

namespace corsac
{

    /**
     * is_array
     *
     * is_array<T>::value == true тогда и только тогда когда соответсвует array type
     * включая неограниченные типы массивов.
     */
    template<typename T>
    struct is_array : public corsac::false_type {};

    template<typename T>
    struct is_array<T[]> : public corsac::true_type {};

    template<typename T, size_t N>
    struct is_array<T[N]> : public corsac::true_type {};

    /**
     * is_union
     *
     * is_union<T>::value == true тогда и только тогда, когда T union type.
     *
     * Невозможно определить, является ли тип объединением, без
     * помощи компилятора. На момент написания этой статьи только
     * Metrowerks v8 + поддерживает такую ​​функциональность через
     * 'msl :: is_union <T> :: value'. Пользователь может заставить
     * что-то оцениваться как объединение через CORSAC_DECLARE_UNION.
     */
    template <typename T> struct is_union : public false_type{};

    #define CORSAC_DECLARE_UNION(T) namespace eastl{ template <> struct is_union<T> : public true_type{}; template <> struct is_union<const T> : public true_type{}; }

     /**
     * is_class
     *
     * is_class<T>::value == true тогда и только тогда, когда T class или struct (если он не union type).
     *
     * Без специальной помощи компилятора невозможно различить
     * объединения и классы. В результате is_class ошибочно оценит
     * значение true для типов объединения.
     */
    typedef char yes_array_type[1];
    typedef char no_array_type[2];
    template <typename U> static yes_array_type& is_class_helper(void (U::*)());
    template <typename U> static no_array_type& is_class_helper(...);

    template <typename T>
    struct is_class : public integral_constant<bool,
            sizeof(is_class_helper<T>(0)) == sizeof(yes_array_type) && !is_union<T>::value
    >{};

     /**
     * bool_constant
     *
     * Это удобный помощник для часто используемого integral_constant<bool, value>.
     */
    template <bool B>
    using bool_constant = integral_constant<bool, B>;

     /**
     * is_scalar
     *
     * is_scalar<T>::value == true тогда и только тогда, когда:
     *      is_arithmetic<T>::value == true, or
     *      is_enum<T>::value == true, or
     *      is_pointer<T>::value == true, or
     *      is_member_pointer<T>::value == true, or
     *      is_null_pointer<T>::value == true
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


    ///////////////////////////////////////////////////////////////////////
    // is_member_pointer
    //
    // is_member_pointer<T>::value == true токда и только тогда:
    //    is_member_object_pointer<T>::value == true, or
    //    is_member_function_pointer<T>::value == true
    //
    ///////////////////////////////////////////////////////////////////////
        template <typename T>
        struct is_member_pointer
                : public corsac::integral_constant<bool, corsac::is_member_function_pointer<T>::value>{};

        template <typename T, typename U>
        struct is_member_pointer<U T::*>
                : public corsac::true_type{};


    ///////////////////////////////////////////////////////////////////////
    // is_pointer
    //
    // is_pointer<T>::value == true тогда и только тогда, когда T тип указателя.
    // В эту категорию входят типы указателей на функции, но не указатели на типы членов.
    //
    ///////////////////////////////////////////////////////////////////////

    template <typename T> struct is_pointer_helper : public false_type{};

    template <typename T> struct is_pointer_helper<T*>                : public true_type{};
    template <typename T> struct is_pointer_helper<T* const>          : public true_type{};
    template <typename T> struct is_pointer_helper<T* volatile>       : public true_type{};
    template <typename T> struct is_pointer_helper<T* const volatile> : public true_type{};

    template <typename T>
    struct is_pointer_value : public type_and<is_pointer_helper<T>::value, type_not<is_member_pointer<T>::value>::value> {};

    template <typename T>
    struct is_pointer : public integral_constant<bool, is_pointer_value<T>::value>{};

}

#endif //CORSAC_TYPE_COMPOUND_H
