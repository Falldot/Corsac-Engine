/**
 * corsac::STL
 *
 * smart_ptr.h
 *
 * Created by Falldot on 18.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_SMART_PTR_H
#define CORSAC_STL_SMART_PTR_H

#pragma once

namespace corsac
{
    namespace internal
    {
        /**
        * Сообщает, имеет ли тип Deleter typedef для указателя на T. Если да, то возвращает его,
        * иначе возвращает T*. В большинстве случаев типом указателя будет T*.
        * Стандарт C++ 11 требует, чтобы scoped_ptr позволял удалителю определять тип указателя.
        *
        * Пример использования:
        *     typedef typename unique_pointer_type<int, SomeDeleter>::type pointer
        */
        template <typename T, typename Deleter>
        class unique_pointer_type
        {
            template <typename U>
            static typename U::pointer test(typename U::pointer*);

            template <typename U>
            static T* test(...);

        public:
            using type = decltype(test<typename corsac::remove_reference<Deleter>::type>(0));
        };

        /**
        * is_array_cv_convertible
        *
        * Сообщает, является ли указатель массива P1 cv-конвертируемым в указатель массива P2.
        * Эти два типа имеют два эквивалентных типа указателей и могут быть преобразованы,
        *  если учесть их постоянные свойства.
        *
        * Пример использования:
        *     is_array_cv_convertible<int, Base*>::value             => false
        *     is_array_cv_convertible<Base, Base*>::value            => false
        *     is_array_cv_convertible<double*, bool*>::value         => false
        *     is_array_cv_convertible<Subclass*, Base*>::value       => false
        *     is_array_cv_convertible<const Base*, Base*>::value     => false
        *     is_array_cv_convertible<Base*, Base*>::value           => true
        *     is_array_cv_convertible<Base*, const Base*>::value     => true
        *     is_array_cv_convertible<Base*, volatile Base*>::value  => true
        */
        template <typename P1, typename P2, bool = corsac::is_same_v<corsac::remove_cv_t<typename pointer_traits<P1>::element_type>,
        corsac::remove_cv_t<typename pointer_traits<P2>::element_type>>>
        struct is_array_cv_convertible_impl
                : public corsac::is_convertible<P1, P2> {}; // Возвращает true, если P1 можно преобразовать в P2.

        template <typename P1, typename P2>
        struct is_array_cv_convertible_impl<P1, P2, false>
                : public corsac::false_type {}; // Базовый тип P1 не совпадает с типом P2, поэтому он не может быть преобразован, даже если P2 относится к подклассу P1. parent == child, но parent[]! = child[]

        template <typename P1, typename P2, bool = corsac::is_scalar_v<P1> && !corsac::is_pointer_v<P1>>
        struct is_array_cv_convertible
                : public is_array_cv_convertible_impl<P1, P2> {};

        template <typename P1, typename P2>
        struct is_array_cv_convertible<P1, P2, true>
                : public corsac::false_type {}; // P1 - это скаляр, а не указатель, поэтому его нельзя преобразовать в указатель.

        /**
        * is_derived
        *
        * Для двух (возможно, идентичных) типов Base и Derived is_base_of<Base, Derived>::value == true
        * тогда и только тогда, когда Base является прямым или косвенным базовым классом Derived. Это похоже на is_base_of <Base, Derived>,
        * но возвращает false, если Derived совпадает с Base. Таким образом, is_dehibited истинно только в том случае, если Derived на самом деле является подклассом
        * Base, а не самим Base. is_dehibited может применяться только к полным типам.
        *
        * is_derived может применяться только к полным типам.
        *
        * Пример использования:
        *     is_derived<int, int>::value             => false
        *     is_derived<int, bool>::value            => false
        *     is_derived<Parent, Child>::value        => true
        *     is_derived<Child, Parent>::value        => false
        */
        template <typename Base, typename Derived>
        struct is_derived : public corsac::integral_constant<bool, corsac::is_base_of<Base, Derived>::value && !corsac::is_same<typename corsac::remove_cv<Base>::type, typename corsac::remove_cv<Derived>::type>::value> {};

        /**
        * is_safe_array_conversion
        *
        * Допустим, у вас есть два типа массивов: T* t и U* u. Вы хотите присвоить u переменной t, но только если это безопасно
        *  Как показано в приведенной ниже логике, преобразование массива безопасно, если U и T конвертируемы,
        * если U является массивом, и если U* или T* не является указателем или U не является производным от T.
        * a pointer or U is not derived from T.
        *
        * Примечание: использование этого класса может заменить на использование is_array_cv_convertible.
        * Что делать: Сделайте эту замену и проверьте ее.
        *
        */

        template <typename T, typename T_pointer, typename U, typename U_pointer>
        struct is_safe_array_conversion : public corsac::integral_constant<bool, corsac::is_convertible<U_pointer, T_pointer>::value &&
                                                                                corsac::is_array<U>::value &&
                                                                                (!corsac::is_pointer<U_pointer>::value || !is_pointer<T_pointer>::value || !internal::is_derived<T, typename corsac::remove_extent<U>::type>::value)> {};
    } // namespace internal

    /**
    * default_delete
    *
    * Класс функции удаления умного указателя C++ 11 по умолчанию.
    *
    * Предоставляет способ удаления объекта по умолчанию. По умолчанию просто вызывается удаление
    * object pointer. Вы можете предоставить альтернативу этому классу или переопределить
    * его для каждого класса, как показано ниже:
    *     template <>
    *     struct smart_ptr_deleter<MyClass>
    *     {
    *         void operator()(MyClass* p) const
    *            { SomeCustomFunction(p); }
    *     };
    */
    template <typename T>
    struct default_delete
    {
        constexpr default_delete() noexcept = default;

        template <typename U>  // Включите, если T* можно построить с помощью U* (т.е. U* можно преобразовать в T*).
        default_delete(const default_delete<U>&, typename corsac::enable_if<is_convertible<U*, T*>::value>::type* = 0) noexcept {}

        void operator()(T* p) const noexcept
        {
            static_assert(corsac::internal::is_complete_type_v<T>, "Attempting to call the destructor of an incomplete type");
            delete p;
        }
    };

    template <typename T>
    struct default_delete<T[]> // Специализация на массивы.
    {
        constexpr default_delete() noexcept = default;

        template <typename U> // Этот ctor включен, если T равно или основано на U, и если U меньше или равно const/volatile, чем T.
        default_delete(const default_delete<U[]>&, typename corsac::enable_if<internal::is_array_cv_convertible<U*, T*>::value>::type* = 0) noexcept {}

        void operator()(T* p) const noexcept
        { delete[] p; }
    };
}

#endif //CORSAC_STL_SMART_PTR_H
