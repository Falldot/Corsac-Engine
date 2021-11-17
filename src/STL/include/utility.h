//
// Created by Falldot on 09.11.2021.
//

#ifndef CORSAC_UTILITY_H
#define CORSAC_UTILITY_H

#pragma once

#include "internal/config.h"
#include "type_traits.h"
#include "iterator.h"
#include "internal/functional_base.h"
#include "internal/move_help.h"
#include "internal/base/have.h"

#include "internal/integer_sequence.h"
#include "internal/tuple_fwd_decls.h"
#include "internal/in_place_t.h"
#include "internal/piecewise_construct_t.h"

namespace corsac
{
    /**
    * swap
    *
    * Присваивает содержимое a элементу b, а содержимое b - элементу a.
    * Временный экземпляр типа T создается и уничтожается в процессе.
    *
    * Эта функция используется множеством других алгоритмов,
    * и поэтому в некоторых случаях может оказаться целесообразным
    * и полезным для пользователя реализовать замещающую версию
    * этой функции, которая в некотором роде более эффективна.
    */
    template <typename T>
    inline void swap(T& a, T& b) noexcept (corsac::is_nothrow_move_constructible<T>::value && corsac::is_nothrow_move_assignable<T>::value)
    {
        T temp(corsac::move(a));
        a = corsac::move(b);
        b = corsac::move(temp);
    }

    /**
    * is_swappable
    *
    * is_swappable Определяет, можно ли поменять местами два типа с помощью функции подкачки.
    * Это определяет только то, есть ли функция подкачки, которая соответствует типам,
    * а не то, действительны ли назначения в реализации подкачки.
    *
    * Это признак типа, но он в настоящее время не встречается в <type_traits.h>,
    * так как он зависит от алгоритма подкачки, который находится на более высоком уровне, чем признаки типа.
    *
    *  Пример использования:
    *     static_assert(is_swappable<int>::value, "int should be swappable");
    */
    corsac::unused swap(corsac::argument_sink, corsac::argument_sink);

    template <typename T>
    struct is_swappable
        // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
            : public integral_constant<bool, !corsac::is_same<decltype(swap(corsac::declval<T&>(), corsac::declval<T&>())), corsac::unused>::value> {};

    template <class T>
        constexpr bool is_swappable_v = is_swappable<T>::value;

    /**
    * is_nothrow_swappable
    *
    * Возвращает значение true, если is_swappable, а swap - функция nothrow.
    *
    * Это признак типа, но он в настоящее время не встречается в <type_traits.h>,
    * так как он зависит от алгоритма подкачки, который находится на более высоком уровне, чем признаки типа.
    */
    template <typename T>
    struct is_nothrow_swappable_helper_noexcept_wrapper
    { const static bool value = noexcept(swap(corsac::declval<T&>(), corsac::declval<T&>())); };

    template <typename T, bool>
    struct is_nothrow_swappable_helper
            // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
            : public corsac::integral_constant<bool, is_nothrow_swappable_helper_noexcept_wrapper<T>::value> {};

    template <typename T>
    struct is_nothrow_swappable_helper<T, false>
            : public corsac::false_type {};

    template <typename T>
    struct is_nothrow_swappable
            : public corsac::is_nothrow_swappable_helper<T, corsac::is_swappable<T>::value> {};

    template <class T>
        constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

    // is_swappable_with
    template <typename T, typename U, bool OneTypeIsVoid = (corsac::is_void<T>::value || corsac::is_void<U>::value)>
    struct is_swappable_with_helper
    {
        // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
        static const bool value =
                !corsac::is_same<decltype(swap(corsac::declval<T>(), corsac::declval<U>())), corsac::unused>::value &&
                !corsac::is_same<decltype(swap(corsac::declval<U>(), corsac::declval<T>())), corsac::unused>::value;
    };

    template <typename T, typename U>
    struct is_swappable_with_helper<T,U, true> { static const bool value = false; };

    template<typename T, typename U>
    struct is_swappable_with
            : public corsac::bool_constant<is_swappable_with_helper<T, U>::value> {};

    template <class T, class U>
        constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

