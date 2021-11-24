//
// Created by Falldot on 09.11.2021.
//

#ifndef CORSAC_FUNCTIONAL_BASE_H
#define CORSAC_FUNCTIONAL_BASE_H

#pragma once

#include "Corsac/internal/config.h"

#include "Corsac/internal/memory_base.h"
#include "Corsac/internal/move_help.h"
#include "Corsac/type_traits.h"

namespace corsac
{
    template <typename T>
    inline void swap(T& a, T& b)
    noexcept (corsac::is_nothrow_move_constructible<T>::value && corsac::is_nothrow_move_assignable<T>::value);

    /**
    * invoke
    *
    * invoke - это обобщенный оператор вызова функции, который работает с указателями функций,
    * указателями функций-членов, вызываемыми объектами и указателями на элементы.
    *
    * Для указателей функций и вызываемых объектов (не членов) он возвращает результат вызова
    * объекта функции с указанными аргументами. Для указателей данных членов он просто возвращает член.
    *
    * Обратите внимание, что есть также специализации reference_wrapper для invoke, которые
    * необходимо определить позже, поскольку reference_wrapper использует invoke в своей реализации.
    * Они определяются сразу после определения reference_wrapper.
    */
    template <typename R, typename C, typename T, typename... Args>
    constexpr auto invoke_impl(R C::*func, T&& obj, Args&&... args) noexcept (noexcept((corsac::forward<T>(obj).*func)(corsac::forward<Args>(args)...)))
    -> typename corsac::enable_if<corsac::is_base_of<C, corsac::decay_t<T>>::value,
    decltype((corsac::forward<T>(obj).*func)(corsac::forward<Args>(args)...))>::type
    {
        return (corsac::forward<T>(obj).*func)(corsac::forward<Args>(args)...);
    }

    template <typename F, typename... Args>
    constexpr auto invoke_impl(F&& func, Args&&... args) noexcept(noexcept(corsac::forward<F>(func)(corsac::forward<Args>(args)...)))
    -> decltype(corsac::forward<F>(func)(corsac::forward<Args>(args)...))
    {
        return corsac::forward<F>(func)(corsac::forward<Args>(args)...);
    }

    template <typename R, typename C, typename T, typename... Args>
    constexpr auto invoke_impl(R C::*func, T&& obj, Args&&... args) noexcept(noexcept(((*corsac::forward<T>(obj)).*func)(corsac::forward<Args>(args)...)))
    -> decltype(((*corsac::forward<T>(obj)).*func)(corsac::forward<Args>(args)...))
    {
        return ((*corsac::forward<T>(obj)).*func)(corsac::forward<Args>(args)...);
    }

    template <typename M, typename C, typename T>
    constexpr auto invoke_impl(M C::*member, T&& obj) noexcept(noexcept(corsac::forward<T>(obj).*member))
    -> typename corsac::enable_if<corsac::is_base_of<C, corsac::decay_t<T>>::value,
    decltype(corsac::forward<T>(obj).*member)
    >::type
    {
        return corsac::forward<T>(obj).*member;
    }

    template <typename M, typename C, typename T>
    constexpr auto invoke_impl(M C::*member, T&& obj) noexcept(noexcept((*corsac::forward<T>(obj)).*member))
    -> decltype((*corsac::forward<T>(obj)).*member)
    {
        return (*corsac::forward<T>(obj)).*member;
    }

    template <typename F, typename... Args>
    constexpr decltype(auto) invoke(F&& func, Args&&... args) noexcept(noexcept(invoke_impl(corsac::forward<F>(func), corsac::forward<Args>(args)...)))
    {
        return invoke_impl(corsac::forward<F>(func), corsac::forward<Args>(args)...);
    }

    template <typename F, typename = void, typename... Args>
    struct invoke_result_impl {};

    template <typename F, typename... Args>
    struct invoke_result_impl<F, corsac::void_t<decltype(invoke_impl(corsac::declval<F>(), corsac::declval<Args>()...))>, Args...>
    {
        using type = decltype(invoke_impl(corsac::declval<F>(), corsac::declval<Args>()...));
    };

    template <typename F, typename... Args>
    struct invoke_result : public invoke_result_impl<F, void, Args...> {};

    template <typename F, typename... Args>
    using invoke_result_t = typename invoke_result<F, Args...>::type;

    template <typename F, typename = void, typename... Args>
    struct is_invocable_impl : public corsac::false_type {};

    template <typename F, typename... Args>
    struct is_invocable_impl<F, corsac::void_t<typename corsac::invoke_result<F, Args...>::type>, Args...> : public corsac::true_type {};

