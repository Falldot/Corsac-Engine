// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_MEMORY_H
#define CORSAC_MEMORY_H

#pragma once

#include "internal/config.h"

#include "internal/type_fundamental.h"
#include "internal/generic_iterator.h"
#include "internal/fill_help.h"
#include "internal/copy_help.h"
#include "internal/type_pod.h"
#include "iterator.h"

namespace corsac
{
    /**
     * addressof
     *
     * From the C++11 Standard, section 20.6.12.1
     *
     * В С++20 был добавлен обновлен см. стандарт
     *
     * Возвращает фактический адрес объекта или функции, на которые ссылается r, даже при наличии перегруженного оператора &.
     */
    template<typename T>
    T* addressof(T& value) noexcept
    {
        // TODO: reinterpret_cast<>() следует заменить на std::bit_cast<>() который был добавлен в C++20 по предложению от SonarLint
        return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(value)));
    }


    namespace Internal
    {
        /**
        * uninitialized_fill_n
        *
        * Копирование, создает диапазон [first, first + n) с
        * заданным входным значением. Возвращает void в
        * соответствии со стандартом C ++, хотя может быть полезно
        * возвращение значения конечного итератора ввода.
        *
        */
        template <typename ForwardIterator, typename Count, typename T>
        inline void uninitialized_fill_n_impl(ForwardIterator first, Count n, const T& value, true_type)
        {
            corsac::fill_n(first, n, value);
        }

        template <typename ForwardIterator, typename Count, typename T>
        void uninitialized_fill_n_impl(ForwardIterator first, Count n, const T& value, false_type)
        {
            using value_type = typename corsac::iterator_traits<ForwardIterator>::value_type;
            ForwardIterator currentDest(first);
            for(; n > 0; --n, ++currentDest)
                ::new(static_cast<void*>(corsac::addressof(*currentDest))) value_type(value);
        }
    }

    template <typename ForwardIterator, typename Count, typename T>
    inline void uninitialized_fill_n(ForwardIterator first, Count n, const T& value)
    {
        using value_type = typename corsac::iterator_traits<ForwardIterator>::value_type;
        Internal::uninitialized_fill_n_impl(first, n, value, corsac::is_trivially_copy_assignable<value_type>());
    }

    /**
     * uninitialized_move_ptr
     *
     * Это специализация uninitialized_move для итераторов, которые являются указателями.
     * Мы используем его, потому что внутри он использует generic_iterator, чтобы указатели работали как обычный corsac::iterator.
     */
    namespace Internal
    {
        template <typename InputIterator, typename ForwardIterator>
        inline ForwardIterator uninitialized_move_impl(InputIterator first, InputIterator last, ForwardIterator dest, true_type)
        {
            return corsac::copy(first, last, dest); // The copy() в свою очередь, будет использовать memcpy для типов is_trivially_copy_assignable (e.g. POD).
        }

        template <typename InputIterator, typename ForwardIterator>
        inline ForwardIterator uninitialized_move_impl(InputIterator first, InputIterator last, ForwardIterator dest, false_type)
        {
            using value_type = typename corsac::iterator_traits<ForwardIterator>::value_type;
            ForwardIterator currentDest(dest);

            // Мы должны запустить цикл по каждому элементу и переместить-построить его в новом месте.
            #if CORSAC_EXCEPTIONS_ENABLED
                try
                {
            #endif
                // Если у value_type есть конструктор перемещения, он будет использоваться здесь.
                for(; first != last; ++first, ++currentDest)
                    ::new(static_cast<void*>(corsac::addressof(*currentDest))) value_type(corsac::move(*first));
            #if CORSAC_EXCEPTIONS_ENABLED
                }
                catch(...)
                {
                    // У нас есть проблема: если во время выполнения цикла ниже возникнет исключение, то у
                    // нас будут значения, которые были перемещены из источника в пункт назначения, которые,
                    // возможно, потребуется переместить обратно в уловку. Что об этом говорится в стандарте
                    // C++ 11? А что будет, если при их обратном перемещении возникнет исключение? Мы можем
                    // захотеть проследить через соответствующую стандартную библиотеку C ++ 11, чтобы
                    // увидеть, что она делает, и сделать что-то подобное. Учитывая, что ссылки rvalue - это
                    // объекты, которые удаляются, нам может не понадобиться перемещать значения назад, хотя
                    // это имеет побочный эффект в виде определенного рода проблемы с потерянными элементами.
                    for(; dest < currentDest; ++dest)
                        (*dest).~value_type();
                    throw;
                }
            #endif
                return currentDest;
                }
    }


    template <typename Iterator>
    inline corsac::move_iterator<Iterator> make_move_if_noexcept_iterator(Iterator i)
    { return corsac::move_iterator<Iterator>(i); }


    /// uninitialized_move
    ///
    /// Moves a source range to a destination, move-constructing the destination with
    /// the source values (and not *assigning* the destination with the source values).
    /// Returns the end of the destination range (i.e. dest + (last - first)).
    ///
    /// uninitialized_move is not part of any current C++ Standard, up to C++14.
    ///
    /// Declaration:
    ///    template <typename InputIterator, typename ForwardIterator>
    ///    ForwardIterator uninitialized_move(InputIterator sourceFirst, InputIterator sourceLast, ForwardIterator destination);
    ///
    /// Example usage:
    ///    SomeClass* pArray = malloc(10 * sizeof(SomeClass));
    ///    uninitialized_move(pSourceDataBegin, pSourceDataBegin + 10, pArray);
    ///
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator dest)
    {
        return corsac::uninitialized_copy(corsac::make_move_iterator(first), corsac::make_move_iterator(last), dest);
    }

    template <typename First, typename Last, typename Result>
    inline Result uninitialized_move_ptr(First first, Last last, Result dest)
    {
        using value_type = typename corsac::iterator_traits<generic_iterator<Result, void> >::value_type;
        const generic_iterator<Result, void> i(Internal::uninitialized_move_impl(corsac::generic_iterator<First, void>(first),
                                                                                 corsac::generic_iterator<Last, void>(last),
                                                                                 corsac::generic_iterator<Result, void>(dest),
                                                                                 corsac::is_trivially_copy_assignable<value_type>()));
        return i.base();
    }

    /**
     * uninitialized_move_if_noexcept
     *
     * From the C++11 Standard, section 20.6.12.1
     *
     * Если итерированный тип можно перемещать без исключений, переместите конструкцию dest с
     * входными данными. Иначе скопируйте-сконструируйте целевой объект с вводом. Если перемещение
     * не поддерживается компилятором, сделайте обычное копирование.
     */
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_move_if_noexcept(InputIterator first, InputIterator last, ForwardIterator dest)
    {
        return corsac::uninitialized_copy(corsac::make_move_if_noexcept_iterator(first), corsac::make_move_if_noexcept_iterator(last), dest);
    }

    /**
     * uninitialized_move_ptr_if_noexcept
     */
    template <typename First, typename Last, typename Result>
    inline Result uninitialized_move_ptr_if_noexcept(First first, Last last, Result dest)
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            return corsac::uninitialized_move_if_noexcept(first, last, dest);
        #else
            return corsac::uninitialized_move_ptr(first, last, dest);
        #endif
    }



    /// uninitialized_fill_n_ptr
    ///
    /// This is a specialization of uninitialized_fill_n for iterators that are pointers.
    /// It exists so that we can declare a value_type for the iterator, which you
    /// can't do with a pointer by itself.
    ///
    template <typename T, typename Count>
    inline void uninitialized_fill_n_ptr(T* first, Count n, const T& value)
    {
        using value_type = typename corsac::iterator_traits<generic_iterator<T*, void> >::value_type;
        Internal::uninitialized_fill_n_impl(corsac::generic_iterator<T*, void>(first), n, value, corsac::is_trivially_copy_assignable<value_type>());
    }


} // namespace corsac


#endif // Header include guard