    // is_nothrow_swappable_with
    template <typename T, typename U, bool OneTypeIsVoid = (corsac::is_void<T>::value || corsac::is_void<U>::value)>
    struct is_nothrow_swappable_with_helper
    {
        // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
        static const bool value = noexcept(swap(corsac::declval<T>(), corsac::declval<U>())) &&
                                  noexcept(swap(corsac::declval<U>(), corsac::declval<T>()));
    };

    template <typename T, typename U>
    struct is_nothrow_swappable_with_helper<T,U, true> { static const bool value = false; };

    template <typename T, typename U>
    struct is_nothrow_swappable_with : public corsac::bool_constant<is_nothrow_swappable_with_helper<T, U>::value> {};

    template <class T, class U>
    constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T, U>::value;

    // iter_swap helper functions
    template <bool bTypesAreEqual>
    struct iter_swap_impl
    {
        // Обрабатывает ложный случай, когда a и b - разные типы.
        template <typename ForwardIterator1, typename ForwardIterator2>
        static void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
        {
            using value_type_a =typename corsac::iterator_traits<ForwardIterator1>::value_type;

            value_type_a temp(corsac::move(*a));
            *a = corsac::move(*b);
            *b = corsac::move(temp);
        }
    };

    template <>
    struct iter_swap_impl<true>
    {
        template <typename ForwardIterator1, typename ForwardIterator2>
        static void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
        {
            // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
            swap(*a, *b);
        }
    };

    /**
    * iter_swap
    *
    * iter_swap Меняет местами значения элементов, на которые указывают данные итераторы.
    *
    * Эквивалентно swap (a, b), хотя пользователь может предоставить переопределение для iter_swap,
    * которое не зависит от переопределения, которое может существовать для swap.
    *
    * Мы предоставляем версию iter_swap, которая использует подкачку, когда заменяемые типы равны,
    * но в противном случае ручную реализацию. Мы делаем это, потому что в стандартном отчете о
    * дефектах C++ говорится, что iter_swap(a, b) должен быть реализован как swap(a, b), когда это возможно.
    */
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    {
        using value_type_a  = typename corsac::iterator_traits<ForwardIterator1>::value_type;
        using value_type_b  = typename corsac::iterator_traits<ForwardIterator2>::value_type;
        using reference_a   = typename corsac::iterator_traits<ForwardIterator1>::reference;
        using reference_b   = typename corsac::iterator_traits<ForwardIterator2>::reference;

        corsac::iter_swap_impl<corsac::type_and<corsac::is_same<value_type_a, value_type_b>::value, corsac::is_same<value_type_a&, reference_a>::value, corsac::is_same<value_type_b&, reference_b>::value >::value >::iter_swap(a, b);
    }


    /**
    * swap_ranges
    *
    * Своды каждая из элементов в диапазоне [First1, Last1) с
    * Соответствующий элемент в диапазоне [first2, firf2 + (last1 - first1)).
    *
    * Эффекты: для каждого неотрицательного целого числа N <(Last1 - First1),
    * Выполняет: Swap (* (first1 + n), * (first2 + n)).
    *
    * требует: два диапазона [first1, last1) и [first2, first2 + (last1 - first1)))
    * не перекрываются.
    *
    * возвращает: first2 + (last1 - first1).То есть возвращает конец второго диапазона.
    *
    * Сложность: Точно 'Last1 - First1' Swaps.
    */
    template <typename ForwardIterator1, typename ForwardIterator2>
    inline ForwardIterator2
    swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
    {
        for(; first1 != last1; ++first1, ++first2)
            // Не используйте префикс swap с помощью corsac::, так как мы хотим разрешить определяемую пользователем замену через поиск, зависящий от аргументов.
            iter_swap(first1, first2);
        return first2;
    }


    // swap
    template <typename T, size_t N>
    inline void
    swap(T (&a)[N], T (&b)[N]) noexcept(corsac::is_nothrow_swappable<T>::value)
    {
        corsac::swap_ranges(a, a + N, b);
    }


