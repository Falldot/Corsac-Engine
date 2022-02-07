/**
 * corsac::STL
 *
 * function.h
 *
 * Created by Falldot on 16.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_FUNCTION_H
#define CORSAC_STL_FUNCTION_H

#pragma once

#include "Corsac/STL/config.h"
#include "Corsac/STL/function_detail.h"

namespace corsac
{
    /**
    * CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE
    *
    * Определяет размер буфера SSO, который используется для хранения указанного состояния захвата вызываемого объекта.
    */
    #ifndef CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE
        #define CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE (2 * sizeof(void*))
    #endif

    static_assert(CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE >= sizeof(void*), "functor storage must be able to hold at least a pointer!");

    template <typename>
    class function;

    template <typename R, typename... Args>
    class function<R(Args...)> : public internal::function_detail<CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE, R(Args...)>
    {
    private:
        using Base = internal::function_detail<CORSAC_FUNCTION_DEFAULT_CAPTURE_SSO_SIZE, R(Args...)>;
    public:
        using typename Base::result_type;

        function() noexcept = default;
        function(std::nullptr_t p) noexcept
                : Base(p)
        {}

        function(const function& other)
                : Base(other)
        {}

        function(function&& other)
                : Base(corsac::move(other))
        {}

        template <typename Functor, typename = CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, function)>
        function(Functor functor)
                : Base(corsac::move(functor))
        {}

        ~function() noexcept = default;

        function& operator=(const function& other)
        {
            Base::operator=(other);
            return *this;
        }

        function& operator=(function&& other)
        {
            Base::operator=(corsac::move(other));
            return *this;
        }

        function& operator=(std::nullptr_t p) noexcept
        {
            Base::operator=(p);
            return *this;
        }

        template <typename Functor, typename = CORSAC_INTERNAL_FUNCTION_VALID_FUNCTION_ARGS(Functor, R, Args..., Base, function)>
        function& operator=(Functor&& functor)
        {
            Base::operator=(corsac::forward<Functor>(functor));
            return *this;
        }

        template <typename Functor>
        function& operator=(corsac::reference_wrapper<Functor> f) noexcept
        {
            Base::operator=(f);
            return *this;
        }

        void swap(function& other) noexcept
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
        #endif // CORSAC_RTTI_ENABLED
    };

    template <typename R, typename... Args>
    bool operator==(const function<R(Args...)>& f, std::nullptr_t) noexcept
    {
        return !f;
    }

    template <typename R, typename... Args>
    bool operator==(std::nullptr_t, const function<R(Args...)>& f) noexcept
    {
        return !f;
    }

    template <typename R, typename... Args>
    bool operator!=(const function<R(Args...)>& f, std::nullptr_t) noexcept
    {
        return !!f;
    }

    template <typename R, typename... Args>
    bool operator!=(std::nullptr_t, const function<R(Args...)>& f) noexcept
    {
        return !!f;
    }

    template <typename R, typename... Args>
    void swap(function<R(Args...)>& lhs, function<R(Args...)>& rhs)
    {
        lhs.swap(rhs);
    }
}

#endif //CORSAC_STL_FUNCTION_H
