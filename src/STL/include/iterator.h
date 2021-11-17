/**
 * corsac::STL
 *
 * iterator.h
 *
 * Created by Falldot on 08.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_ITERATOR_H
#define CORSAC_ITERATOR_H

#pragma once

#include "internal/config.h"
#include "internal/move_help.h"
#include "initializer_list.h"

namespace corsac
{
    /**
     * iterator_status_flag
     *
     * Определяет статус действительности итератора. Это в основном используется
     * для проверки итератора в отладочных сборках. Они реализованы как флаги с
     * возможностью ИЛИ (в отличие от взаимоисключающих значений), чтобы иметь
     * дело с характером статуса итератора. В частности, итератор может быть
     * действительным, но не может быть разыменован, как в случае с итератором
     * для контейнера end (). Итератор может быть действительным, но также может
     * быть разыменован, как в случае с итератором для контейнера begin().
     */
    enum class iterator_status_flag
    {
        isf_none            = 0x00, /// Это называется none и не недействительным, потому что это не является строго противоположностью недействительности.
        isf_valid           = 0x01, /// Итератор действителен, что означает, что он находится в диапазоне [начало, конец].
        isf_current         = 0x02, /// Итератор действителен и указывает на тот же элемент, что и при создании. Например, если итератор указывает на vector :: begin (), но элемент вставлен спереди, итератор действителен, но не является текущим. Модификация элементов на месте не делает итераторы устаревшими.
        isf_can_dereference = 0x04  /// Итератор может быть разыменован, что означает, что он находится в диапазоне [начало, конец). Он может быть текущим, а может и не быть.
    };


    /**
     * Следующие объявления взяты непосредственно из стандартного документа C++.
     *      input_iterator_tag, etc.
     *      iterator
     *      iterator_traits
     *      reverse_iterator
     *
     * Категории итераторов Каждый итератор определяется как принадлежащий к
     * одной из категорий итераторов, которые мы определяем здесь.
     * Эти категории взяты непосредственно из стандарта C++.
     */
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag       : public input_iterator_tag { };
    struct bidirectional_iterator_tag : public forward_iterator_tag { };
    struct random_access_iterator_tag : public bidirectional_iterator_tag { };
    struct contiguous_iterator_tag    : public random_access_iterator_tag { };

    // struct iterator
    template <typename Category, typename T, typename Distance = ptrdiff_t,
            typename Pointer = T*, typename Reference = T&>
    struct iterator
    {
        using iterator_category = Category;
        using value_type        = T;
        using difference_type   = Distance;
        using pointer           = Pointer;
        using reference         = Reference;
    };


    // struct iterator_traits
    template <typename Iterator>
    struct iterator_traits
    {
        using iterator_category = typename Iterator::iterator_category;
        using value_type        = typename Iterator::value_type;
        using difference_type   = typename Iterator::difference_type;
        using pointer           = typename Iterator::pointer;
        using reference         = typename Iterator::reference;
    };

    template <typename T>
    struct iterator_traits<T*>
    {
        using iterator_category = corsac::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    };

    template <typename T>
    struct iterator_traits<const T*>
    {
        using iterator_category = corsac::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;
    };

    /**
    * is_iterator_wrapper
    *
    * Сообщает, является ли тип Iterator типом-оболочкой, а не обычным типом.
    * Опирается на класс, объявляющий определение типа с именем wrapped_iterator_type.
    *
    * Примеры обертывания итераторов:
    *     reverse_iterator
    *     generic_iterator
    *     move_iterator
    * Примеры итераторов без обертывания:
    *     iterator
    *     list::iterator
    *     char*
    *
    * Пример поведения:
    *     is_iterator_wrapper(int*)::value                                              => false
    *     is_iterator_wrapper(corsac::array<char>*)::value                               => false
    *     is_iterator_wrapper(corsac::vector<int>::iterator)::value                      => false
    *     is_iterator_wrapper(corsac::generic_iterator<int*>)::value                     => true
    *     is_iterator_wrapper(corsac::move_iterator<corsac::array<int>::iterator>)::value => true
    */
    template<typename Iterator>
    class is_iterator_wrapper
    {
        template<typename>
        static corsac::no_type test(...);

        template<typename U>
        static corsac::yes_type test(typename U::wrapped_iterator_type*, typename corsac::enable_if<corsac::is_class<U>::value>::type* = 0);

    public:
        static const bool value = (sizeof(test<Iterator>(NULL)) == sizeof(corsac::yes_type));
    };

    /**
    * unwrap_iterator
    *
    * Принимает экземпляр Iterator-оболочки (например, move_iterator, reverse_iterator, generic_iterator)
    * и возвращает обернутый тип итератора. Если Iterator не является оболочкой (включая указатель)
    * или не является итератором, эта функция возвращает его как есть.
    * unwrap_iterator разворачивает только один слой итератора за раз. Вам нужно вызвать его
    * дважды, например, чтобы развернуть два слоя итераторов.
    *
    * Пример использования:
    *     int* pInt             = unwrap_iterator(&pIntArray[15]);
    *     int* pInt             = unwrap_iterator(generic_iterator(&pIntArray[15]));
    *     MyVector::iterator it = unwrap_iterator(myVector.begin());
    *     MyVector::iterator it = unwrap_iterator(move_iterator(myVector.begin()));
    */
    template <typename Iterator, bool isWrapper>
    struct is_iterator_wrapper_helper
    {
        using iterator_type = Iterator;

        static iterator_type get_base(Iterator it)
        { return it; }
    };


    template <typename Iterator>
    struct is_iterator_wrapper_helper<Iterator, true>
    {
        using iterator_type = typename Iterator::iterator_type;

        static iterator_type get_base(Iterator it)
        { return it.base(); }
    };

    template <typename Iterator>
    inline typename is_iterator_wrapper_helper<Iterator, corsac::is_iterator_wrapper<Iterator>::value>::iterator_type unwrap_iterator(Iterator it)
    { return corsac::is_iterator_wrapper_helper<Iterator, corsac::is_iterator_wrapper<Iterator>::value>::get_base(it); }

    /**
    * reverse_iterator
    *
    * Из стандарта C++:
    * итераторы двунаправленного и произвольного доступа имеют соответствующие
    * адаптеры обратных итераторов, которые выполняют итерацию по структуре данных в
    * противоположном направлении. Они имеют те же сигнатуры, что и соответствующие итераторы.
    * Фундаментальная связь между обратным итератором и соответствующим ему итератором i
    * устанавливается тождеством:
    *     &*(reverse_iterator(i)) == &*(i - 1).
    *
    * Это сопоставление продиктовано тем фактом, что, хотя всегда есть указатель
    * за концом массива, может не быть действительного указателя перед началом массива.
    */
    template <typename Iterator>
    class reverse_iterator : public iterator<typename corsac::iterator_traits<Iterator>::iterator_category,
            typename corsac::iterator_traits<Iterator>::value_type,
            typename corsac::iterator_traits<Iterator>::difference_type,
            typename corsac::iterator_traits<Iterator>::pointer,
            typename corsac::iterator_traits<Iterator>::reference>
    {
    public:
        using iterator_type             = Iterator;
        using wrapped_iterator_type     = iterator_type; // Этого нет в стандарте C++; он используется, чтобы идентифицировать его как тип итератора-оболочки.
        using pointer                   = typename corsac::iterator_traits<Iterator>::pointer;
        using reference                 = typename corsac::iterator_traits<Iterator>::reference;
        using difference_type           = typename corsac::iterator_traits<Iterator>::difference_type;

    protected:
        Iterator mIterator;

    public:
        // Важно, чтобы мы построили mIterator, потому что если Iterator является указателем.
        constexpr reverse_iterator()
                : mIterator() { }

        constexpr explicit reverse_iterator(iterator_type i)
                : mIterator(i) { }

        constexpr reverse_iterator(const reverse_iterator& ri)
                : mIterator(ri.mIterator) { }

        template <typename U>
        constexpr explicit reverse_iterator(const reverse_iterator<U>& ri)
                : mIterator(ri.base()) { }

        // Этот оператор = не входит в стандарт, но рабочая группа библиотеки C++
        // предварительно одобрила его, поскольку он позволяет
        //  взаимодействовать константным и неконстантным reverse_iterators.
        template <typename U>
        constexpr reverse_iterator<Iterator>& operator=(const reverse_iterator<U>& ri)
        { mIterator = ri.base(); return *this; }

        constexpr iterator_type base() const
        { return mIterator; }

        constexpr reference operator*() const
        {
            iterator_type i(mIterator);
            return *--i;
        }

        constexpr pointer operator->() const
        { return &(operator*()); }

        constexpr reverse_iterator& operator++()
        { --mIterator; return *this; }

        constexpr reverse_iterator operator++(int)
        {
            reverse_iterator ri(*this);
            --mIterator;
            return ri;
        }

        constexpr reverse_iterator& operator--()
        { ++mIterator; return *this; }

        constexpr reverse_iterator operator--(int)
        {
            reverse_iterator ri(*this);
            ++mIterator;
            return ri;
        }

        constexpr reverse_iterator operator+(difference_type n) const
        { return reverse_iterator(mIterator - n); }

        constexpr reverse_iterator& operator+=(difference_type n)
        { mIterator -= n; return *this; }

        constexpr reverse_iterator operator-(difference_type n) const
        { return reverse_iterator(mIterator + n); }

        constexpr reverse_iterator& operator-=(difference_type n)
        { mIterator += n; return *this; }

        // http://cplusplus.github.io/LWG/lwg-defects.html#386,
        // http://llvm.org/bugs/show_bug.cgi?id=17883
        // random_access_iterator operator[] просто необходим для возврата чего-то, что можно преобразовать в ссылку.
        // reverse_iterator operator[] не обязательно может знать, что возвращать, поскольку основной итератор
        // operator[] может возвращать что-то, кроме ссылки.
        constexpr reference operator[](difference_type n) const
        { return mIterator[-n - 1]; }
    };

    // Рабочая группа библиотеки C++ предварительно одобрила использование двух
    // параметров шаблона (Iterator1 и Iterator2), чтобы обеспечить сопоставимость итераторов reverse_iterators
    // и const_reverse. Это аналогичная проблема с отчетом о дефектах C++ 179 относительно сравнения
    // итераторов контейнера и const_iterators.
    //
    // libstdc++ сообщает, что std::relops нарушает использование двух типов итераторов, и если мы
    // хотим поддерживать повторные операции, нам также необходимо сделать версии каждого из нижеприведенных с одним
    // параметром шаблона для размещения std::relops. Но relops почти не используется
    // из-за проблем, которые они вызывают, и поэтому мы избегаем поддержки здесь, пока кто-то не пожалуется на это.
    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator==(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() == b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator<(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() > b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator!=(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() != b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator>(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() < b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator<=(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() >= b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline bool
    operator>=(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return a.base() <= b.base(); }

    template <typename Iterator1, typename Iterator2>
    constexpr inline typename reverse_iterator<Iterator1>::difference_type
    operator-(const reverse_iterator<Iterator1>& a, const reverse_iterator<Iterator2>& b)
    { return b.base() - a.base(); }

    template <typename Iterator>
    constexpr inline reverse_iterator<Iterator>
    operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& a)
    { return reverse_iterator<Iterator>(a.base() - n); }

    /**
    * is_reverse_iterator
    *
    * Это служебная программа для расширения свойств типов.
    * Учитывая итератор, сообщает, является ли это reverse_iterator по сравнению с чем-либо еще.
    * Если это обратный итератор, обернутый другим итератором, тогда значение false.
    * Для рассмотрения: Определите, что если это move_iterator<reverse_iterator> и разверните
    * move_iterator  чтобы мы могли обнаружить это под ним reverse_iterator.
    */
    template <typename T>
    struct is_reverse_iterator
            : public corsac::false_type {};

    template<typename Iterator>
    struct is_reverse_iterator< corsac::reverse_iterator<Iterator> >
    : public corsac::true_type {};


    /// move_iterator
    ///
    /// From the C++11 Standard, section 24.5.3.1:
    /// Class template move_iterator is an iterator adaptor with the same behavior as the underlying iterator
    /// except that its dereference operator implicitly converts the value returned by the underlying iterator's
    /// dereference operator to an rvalue reference. Some generic algorithms can be called with move iterators to
    /// replace copying with moving.

    template<typename Iterator>
    class move_iterator // Don't inherit from iterator.
    {
    private:
        using WrappedIteratorReference = typename iterator_traits<Iterator>::reference;

    public:
        using iterator_type             = Iterator;
        using wrapped_iterator_type     = iterator_type; // Этого нет в стандарте C++; он используется, чтобы идентифицировать его как тип итератора-оболочки.
        using traits_type               = iterator_traits<Iterator>;
        using iterator_category         = typename traits_type::iterator_category;
        using value_type                = typename traits_type::value_type;
        using difference_type           = typename traits_type::difference_type;
        using pointer                   = Iterator;
        using reference                 = conditional_t<is_reference<WrappedIteratorReference>::value,
                                            remove_reference_t<WrappedIteratorReference>&&,
                                                WrappedIteratorReference>;
    protected:
        iterator_type mIterator;

    public:
        move_iterator()
                : mIterator()
        {}

        explicit move_iterator(iterator_type mi)
                : mIterator(mi)
        {}

        template<typename U>
        explicit move_iterator(const move_iterator<U>& mi)
                : mIterator(mi.base())
        {}

        iterator_type base() const
        { return mIterator; }

        reference operator*() const { return static_cast<reference>(*mIterator); }

        pointer operator->() const
        { return mIterator; }

        move_iterator& operator++()
        {
            ++mIterator;
            return *this;
        }

        move_iterator operator++(int)
        {
            move_iterator tempMoveIterator = *this;
            ++mIterator;
            return tempMoveIterator;
        }

        move_iterator& operator--()
        {
            --mIterator;
            return *this;
        }

        move_iterator operator--(int)
        {
            move_iterator tempMoveIterator = *this;
            --mIterator;
            return tempMoveIterator;
        }

        move_iterator operator+(difference_type n) const
        { return move_iterator(mIterator + n); }

        move_iterator& operator+=(difference_type n)
        {
            mIterator += n;
            return *this;
        }

        move_iterator operator-(difference_type n) const
        { return move_iterator(mIterator - n); }

        move_iterator& operator-=(difference_type n)
        {
            mIterator -= n;
            return *this;
        }

        reference operator[](difference_type n) const
        { return corsac::move(mIterator[n]); }
    };

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator==(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return a.base() == b.base(); }

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator!=(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return !(a == b); }

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator<(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return a.base() < b.base(); }

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator<=(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return !(b < a); }

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator>(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return b < a; }

    template<typename Iterator1, typename Iterator2>
    inline bool
    operator>=(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b)
    { return !(a < b); }

    template<typename Iterator1, typename Iterator2>
    inline auto
    operator-(const move_iterator<Iterator1>& a, const move_iterator<Iterator2>& b) -> decltype(a.base() - b.base())
    { return a.base() - b.base(); }

    template<typename Iterator>
    inline move_iterator<Iterator>
    operator+(typename move_iterator<Iterator>::difference_type n, const move_iterator<Iterator>& a)
    { return a + n; }

    template<typename Iterator>
    inline move_iterator<Iterator> make_move_iterator(Iterator i)
    { return move_iterator<Iterator>(i); }

    /**
    * make_move_if_noexcept_iterator возвращает move_iterator<Iterator> если Iterator имеет тип noexcept;
    * в противном случае возвращает Iterator как есть. Смысл этого в том, чтобы иметь возможность избежать перемещений,
    * которые могут генерировать исключения, и вместо этого вернуться к копиям или чему-то еще, что возвращает
    * оператор IteratorType:: по умолчанию для использования алгоритмами copymove.
    * Для рассмотрения: объедините использование условного выражения здесь с тем, которое используется в move_if_noexcept,
    * поскольку это одно и то же условие.
    */
    #if CORSAC_EXCEPTIONS_ENABLED
        template <typename Iterator, typename IteratorType = typename corsac::conditional<corsac::is_nothrow_move_constructible<typename corsac::iterator_traits<Iterator>::value_type>::value ||
                                                                                             !corsac::is_copy_constructible<typename corsac::iterator_traits<Iterator>::value_type>::value,
                                                                                             corsac::move_iterator<Iterator>, Iterator>::type>
            inline IteratorType make_move_if_noexcept_iterator(Iterator i)
                { return IteratorType(i); }
    #else
        // В остальном нет никаких исключений, поэтому мы всегда возвращаем move_iterator.
        template <typename Iterator>
        inline corsac::move_iterator<Iterator> make_move_if_noexcept_iterator(Iterator i)
        { return corsac::move_iterator<Iterator>(i); }
    #endif

    /**
     * is_move_iterator
     *
     * Это служебная программа для расширения свойств типов
     * Учитывая итератор, сообщает, является ли итератор перемещения по сравнению с чем-либо еще.
     * Пример использования (хотя и несколько бесполезный):
     *      template <typename T>
     *      bool IsMoveIterator() { return typename corsac::is_move_iterator<T>::value; }
     */
    template <typename T>
    struct is_move_iterator
            : public corsac::false_type {};

    template<typename Iterator>
    struct is_move_iterator< corsac::move_iterator<Iterator>>
    : public corsac::true_type {};


    /**
    * unwrap_move_iterator
    *
    * Возвращает Iterator::get_base() если это move_iterator, иначе возвращает Iterator как есть.
    *
    * Пример использования:
    *      vector<int> intVector;
    *      corsac::move_iterator<vector<int>::iterator> moveIterator(intVector.begin());
    *      vector<int>::iterator it = unwrap_move_iterator(moveIterator);
    */
    template <typename Iterator>
    inline typename corsac::is_iterator_wrapper_helper<Iterator, corsac::is_move_iterator<Iterator>::value>::iterator_type unwrap_move_iterator(Iterator it)
    { return corsac::is_iterator_wrapper_helper<Iterator, corsac::is_move_iterator<Iterator>::value>::get_base(it); }

    /**
    * back_insert_iterator
    *
    * A back_insert_iterator  это просто класс, который действует как итератор, но когда вы
    * присваиваете ему значение, он вызывает push_back в контейнере со значением.
    */
    template <typename Container>
    class back_insert_iterator : public iterator<corsac::output_iterator_tag, void, void, void, void>
    {
    public:
        using this_type         = back_insert_iterator<Container>;
        using container_type    = Container;
        using const_reference   = typename Container::const_reference;

    protected:
        Container& container;

    public:
        //back_insert_iterator(); // Not valid. Must construct with a Container.

        //back_insert_iterator(const this_type& x) // Compiler-implemented
        //: container(x.container) { }

        explicit back_insert_iterator(Container& x)
                : container(x) { }

        back_insert_iterator& operator=(const_reference value)
        { container.push_back(value); return *this; }

        back_insert_iterator& operator=(typename Container::value_type&& value)
        { container.push_back(corsac::move(value)); return *this; }

        back_insert_iterator& operator*()
        { return *this; }

        back_insert_iterator& operator++()
        { return *this; } // This is by design.

        back_insert_iterator operator++(int)
        { return *this; } // This is by design.

    protected:
        void operator=(const this_type&) {} // Заявлено, чтобы избежать предупреждений компилятора о невозможности сгенерировать эту функцию.
    };

    /**
    * back_inserter
    *
    * Создает экземпляр back_insert_iterator.
    */
    template <typename Container>
    inline back_insert_iterator<Container>
    back_inserter(Container& x)
    { return back_insert_iterator<Container>(x); }

    /**
    * front_insert_iterator
    *
    * это просто класс, который действует как итератор, но когда вы
    * присваиваете ему значение, он вызывает push_front в контейнере со значением.
    */
    template <typename Container>
    class front_insert_iterator : public iterator<corsac::output_iterator_tag, void, void, void, void>
    {
    public:
        using this_type         = front_insert_iterator<Container>;
        using container_type    = Container;
        using const_reference   = typename Container::const_reference;

    protected:
        Container& container;

    public:
        //front_insert_iterator(); // Not valid. Must construct with a Container.

        //front_insert_iterator(const this_type& x) // Compiler-implemented
        //: container(x.container) { }

        explicit front_insert_iterator(Container& x)
                : container(x) { }

        front_insert_iterator& operator=(const_reference value)
        { container.push_front(value); return *this; }

        front_insert_iterator& operator*()
        { return *this; }

        front_insert_iterator& operator++()
        { return *this; } // Это сделано намеренно.

        front_insert_iterator operator++(int)
        { return *this; } // Это сделано намеренно.

    protected:
        void operator=(const this_type&){} // Заявлено, чтобы избежать предупреждений компилятора о невозможности сгенерировать эту функцию.
    };


    /// front_inserter
    ///
    /// Создает экземпляр front_insert_iterator.
    ///
    template <typename Container>
    inline front_insert_iterator<Container>
    front_inserter(Container& x)
    { return front_insert_iterator<Container>(x); }

    /**
    * insert_iterator
    *
    * An insert_iterator is like an iterator except that when you assign a value to it,
    * the insert_iterator inserts the value into the container and increments the iterator.
    *
    * insert_iterator is an iterator adaptor that functions as an OutputIterator:
    * assignment through an insert_iterator inserts an object into a container.
    * Specifically, if ii is an insert_iterator, then ii keeps track of a container c and
    * an insertion point p; the expression *ii = x performs the insertion container.insert(p, x).
    *
    * If you assign through an insert_iterator several times, then you will be inserting
    * several elements into the underlying container. In the case of a sequence, they will
    * appear at a particular location in the underlying sequence, in the order in which
    * they were inserted: one of the arguments to insert_iterator's constructor is an
    * iterator p, and the new range will be inserted immediately before p.
    */
    template <typename Container>
    class insert_iterator : public iterator<corsac::output_iterator_tag, void, void, void, void>
    {
    public:
        using container_type    = Container;
        using iterator_type     = typename Container::iterator;
        using const_reference   = typename Container::const_reference;

    protected:
        Container&     container;
        iterator_type  it;

    public:
        // Этот оператор присваивания определен больше для остановки предупреждений компилятора (например, VC ++ C4512),
        // чем для того, чтобы быть полезным. Однако он позволяет назначить insert_iterator другому
        // итератору вставки при условии, что они указывают на тот же контейнер.
        insert_iterator& operator=(const insert_iterator& x)
        {
            it = x.it;
            return *this;
        }

        insert_iterator(Container& x, iterator_type itNew)
                : container(x), it(itNew) {}

        insert_iterator& operator=(const_reference value)
        {
            it = container.insert(it, value);
            ++it;
            return *this;
        }

        insert_iterator& operator*()
        { return *this; }

        insert_iterator& operator++()
        { return *this; } // Это сделано намеренно.

        insert_iterator& operator++(int)
        { return *this; } // Это сделано намеренно.

    }; // insert_iterator

    /**
    * inserter
    *
    * Создает экземпляр insert_iterator.
    */
    template <typename Container, typename Iterator>
    inline corsac::insert_iterator<Container>
    inserter(Container& x, Iterator i)
    {
        typedef typename Container::iterator iterator;
        return corsac::insert_iterator<Container>(x, iterator(i));
    }

    /**
    * is_insert_iterator
    *
    * Это служебная программа для расширения свойств типов.
    * Учитывая итератор, сообщает, является ли он insert_iterator по сравнению с чем-то еще.
    * Если это insert_iterator, обернутый другим итератором, тогда значение false.
    */
    template <typename T>
    struct is_insert_iterator
            : public corsac::false_type {};

    template<typename Iterator>
    struct is_insert_iterator< corsac::insert_iterator<Iterator> >
    : public corsac::true_type {};

    /**
    * distance
    *
    * Реализует функцию distance(). Существует две версии:
    * одна для итераторов произвольного доступа (например, с вектором)
    * и одна для обычных итераторов ввода (например, со списком).
    * Первый более эффективен.
    */
    template <typename InputIterator>
    constexpr inline typename corsac::iterator_traits<InputIterator>::difference_type
    distance_impl(InputIterator first, InputIterator last, corsac::input_iterator_tag)
    {
        typename corsac::iterator_traits<InputIterator>::difference_type n = 0;

        while(first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    template <typename RandomAccessIterator>
    constexpr inline typename corsac::iterator_traits<RandomAccessIterator>::difference_type
    distance_impl(RandomAccessIterator first, RandomAccessIterator last, corsac::random_access_iterator_tag)
    {
        return last - first;
    }

    template <typename InputIterator>
    constexpr inline typename corsac::iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;
        return corsac::distance_impl(first, last, IC());
    }

    /**
    * advance
    *
    * Реализует функцию advance(). Существует три версии:
    * одна для итераторов произвольного доступа (например, с вектором),
    * другая для двунаправленных итераторов (список)
    * и последняя для обычных итераторов ввода (например, со списком).
    */
    template <typename InputIterator, typename Distance>
    inline void advance_impl(InputIterator& i, Distance n, corsac::input_iterator_tag)
    {
        while(n--)
            ++i;
    }

    template <bool signedDistance>
    struct advance_bi_impl
    {
        template <typename BidirectionalIterator, typename Distance>
        static void advance_impl(BidirectionalIterator& i, Distance n) // Специализация на беззнаковый дистанционный тип.
        {
            while(n--)
                ++i;
        }
    };

    template <>
    struct advance_bi_impl<true>
    {
        template <typename BidirectionalIterator, typename Distance>
        static void advance_impl(BidirectionalIterator& i, Distance n) // Специализация для знакового дистанционного типа.
        {
            if(n > 0)
            {
                while(n--)
                    ++i;
            }
            else
            {
                while(n++)
                    --i;
            }
        }
    };

    template <typename BidirectionalIterator, typename Distance>
    inline void advance_impl(BidirectionalIterator& i, Distance n, corsac::bidirectional_iterator_tag)
    {
        advance_bi_impl<corsac::is_signed<Distance>::value>::advance_impl(i, n);
    }

    template <typename RandomAccessIterator, typename Distance>
    inline void advance_impl(RandomAccessIterator& i, Distance n, corsac::random_access_iterator_tag)
    {
        i += n;
    }

    template <typename InputIterator, typename Distance>
    constexpr inline void advance(InputIterator& i, Distance n)
    {
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;
        corsac::advance_impl(i, n, IC());
    }

    /**
    * corsac::next / corsac::prev
    *
    * Вернуть n-й преемник итератора it.
    */
    template<typename InputIterator>
    inline InputIterator
    next(InputIterator it, typename corsac::iterator_traits<InputIterator>::difference_type n = 1)
    {
        corsac::advance(it, n);
        return it;
    }

    template<typename InputIterator>
    inline InputIterator
    prev(InputIterator it, typename corsac::iterator_traits<InputIterator>::difference_type n = 1)
    {
        corsac::advance(it, -n);
        return it;
    }

    /**
    * corsac::data
    */
    template <class Container>
    constexpr auto data(Container& c) -> decltype(c.data())
    { return c.data(); }

    template <class Container>
    constexpr auto data(const Container& c) -> decltype(c.data())
    { return c.data(); }

    template <class T, size_t N>
    constexpr T* data(T(&array)[N]) noexcept
    { return array; }

    template <class E>
    constexpr const E* data(std::initializer_list<E> il) noexcept
    { return il.begin(); }

    /**
    * corsac::size
    */
    template <class C>
    constexpr auto size(const C& c) -> decltype(c.size())
    { return c.size(); }

    template <class T, size_t N>
    constexpr size_t size(const T (&)[N]) noexcept
    { return N; }

    /**
    * corsac::ssize
    */
    template <class T, ptrdiff_t N>
    constexpr ptrdiff_t ssize(const T(&)[N]) noexcept
    { return N; }

    template <class C>
    constexpr auto ssize(const C& c)
    -> corsac::common_type_t<ptrdiff_t, corsac::make_signed_t<decltype(c.size())>>
    {
        using R = corsac::common_type_t<ptrdiff_t, corsac::make_signed_t<decltype(c.size())>>;
        return static_cast<R>(c.size());
    }

    /**
    * corsac::empty
    */
    template <class Container>
    constexpr auto empty(const Container& c) -> decltype(c.empty())
    { return c.empty(); }

    template <class T, size_t N>
    constexpr bool empty(const T (&)[N]) noexcept
    { return false; }

    template <class E>
    constexpr bool empty(std::initializer_list<E> il) noexcept
    { return il.size() == 0; }

    /**
    * corsac::begin / corsac::end
    */
    template <typename Container>
    constexpr auto begin(Container& container) -> decltype(container.begin())
    {
        return container.begin();
    }

    template <typename Container>
    constexpr auto begin(const Container& container) -> decltype(container.begin())
    {
        return container.begin();
    }

    template<typename T, size_t arraySize>
    constexpr T* begin(T (&arrayObject)[arraySize]) noexcept
    {
        return arrayObject;
    }

    template <typename Container>
    constexpr auto cbegin(const Container& container) -> decltype(corsac::begin(container))
    {
        return corsac::begin(container);
    }

    template <typename Container>
    constexpr auto end(Container& container) -> decltype(container.end())
    {
        return container.end();
    }

    template <typename Container>
    constexpr auto end(const Container& container) -> decltype(container.end())
    {
        return container.end();
    }

    template<typename T, size_t arraySize>
    constexpr T* end(T (&arrayObject)[arraySize]) noexcept
    {
        return (arrayObject + arraySize);
    }

    template <typename Container>
    constexpr auto cend(const Container& container) -> decltype(corsac::end(container))
    {
        return corsac::end(container);
    }

    template <typename Container>
    constexpr auto rbegin(Container& container) -> decltype(container.rbegin())
    {
        return container.rbegin();
    }

    template <typename Container>
    constexpr auto rbegin(const Container& container) -> decltype(container.rbegin())
    {
        return container.rbegin();
    }

    template <typename Container>
    constexpr auto rend(Container& container) -> decltype(container.rend())
    {
        return container.rend();
    }

    template <typename Container>
    constexpr auto rend(const Container& container) -> decltype(container.rend())
    {
        return container.rend();
    }

    template <typename Container>
    constexpr auto crbegin(const Container& container) -> decltype(corsac::rbegin(container))
    {
        return container.rbegin();
    }

    template <typename Container>
    constexpr auto crend(const Container& container) -> decltype(corsac::rend(container))
    {
        return container.rend();
    }

    template <typename T, size_t arraySize>
    constexpr reverse_iterator<T*> rbegin(T (&arrayObject)[arraySize])
    {
        return reverse_iterator<T*>(arrayObject + arraySize);
    }

    template <typename T, size_t arraySize>
    constexpr reverse_iterator<T*> rend(T (&arrayObject)[arraySize])
    {
        return reverse_iterator<T*>(arrayObject);
    }

    template <typename E>
    constexpr reverse_iterator<const E*> rbegin(std::initializer_list<E> ilist)
    {
        return corsac::reverse_iterator<const E*>(ilist.end());
    }

    template <typename E>
    constexpr reverse_iterator<const E*> rend(std::initializer_list<E> ilist)
    {
        return corsac::reverse_iterator<const E*>(ilist.begin());
    }

    template <typename Iterator>
    constexpr reverse_iterator<Iterator> make_reverse_iterator(Iterator i)
    { return reverse_iterator<Iterator>(i); }

} // namespace corsac

#endif // Header include guard