    /**
    * exchange
    *
    * Заменяет значение первого аргумента с указанным новым значением.
    * Возвращаемое значение - предыдущее значение первого аргумента.
    *
    * http://en.cppreference.com/w/cpp/utility/exchange
    */
    template <typename T, typename U = T>
    inline T exchange(T& obj, U&& new_value)
    {
        T old_value = corsac::move(obj);
        obj = corsac::forward<U>(new_value);
        return old_value;
    }


    /**
    * as_const
    *
    * Преобразует «T &» в «Const T &», что упрощает вызов Const функционирует на объектах не-COND.
    *
    * http://en.cppreference.com/w/cpp/utity/as_const.
    *
    * Предложение C++:
    * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4380.html.
    */
    template <class T>
    constexpr typename corsac::add_const<T>::type& as_const(T& t) noexcept
    { return t; }

    // C++ 17 запрещает «Corsac::As_Const» от принятия RValues. Передача ссылки на RVALUE на «CORSAC::AS_CONST»
    // генерирует ссылку «Const T &» или Const Lvalue на временный объект.
    template <class T>
    void as_const(const T&&) = delete;


    /**
    * rel_ops
    *
    * Rel_ops Разрешить автоматическое поколение операторов! =,>, <=,> = от
    * просто операторы == и <.Они намеренно в пространстве имен REL_OPS
    * Так что они не противоречат другим подобным операторам.Использовать их
    * Операторы, добавьте «Использование пространства имен STD :: Rel_ops;»в соответствующее место в
    * Ваш код, как правило, прямо в функции, которые вам нужны для работы.
    * на самом деле, вы, скорее всего, будут проблемы с столкновением, если вы поставите такие
    * с использованием утверждений в любом месте, кроме как в файле .cpp, как так и может
    * также имеют столкновения, когда вы делаете, так как оператор использования повлияет на все
    * код в модуле.Вы должны быть осторожны с использованием Rel_ops.
    */
    namespace rel_ops
    {
        template <typename T>
        inline bool operator!=(const T& x, const T& y)
        { return !(x == y); }

        template <typename T>
        inline bool operator>(const T& x, const T& y)
        { return (y < x); }

        template <typename T>
        inline bool operator<=(const T& x, const T& y)
        { return !(y < x); }

        template <typename T>
        inline bool operator>=(const T& x, const T& y)
        { return !(x < y); }
    }

    /**
    * pair_first_construct
    *
    * Осадки, когда пользователь запрашивает «один первой элемент» парной конструктора.
    */
    struct pair_first_construct_t {};
    constexpr pair_first_construct_t pair_first_construct = pair_first_construct_t();

    /**
    * pair
    *
    * Реализует простую пару, как C++ STD :: Pair.
    */
    template <typename T1, typename T2>
    struct pair
    {
        typedef T1           first_type;
        typedef T2           second_type;
        typedef pair<T1, T2> this_type;

        T1 first;
        T2 second;

        template <typename TT1 = T1,
                typename TT2 = T2,
                class = corsac::enable_if_t<corsac::is_default_constructible_v<TT1> &&
        corsac::is_default_constructible_v<TT2>>>
        constexpr pair()
                : first(), second()
        {}

        // Примечание (Rparolin):
        // Это обходной путь к внутренней ошибке компилятора, которая не может правильно идентифицировать вложенный класс, используя
        // Нестатическое инициализация элемента данных по умолчанию.
        //
        // См. Ошибка, представленная в LLVM для более подробной информации.
        // https://bugs.llvm.org/show_bug.cgi?id=38374
        #if !defined(CORSAC_COMPILER_CLANG)
            template<typename T>
            using single_pair_ctor_sfinae = corsac::enable_if_t<corsac::is_default_constructible_v<T>>;
        #else
            template<typename>
                using single_pair_ctor_sfinae = void;
        #endif

        template <typename TT1 = T1, typename TT2 = T2, typename = single_pair_ctor_sfinae<TT2>>
        constexpr pair(pair_first_construct_t, const TT1& x)
                : first(x), second()
        {}

