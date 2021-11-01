#ifndef CORSAC_ITERATOR_H
#define CORSAC_ITERATOR_H

#pragma once


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
        typedef Iterator                                iterator_type;
        typedef iterator_type                           wrapped_iterator_type;   // This is not in the C++ Standard; it's used by use to identify it as a wrapping iterator type.
        typedef iterator_traits<Iterator>               traits_type;
        typedef typename traits_type::iterator_category iterator_category;
        typedef typename traits_type::value_type        value_type;
        typedef typename traits_type::difference_type   difference_type;
        typedef Iterator                                pointer;
        using reference = conditional_t<is_reference<WrappedIteratorReference>::value,
        remove_reference_t<WrappedIteratorReference>&&,
        WrappedIteratorReference>;

    protected:
        iterator_type mIterator;

    public:
        move_iterator()
                : mIterator()
        {
        }

        explicit move_iterator(iterator_type mi)
                : mIterator(mi) { }

        template<typename U>
        move_iterator(const move_iterator<U>& mi)
                : mIterator(mi.base())
        {
        }

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
        { return eastl::move(mIterator[n]); }
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

    // make_move_if_noexcept_iterator returns move_iterator<Iterator> if the Iterator is of a noexcept type;
    // otherwise returns Iterator as-is. The point of this is to be able to avoid moves that can generate exceptions and instead
    // fall back to copies or whatever the default IteratorType::operator* returns for use by copy/move algorithms.
    // To consider: merge the conditional expression usage here with the one used by move_if_noexcept, as they are the same condition.
    template <typename Iterator, typename IteratorType = typename corsac::conditional<corsac::is_nothrow_move_constructible<typename corsac::iterator_traits<Iterator>::value_type>::value ||
                                                                                     !corsac::is_copy_constructible<typename corsac::iterator_traits<Iterator>::value_type>::value,
                                                                                     corsac::move_iterator<Iterator>, Iterator>::type>
    inline IteratorType make_move_if_noexcept_iterator(Iterator i)
        { return IteratorType(i); }

    template <typename Iterator>
    inline corsac::move_iterator<Iterator> make_move_if_noexcept_iterator(Iterator i)
    { return corsac::move_iterator<Iterator>(i); }

    /**
     * is_move_iterator
     *
     * Это служебная программа для расширения свойств типов
     * Учитывая итератор, сообщает, является ли итератор перемещения по сравнению с чем-либо еще.
     * Пример использования (хотя и несколько бесполезный):
     *      template <typename T>
     *      bool IsMoveIterator() { return typename eastl::is_move_iterator<T>::value; }
     */
    template <typename T>
    struct is_move_iterator
            : public corsac::false_type {};

    template<typename Iterator>
    struct is_move_iterator< corsac::move_iterator<Iterator>>
    : public corsac::true_type {};


    /**
    * is_iterator_wrapper
    *
    * Сообщает, является ли тип Iterator типом-оболочкой, а не обычным типом.
    * Опирается на класс, объявляющий определение типа с именем wrapped_iterator_type.
    *
    * Examples of wrapping iterators:
    *     reverse_iterator
    *     generic_iterator
    *     move_iterator
    * Examples of non-wrapping iterators:
    *     iterator
    *     list::iterator
    *     char*
    *
    * Example behavior:
    *     is_iterator_wrapper(int*)::value                                              => false
    *     is_iterator_wrapper(eastl::array<char>*)::value                               => false
    *     is_iterator_wrapper(eastl::vector<int>::iterator)::value                      => false
    *     is_iterator_wrapper(eastl::generic_iterator<int*>)::value                     => true
    *     is_iterator_wrapper(eastl::move_iterator<eastl::array<int>::iterator>)::value => true
    *
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
    * и возвращает обернутый тип итератора. Если Iterator не является оболочкой
    * (включая указатель) или не является итератором, эта функция возвращает его как есть.
    * unwrap_iterator разворачивает только один слой итератора за раз. Вам нужно вызвать его
    * дважды, например, чтобы развернуть два слоя итераторов.
    * Example usage:
    *     int* pInt             = unwrap_iterator(&pIntArray[15]);
    *     int* pInt             = unwrap_iterator(generic_iterator(&pIntArray[15]));
    *     MyVector::iterator it = unwrap_iterator(myVector.begin());
    *     MyVector::iterator it = unwrap_iterator(move_iterator(myVector.begin()));
    *
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
     * move_iterator
     *
     * From the C++11 Standard, section 24.5.3.1:
     *
     * Шаблон класса move_iterator - это адаптер итератора с таким же поведением, что и базовый
     * итератор, за исключением того, что его оператор разыменования неявно преобразует значение,
     * возвращаемое оператором разыменования базового итератора, в ссылку rvalue. Некоторые общие\
     * алгоритмы можно вызывать с итераторами перемещения, чтобы заменить копирование перемещением.
     */
    template<typename Iterator>
    class move_iterator // Не наследовать от итератора.
    {
    private:
        using WrappedIteratorReference = typename iterator_traits<Iterator>::reference;

    public:
        using iterator_type         = Iterator;
        using wrapped_iterator_type = iterator_type;
        using traits_type           = iterator_traits<Iterator>;
        using iterator_category     = typename traits_type::iterator_category;
        using value_type            = typename traits_type::value_type;
        using difference_type       = typename traits_type::difference_type;
        using pointer               = Iterator;
        using reference = conditional_t<is_reference<WrappedIteratorReference>::value,
                            remove_reference_t<WrappedIteratorReference>&&,
                            WrappedIteratorReference>;
    private:
        iterator_type mIterator;

    public:
        move_iterator()
                : mIterator()
        {
        }

        explicit move_iterator(iterator_type mi)
                : mIterator(mi) { }

        template<typename U>
        explicit move_iterator(const move_iterator<U>& mi)
                : mIterator(mi.base())
        {
        }

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
} // namespace corsac

#endif // Header include guard


