/**
 * corsac::STL
 *
 * ratio.h
 *
 * Created by Falldot on 16.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_FUNCTIONAL_H
#define CORSAC_FUNCTIONAL_H

#pragma once
/**
 * Описание (Falldot 22.11.2021)
 *
 * Реализует функции стандартной библиотеки C++, которые помогают конструировать объекты функций,
 * также известные как операторов, и их связыватели. Объект-функция — это объект типа,
 * который определяет operator(). Объект-функция может быть указателем на функцию, но,
 * как правило, он используется для хранения дополнительных сведений, которые могут потребоваться
 * при вызове функции.
 *
 * ---------------------------------------------------------------------------------------------------
 * === Классы:
 *
 *      bad_function_call           Класс, который описывает исключение, указывающий, что вызов operator() в объекте-function завершился ошибкой, так как объект был пуст.
 *      function                    Класс, создающий оболочку для вызываемого объекта.
 *      hash                        Класс, который вычисляет хэш-код для значения.
 *      reference_wrapper           Класс, который создает оболочку для ссылки.
 *
 * === Функции:
 *
 *      cref                        Создает конструкцию reference_wrapper из аргумента.
 *      invoke                      Создает обобщенный оператор вызова функции, который работает с указателями функций, указателями функций-членов, вызываемыми объектами и указателями на элементы.
 *      mem_fn                      Создает простую оболочку вызова.
 *      not_fn                      Возвращает дополнение результата объекта функции.
 *      ref                         Создает reference_wrapper из аргумента.
 *      swap                        Меняет местами два объекта function.
 *
 * === Шаблоны:
 *
 *      divides                     Бинарный предикат, который выполняет арифметическую операцию деления элементов заданного типа значения.
 *      equal_to                    Бинарный предикат, который проверяет, равно ли значение заданного типа другому значению этого типа.
 *      greater                     Бинарный предикат, который проверяет, больше ли значение заданного типа другого значения этого типа.
 *      greater_equal               Бинарный предикат, который проверяет, больше или равно ли значение заданного типа другого значения этого типа.
 *      less                        Бинарный предикат, который проверяет, меньше ли значение заданного типа другого значения этого типа.
 *      less_equal                  Бинарный предикат, который проверяет, меньше или равно ли значение заданного типа другого значения этого типа.
 *      logical_and                 Бинарный предикат, который применяет логическую операцию конъюнкции к элементам заданного типа значения и проверяет истинность или ложность результата.
 *      logical_not                 Бинарный предикат, который применяет логическую операцию отрицания к элементам заданного типа значения и проверяет истинность или ложность результата.
 *      logical_or                  Бинарный предикат, который применяет логическую операцию логического сложения к элементам заданного типа значения и проверяет истинность или ложность результата.
 *      minus                       Бинарный предикат, который выполняет арифметическую операцию вычитания для элементов заданного типа значения.
 *      module                      Бинарный предикат, который выполняет арифметическую операцию модуля для элементов заданного типа значения.
 *      multiplies                  Бинарный предикат, который выполняет арифметическую операцию умножения для элементов заданного типа значения.
 *      negate                      Бинарный предикат, который возвращает отрицательное значение элемента.
 *      not_equal_to                Бинарный предикат, который проверяет, не равно ли значение заданного типа другому значению этого типа.
 *      plus                        Бинарный предикат, который выполняет арифметическую операцию сложения для элементов заданного типа значения.
 *
 * === Взаимствование с corsac:
 *
 *      is_reference_wrapper<T>     Т являеться оболочкой для ссылки.
 *      remove_reference_wrapper    Добавляет ссылку на тип при задании reference_wrapper этого типа.
 */
/**
 * === TODO: (С++ 17)
 *      is_bind_expression          Класс, который проверяет, можно ли создать определенный тип, вызывая bind.
 *      is_placeholder              Класс, который проверяет, является ли определенный тип заполнителем.
 *      bind                        Привязывает аргументы к вызываемому объекту.
 *      bit_and                     Возвращает результат применения побитовой логической операции И (бинарный оператор &) между двумя параметрами.
 *      bit_not                     Возвращает результат применения логического дополнения (оператор ~) к параметру.
 *      bit_or                      Возвращает результат применения логической операции ИЛИ (оператор |) между двумя параметрами.
 *      bit_xor                     Возвращает результат применения логической операции ИСКЛЮЧАЮЩЕЕ ИЛИ (оператор ^) между двумя параметрами.
 */

#include "Corsac/internal/config.h""
#include "Corsac/internal/move_help.h"
#include "Corsac/type_traits.h"
#include "Corsac/internal/functional_base.h"
#include "Corsac/internal/mem_fn.h"