        // GCC имеет ошибку с перегрузкой шаблонов функций RValue и Lvalue.
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54425.
        //
        // Ошибка: «corsac::Pair <T1, T2> :: Pair (T1 &&) [с t1 = const int &;T2 = const int &] 'не может быть перегружена
        // Ошибка: с 'corsac::pair <t1, t2> :: pair (const t1 &) [с t1 = const int &;T2 = const int &] '
        template <typename TT2 = T2, typename = single_pair_ctor_sfinae<TT2>>
        constexpr pair(pair_first_construct_t, T1&& x)
                : first(corsac::move(x)), second()
        {}

        template <
                typename TT1 = T1,
                typename TT2 = T2,
                class = corsac::enable_if_t<corsac::is_copy_constructible_v<TT1> && corsac::is_copy_constructible_v<TT2>>>
        constexpr pair(const T1& x, const T2& y)
                : first(x), second(y)
        {}

        constexpr pair(pair&& p) = default;
        constexpr pair(const pair&) = default;

        template <
                typename U,
                typename V,
                class = corsac::enable_if_t<corsac::is_convertible_v<const U&, T1> && corsac::is_convertible_v<const V&, T2>>>
        constexpr pair(const pair<U, V>& p)
                : first(p.first), second(p.second)
        {}

        template <typename U,
                typename V,
                typename = corsac::enable_if_t<corsac::is_convertible_v<U, T1> && corsac::is_convertible_v<V, T2>>>
        constexpr pair(U&& u, V&& v)
                : first(corsac::forward<U>(u)), second(corsac::forward<V>(v))
        {}

        template <typename U, typename = corsac::enable_if_t<corsac::is_convertible_v<U, T1>>>
        constexpr pair(U&& x, const T2& y)
                : first(corsac::forward<U>(x)), second(y)
        {}

        template <typename V, typename = corsac::enable_if_t<corsac::is_convertible_v<V, T2>>>
        constexpr pair(const T1& x, V&& y)
                : first(x), second(corsac::forward<V>(y))
        {}

        template <typename U,
                typename V,
                typename = corsac::enable_if_t<corsac::is_convertible_v<U, T1> && corsac::is_convertible_v<V, T2>>>
        constexpr pair(pair<U, V>&& p)
                : first(corsac::forward<U>(p.first)), second(corsac::forward<V>(p.second))
        {}
        // Инициализируется сначала с аргументами типа Args1 ... Получено путем пересылки элементов first_args и
        // инициализирует вторую с аргументами типов Args2 ... Получено путем пересылки элементов Second_args.
        template <class... Args1,
                class... Args2, 
                typename = corsac::enable_if_t<corsac::is_constructible_v<first_type, Args1&&...> &&
                corsac::is_constructible_v<second_type, Args2&&...>>>
        pair(corsac::piecewise_construct_t pwc, corsac::tuple<Args1...> first_args, corsac::tuple<Args2...> second_args)
                : pair(pwc,
                       corsac::move(first_args),
                       corsac::move(second_args),
                       corsac::make_index_sequence<sizeof...(Args1)>(),
                       corsac::make_index_sequence<sizeof...(Args2)>())
        {}

    private:
        // Примечание: внутренний конструктор, используемый для расширения index_sequence, необходимого для раскрытия элементов кортежа.
        template <class... Args1, class... Args2, size_t... I1, size_t... I2>
        pair(corsac::piecewise_construct_t,
             corsac::tuple<Args1...> first_args,
             corsac::tuple<Args2...> second_args,
             corsac::index_sequence<I1...>,
             corsac::index_sequence<I2...>)
                : first(corsac::forward<Args1>(corsac::get<I1>(first_args))...)
                , second(corsac::forward<Args2>(corsac::get<I2>(second_args))...)
        {}

    public:
        pair& operator=(const pair& p)
        noexcept(corsac::is_nothrow_copy_assignable_v<T1>&& corsac::is_nothrow_copy_assignable_v<T2>)
        {
            first = p.first;
            second = p.second;
            return *this;
        }

        template <typename U,
                typename V,
                typename = corsac::enable_if_t<corsac::is_convertible_v<U, T1> && corsac::is_convertible_v<V, T2>>>
        pair& operator=(const pair<U, V>& p)
        {
            first = p.first;
            second = p.second;
            return *this;
        }

