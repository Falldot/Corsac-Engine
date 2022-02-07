/**
 * corsac::STL
 *
 * fixed_tuple_vector.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_FIXED_FUNCTION_H
#define CORSAC_STL_FIXED_FUNCTION_H

#pragma once

#include "Corsac/STL/function_detail.h"

namespace corsac
{
    template <int, typename>
    class fixed_function;

    namespace internal
    {
        template <typename>
        struct is_fixed_function
                : public corsac::false_type {};

        template <int SIZE_IN_BYTES, typename R, typename... Args>
        struct is_fixed_function<corsac::fixed_function<SIZE_IN_BYTES, R(Args...)>>
        : public corsac::true_type {};

        template<typename T>
        constexpr bool is_fixed_function_v = is_fixed_function<T>::value;
    }

    #define CORSAC_INTERNAL_FIXED_FUNCTION_STATIC_ASSERT(TYPE)                    \
            static_assert(sizeof(TYPE) <= sizeof(typename Base::FunctorStorageType), \
                          "fixed_function local buffer is not large enough to hold the callable object.")

    #define CORSAC_INTERNAL_FIXED_FUNCTION_NEW_SIZE_STATIC_ASSERT(NEW_SIZE_IN_BYTES) \
            static_assert(SIZE_IN_BYTES >= NEW_SIZE_IN_BYTES,                           \
                          "fixed_function local buffer is not large enough to hold the new fixed_function type.")

    template <typename Functor>
    using CORSAC_DISABLE_OVERLOAD_IF_FIXED_FUNCTION =
    corsac::disable_if_t<internal::is_fixed_function_v<corsac::decay_t<Functor>>>;

    // fixed_function
    template <int SIZE_IN_BYTES, typename R, typename... Args>
    class fixed_function<SIZE_IN_BYTES, R(Args...)> : public internal::function_detail<SIZE_IN_BYTES, R(Args...)>
    {
        using Base = internal::function_detail<SIZE_IN_BYTES, R(Args...)>;

    public:
        using typename Base::result_type;

        fixed_function() noexcept = default;
        fixed_function(std::nullptr_t p) noexcept
                : Base(p)
        {}

        fixed_function(const fixed_function& other)
                : Base(other)
        {}

        fixed_function(fixed_function&& other)
                : Base(corsac::move(other))
        {}

        template <typename Functor,
                typename = CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, fixed_function),
                typename = CORSAC_DISABLE_OVERLOAD_IF_FIXED_FUNCTION<Functor>>
        fixed_function(Functor functor)
                : Base(corsac::move(functor))
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_STATIC_ASSERT(Functor);
        }

        template<int NEW_SIZE_IN_BYTES>
        fixed_function(const fixed_function<NEW_SIZE_IN_BYTES, R(Args...)>& other)
                : Base(other)
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_NEW_SIZE_STATIC_ASSERT(NEW_SIZE_IN_BYTES);
        }

        template<int NEW_SIZE_IN_BYTES>
        fixed_function(fixed_function<NEW_SIZE_IN_BYTES, R(Args...)>&& other)
                : Base(corsac::move(other))
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_NEW_SIZE_STATIC_ASSERT(NEW_SIZE_IN_BYTES);
        }

        ~fixed_function() noexcept = default;

        fixed_function& operator=(const fixed_function& other)
        {
            Base::operator=(other);
            return *this;
        }

        fixed_function& operator=(fixed_function&& other)
        {
            Base::operator=(corsac::move(other));
            return *this;
        }

        fixed_function& operator=(std::nullptr_t p) noexcept
        {
            Base::operator=(p);
            return *this;
        }

        template<int NEW_SIZE_IN_BYTES>
        fixed_function& operator=(const fixed_function<NEW_SIZE_IN_BYTES, R(Args...)>& other)
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_NEW_SIZE_STATIC_ASSERT(NEW_SIZE_IN_BYTES);
            Base::operator=(other);
            return *this;
        }

        template<int NEW_SIZE_IN_BYTES>
        fixed_function& operator=(fixed_function<NEW_SIZE_IN_BYTES, R(Args...)>&& other)
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_NEW_SIZE_STATIC_ASSERT(NEW_SIZE_IN_BYTES);
            Base::operator=(corsac::move(other));
            return *this;
        }

        template <typename Functor,
                typename = CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, fixed_function),
                typename = CORSAC_DISABLE_OVERLOAD_IF_FIXED_FUNCTION<Functor>>
        fixed_function& operator=(Functor&& functor)
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_STATIC_ASSERT(corsac::decay_t<Functor>);
            Base::operator=(corsac::forward<Functor>(functor));
            return *this;
        }

        template <typename Functor>
        fixed_function& operator=(corsac::reference_wrapper<Functor> f) noexcept
        {
            CORSAC_INTERNAL_FIXED_FUNCTION_STATIC_ASSERT(corsac::reference_wrapper<Functor>);
            Base::operator=(f);
            return *this;
        }

        void swap(fixed_function& other) noexcept
        {
            Base::swap(other);
        }

        explicit operator bool() const noexcept
        {
            return Base::operator bool();
        }

        R operator ()(Args... args) const
        {
            return Base::operator ()(corsac::forward<Args>(args)...);
        }

        #if CORSAC_RTTI_ENABLED
            const std::type_info& target_type() const noexcept
            {
                return Base::target_type();
            }

            template <typename Functor>
            Functor* target() noexcept
            {
                return Base::target();
            }

            template <typename Functor>
            const Functor* target() const noexcept
            {
                return Base::target();
            }
        #endif
    };

    template <int S, typename R, typename... Args>
    bool operator==(const fixed_function<S, R(Args...)>& f, std::nullptr_t) noexcept
    {
        return !f;
    }

    template <int S, typename R, typename... Args>
    bool operator==(std::nullptr_t, const fixed_function<S, R(Args...)>& f) noexcept
    {
        return !f;
    }

    template <int S, typename R, typename... Args>
    bool operator!=(const fixed_function<S, R(Args...)>& f, std::nullptr_t) noexcept
    {
        return !!f;
    }

    template <int S, typename R, typename... Args>
    bool operator!=(std::nullptr_t, const fixed_function<S, R(Args...)>& f) noexcept
    {
        return !!f;
    }

    template <int S, typename R, typename... Args>
    void swap(fixed_function<S, R(Args...)>& lhs, fixed_function<S, R(Args...)>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif // CORSAC_STL_FIXED_FUNCTION_H
