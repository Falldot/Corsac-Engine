//
// Created by Falldot on 16.11.2021.
//

#ifndef CORSAC_FUNCTIONAL_H
#define CORSAC_FUNCTIONAL_H

#pragma once

#include "internal/config.h"
#include "internal/move_help.h"
#include "type_traits.h"
#include "internal/functional_base.h"
#include "internal/mem_fn.h"

namespace corsac
{
    // Primary C++ functions
    template <typename T = void>
    struct plus : public binary_function<T, T, T>
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a + b; }
    };

    template <>
    struct plus<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) + corsac::forward<B>(b))
        { return corsac::forward<A>(a) + corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct minus : public binary_function<T, T, T>
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a - b; }
    };

    template <>
    struct minus<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) - corsac::forward<B>(b))
        { return corsac::forward<A>(a) - corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct multiplies : public binary_function<T, T, T>
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a * b; }
    };

    template <>
    struct multiplies<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) * corsac::forward<B>(b))
        { return corsac::forward<A>(a) * corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct divides : public binary_function<T, T, T>
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a / b; }
    };

    template <>
    struct divides<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) / corsac::forward<B>(b))
        { return corsac::forward<A>(a) / corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct modulus : public binary_function<T, T, T>
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a % b; }
    };

    template <>
    struct modulus<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) % corsac::forward<B>(b))
        { return corsac::forward<A>(a) % corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct negate : public unary_function<T, T>
    {
        constexpr T operator()(const T& a) const
        { return -a; }
    };

    template <>
    struct negate<void>
    {
        typedef int is_transparent;
        template<typename T>
        constexpr auto operator()(T&& t) const
        -> decltype(-corsac::forward<T>(t))
        { return -corsac::forward<T>(t); }
    };

    template <typename T = void>
    struct equal_to : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a == b; }
    };

    template <>
    struct equal_to<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) == corsac::forward<B>(b))
        { return corsac::forward<A>(a) == corsac::forward<B>(b); }
    };

    template <typename T, typename Compare>
    bool validate_equal_to(const T& a, const T& b, Compare compare)
    {
        return compare(a, b) == compare(b, a);
    }

    template <typename T = void>
    struct not_equal_to : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a != b; }
    };

    template <>
    struct not_equal_to<void>
    {
        typedef int is_transparent;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) != corsac::forward<B>(b))
        { return corsac::forward<A>(a) != corsac::forward<B>(b); }
    };

    template <typename T, typename Compare>
    bool validate_not_equal_to(const T& a, const T& b, Compare compare)
    {
        return compare(a, b) == compare(b, a); // Мы хотим, чтобы не равные результаты сравнения были равны.
    }

    /**
    * str_equal_to
    *
    * Сравнивает два типа строк с завершающим нулем.
    * Ожидается, что типы T будут итераторами или действовать как итераторы.
    * Ожидаемое поведение str_less такое же, как и (strcmp (p1, p2) == 0).
    *
    * Пример использования:
    *     hash_set<const char*, hash<const char*>, str_equal_to<const char*>> stringHashSet;
    *
    * Примечание:
    * Вы не могли использовать str_equal_to так:
    *     bool result = equal("hi", "hi" + 2, "ho", str_equal_to<const char*>());
    * Это связано с тем, что equal проверяет массив чего-либо, с каждым элементом
    * функцией сравнения. Но str_equal_to проверяет массив чего-то самого.
    *
    * На заметку: Обновите этот код, чтобы использовать существующие оптимизации сравнения на
    * основе слов, такие как та, которая используется в функции CorsacStdC Strcmp.
    */
    template <typename T>
    struct str_equal_to : public binary_function<T, T, bool>
    {
        constexpr bool operator()(T a, T b) const
        {
            while(*a && (*a == *b))
            {
                ++a;
                ++b;
            }
            return (*a == *b);
        }
    };

    template <typename T = void>
    struct greater : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a > b; }
    };

    template <>
    struct greater<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) > corsac::forward<B>(b))
        { return corsac::forward<A>(a) > corsac::forward<B>(b); }
    };

    template <typename T, typename Compare>
    bool validate_greater(const T& a, const T& b, Compare compare)
    {
        return !compare(a, b) || !compare(b, a); // If (a > b), then !(b > a)
    }


    template <typename T, typename Compare>
    bool validate_less(const T& a, const T& b, Compare compare)
    {
        return !compare(a, b) || !compare(b, a); // If (a < b), then !(b < a)
    }

    /**
    * str_less
    *
    * Сравнивает два типа строк с завершающим нулем.
    * Ожидается, что типы T будут итераторами или будут действовать как итераторы,
    * включая указатель на массив символов C.
    * Ожидаемое поведение str_less такое же, как и (strcmp (p1, p2) <0).
    *  Эта функция некорректна для Юникода и не гарантирует работу
    * со всеми строками Юникода.
    *
    * Пример использования:
    *     set<const char*, str_less<const char*> > stringSet;
    *
    * На заметку: Обновите этот код, чтобы использовать существующие оптимизации
    * сравнения на основе слов, такие как та, которая используется в функции CorsacStdC Strcmp.
    */
    template <typename T>
    struct str_less : public binary_function<T, T, bool>
    {
        bool operator()(T a, T b) const
        {
            while(static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*a) ==
                  static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*b))
            {
                if(*a == 0)
                    return (*b != 0);
                ++a;
                ++b;
            }

            char aValue = static_cast<typename remove_pointer<T>::type>(*a);
            char bValue = static_cast<typename remove_pointer<T>::type>(*b);

            typename make_unsigned<char>::type aValueU = static_cast<typename make_unsigned<char>::type>(aValue);
            typename make_unsigned<char>::type bValueU = static_cast<typename make_unsigned<char>::type>(bValue);

            return aValueU < bValueU;

            //return (static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*a) <
            //        static_cast<typename make_unsigned<typename remove_pointer<T>::type>::type>(*b));
        }
    };

    template <typename T = void>
    struct greater_equal : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a >= b; }
    };

    template <>
    struct greater_equal<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) >= corsac::forward<B>(b))
        { return corsac::forward<A>(a) >= corsac::forward<B>(b); }
    };

    template <typename T, typename Compare>
    bool validate_greater_equal(const T& a, const T& b, Compare compare)
    {
        return !compare(a, b) || !compare(b, a); // If (a >= b), then !(b >= a)
    }

    template <typename T = void>
    struct less_equal : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a <= b; }
    };

    template <>
    struct less_equal<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) <= corsac::forward<B>(b))
        { return corsac::forward<A>(a) <= corsac::forward<B>(b); }
    };

    template <typename T, typename Compare>
    bool validate_less_equal(const T& a, const T& b, Compare compare)
    {
        return !compare(a, b) || !compare(b, a); // If (a <= b), then !(b <= a)
    }

    template <typename T = void>
    struct logical_and : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a && b; }
    };

    template <>
    struct logical_and<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) && corsac::forward<B>(b))
        { return corsac::forward<A>(a) && corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct logical_or : public binary_function<T, T, bool>
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a || b; }
    };

    template <>
    struct logical_or<void>
    {
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) || corsac::forward<B>(b))
        { return corsac::forward<A>(a) || corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct logical_not : public unary_function<T, bool>
    {
        constexpr bool operator()(const T& a) const
        { return !a; }
    };

    template <>
    struct logical_not<void>
    {
        template<typename T>
        constexpr auto operator()(T&& t) const
        -> decltype(!corsac::forward<T>(t))
        { return !corsac::forward<T>(t); }
    };

    // Dual type functions
    template <typename T, typename U>
    struct equal_to_2 : public binary_function<T, U, bool>
    {
        constexpr bool operator()(const T& a, const U& b) const
        { return a == b; }

        template <typename T_ = T, typename U_ = U, typename = corsac::enable_if_t<!corsac::is_same_v<corsac::remove_const_t<T_>, corsac::remove_const_t<U_>>>>
        constexpr bool operator()(const U& b, const T& a) const
        { return b == a; }
    };

    template <typename T, typename U>
    struct not_equal_to_2 : public binary_function<T, U, bool>
    {
        constexpr bool operator()(const T& a, const U& b) const
        { return a != b; }

        template <typename T_ = T, typename U_ = U, typename = corsac::enable_if_t<!corsac::is_same_v<corsac::remove_const_t<T_>, corsac::remove_const_t<U_>>>>
        constexpr bool operator()(const U& b, const T& a) const
        { return b != a; }
    };


    template <typename T, typename U>
    struct less_2 : public binary_function<T, U, bool>
    {
        constexpr bool operator()(const T& a, const U& b) const
        { return a < b; }

        template <typename T_ = T, typename U_ = U, typename = corsac::enable_if_t<!corsac::is_same_v<corsac::remove_const_t<T_>, corsac::remove_const_t<U_>>>>
        constexpr bool operator()(const U& b, const T& a) const
        { return b < a; }
    };


    /// unary_negate
    template <typename Predicate>
    class unary_negate : public unary_function<typename Predicate::argument_type, bool>
    {
    protected:
        Predicate mPredicate;
    public:
        explicit unary_negate(const Predicate& a)
                : mPredicate(a) {}
        constexpr bool operator()(const typename Predicate::argument_type& a) const
        { return !mPredicate(a); }
    };

    template <typename Predicate>
    inline constexpr unary_negate<Predicate> not1(const Predicate& predicate)
    { return unary_negate<Predicate>(predicate); }



    /// binary_negate
    template <typename Predicate>
    class binary_negate : public binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool>
    {
    protected:
        Predicate mPredicate;
    public:
        explicit binary_negate(const Predicate& a)
                : mPredicate(a) { }
        constexpr bool operator()(const typename Predicate::first_argument_type& a, const typename Predicate::second_argument_type& b) const
        { return !mPredicate(a, b); }
    };

    template <typename Predicate>
    inline constexpr binary_negate<Predicate> not2(const Predicate& predicate)
    { return binary_negate<Predicate>(predicate); }



    /// unary_compose
    template<typename Operation1, typename Operation2>
    struct unary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type>
    {
    protected:
        Operation1 op1;
        Operation2 op2;

    public:
        unary_compose(const Operation1& x, const Operation2& y)
                : op1(x), op2(y) {}

        typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const
        { return op1(op2(x)); }

        typename Operation1::result_type operator()(typename Operation2::argument_type& x) const
        { return op1(op2(x)); }
    };

    template<typename Operation1,typename Operation2>
    inline unary_compose<Operation1,Operation2>
    compose1(const Operation1& op1, const Operation2& op2)
    {
        return unary_compose<Operation1, Operation2>(op1,op2);
    }


    /// binary_compose
    template <class Operation1, class Operation2, class Operation3>
    class binary_compose : public unary_function<typename Operation2::argument_type, typename Operation1::result_type>
    {
    protected:
        Operation1 op1;
        Operation2 op2;
        Operation3 op3;

    public:
        // Support binary functors too.
        typedef typename Operation2::argument_type first_argument_type;
        typedef typename Operation3::argument_type second_argument_type;

        binary_compose(const Operation1& x, const Operation2& y, const Operation3& z)
                : op1(x), op2(y), op3(z) { }

        typename Operation1::result_type operator()(const typename Operation2::argument_type& x) const
        { return op1(op2(x),op3(x)); }

        typename Operation1::result_type operator()(typename Operation2::argument_type& x) const
        { return op1(op2(x),op3(x)); }

        typename Operation1::result_type operator()(const typename Operation2::argument_type& x,const typename Operation3::argument_type& y) const
        { return op1(op2(x),op3(y)); }

        typename Operation1::result_type operator()(typename Operation2::argument_type& x, typename Operation3::argument_type& y) const
        { return op1(op2(x),op3(y)); }
    };


    template <class Operation1, class Operation2, class Operation3>
    inline binary_compose<Operation1, Operation2, Operation3>
    compose2(const Operation1& op1, const Operation2& op2, const Operation3& op3)
    {
        return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
    }

    /**
    * pointer_to_unary_function
    *
    * Это шаблон адаптера, который преобразует указатель на автономную функцию в объект функции.
    * Это позволяет автономным функциям работать во многих случаях,
    * когда системе требуется объект функции.
    *
    * Пример использования:
    *     ptrdiff_t Rand(ptrdiff_t n) { return rand() % n; } // Note: The C rand function is poor and slow.
    *     pointer_to_unary_function<ptrdiff_t, ptrdiff_t> randInstance(Rand);
    *     random_shuffle(pArrayBegin, pArrayEnd, randInstance);
    */
    template <typename Arg, typename Result>
    class pointer_to_unary_function : public unary_function<Arg, Result>
    {
    protected:
        Result (*mpFunction)(Arg);

    public:
        pointer_to_unary_function()
        { }

        explicit pointer_to_unary_function(Result (*pFunction)(Arg))
                : mpFunction(pFunction) { }

        Result operator()(Arg x) const
        { return mpFunction(x); }
    };

    /**
    * ptr_fun
    *
    * Этот ptr_fun - это просто сокращение для использования pointer_to_unary_function.
    *
    * Пример использования (на самом деле вам не нужно здесь использовать ptr_fun, но он все равно работает):
    *    int factorial(int x) { return (x > 1) ? (x * factorial(x - 1)) : x; }
    *    transform(pIntArrayBegin, pIntArrayEnd, pIntArrayBegin, ptr_fun(factorial));
    */
    template <typename Arg, typename Result>
    inline pointer_to_unary_function<Arg, Result>
    ptr_fun(Result (*pFunction)(Arg))
    { return pointer_to_unary_function<Arg, Result>(pFunction); }

    /**
    * pointer_to_binary_function
    *
    * Это шаблон адаптера, который преобразует указатель на автономную функцию в объект функции.
    * Это позволяет автономным функциям работать во многих случаях,
    * когда системе требуется объект функции.
    */
    template <typename Arg1, typename Arg2, typename Result>
    class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result>
    {
    protected:
        Result (*mpFunction)(Arg1, Arg2);

    public:
        pointer_to_binary_function()
        {}

        explicit pointer_to_binary_function(Result (*pFunction)(Arg1, Arg2))
                : mpFunction(pFunction) {}

        Result operator()(Arg1 x, Arg2 y) const
        { return mpFunction(x, y); }
    };

    /**
    * Этот ptr_fun - это просто сокращение для использования pointer_to_binary_function.
    *
    * Пример использования (на самом деле, вам не нужно использовать ptr_fun здесь, но он все равно работает):
    *    int multiply(int x, int y) { return x * y; }
    *    transform(pIntArray1Begin, pIntArray1End, pIntArray2Begin, pIntArray1Begin, ptr_fun(multiply));
    */
    template <typename Arg1, typename Arg2, typename Result>
    inline pointer_to_binary_function<Arg1, Arg2, Result>
    ptr_fun(Result (*pFunction)(Arg1, Arg2))
    { return pointer_to_binary_function<Arg1, Arg2, Result>(pFunction); }

    /**
    * not_fn_ret
    *
    * not_fn_ret - это тип возвращаемого значения corsac::not_fn, указанный в реализации.
    * Имя типа не указано, но у него есть обязательные функции, которые должны поддерживать соответствующие реализации.
    */
    template <typename F>
    struct not_fn_ret
    {
        explicit not_fn_ret(F&& f) : mDecayF(corsac::forward<F>(f)) {}
        not_fn_ret(not_fn_ret&& f) = default;
        not_fn_ret(const not_fn_ret& f) = default;

        // перегрузки для lvalues
        template <class... Args>
        auto operator()(Args&&... args) &
        -> decltype(!corsac::declval<corsac::invoke_result_t<corsac::decay_t<F>&, Args...>>())
        { return !corsac::invoke(mDecayF, corsac::forward<Args>(args)...); }

        template <class... Args>
        auto operator()(Args&&... args) const &
                -> decltype(!corsac::declval<corsac::invoke_result_t<corsac::decay_t<F> const&, Args...>>())
        { return !corsac::invoke(mDecayF, corsac::forward<Args>(args)...); }

        // перегрузки для lvalues
        template <class... Args>
        auto operator()(Args&&... args) &&
        -> decltype(!corsac::declval<corsac::invoke_result_t<corsac::decay_t<F>, Args...>>())
        { return !corsac::invoke(corsac::move(mDecayF), corsac::forward<Args>(args)...); }

        template <class... Args>
        auto operator()(Args&&... args) const &&
                -> decltype(!corsac::declval<corsac::invoke_result_t<corsac::decay_t<F> const, Args...>>())
        { return !corsac::invoke(corsac::move(mDecayF), corsac::forward<Args>(args)...); }

        corsac::decay_t<F> mDecayF;
    };

    /**
    * not_fn
    *
    * Создает функтор, заданный реализацией, который возвращает дополнение переданного вызываемого объекта.
    *
    * Пример использования:
    *		auto nf = corsac::not_fn([]{ return false; });
    *       assert(nf());  // return true
    */
    template <class F>
    constexpr not_fn_ret<F> not_fn(F&& f)
    {
        return not_fn_ret<F>(corsac::forward<F>(f));
    }

    // hash
    namespace internal
    {
        // утилита для отключения универсальной специализации шаблона, которая используется только для типов перечислений.
        template <typename T, bool Enabled>
        struct EnableHashIf {};

        template <typename T>
        struct EnableHashIf<T, true>
        {
            size_t operator()(T p) const { return size_t(p); }
        };
    } // namespace internal


    template <typename T> struct hash;

    template <typename T>
    struct hash : internal::EnableHashIf<T, is_enum_v<T>> {};

    // Обратите внимание, что мы используем указатель как есть и не делим на sizeof(T). Это потому, что таблица имеет простой размер, и это деление не способствует распределению.
    template <typename T> struct hash<T*>
    { size_t operator()(T* p) const { return size_t(uintptr_t(p)); } };

    template <> struct hash<bool>
    { size_t operator()(bool val) const { return static_cast<size_t>(val); } };

    template <> struct hash<char>
    { size_t operator()(char val) const { return static_cast<size_t>(val); } };

    template <> struct hash<signed char>
    { size_t operator()(signed char val) const { return static_cast<size_t>(val); } };

    template <> struct hash<unsigned char>
    { size_t operator()(unsigned char val) const { return static_cast<size_t>(val); } };

    #if defined(CORSAC_CHAR8_UNIQUE) && CORSAC_CHAR8_UNIQUE
        template <> struct hash<char8_t>
                { size_t operator()(char8_t val) const { return static_cast<size_t>(val); } };
    #endif

    #if defined(CORSAC_CHAR16_NATIVE) && CORSAC_CHAR16_NATIVE
        template <> struct hash<char16_t>
                { size_t operator()(char16_t val) const { return static_cast<size_t>(val); } };
    #endif

    #if defined(CORSAC_CHAR32_NATIVE) && CORSAC_CHAR32_NATIVE
        template <> struct hash<char32_t>
                { size_t operator()(char32_t val) const { return static_cast<size_t>(val); } };
    #endif

    // Если wchar_t - это собственный тип, а не просто определение существующего типа ...
    #if !defined(CORSAC_WCHAR_T_NON_NATIVE)
        template <> struct hash<wchar_t>
        { size_t operator()(wchar_t val) const { return static_cast<size_t>(val); } };
    #endif

    template <> struct hash<signed short>
    { size_t operator()(signed short val) const { return static_cast<size_t>(val); } };

    template <> struct hash<unsigned short>
    { size_t operator()(unsigned short val) const { return static_cast<size_t>(val); } };

    template <> struct hash<signed int>
    { size_t operator()(signed int val) const { return static_cast<size_t>(val); } };

    template <> struct hash<unsigned int>
    { size_t operator()(unsigned int val) const { return static_cast<size_t>(val); } };

    template <> struct hash<signed long>
    { size_t operator()(signed long val) const { return static_cast<size_t>(val); } };

    template <> struct hash<unsigned long>
    { size_t operator()(unsigned long val) const { return static_cast<size_t>(val); } };

    template <> struct hash<signed long long>
    { size_t operator()(signed long long val) const { return static_cast<size_t>(val); } };

    template <> struct hash<unsigned long long>
    { size_t operator()(unsigned long long val) const { return static_cast<size_t>(val); } };

    template <> struct hash<float>
    { size_t operator()(float val) const { return static_cast<size_t>(val); } };

    template <> struct hash<double>
    { size_t operator()(double val) const { return static_cast<size_t>(val); } };

    template <> struct hash<long double>
    { size_t operator()(long double val) const { return static_cast<size_t>(val); } };

    #if defined(CORSAC_HAVE_INT128) && CORSAC_HAVE_INT128
        template <> struct hash<uint128_t>
            { size_t operator()(uint128_t val) const { return static_cast<size_t>(val); } };
    #endif


    /**
    * string hashes
    *
    * Обратите внимание, что наши строковые хэши здесь намеренно медленные для длинных строк.
    * Причина этого заключается в следующем:
    *    - Большая часть хешированных строк имеет длину всего несколько байтов.
    *    - Хеш-функция будет значительно более эффективной, если она может сделать это предположение.
    *    - Пользователь может создать собственный хеш для тех редких случаев,
    *      когда необходимо хешировать длинные строки. Действительно, пользователь,
    *      вероятно, может создать специальный хэш, настроенный для таких строк, который лучше,
     *     чем то, что мы предоставляем.
    */
    template <> struct hash<char*>
    {
        size_t operator()(const char* p) const
        {
            uint32_t c, result = 2166136261U;   // Хеш FNV1. Пожалуй, лучший строковый хеш. Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = (uint8_t)*p++) != 0)     // Использование '!=' Отключает предупреждения компилятора.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

    template <> struct hash<const char*>
    {
        size_t operator()(const char* p) const
        {
            uint32_t c, result = 2166136261U;   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = (uint8_t)*p++) != 0)     // приведен к беззнаковому 8 бит.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

#if CORSAC_CHAR8_UNIQUE
    template <> struct hash<char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Хеш FNV1. Пожалуй, лучший строковый хеш. Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while((c = (uint8_t)*p++) != 0)     // Использование '!=' Отключает предупреждения компилятора.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template <> struct hash<const char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while((c = (uint8_t)*p++) != 0)     // приведен к беззнаковому 8 бит.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};
#endif

    template <> struct hash<char16_t*>
    {
        size_t operator()(const char16_t* p) const
        {
            uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
            while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

    template <> struct hash<const char16_t*>
    {
        size_t operator()(const char16_t* p) const
        {
            uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
            while((c = (uint16_t)*p++) != 0)    // cast to unsigned 16 bit.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

    template <> struct hash<char32_t*>
    {
        size_t operator()(const char32_t* p) const
        {
            uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
            while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

    template <> struct hash<const char32_t*>
    {
        size_t operator()(const char32_t* p) const
        {
            uint32_t c, result = 2166136261U;   // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
            while((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };

#if defined(CORSAC_WCHAR_UNIQUE) && CORSAC_WCHAR_UNIQUE
    template<> struct hash<wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;    // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while ((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};

	template<> struct hash<const wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;    // Intentionally uint32_t instead of size_t, so the behavior is the same regardless of size.
			while ((c = (uint32_t)*p++) != 0)    // cast to unsigned 32 bit.
				result = (result * 16777619) ^ c;
			return (size_t)result;
		}
	};
#endif

    /**
    * string_hash
    *
    * Определяет общий хэш строки для произвольного контейнера basic_string CORSAC.
    *
    * Пример использования:
    *    corsac::hash_set<MyString, corsac::string_hash<MyString> > hashSet;
    */
    template <typename String>
    struct string_hash
    {
        typedef String                                              string_type;
        typedef typename String::value_type                         value_type;
        typedef typename corsac::make_unsigned<value_type>::type    unsigned_value_type;

        size_t operator()(const string_type& s) const
        {
            const unsigned_value_type* p = (const unsigned_value_type*)s.c_str();
            uint32_t c, result = 2166136261U;
            while((c = *p++) != 0)
                result = (result * 16777619) ^ c;
            return (size_t)result;
        }
    };
}

#include "internal/function.h"

#endif //CORSAC_FUNCTIONAL_H