    template <typename F, typename... Args>
    struct is_invocable : public is_invocable_impl<F, void, Args...> {};

    template <typename R, typename F, typename = void, typename... Args>
    struct is_invocable_r_impl : public corsac::false_type {};

    template <typename R, typename F, typename... Args>
    struct is_invocable_r_impl<R, F, corsac::void_t<typename invoke_result<F, Args...>::type>, Args...>
    : public corsac::is_convertible<typename invoke_result<F, Args...>::type, R> {};

    template <typename R, typename F, typename... Args>
    struct is_invocable_r : public is_invocable_r_impl<R, F, void, Args...> {};

    template <typename F, typename... Args>
    inline constexpr bool is_invocable_v = is_invocable<F, Args...>::value;

    template <typename R, typename F, typename... Args>
    inline constexpr bool is_invocable_r_v = is_invocable_r<R, F, Args...>::value;

    template <typename F, typename = void, typename... Args>
    struct is_nothrow_invocable_impl : public corsac::false_type	{};

    template <typename F, typename... Args>
    struct is_nothrow_invocable_impl<F, corsac::void_t<typename corsac::invoke_result<F, Args...>::type>, Args...>
    : public corsac::bool_constant<noexcept(corsac::invoke(corsac::declval<F>(), corsac::declval<Args>()...))>	{};

    template <typename F, typename... Args>
    struct is_nothrow_invocable : public is_nothrow_invocable_impl<F, void, Args...> {};

    template <typename R, typename F, typename = void, typename... Args>
    struct is_nothrow_invocable_r_impl : public corsac::false_type {};

    template <typename R, typename F, typename... Args>
    struct is_nothrow_invocable_r_impl<R, F, corsac::void_t<typename corsac::invoke_result<F, Args...>::type>, Args...>
    {
        static constexpr bool value = corsac::is_convertible<typename corsac::invoke_result<F, Args...>::type, R>::value
                                              && corsac::is_nothrow_invocable<F, Args...>::value;
    };

    template <typename R, typename F, typename... Args>
    struct is_nothrow_invocable_r : public is_nothrow_invocable_r_impl<R, F, void, Args...> {};

    template <typename F, typename... Args>
    inline constexpr bool is_no_throw_invocable_v = is_nothrow_invocable<F, Args...>::value;

    template <typename R, typename F, typename... Args>
    inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, F, Args...>::value;

    /**
    * allocator_arg_t
    *
    * allocator_arg_t - это пустой тип класса, используемый для устранения неоднозначности перегрузок
    * конструкторов и функций-членов объектов, поддерживающих распределитель, включая кортеж,
    * функцию, обещание и packaged_task.
    */
    struct allocator_arg_t
    {};

    /**
    * allocator_arg
    *
    * allocator_arg - это константа типа allocator_arg_t, используемая для устранения
    * неоднозначности в месте вызова перегрузок конструкторов и функций-членов объектов,
    * поддерживающих распределитель, таких как кортеж, функция, обещание и packaged_task.
    */
    inline constexpr allocator_arg_t allocator_arg = allocator_arg_t();