        pair& operator=(pair&& p)
        noexcept(corsac::is_nothrow_move_assignable_v<T1>&& corsac::is_nothrow_move_assignable_v<T2>)
        {
            first = corsac::forward<T1>(p.first);
            second = corsac::forward<T2>(p.second);
            return *this;
        }

        template <typename U,
                typename V,
                typename = corsac::enable_if_t<corsac::is_convertible_v<U, T1> && corsac::is_convertible_v<V, T2>>>
        pair& operator=(pair<U, V>&& p)
        {
            first = corsac::forward<U>(p.first);
            second = corsac::forward<V>(p.second);
            return *this;
        }

        void swap(pair& p) noexcept(corsac::is_nothrow_swappable_v<T1>&& corsac::is_nothrow_swappable_v<T2>)
        {
            corsac::iter_swap(&first, &p.first);
            corsac::iter_swap(&second, &p.second);
        }
    };

    /**
    * use_self.
    *
    * Оператор () (x) просто возвращает x.Используется в наборах, в отличие от карт.
    * Это реализация политики шаблона;Это альтернатива
    * Использование_First шаблон реализации.
    *
    * существование использования_ сами может показаться странным, учитывая, что это ничего не делает,
    * Но такие вещи полезны, практически требуются, для оптимального
    * Общее программирование.
    */
    template <typename T>
    struct use_self             //: private unary_function <t, t> // Возможно, мы хотим сделать его подклассом unary_function.
    {
        using result_type = T;

        const T& operator()(const T& x) const
        { return x; }
    };

    /**
    * use_first.
    *
    * Оператор () (x) просто возвращает x.first. Используется в картах, в отличие от наборов.
    */
    template <typename Pair>
    struct use_first
    {
        using argument_type = Pair;
        using result_type   = typename Pair::first_type;

        const result_type& operator()(const Pair& x) const
        { return x.first; }
    };

    /**
    * use_second.
    *
    * Оператор () (x) просто возвращает x.second.
    */
    template <typename Pair>
    struct use_second           // : public unary_function<Pair, typename Pair::second_type> // Возможно, мы хотим сделать его подклассом unary_function.
    {
        using argument_type = Pair;
        using result_type   = typename Pair::second_type;

        const result_type& operator()(const Pair& x) const
        { return x.second; }
    };

    /**
    * global operators
    */

    template <typename T1, typename T2>
    constexpr inline bool operator==(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return ((a.first == b.first) && (a.second == b.second));
    }


    template <typename T1, typename T2>
    constexpr inline bool operator<(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        // Обратите внимание, что мы используем только оператор <в этом выражении.В противном случае мы могли бы
        // Используйте проще: return (a.m1 == b.m1)?(A.M2 <B.M2): (A.M1 <B.M1);
        // Пользователь может написать специализацию для этого оператора, чтобы обойти это
        // В случаях, когда требуется самая высокая производительность.
        return ((a.first < b.first) || (!(b.first < a.first) && (a.second < b.second)));
    }


    template <typename T1, typename T2>
    constexpr inline bool operator!=(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return !(a == b);
    }


    template <typename T1, typename T2>
    constexpr inline bool operator>(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return b < a;
    }


    template <typename T1, typename T2>
    constexpr inline bool operator>=(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return !(a < b);
    }


    template <typename T1, typename T2>
    constexpr inline bool operator<=(const pair<T1, T2>& a, const pair<T1, T2>& b)
    {
        return !(b < a);
    }

    /**
    * make_pair
    *
    * make_pair такой же, как std::make_pair, указанный стандартом C++.
    * Если вы посмотрите на стандарт C++, вы увидите, что он указывает T & вместо T.
    * однако, было определено, что стандарт C++ неверен и имеет
    * пометили его как дефект (http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#181).
    * Если вы чувствуете, что вы хотите более эффективную версию, которая использует ссылки,
    *
    * Примечание: вам обычно не нужно использовать make_pair, чтобы сделать пару.
    * Следующий код эквивалентен, а последний избегает еще одного уровня встроенного:
    * возвращает make_pair (charptr, charptr);
    * возвращается пару <char *, char *> (charptr, charptr);
    */
    template <typename T1, typename T2>
    constexpr inline pair<typename corsac::remove_reference_wrapper<typename corsac::decay<T1>::type>::type,
            typename corsac::remove_reference_wrapper<typename corsac::decay<T2>::type>::type>
    make_pair(T1&& a, T2&& b)
    {
        using T1Type = typename corsac::remove_reference_wrapper<typename corsac::decay<T1>::type>::type;
        using T2Type = typename corsac::remove_reference_wrapper<typename corsac::decay<T2>::type>::type;

        return corsac::pair<T1Type, T2Type>(corsac::forward<T1>(a), corsac::forward<T2>(b));
    }