namespace corsac
{
    // Primary C++ functions
    template <typename T = void>
    struct plus
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a + b; }
    };

    template <>
    struct plus<void>
    {
        using is_transparent = int;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) + corsac::forward<B>(b))
        { return corsac::forward<A>(a) + corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct minus
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a - b; }
    };

    template <>
    struct minus<void>
    {
        using is_transparent = int;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) - corsac::forward<B>(b))
        { return corsac::forward<A>(a) - corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct multiplies
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a * b; }
    };

    template <>
    struct multiplies<void>
    {
        using is_transparent = int;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) * corsac::forward<B>(b))
        { return corsac::forward<A>(a) * corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct divides
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a / b; }
    };

    template <>
    struct divides<void>
    {
        using is_transparent = int;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) / corsac::forward<B>(b))
        { return corsac::forward<A>(a) / corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct modulus
    {
        constexpr T operator()(const T& a, const T& b) const
        { return a % b; }
    };

    template <>
    struct modulus<void>
    {
        using is_transparent = int;
        template<typename A, typename B>
        constexpr auto operator()(A&& a, B&& b) const
        -> decltype(corsac::forward<A>(a) % corsac::forward<B>(b))
        { return corsac::forward<A>(a) % corsac::forward<B>(b); }
    };

    template <typename T = void>
    struct negate
    {
        constexpr T operator()(const T& a) const
        { return -a; }
    };

    template <>
    struct negate<void>
    {
        using is_transparent = int;
        template<typename T>
        constexpr auto operator()(T&& t) const
        -> decltype(-corsac::forward<T>(t))
        { return -corsac::forward<T>(t); }
    };

    template <typename T = void>
    struct equal_to
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a == b; }
    };

    template <>
    struct equal_to<void>
    {
        using is_transparent = int;
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
    struct not_equal_to
    {
        constexpr bool operator()(const T& a, const T& b) const
        { return a != b; }
    };

    template <>
    struct not_equal_to<void>
    {
        using is_transparent = int;
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
    struct str_equal_to
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
    struct greater
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
    struct str_less
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
    struct greater_equal
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
    struct less_equal
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
    struct logical_and
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
    struct logical_or
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
    struct logical_not
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
            while((c = static_cast<uint8_t>(*p++)) != 0)     // Использование '!=' Отключает предупреждения компилятора.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

    template <> struct hash<const char*>
    {
        size_t operator()(const char* p) const
        {
            uint32_t c, result = 2166136261U;   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = static_cast<uint8_t>(*p++)) != 0)     // приведен к беззнаковому 8 бит.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

#if CORSAC_CHAR8_UNIQUE
    template <> struct hash<char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Хеш FNV1. Пожалуй, лучший строковый хеш. Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while((c = static_cast<uint8_t>(*p++)) != 0)     // Использование '!=' Отключает предупреждения компилятора.
				result = (result * 16777619) ^ c;
			return static_cast<size_t>(result);
		}
	};

	template <> struct hash<const char8_t*>
	{
		size_t operator()(const char8_t* p) const
		{
			uint32_t c, result = 2166136261U;   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while((c = static_cast<uint8_t>(*p++)) != 0)     // приведен к беззнаковому 8 бит.
				result = (result * 16777619) ^ c;
			return static_cast<size_t>(result);
		}
	};
#endif

    template <> struct hash<char16_t*>
    {
        size_t operator()(const char16_t* p) const
        {
            uint32_t c, result = 2166136261U;               // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = static_cast<uint16_t>(*p++)) != 0)   // приведен к беззнаковому 16 бит.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

    template <> struct hash<const char16_t*>
    {
        size_t operator()(const char16_t* p) const
        {
            uint32_t c, result = 2166136261U;               // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = static_cast<uint16_t>(*p++)) != 0)   // приведен к беззнаковому 16 бит.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

    template <> struct hash<char32_t*>
    {
        size_t operator()(const char32_t* p) const
        {
            uint32_t c, result = 2166136261U;                   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = static_cast<uint32_t>(*p++)) != 0)       // приведен к беззнаковому 32 бит.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

    template <> struct hash<const char32_t*>
    {
        size_t operator()(const char32_t* p) const
        {
            uint32_t c, result = 2166136261U;                   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
            while((c = static_cast<uint32_t>(*p++)) != 0)       // приведен к беззнаковому 32 бит.
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };

#if defined(CORSAC_WCHAR_UNIQUE) && CORSAC_WCHAR_UNIQUE
    template<> struct hash<wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;                   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while ((c = static_cast<uint32_t>(*p++)) != 0)      // приведен к беззнаковому 32 бит.
				result = (result * 16777619) ^ c;
			return static_cast<size_t>(result);
		}
	};

	template<> struct hash<const wchar_t*>
	{
		size_t operator()(const wchar_t* p) const
		{
			uint32_t c, result = 2166136261U;                   // Намеренно uint32_t вместо size_t, поэтому поведение одинаково независимо от размера.
			while ((c = static_cast<uint32_t>(*p++)) != 0)      // приведен к беззнаковому 32 бит.
				result = (result * 16777619) ^ c;
			return static_cast<size_t>(result);
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
        using string_type           = String;
        using value_type            = typename String::value_type;
        using unsigned_value_type   = typename corsac::make_unsigned<value_type>::type;

        size_t operator()(const string_type& s) const
        {
            const auto* p = static_cast<const unsigned_value_type*>(s.c_str());
            uint32_t c, result = 2166136261U;
            while((c = *p++) != 0)
                result = (result * 16777619) ^ c;
            return static_cast<size_t>(result);
        }
    };
}

#include "Corsac/internal/function.h"

#endif //CORSAC_FUNCTIONAL_H