    /// less<T>
    template <typename T = void>
    struct less
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a < b; }
    };

    template <>
    struct less<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) < corsac::forward<B>(b))
        { return corsac::forward<A>(a) < corsac::forward<B>(b); }
    };

    /// reference_wrapper
    template <typename T>
    class reference_wrapper
    {
    public:
        using type = T;

        reference_wrapper(T&) noexcept;
        reference_wrapper(T&&) = delete;
        reference_wrapper(const reference_wrapper<T>& x) noexcept;

        reference_wrapper& operator=(const reference_wrapper<T>& x) noexcept;

        operator T& () const noexcept;
        T& get() const noexcept;

        template <typename... ArgTypes>
        typename corsac::result_of<T&(ArgTypes&&...)>::type operator() (ArgTypes&&...) const;

    private:
        T* val;
    };

    template <typename T>
    reference_wrapper<T>::reference_wrapper(T &v) noexcept
    : val(corsac::addressof(v))
    {}

    template <typename T>
    reference_wrapper<T>::reference_wrapper(const reference_wrapper<T>& other) noexcept
    : val(other.val)
    {}

    template <typename T>
    reference_wrapper<T>& reference_wrapper<T>::operator=(const reference_wrapper<T>& other) noexcept
    {
        val = other.val;
        return *this;
    }

    template <typename T>
    reference_wrapper<T>::operator T&() const noexcept
    {
        return *val;
    }

    template <typename T>
    T& reference_wrapper<T>::get() const noexcept
    {
        return *val;
    }

    template <typename T>
    template <typename... ArgTypes>
    typename corsac::result_of<T&(ArgTypes&&...)>::type reference_wrapper<T>::operator() (ArgTypes&&... args) const
    {
        return corsac::invoke(*val, corsac::forward<ArgTypes>(args)...);
    }

    // reference_wrapper-specific utilties
    template <typename T>
    reference_wrapper<T> ref(T& t) noexcept
    {
        return corsac::reference_wrapper<T>(t);
    }

    template <typename T>
    void ref(const T&&) = delete;

    template <typename T>
    reference_wrapper<T> ref(reference_wrapper<T>t) noexcept
    {
        return corsac::ref(t.get());
    }

    template <typename T>
    reference_wrapper<const T> cref(const T& t) noexcept
    {
        return corsac::reference_wrapper<const T>(t);
    }

    template <typename T>
    void cref(const T&&) = delete;

    template <typename T>
    reference_wrapper<const T> cref(reference_wrapper<T> t) noexcept
    {
        return corsac::cref(t.get());
    }

    // reference_wrapper-specific type traits
    template <typename T>
    struct is_reference_wrapper_helper
            : public corsac::false_type {};

    template <typename T>
    struct is_reference_wrapper_helper<corsac::reference_wrapper<T> >
    : public corsac::true_type {};

    template <typename T>
    struct is_reference_wrapper
    : public corsac::is_reference_wrapper_helper<typename corsac::remove_cv<T>::type> {};

    // Помощник, который добавляет ссылку на тип при задании reference_wrapper этого типа.
    template <typename T>
    struct remove_reference_wrapper
    { using type = T; };

    template <typename T>
    struct remove_reference_wrapper< corsac::reference_wrapper<T> >
    { using type = T&; };

    template <typename T>
    struct remove_reference_wrapper< const corsac::reference_wrapper<T> >
    { using type = T&; };

    /**
    * Специализации reference_wrapper для invoke Они должны появиться после определения
    * reference_wrapper, но reference_wrapper должно иметь определение invoke, поэтому эти
    * специализации должны появиться после того, как было определено все остальное.
    */
    template <typename R, typename C, typename T, typename... Args>
    constexpr auto invoke_impl(R C::*func, T&& obj, Args&&... args) noexcept (noexcept((obj.get().*func)(corsac::forward<Args>(args)...)))
    -> typename corsac::enable_if<is_reference_wrapper<corsac::decay_t<T>>::value,
    decltype((obj.get().*func)(corsac::forward<Args>(args)...))>::type
    {
        return (obj.get().*func)(corsac::forward<Args>(args)...);
    }

    template <typename M, typename C, typename T>
    constexpr auto invoke_impl(M C::*member, T&& obj) noexcept(noexcept(obj.get().*member))
    ->  typename corsac::enable_if<is_reference_wrapper<corsac::decay_t<T>>::value,
    decltype(obj.get().*member)>::type
    {
        return obj.get().*member;
    }

    // bind

    /// bind1st
    template <typename Operation>
    class binder1st //: public unary_function<typename Operation::second_argument_type, typename Operation::result_type>
    {
    protected:
        typename Operation::first_argument_type value;
        Operation op;

    public:
        binder1st(const Operation& x, const typename Operation::first_argument_type& y)
                : value(y), op(x) {}

        typename Operation::result_type operator()(const typename Operation::second_argument_type& x) const
        { return op(value, x); }

        typename Operation::result_type operator()(typename Operation::second_argument_type& x) const
        { return op(value, x); }
    };

    template <typename Operation, typename T>
    inline binder1st<Operation> bind1st(const Operation& op, const T& x)
    {
        using value = typename Operation::first_argument_type;
        return binder1st<Operation>(op, value(x));
    }

    /// bind2nd
    template <typename Operation>
    class binder2nd //: public unary_function<typename Operation::first_argument_type, typename Operation::result_type>
    {
    protected:
        Operation op;
        typename Operation::second_argument_type value;

    public:
        binder2nd(const Operation& x, const typename Operation::second_argument_type& y)
                : op(x), value(y) { }

        typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const
        { return op(x, value); }

        typename Operation::result_type operator()(typename Operation::first_argument_type& x) const
        { return op(x, value); }
    };


    template <typename Operation, typename T>
    inline binder2nd<Operation> bind2nd(const Operation& op, const T& x)
    {
        using value = typename Operation::second_argument_type;
        return binder2nd<Operation>(op, value(x));
    }
}

#endif //CORSAC_FUNCTIONAL_BASE_H