    // Без следующего VC ++ не выполняет компиляционный код, как это: пара <const char *, int> p = corsac::make_pair <const char *, int> ("hello", 0);
    // Мы определяем ссылочную версию Const альтернативу вышеупомянутым.«Hello» имеет тип Char Const (&) [6] (массив 6-го cons chars),
    // Но VC ++ распадает его на Const Char * и позволяет назвать этот make_pair с этим.VC ++ не удается ниже Make_pair («Hello», «люди»)
    // Потому что вы не можете назначать массивы и до тех пор, пока у нас будет лучшее решение, мы просто отключаем эту специализацию make_pair, когда T1 или T2
    // имеют тип CHAR COND (&) [].
    #if defined(_MSC_VER)
        template <typename T1, typename T2>
        constexpr inline pair<T1, T2> make_pair(
            const T1& a,
            const T2& b,
            typename corsac::enable_if<!corsac::is_array<T1>::value && !corsac::is_array<T2>::value>::type* = 0)
        {
            return corsac::pair<T1, T2>(a, b);
        }
    #endif

    template <typename T1, typename T2>
    class tuple_size<pair<T1, T2>> : public integral_constant<size_t, 2>
    {};

    template <typename T1, typename T2>
    class tuple_size<const pair<T1, T2>> : public integral_constant<size_t, 2>
    {};

    template <typename T1, typename T2>
    class tuple_element<0, pair<T1, T2>>
    {
    public:
        using type = T1;
    };

    template <typename T1, typename T2>
    class tuple_element<1, pair<T1, T2>>
    {
    public:
        using type = T2;
    };

    template <typename T1, typename T2>
    class tuple_element<0, const pair<T1, T2>>
    {
    public:
        using type = const T1;
    };

    template <typename T1, typename T2>
    class tuple_element<1, const pair<T1, T2>>
    {
    public:
        using type = const T2;
    };

    template <size_t I>
    struct GetPair;

    template <>
    struct GetPair<0>
    {
        template <typename T1, typename T2>
        static constexpr T1& getinternal(pair<T1, T2>& p)
        {
            return p.first;
        }

        template <typename T1, typename T2>
        static constexpr const T1& getinternal(const pair<T1, T2>& p)
        {
            return p.first;
        }

        template <typename T1, typename T2>
        static constexpr T1&& getinternal(pair<T1, T2>&& p)
        {
            return corsac::forward<T1>(p.first);
        }
    };

    template <>
    struct GetPair<1>
    {
        template <typename T1, typename T2>
        static constexpr T2& getinternal(pair<T1, T2>& p)
        {
            return p.second;
        }

        template <typename T1, typename T2>
        static constexpr const T2& getinternal(const pair<T1, T2>& p)
        {
            return p.second;
        }

        template <typename T1, typename T2>
        static constexpr T2&& getinternal(pair<T1, T2>&& p)
        {
            return corsac::forward<T2>(p.second);
        }
    };

    template <size_t I, typename T1, typename T2>
    tuple_element_t<I, pair<T1, T2>>& get(pair<T1, T2>& p)
    {
        return GetPair<I>::getinternal(p);
    }

    template <size_t I, typename T1, typename T2>
    const tuple_element_t<I, pair<T1, T2>>& get(const pair<T1, T2>& p)
    {
        return GetPair<I>::getinternal(p);
    }

    template <size_t I, typename T1, typename T2>
    tuple_element_t<I, pair<T1, T2>>&& get(pair<T1, T2>&& p)
    {
        return GetPair<I>::getinternal(corsac::move(p));
    }

} // namespace corsac

#endif //CORSAC_UTILITY_H
