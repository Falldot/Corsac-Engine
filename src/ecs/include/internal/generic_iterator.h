// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_GENERIC_ITERATOR_H
#define CORSAC_GENERIC_ITERATOR_H

#pragma once

#include "internal/config.h"
#include "iterator.h"
#include "type_traits.h"

// Реализует универсальный итератор из заданного итеративного типа, например указателя
// Мы не можем поместить этот файл в наш собственный файл iterator.h, потому что
// нам все еще нужно иметь возможность использовать этот файл, когда у нас отключен наш iterator.h.

namespace corsac
{
    /**
    * generic_iterator
    *
    * Преобразует то, что можно повторить, в формальный итератор. Хотя основная цель этого класса
    * - разрешить преобразование указателя в итератор, вы можете преобразовать все остальное в
    * итератор, определив специализацию iterator_traits<> для этого типа объекта.
    *
    * Example usage:
    *      typedef generic_iterator<int*>       IntArrayIterator;
    *      typedef generic_iterator<int*, char> IntArrayIteratorOther;
    *
    */
    template <typename Iterator, typename Container = void>
    class generic_iterator
    {
        Iterator mIterator;
    public:
        using iterator_category     = typename corsac::iterator_traits<Iterator>::iterator_category;
        using value_type            = typename corsac::iterator_traits<Iterator>::value_type;
        using difference_type       = typename corsac::iterator_traits<Iterator>::difference_type;
        using reference             = typename corsac::iterator_traits<Iterator>::reference;
        using pointer               = typename corsac::iterator_traits<Iterator>::pointer;
        using iterator_type         = Iterator;
        using wrapped_iterator_type = iterator_type;
        using container_type        = Container;
        using this_type             = generic_iterator<Iterator, Container>;

        generic_iterator()
                : mIterator(iterator_type()) { }

        explicit generic_iterator(const iterator_type& x)
                : mIterator(x) { }

        this_type& operator=(const iterator_type& x)
        { mIterator = x; return *this; }

        template <typename Iterator2>
        explicit generic_iterator(const generic_iterator<Iterator2, Container>& x)
                : mIterator(x.base()) { }

        reference operator*() const
        { return *mIterator; }

        pointer operator->() const
        { return mIterator; }

        this_type& operator++()
        { ++mIterator; return *this; }

        this_type operator++(int)
        { return this_type(mIterator++); }

        this_type& operator--()
        { --mIterator; return *this; }

        this_type operator--(int)
        { return this_type(mIterator--); }

        reference operator[](const difference_type& n) const
        { return mIterator[n]; }

        this_type& operator+=(const difference_type& n)
        { mIterator += n; return *this; }

        this_type operator+(const difference_type& n) const
        { return this_type(mIterator + n); }

        this_type& operator-=(const difference_type& n)
        { mIterator -= n; return *this; }

        this_type operator-(const difference_type& n) const
        { return this_type(mIterator - n); }

        const iterator_type& base() const
        { return mIterator; }

    }; // class generic_iterator

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator==(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() == rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator==(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() == rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator!=(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() != rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator!=(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() != rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator<(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() < rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator<(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() < rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator>(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() > rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator>(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() > rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator<=(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() <= rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator<=(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() <= rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline bool operator>=(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() >= rhs.base(); }

    template <typename Iterator, typename Container>
    inline bool operator>=(const generic_iterator<Iterator, Container>& lhs, const generic_iterator<Iterator, Container>& rhs)
    { return lhs.base() >= rhs.base(); }

    template <typename IteratorL, typename IteratorR, typename Container>
    inline typename generic_iterator<IteratorL, Container>::difference_type
    operator-(const generic_iterator<IteratorL, Container>& lhs, const generic_iterator<IteratorR, Container>& rhs)
    { return lhs.base() - rhs.base(); }

    template <typename Iterator, typename Container>
    inline generic_iterator<Iterator, Container>
    operator+(typename generic_iterator<Iterator, Container>::difference_type n, const generic_iterator<Iterator, Container>& x)
    { return generic_iterator<Iterator, Container>(x.base() + n); }

    /**
    * is_generic_iterator
    *
    * Сообщает, является ли итератор одним из этих generic_iterators. Это полезно, если вы хотите
    * написать код, который использует разные итераторы, но хочет узнать, являются ли они generic_iterators.
    * Основная причина для этого заключается в том, что вы можете получить указатель внутри generic_iterator.
    */
    template <typename Iterator>
    struct is_generic_iterator : public false_type { };

    template <typename Iterator, typename Container>
    struct is_generic_iterator<generic_iterator<Iterator, Container> > : public true_type { };

    /**
    * unwrap_generic_iterator
    *
    * Возвращает Iterator::get_base() если он не являеться generic_iterator, вернеть Iterator какой есть.
    *
    * Example usage:
    *      vector<int> intVector;
    *      corsac::generic_iterator<vector<int>::iterator> genericIterator(intVector.begin());
    *      vector<int>::iterator it = unwrap_generic_iterator(genericIterator);
    */
    template <typename Iterator>
    inline typename corsac::is_iterator_wrapper_helper<Iterator, corsac::is_generic_iterator<Iterator>::value>::iterator_type unwrap_generic_iterator(Iterator it)
    { return corsac::is_iterator_wrapper_helper<Iterator, corsac::is_generic_iterator<Iterator>::value>::get_base(it); }

} // namespace corsac

#endif //CORSAC_GENERIC_ITERATOR_H
