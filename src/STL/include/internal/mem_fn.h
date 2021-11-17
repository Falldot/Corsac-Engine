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

    // derives_from_unary_function
    // derives_from_binary_function
    template <class T>
    struct derives_from_unary_function
    {
    private:
        static corsac::no_type test(...);

        template <class A, class R>
        static unary_function<A, R> test(const volatile unary_function<A, R>*);

    public:
        static const bool value = !corsac::is_same<decltype(test((T*)0)), corsac::no_type>::value;
        using type = decltype(test((T*)0));
    };

    template <class T>
    struct derives_from_binary_function
    {
    private:
        static corsac::no_type test(...);
        template <class A1, class A2, class R>
        static binary_function<A1, A2, R> test(const volatile binary_function<A1, A2, R>*);

    public:
        static const bool value = !corsac::is_same<decltype(test((T*)0)), corsac::no_type>::value;
        using type = decltype(test((T*)0));
    };

    // maybe_derives_from_unary_function
    // maybe_derives_from_binary_function
    //
    template <class T, bool = derives_from_unary_function<T>::value>
    struct maybe_derive_from_unary_function // bool is true
            : public derives_from_unary_function<T>::type { };

    template <class T>
    struct maybe_derive_from_unary_function<T, false> { };

    template <class T, bool = derives_from_binary_function<T>::value>
    struct maybe_derive_from_binary_function // bool is true
            : public derives_from_binary_function<T>::type { };

    template <class T>
    struct maybe_derive_from_binary_function<T, false> { };

    // weak_result_type_imp
    template <class T, bool = has_result_type<T>::value>
    struct weak_result_type_imp // bool is true
            : public maybe_derive_from_unary_function<T>,
              public maybe_derive_from_binary_function<T>
    {
        using result_type = typename T::result_type;
    };

    template <class T>
    struct weak_result_type_imp<T, false> : public maybe_derive_from_unary_function<T>,
                                            public maybe_derive_from_binary_function<T> { };

    // weak_result_type
    template <class T>
    struct weak_result_type : public weak_result_type_imp<T> { };

    // 0 argument case
    template <class R> struct weak_result_type<R()> { using result_type = R; };
    template <class R> struct weak_result_type<R(&)()> { using result_type = R; };
    template <class R> struct weak_result_type<R (*)()> { using result_type = R; };

    // 1 argument case
    template <class R, class A1> struct weak_result_type<R(A1)> : public unary_function<A1, R> { };
    template <class R, class A1> struct weak_result_type<R(&)(A1)> : public unary_function<A1, R> { };
    template <class R, class A1> struct weak_result_type<R (*)(A1)> : public unary_function<A1, R> { };
    template <class R, class C> struct weak_result_type<R (C::*)()> : public unary_function<C*, R> { };
    template <class R, class C> struct weak_result_type<R (C::*)() const> : public unary_function<const C*, R> { };
    template <class R, class C> struct weak_result_type<R (C::*)() volatile> : public unary_function<volatile C*, R> { };
    template <class R, class C> struct weak_result_type<R (C::*)() const volatile> : public unary_function<const volatile C*, R> { };

    // 2 argument case
    template <class R, class A1, class A2> struct weak_result_type<R(A1, A2)> : public binary_function<A1, A2, R> { };
    template <class R, class A1, class A2> struct weak_result_type<R (*)(A1, A2)> : public binary_function<A1, A2, R> { };
    template <class R, class A1, class A2> struct weak_result_type<R(&)(A1, A2)> : public binary_function<A1, A2, R> { };
    template <class R, class C, class A1> struct weak_result_type<R (C::*)(A1)> : public binary_function<C*, A1, R> { };
    template <class R, class C, class A1> struct weak_result_type<R (C::*)(A1) const> : public binary_function<const C*, A1, R> { };
    template <class R, class C, class A1> struct weak_result_type<R (C::*)(A1) volatile> : public binary_function<volatile C*, A1, R> { };
    template <class R, class C, class A1> struct weak_result_type<R (C::*)(A1) const volatile> : public binary_function<const volatile C*, A1, R> { };

    // 3 or more arguments
    template <class R, class A1, class A2, class A3, class... A4> struct weak_result_type<R(A1, A2, A3, A4...)> { using result_type = R; };
	template <class R, class A1, class A2, class A3, class... A4> struct weak_result_type<R(&)(A1, A2, A3, A4...)> { using result_type = R; };
	template <class R, class A1, class A2, class A3, class... A4> struct weak_result_type<R (*)(A1, A2, A3, A4...)> { using result_type = R; };
	template <class R, class C, class A1, class A2, class... A3> struct weak_result_type<R (C::*)(A1, A2, A3...)> { using result_type = R; };
	template <class R, class C, class A1, class A2, class... A3> struct weak_result_type<R (C::*)(A1, A2, A3...) const> { using result_type = R; };
	template <class R, class C, class A1, class A2, class... A3> struct weak_result_type<R (C::*)(A1, A2, A3...) volatile> { using result_type = R; };
	template <class R, class C, class A1, class A2, class... A3> struct weak_result_type<R (C::*)(A1, A2, A3...) const volatile> { using result_type = R; };

    // mem_fn_impl
    template <class T>
    class mem_fn_impl
    #if defined(CORSAC_COMPILER_MSVC)
            : public weak_result_type<T>
    #endif
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
