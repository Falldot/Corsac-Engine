//
// Created by Falldot on 16.11.2021.
//

#ifndef CORSAC_MEM_FN_H
#define CORSAC_MEM_FN_H

#pragma once

namespace corsac
{
    // apply_cv
    template <class T, class U,
            bool = corsac::is_const<typename corsac::remove_reference<T>::type>::value,
            bool = corsac::is_volatile<typename corsac::remove_reference<T>::type>::value>
    struct apply_cv { using type = U; };

    template <class T, class U> struct apply_cv<T, U, true, false>   { using type = const U; };
    template <class T, class U> struct apply_cv<T, U, false, true>   { using type = volatile U; };
    template <class T, class U> struct apply_cv<T, U, true, true>    { using type = const volatile U; };
    template <class T, class U> struct apply_cv<T&, U, false, false> { using type = U&; };
    template <class T, class U> struct apply_cv<T&, U, true, false>  { using type = const U&; };
    template <class T, class U> struct apply_cv<T&, U, false, true>  { using type = volatile U&; };
    template <class T, class U> struct apply_cv<T&, U, true, true>   { using type = const volatile U&; };


    // has_result_type
    template <class T>
    struct has_result_type
    {
    private:
        template <class U>
        static corsac::no_type test(...);

        template <class U>
        static corsac::yes_type test(typename U::result_type* = 0);

    public:
        static const bool value = sizeof(test<T>(0)) == sizeof(corsac::yes_type);
    };

    // mem_fn_impl
    template <class T>
    class mem_fn_impl
    {
    public:
        using type = T;

    private:
        type func;

    public:
        constexpr mem_fn_impl(type _func) : func(_func) {}

        template <class... ArgTypes>
        typename invoke_result<type, ArgTypes...>::type operator()(ArgTypes&&... args) const
        {
            return invoke(func, corsac::forward<ArgTypes>(args)...);
        }
    };  // mem_fn_impl

    // mem_fn -> mem_fn_impl adapters
    template <class R, class T>
    constexpr mem_fn_impl<R T::*> mem_fn(R T::*pm)
    { return mem_fn_impl<R T::*>(pm); }

    template <class R, class T>
    constexpr mem_fn_impl<R (T::*)()> mem_fn(R (T::*pm)())
    { return mem_fn_impl<R (T::*)()>(pm); }

    template <class R, class T, class A0>
    constexpr mem_fn_impl<R (T::*)(A0)> mem_fn(R (T::*pm)(A0))
    { return mem_fn_impl<R (T::*)(A0)>(pm); }

    template <class R, class T, class A0, class A1>
    constexpr mem_fn_impl<R (T::*)(A0, A1)> mem_fn(R (T::*pm)(A0, A1))
    { return mem_fn_impl<R (T::*)(A0, A1)>(pm); }

    template <class R, class T, class A0, class A1, class A2>
    constexpr mem_fn_impl<R (T::*)(A0, A1, A2)> mem_fn(R (T::*pm)(A0, A1, A2))
    { return mem_fn_impl<R (T::*)(A0, A1, A2)>(pm); }

    template <class R, class T>
    constexpr mem_fn_impl<R (T::*)() const> mem_fn(R (T::*pm)() const)
    { return mem_fn_impl<R (T::*)() const>(pm); }

    template <class R, class T, class A0>
    constexpr mem_fn_impl<R (T::*)(A0) const> mem_fn(R (T::*pm)(A0) const)
    { return mem_fn_impl<R (T::*)(A0) const>(pm); }

    template <class R, class T, class A0, class A1>
    constexpr mem_fn_impl<R (T::*)(A0, A1) const> mem_fn(R (T::*pm)(A0, A1) const)
    { return mem_fn_impl<R (T::*)(A0, A1) const>(pm); }

    template <class R, class T, class A0, class A1, class A2>
    constexpr mem_fn_impl<R (T::*)(A0, A1, A2) const> mem_fn(R (T::*pm)(A0, A1, A2) const)
    { return mem_fn_impl<R (T::*)(A0, A1, A2) const>(pm); }

    template <class R, class T>
    constexpr mem_fn_impl<R (T::*)() volatile> mem_fn(R (T::*pm)() volatile)
    { return mem_fn_impl<R (T::*)() volatile>(pm); }

    template <class R, class T, class A0>
    constexpr mem_fn_impl<R (T::*)(A0) volatile> mem_fn(R (T::*pm)(A0) volatile)
    { return mem_fn_impl<R (T::*)(A0) volatile>(pm); }

    template <class R, class T, class A0, class A1>
    constexpr mem_fn_impl<R (T::*)(A0, A1) volatile> mem_fn(R (T::*pm)(A0, A1) volatile)
    { return mem_fn_impl<R (T::*)(A0, A1) volatile>(pm); }

    template <class R, class T, class A0, class A1, class A2>
    constexpr mem_fn_impl<R (T::*)(A0, A1, A2) volatile> mem_fn(R (T::*pm)(A0, A1, A2) volatile)
    { return mem_fn_impl<R (T::*)(A0, A1, A2) volatile>(pm); }

    template <class R, class T>
    constexpr mem_fn_impl<R (T::*)() const volatile> mem_fn(R (T::*pm)() const volatile)
    { return mem_fn_impl<R (T::*)() const volatile>(pm); }

    template <class R, class T, class A0>
    constexpr mem_fn_impl<R (T::*)(A0) const volatile> mem_fn(R (T::*pm)(A0) const volatile)
    { return mem_fn_impl<R (T::*)(A0) const volatile>(pm); }

    template <class R, class T, class A0, class A1>
    constexpr mem_fn_impl<R (T::*)(A0, A1) const volatile> mem_fn(R (T::*pm)(A0, A1) const volatile)
    { return mem_fn_impl<R (T::*)(A0, A1) const volatile>(pm); }

    template <class R, class T, class A0, class A1, class A2>
    constexpr mem_fn_impl<R (T::*)(A0, A1, A2) const volatile> mem_fn(R (T::*pm)(A0, A1, A2) const volatile)
    { return mem_fn_impl<R (T::*)(A0, A1, A2) const volatile>(pm); }
}

#endif //CORSAC_MEM_FN_H
