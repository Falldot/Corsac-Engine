// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_VECTOR_H
#define CORSAC_VECTOR_H

#pragma once

#include "internal/config.h"
#include "allocator.h"
#include "type_traits.h"
#include "iterator.h"
#include "algorithm.h"
#include "initializer_list.h"
#include "memory.h"

#include <new>
#include <stddef.h>

#if CORSAC_EXCEPTIONS_ENABLED
    #include <stdexcept> // std::out_of_range, std::length_error.
#endif

namespace corsac
{
    // CORSAC_VECTOR_DEFAULT_NAME
    #ifndef CORSAC_VECTOR_DEFAULT_NAME
        #define CORSAC_VECTOR_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " vector"
    #endif

    // CORSAC_VECTOR_DEFAULT_ALLOCATOR
    #ifndef CORSAC_VECTOR_DEFAULT_ALLOCATOR
        #define CORSAC_VECTOR_DEFAULT_ALLOCATOR allocator_type(CORSAC_VECTOR_DEFAULT_NAME)
    #endif

    /**
    * VectorBase
    *
    * Причина, по которой у нас есть класс VectorBase, заключается в том, что он упрощает
    * реализацию обработки исключений, поскольку выделение памяти полностью реализовано в этом
    * классе. Если пользователь создает вектор, которому необходимо выделить память в
    * конструкторе, VectorBase обрабатывает его. Если распределитель генерирует исключение, то
    * генерирование исключения переходит обратно к пользовательскому коду, и не нужно писать код
    * try/catch в конструкторе vector или VectorBase. Если в конструкторе vector (не VectorBase)
    * возникает исключение, деструктор для VectorBase будет вызываться автоматически (и
    * освобождать выделенную память) до того, как выполнение перейдет к пользовательскому коду.
    * Однако, если бы векторный класс обрабатывал и выделение, и инициализацию, у него не было бы
    * другого выбора, кроме как реализовать явный оператор try/catch для всех путей, которые
    * выделяют память. Это увеличивает размер кода и снижает производительность, а также
    * затрудняет чтение и сопровождение кода.
    */
    template <typename T, typename Allocator>
    struct VectorBase
    {
        using allocator_type = Allocator;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        // 'npos' означает недопустимую позицию или просто непозицию.
        static const size_type npos     = static_cast<size_type>(-1);
        // -1 зарезервировано для npos. Также немного выгодно, что kMaxSize имеет значение меньше -1,
        // поскольку это помогает нам справиться с потенциальными проблемами переноса целых чисел.
        static const size_type kMaxSize = static_cast<size_type>(-2);

    protected:
        T*              mpBegin;
        T*              mpEnd;
        T*              mCapacity;
        allocator_type  mAllocator;

        T*& internalCapacityPtr() noexcept { return mCapacity; }
        T* const& internalCapacityPtr() const noexcept { return mCapacity; }
        allocator_type&  internalAllocator() noexcept { return mAllocator; }
        const allocator_type&  internalAllocator() const noexcept { return mAllocator; }

    public:
        VectorBase();
        VectorBase(const allocator_type& allocator);
        VectorBase(size_type n, const allocator_type& allocator);

        ~VectorBase();

        const allocator_type& get_allocator() const noexcept;
        allocator_type&       get_allocator() noexcept;
        void                  set_allocator(const allocator_type& allocator);

    protected:
        T*        DoAllocate(size_type n);
        void      DoFree(T* p, size_type n);
        size_type GetNewCapacity(size_type currentCapacity);
    }; // VectorBase

    /**
    * vector
    *
    * Динамический массив.
    */
    template <typename T, typename Allocator = CORSAC_AllocatorType>
    class vector : public VectorBase<T, Allocator>
    {
        using base_type = VectorBase<T, Allocator>;
        using this_type = vector<T, Allocator>;

    public:
        using value_type                = T;
        using pointer                   = T*;
        using const_pointer             = const T*;
        using reference                 = T&;
        using const_reference           = const T&;
        using iterator                  = T*;
        using const_iterator            = const T*;
        using reverse_iterator          = corsac::reverse_iterator<iterator>;
        using const_reverse_iterator    = corsac::reverse_iterator<const_iterator>;
        using size_type                 = typename base_type::size_type;
        using difference_type           = typename base_type::difference_type;
        using allocator_type            = typename base_type::allocator_type;

        using base_type::mpBegin;
        using base_type::mpEnd;
        using base_type::mCapacity;
        using base_type::mAllocator;
        using base_type::npos;
        using base_type::GetNewCapacity;
        using base_type::DoAllocate;
        using base_type::DoFree;
        using base_type::internalCapacityPtr;
        using base_type::internalAllocator;

    public:
        vector() noexcept;
        explicit vector(const allocator_type& allocator) noexcept;
        explicit vector(size_type n, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        vector(size_type n, const value_type& value, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        vector(const this_type& x);
        vector(const this_type& x, const allocator_type& allocator);
        vector(this_type&& x) noexcept;
        vector(this_type&& x, const allocator_type& allocator);
        vector(std::initializer_list<value_type> ilist, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);

        template <typename InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);

        ~vector();

        this_type& operator=(const this_type& x);
        this_type& operator=(std::initializer_list<value_type> ilist);
        this_type& operator=(this_type&& x); // TODO(c++17): noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value)

        void swap(this_type& x); // TODO(c++17): noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value || allocator_traits<Allocator>::is_always_equal::value)

        void assign(size_type n, const value_type& value);

        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last);

        void assign(std::initializer_list<value_type> ilist);

        iterator       begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator       end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator       rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        reverse_iterator       rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        bool      empty() const noexcept;
        size_type size() const noexcept;
        size_type capacity() const noexcept;

        void resize(size_type n, const value_type& value);
        void resize(size_type n);
        void reserve(size_type n);
        void set_capacity(size_type n = base_type::npos);
        void shrink_to_fit();

        pointer       data() noexcept;
        const_pointer data() const noexcept;

        reference       operator[](size_type n);
        const_reference operator[](size_type n) const;

        reference       at(size_type n);
        const_reference at(size_type n) const;

        reference       front();
        const_reference front() const;

        reference       back();
        const_reference back() const;

        void      push_back(const value_type& value);
        reference push_back();
        void*     push_back_uninitialized();
        void      push_back(value_type&& value);
        void      pop_back();

        template<class... Args>
        iterator emplace(const_iterator position, Args&&... args);

        template<class... Args>
        reference emplace_back(Args&&... args);

        iterator insert(const_iterator position, const value_type& value);
        iterator insert(const_iterator position, size_type n, const value_type& value);
        iterator insert(const_iterator position, value_type&& value);
        iterator insert(const_iterator position, std::initializer_list<value_type> ilist);

        template <typename InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last);

        iterator erase_first(const T& value);
        iterator erase_first_unsorted(const T& value);
        reverse_iterator erase_last(const T& value);
        reverse_iterator erase_last_unsorted(const T& value);

        iterator erase(const_iterator position);
        iterator erase(const_iterator first, const_iterator last);
        iterator erase_unsorted(const_iterator position);

        reverse_iterator erase(const_reverse_iterator position);
        reverse_iterator erase(const_reverse_iterator first, const_reverse_iterator last);
        reverse_iterator erase_unsorted(const_reverse_iterator position);

        void clear() noexcept;
        void reset_lose_memory() noexcept;

        bool validate() const noexcept;
        int  validate_iterator(const_iterator i) const noexcept;

    protected:
        template <bool bMove> struct should_move_or_copy_tag{};
        using should_copy_tag = should_move_or_copy_tag<false>;
        using should_move_tag = should_move_or_copy_tag<true>;

        template <typename ForwardIterator> // Выделяет указатель на массив count n и копирует его с помощью [first, last).
        pointer DoRealloc(size_type n, ForwardIterator first, ForwardIterator last, should_copy_tag);

        template <typename ForwardIterator> // Выделяет указатель на массив count n и копирует его с помощью [first, last).
        pointer DoRealloc(size_type n, ForwardIterator first, ForwardIterator last, should_move_tag);

        template <typename Integer>
        void DoInit(Integer n, Integer value, true_type);

        template <typename InputIterator>
        void DoInit(InputIterator first, InputIterator last, false_type);

        template <typename InputIterator>
        void DoInitFromIterator(InputIterator first, InputIterator last, corsac::input_iterator_tag);

        template <typename ForwardIterator>
        void DoInitFromIterator(ForwardIterator first, ForwardIterator last, corsac::forward_iterator_tag);

        template <typename Integer, bool bMove>
        void DoAssign(Integer n, Integer value, true_type);

        template <typename InputIterator, bool bMove>
        void DoAssign(InputIterator first, InputIterator last, false_type);

        void DoAssignValues(size_type n, const value_type& value);

        template <typename InputIterator, bool bMove>
        void DoAssignFromIterator(InputIterator first, InputIterator last, corsac::input_iterator_tag);

        template <typename RandomAccessIterator, bool bMove>
        void DoAssignFromIterator(RandomAccessIterator first, RandomAccessIterator last, corsac::random_access_iterator_tag);

        template <typename Integer>
        void DoInsert(const_iterator position, Integer n, Integer value, true_type);

        template <typename InputIterator>
        void DoInsert(const_iterator position, InputIterator first, InputIterator last, false_type);

        template <typename InputIterator>
        void DoInsertFromIterator(const_iterator position, InputIterator first, InputIterator last, corsac::input_iterator_tag);

        template <typename BidirectionalIterator>
        void DoInsertFromIterator(const_iterator position, BidirectionalIterator first, BidirectionalIterator last, corsac::bidirectional_iterator_tag);

        void DoInsertValues(const_iterator position, size_type n, const value_type& value);

        void DoInsertValuesEnd(size_type n);
        void DoInsertValuesEnd(size_type n, const value_type& value);

        template<typename... Args>
        void DoInsertValue(const_iterator position, Args&&... args);

        template<typename... Args>
        void DoInsertValueEnd(Args&&... args);

        void DoClearCapacity();

        void DoGrow(size_type n);

        void DoSwap(this_type& x);

    }; // class vector

    ///////////////////////////////////////////////////////////////////////
    // VectorBase
    ///////////////////////////////////////////////////////////////////////

    template <typename T, typename Allocator>
    inline VectorBase<T, Allocator>::VectorBase()
            : mpBegin(NULL),
              mpEnd(NULL),
              mCapacity(NULL),
              mAllocator(allocator_type(CORSAC_VECTOR_DEFAULT_NAME))
    {}


    template <typename T, typename Allocator>
    inline VectorBase<T, Allocator>::VectorBase(const allocator_type& allocator)
            : mpBegin(NULL),
              mpEnd(NULL),
              mCapacity(NULL),
              mAllocator(allocator)
    {}


    template <typename T, typename Allocator>
    inline VectorBase<T, Allocator>::VectorBase(size_type n, const allocator_type& allocator)
            : mAllocator(allocator)
    {
        mpBegin    = DoAllocate(n);
        mpEnd      = mpBegin;
        internalCapacityPtr() = mpBegin + n;
    }


    template <typename T, typename Allocator>
    inline VectorBase<T, Allocator>::~VectorBase()
    {
        if(mpBegin)
            CORSAC_Free(internalAllocator(), mpBegin, (internalCapacityPtr() - mpBegin) * sizeof(T));
    }


    template <typename T, typename Allocator>
    inline const typename VectorBase<T, Allocator>::allocator_type&
    VectorBase<T, Allocator>::get_allocator() const noexcept
    {
        return internalAllocator();
    }

    template <typename T, typename Allocator>
    inline typename VectorBase<T, Allocator>::allocator_type&
    VectorBase<T, Allocator>::get_allocator() noexcept
    {
        return internalAllocator();
    }

    template <typename T, typename Allocator>
    inline void VectorBase<T, Allocator>::set_allocator(const allocator_type& allocator)
    {
        internalAllocator() = allocator;
    }

    template <typename T, typename Allocator>
    inline T* VectorBase<T, Allocator>::DoAllocate(size_type n)
    {
        #if CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(n >= 0x80000000))
                        CORSAC_FAIL_MSG("vector::DoAllocate -- improbably large request.");
        #endif
        // Если n равно нулю, мы не выделяем память и просто возвращаем nullptr.
        // Это нормально, поскольку наш ctor по умолчанию инициализируется указателями NULL.
        if(CORSAC_LIKELY(n))
        {
            auto* p = (T*)allocate_memory(internalAllocator(), n * sizeof(T), alignof(T), 0);
            CORSAC_ASSERT_MSG(p != nullptr, "the behaviour of corsac::allocators that return nullptr is not defined.");
            return p;
        }
        else
        {
            return nullptr;
        }
    }

    template <typename T, typename Allocator>
    inline void VectorBase<T, Allocator>::DoFree(T* p, size_type n)
    {
        if(p)
            CORSAC_Free(internalAllocator(), p, n * sizeof(T));
    }

    template <typename T, typename Allocator>
    inline typename VectorBase<T, Allocator>::size_type
    VectorBase<T, Allocator>::GetNewCapacity(size_type currentCapacity)
    {
        // Это должно вернуть значение не менее currentCapacity и не менее 1.
        return (currentCapacity > 0) ? (2 * currentCapacity) : 1;
    }

    ///////////////////////////////////////////////////////////////////////
    // vector
    ///////////////////////////////////////////////////////////////////////

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector() noexcept
    : base_type()
    {}

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(const allocator_type& allocator) noexcept
    : base_type(allocator)
            {}

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(size_type n, const allocator_type& allocator)
            : base_type(n, allocator)
    {
        corsac::uninitialized_default_fill_n(mpBegin, n);
        mpEnd = mpBegin + n;
    }

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(size_type n, const value_type& value, const allocator_type& allocator)
            : base_type(n, allocator)
    {
        corsac::uninitialized_fill_n_ptr(mpBegin, n, value);
        mpEnd = mpBegin + n;
    }

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(const this_type& x)
            : base_type(x.size(), x.internalAllocator())
    {
        mpEnd = corsac::uninitialized_copy_ptr(x.mpBegin, x.mpEnd, mpBegin);
    }

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(const this_type& x, const allocator_type& allocator)
            : base_type(x.size(), allocator)
    {
        mpEnd = corsac::uninitialized_copy_ptr(x.mpBegin, x.mpEnd, mpBegin);
    }


    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(this_type&& x) noexcept
    : base_type(corsac::move(x.internalAllocator()))  // vector в этом случае требует перестановки аллокатора.
    {
        DoSwap(x);
    }

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(this_type&& x, const allocator_type& allocator)
            : base_type(allocator)
    {
        if (internalAllocator() == x.internalAllocator()) // Если распределители эквивалентны ...
            DoSwap(x);
        else
        {
            // move конструктор, поэтому мы не требуем использования копирующих акторов,
            // которые предотвращают использование типов, предназначенных только для перемещения.
            this_type temp(corsac::move(*this));
            temp.swap(x);
        }
    }

    template <typename T, typename Allocator>
    inline vector<T, Allocator>::vector(std::initializer_list<value_type> ilist, const allocator_type& allocator)
            : base_type(allocator)
    {
        DoInit(ilist.begin(), ilist.end(), false_type());
    }


    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline vector<T, Allocator>::vector(InputIterator first, InputIterator last, const allocator_type& allocator)
            : base_type(allocator)
    {
        DoInit(first, last, is_integral<InputIterator>());
    }


    template <typename T, typename Allocator>
    inline vector<T, Allocator>::~vector()
    {
        // Вызвать деструктор значений. Родительский класс освободит память.
        corsac::destruct(mpBegin, mpEnd);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::this_type&
    vector<T, Allocator>::operator=(const this_type& x)
    {
        if(this != &x) // Если не равен себе ...
        {
            // Если (CORSAC_ALLOCATOR_COPY_ENABLED == 1) и текущее содержимое выделяется распределителем,
            // который не равен распределителю x, нам нужно перераспределить наши элементы с помощью нашего
            // текущего распределителя и перераспределить его с помощью распределителя x. Если
            // распределители равны, мы можем использовать более оптимальный алгоритм, который не
            // перераспределяет наши элементы, а вместо этого может копировать их на место.
            #if CORSAC_ALLOCATOR_COPY_ENABLED
                    bool bSlowerPathwayRequired = (internalAllocator() != x.internalAllocator());
            #else
                    bool bSlowerPathwayRequired = false;
            #endif
                if(bSlowerPathwayRequired)
                {
                    DoClearCapacity(); // Должен очищать емкость вместо очистки, потому что set_capacity освобождает нашу память, в отличие от clear.
            #if CORSAC_ALLOCATOR_COPY_ENABLED
                internalAllocator() = x.internalAllocator();
            #endif
                }
                DoAssign<const_iterator, false>(x.begin(), x.end(), corsac::false_type());
        }
        return *this;
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::this_type&
    vector<T, Allocator>::operator=(std::initializer_list<value_type> ilist)
    {
        using InputIterator = typename std::initializer_list<value_type>::iterator;
        using IC            = typename corsac::iterator_traits<InputIterator>::iterator_category;
        // initializer_list имеет константные элементы, поэтому мы не можем от них перейти.
        DoAssignFromIterator<InputIterator, false>(ilist.begin(), ilist.end(), IC());
        return *this;
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::this_type&
    vector<T, Allocator>::operator=(this_type&& x)
    {
        if(this != &x)
        {
            DoClearCapacity(); // Подумате: действительно ли мы обязаны здесь проходить очистку? x скоро уйдет и очистится в своем dtor. своп (х);
            swap(x);       // подкачка членов обрабатывает случай, когда x имеет другой распределитель, чем наш распределитель, делая копию.
        }
        return *this;
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::assign(size_type n, const value_type& value)
    {
        DoAssignValues(n, value);
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline void vector<T, Allocator>::assign(InputIterator first, InputIterator last)
    {
        // Оказывается, C ++ std::vector<int, int> указывает версию assign с двумя аргументами,
        // которая принимает (размер int, значение int). Это не итераторы, поэтому нам нужно
        // использовать трюк с компилятором шаблонов, чтобы поступать правильно.
        DoAssign<InputIterator, false>(first, last, is_integral<InputIterator>());
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::assign(std::initializer_list<value_type> ilist)
    {
        using InputIterator = typename std::initializer_list<value_type>::iterator;
        using IC            = typename corsac::iterator_traits<InputIterator>::iterator_category;
        // initializer_list имеет константные элементы, поэтому мы не можем от них перейти.
        DoAssignFromIterator<InputIterator, false>(ilist.begin(), ilist.end(), IC());
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::begin() noexcept
    {
        return mpBegin;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_iterator
    vector<T, Allocator>::begin() const noexcept
    {
        return mpBegin;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_iterator
    vector<T, Allocator>::cbegin() const noexcept
    {
        return mpBegin;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::end() noexcept
    {
        return mpEnd;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_iterator
    vector<T, Allocator>::end() const noexcept
    {
        return mpEnd;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_iterator
    vector<T, Allocator>::cend() const noexcept
    {
        return mpEnd;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::rbegin() noexcept
    {
        return reverse_iterator(mpEnd);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reverse_iterator
    vector<T, Allocator>::rbegin() const noexcept
    {
        return const_reverse_iterator(mpEnd);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reverse_iterator
    vector<T, Allocator>::crbegin() const noexcept
    {
        return const_reverse_iterator(mpEnd);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::rend() noexcept
    {
        return reverse_iterator(mpBegin);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reverse_iterator
    vector<T, Allocator>::rend() const noexcept
    {
        return const_reverse_iterator(mpBegin);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reverse_iterator
    vector<T, Allocator>::crend() const noexcept
    {
        return const_reverse_iterator(mpBegin);
    }

    template <typename T, typename Allocator>
    bool vector<T, Allocator>::empty() const noexcept
    {
        return (mpBegin == mpEnd);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::size_type
    vector<T, Allocator>::size() const noexcept
    {
        return static_cast<size_type>(mpEnd - mpBegin);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::size_type
    vector<T, Allocator>::capacity() const noexcept
    {
        return static_cast<size_type>(internalCapacityPtr() - mpBegin);
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::resize(size_type n, const value_type& value)
    {
        if (n > static_cast<size_type>(mpEnd - mpBegin)) // Мы ожидаем, что чаще всего изменение размера будет увеличиваться.
            DoInsertValuesEnd(n - (static_cast<size_type>(mpEnd - mpBegin)), value);
        else
        {
            corsac::destruct(mpBegin + n, mpEnd);
            mpEnd = mpBegin + n;
        }
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::resize(size_type n)
    {
        // Альтернативная реализация:
        // resize(n, value_type());

        if(n > static_cast<size_type>(mpEnd - mpBegin))  // Мы ожидаем, что чаще всего изменение размера будет увеличиваться.
            DoInsertValuesEnd(n - (static_cast<size_type>(mpEnd - mpBegin)));
        else
        {
            corsac::destruct(mpBegin + n, mpEnd);
            mpEnd = mpBegin + n;
        }
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::reserve(size_type n)
    {
        // Если пользователь хочет уменьшить зарезервированную память, есть функция set_capacity.
        if(n > size_type(internalCapacityPtr() - mpBegin)) // Если n > емкость ...
            DoGrow(n);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::set_capacity(size_type n)
    {
        if((n == npos) || (n <= static_cast<size_type>(mpEnd - mpBegin))) // Если новая емкость <= размер ...
        {
            if(n == 0)  // Очень часто n будет равно 0, и очистка будет быстрее, чем изменение размера и использование меньшего пространства стека.
                clear();
            else if(n < static_cast<size_type>(mpEnd - mpBegin))
                resize(n);

            shrink_to_fit();
        }
        else // Остальная новая емкость > размер.
        {
            pointer const pNewData = DoRealloc(n, mpBegin, mpEnd, should_move_tag());
            corsac::destruct(mpBegin, mpEnd);
            DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

            const ptrdiff_t nPrevSize = mpEnd - mpBegin;
            mpBegin    = pNewData;
            mpEnd      = pNewData + nPrevSize;
            internalCapacityPtr() = mpBegin + n;
        }
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::shrink_to_fit()
    {
        // Это самый простой способ добиться этого, и он так же эффективен, как и любой другой.
        this_type temp = this_type(move_iterator<iterator>(begin()), move_iterator<iterator>(end()), internalAllocator());

        // Вызов DoSwap(), а не swap(), поскольку мы знаем, что наши распределители совпадают, и мы не хотим вызывать путь кода
        // обработка несовпадающих распределителей, поскольку это накладывает дополнительные ограничения на копируемый тип T
        DoSwap(temp);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::pointer
    vector<T, Allocator>::data() noexcept
    {
        return mpBegin;
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_pointer
    vector<T, Allocator>::data() const noexcept
    {
        return mpBegin;
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::operator[](size_type n)
    {
        #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        CORSAC_FAIL_MSG("vector::operator[] -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            // Мы разрешаем пользователю использовать ссылку на v[0] пустого контейнера. Но это было просто дедушкой, и в идеале мы не должны разрешать такой доступ к [0].
                    if (CORSAC_UNLIKELY((n != 0) && (n >= (static_cast<size_type>(mpEnd - mpBegin)))))
                        CORSAC_FAIL_MSG("vector::operator[] -- out of range");
        #endif
        return *(mpBegin + n);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reference
    vector<T, Allocator>::operator[](size_type n) const
    {
        #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        CORSAC_FAIL_MSG("vector::operator[] -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            // We allow the user to use a reference to v[0] of an empty container. But this was merely grandfathered in and ideally we shouldn't allow such access to [0].
                    if (CORSAC_UNLIKELY((n != 0) && (n >= (static_cast<size_type>(mpEnd - mpBegin)))))
                        CORSAC_FAIL_MSG("vector::operator[] -- out of range");
        #endif
        return *(mpBegin + n);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::at(size_type n)
    {
        // Разница между at () и operator [] заключается в том,
        // что он сигнализирует, что запрошенная позиция выходит за
        // пределы допустимого диапазона, вызывая исключение out_of_range.
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        throw std::out_of_range("vector::at -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        CORSAC_FAIL_MSG("vector::at -- out of range");
        #endif
        return *(mpBegin + n);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reference
    vector<T, Allocator>::at(size_type n) const
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        throw std::out_of_range("vector::at -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(n >= (static_cast<size_type>(mpEnd - mpBegin))))
                        CORSAC_FAIL_MSG("vector::at -- out of range");
        #endif
        return *(mpBegin + n);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::front()
    {
        #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
            if (CORSAC_UNLIKELY((mpBegin == nullptr) || (mpEnd <= mpBegin))) // We don't allow the user to reference an empty container.
                        CORSAC_FAIL_MSG("vector::front -- empty vector");
        #else
            // Мы позволяем пользователю ссылаться на пустой контейнер.
        #endif
        return *mpBegin;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reference
    vector<T, Allocator>::front() const
    {
        #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
            if (CORSAC_UNLIKELY((mpBegin == nullptr) || (mpEnd <= mpBegin))) // We don't allow the user to reference an empty container.
                        CORSAC_FAIL_MSG("vector::front -- empty vector");
        #else
            // Мы позволяем пользователю ссылаться на пустой контейнер.
        #endif
        return *mpBegin;
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::back()
    {
    #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
        if (CORSAC_UNLIKELY((mpBegin == nullptr) || (mpEnd <= mpBegin))) // We don't allow the user to reference an empty container.
                    CORSAC_FAIL_MSG("vector::back -- empty vector");
    #else
        // Мы позволяем пользователю ссылаться на пустой контейнер.
    #endif

        return *(mpEnd - 1);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::const_reference
    vector<T, Allocator>::back() const
    {
        #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
            if (CORSAC_UNLIKELY((mpBegin == nullptr) || (mpEnd <= mpBegin))) // We don't allow the user to reference an empty container.
                        CORSAC_FAIL_MSG("vector::back -- empty vector");
        #else
            // Мы позволяем пользователю ссылаться на пустой контейнер.
        #endif
        return *(mpEnd - 1);
    }


    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::push_back(const value_type& value)
    {
        if(mpEnd < internalCapacityPtr())
            ::new(static_cast<void*>(mpEnd++)) value_type(value);
        else
            DoInsertValueEnd(value);
    }


    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::push_back(value_type&& value)
    {
        if (mpEnd < internalCapacityPtr())
            ::new(static_cast<void*>(mpEnd++)) value_type(corsac::move(value));
        else
            DoInsertValueEnd(corsac::move(value));
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::push_back()
    {
        if(mpEnd < internalCapacityPtr())
            ::new(static_cast<void*>(mpEnd++)) value_type();
        else // Обратите внимание, что в этом случае мы создаем временный, что менее желательно.
            DoInsertValueEnd(value_type());
        return *(mpEnd - 1); // То же, что возвращает back();
    }

    template <typename T, typename Allocator>
    inline void* vector<T, Allocator>::push_back_uninitialized()
    {
        if(mpEnd == internalCapacityPtr())
        {
            const size_type newSize = static_cast<size_type>(mpEnd - mpBegin) + 1;
            reserve(newSize);
        }
        return mpEnd++;
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::pop_back()
    {
    #if CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(mpEnd <= mpBegin))
                    CORSAC_FAIL_MSG("vector::pop_back -- empty vector");
    #endif
        --mpEnd;
        mpEnd->~value_type();
    }

    template <typename T, typename Allocator>
    template<class... Args>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::emplace(const_iterator position, Args&&... args)
    {
        const ptrdiff_t n = position - mpBegin; // Сохраните это, потому что мы можем перераспределить.

        if((mpEnd == internalCapacityPtr()) || (position != mpEnd))
            DoInsertValue(position, corsac::forward<Args>(args)...);
        else
        {
            ::new(static_cast<void*>(mpEnd)) value_type(corsac::forward<Args>(args)...);
            ++mpEnd; // Увеличьте это значение после конструкции, описанной выше, на случай, если конструкция вызовет исключение.
        }

        return mpBegin + n;
    }

    template <typename T, typename Allocator>
    template<class... Args>
    inline typename vector<T, Allocator>::reference
    vector<T, Allocator>::emplace_back(Args&&... args)
    {
        if(mpEnd < internalCapacityPtr())
        {
            // Если value_type имеет конструктор перемещения, он будет использовать его, и эта операция может быть быстрее, чем в другом случае.
            ::new(static_cast<void*>(mpEnd)) value_type(corsac::forward<Args>(args)...);
            ++mpEnd; // Увеличьте это значение после конструкции, описанной выше, на случай, если конструкция вызовет исключение.
        }
        else
            DoInsertValueEnd(corsac::forward<Args>(args)...);
        return back();
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(const_iterator position, const value_type& value)
    {
    #if CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY((position < mpBegin) || (position > mpEnd)))
                    CORSAC_FAIL_MSG("vector::insert -- invalid position");
    #endif
        // Мы реализуем быстрый путь для случая, когда позиция вставки находится в конце, и у нас есть свободная емкость для нее.
        const ptrdiff_t n = position - mpBegin; // Сохраните это, потому что мы можем перераспределить.

        if((mpEnd == internalCapacityPtr()) || (position != mpEnd))
            DoInsertValue(position, value);
        else
        {
            ::new(static_cast<void*>(mpEnd)) value_type(value);
            ++mpEnd; // Увеличьте это значение после конструкции, описанной выше, на случай, если конструкция вызовет исключение.
        }
        return mpBegin + n;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(const_iterator position, value_type&& value)
    {
        return emplace(position, corsac::move(value));
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(const_iterator position, size_type n, const value_type& value)
    {
        const ptrdiff_t p = position - mpBegin; // Сохраните это, потому что мы можем перераспределить.
        DoInsertValues(position, n, value);
        return mpBegin + p;
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(const_iterator position, InputIterator first, InputIterator last)
    {
        const ptrdiff_t n = position - mpBegin; // Сохраните это, потому что мы можем перераспределить.
        DoInsert(position, first, last, is_integral<InputIterator>());
        return mpBegin + n;
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::insert(const_iterator position, std::initializer_list<value_type> ilist)
    {
        const ptrdiff_t n = position - mpBegin; // Сохраните это, потому что мы можем перераспределить.
        DoInsert(position, ilist.begin(), ilist.end(), false_type());
        return mpBegin + n;
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::erase(const_iterator position)
    {
    #if CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY((position < mpBegin) || (position >= mpEnd)))
                    CORSAC_FAIL_MSG("vector::erase -- invalid position");
    #endif
        // C++11 оговаривает, что позиция - это const_iterator, но возвращаемое значение - итератор.
        iterator destPosition = const_cast<value_type*>(position);

        if((position + 1) < mpEnd)
            corsac::move(destPosition + 1, mpEnd, destPosition);
        --mpEnd;
        mpEnd->~value_type();
        return destPosition;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::erase(const_iterator first, const_iterator last)
    {
    #if CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY((first < mpBegin) || (first > mpEnd) || (last < mpBegin) || (last > mpEnd) || (last < first)))
                    CORSAC_FAIL_MSG("vector::erase -- invalid position");
    #endif
        if (first != last)
        {
            iterator const position = const_cast<value_type*>(corsac::move(const_cast<value_type*>(last), const_cast<value_type*>(mpEnd), const_cast<value_type*>(first)));
            corsac::destruct(position, mpEnd);
            mpEnd -= (last - first);
        }
        return const_cast<value_type*>(first);
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::erase_unsorted(const_iterator position)
    {
       #if CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY((position < mpBegin) || (position >= mpEnd)))
                        CORSAC_FAIL_MSG("vector::erase -- invalid position");
        #endif
        // C++11 оговаривает, что позиция - это const_iterator, но возвращаемое значение - итератор.
        iterator destPosition = const_cast<value_type*>(position);
        *destPosition = corsac::move(*(mpEnd - 1));

        // pop_back();
        --mpEnd;
        mpEnd->~value_type();

        return destPosition;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator vector<T, Allocator>::erase_first(const T& value)
    {
        static_assert(corsac::has_equality_v<T>, "T must be comparable");

        iterator it = corsac::find(begin(), end(), value);

        if (it != end())
            return erase(it);
        else
            return it;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::iterator
    vector<T, Allocator>::erase_first_unsorted(const T& value)
    {
        static_assert(corsac::has_equality_v<T>, "T must be comparable");

        iterator it = corsac::find(begin(), end(), value);

        if (it != end())
            return erase_unsorted(it);
        else
            return it;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::erase_last(const T& value)
    {
        static_assert(corsac::has_equality_v<T>, "T must be comparable");

        reverse_iterator it = corsac::find(rbegin(), rend(), value);

        if (it != rend())
            return erase(it);
        else
            return it;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::erase_last_unsorted(const T& value)
    {
        static_assert(corsac::has_equality_v<T>, "T must be comparable");

        reverse_iterator it = corsac::find(rbegin(), rend(), value);

        if (it != rend())
            return erase_unsorted(it);
        else
            return it;
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::erase(const_reverse_iterator position)
    {
        return reverse_iterator(erase((++position).base()));
    }


    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::erase(const_reverse_iterator first, const_reverse_iterator last)
    {
        // Версия, которая стирается в порядке от первой до последней.
        // difference_type i(first.base() - last.base());
        // while(i--)
        //     first = erase(first);
        // return first;

        // Версия, которая стирается в порядке от последнего к первому, но немного более эффективна:
        return reverse_iterator(erase(last.base(), first.base()));
    }

    template <typename T, typename Allocator>
    inline typename vector<T, Allocator>::reverse_iterator
    vector<T, Allocator>::erase_unsorted(const_reverse_iterator position)
    {
        return reverse_iterator(erase_unsorted((++position).base()));
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::clear() noexcept
    {
        corsac::destruct(mpBegin, mpEnd);
        mpEnd = mpBegin;
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::reset_lose_memory() noexcept
    {
        // Функция сброса - это специальная функция расширения, которая в одностороннем порядке
        // сбрасывает контейнер в пустое состояние, не освобождая память содержащихся в нем объектов.
        // Это полезно для очень быстрого удаления контейнера, встроенного в рабочую память.
        mpBegin = mpEnd = internalCapacityPtr() = NULL;
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::swap(this_type& x)
    {
        DoSwap(x);
    }

    template <typename T, typename Allocator>
    template <typename ForwardIterator>
    inline typename vector<T, Allocator>::pointer
    vector<T, Allocator>::DoRealloc(size_type n, ForwardIterator first, ForwardIterator last, should_copy_tag)
    {
        T* const p = DoAllocate(n); // p имеет тип T, но не построен.
        corsac::uninitialized_copy_ptr(first, last, p); // копировать-конструирует p из [first, last).
        return p;
    }

    template <typename T, typename Allocator>
    template <typename ForwardIterator>
    inline typename vector<T, Allocator>::pointer
    vector<T, Allocator>::DoRealloc(size_type n, ForwardIterator first, ForwardIterator last, should_move_tag)
    {
        T* const p = DoAllocate(n); // p имеет тип T, но не построен.
        corsac::uninitialized_move_ptr_if_noexcept(first, last, p); // копировать-конструирует p из [first, last).
        return p;
    }

    template <typename T, typename Allocator>
    template <typename Integer>
    inline void vector<T, Allocator>::DoInit(Integer n, Integer value, true_type)
    {
        mpBegin    = DoAllocate(static_cast<size_type>(n));
        internalCapacityPtr() = mpBegin + n;
        mpEnd      = internalCapacityPtr();

        // Если T является константным типом (например, const int), нам нужно инициализировать его, как если бы он не был константным.
        using non_const_value_type = typename corsac::remove_const<T>::type;
        corsac::uninitialized_fill_n_ptr<value_type, Integer>(static_cast<non_const_value_type*>(mpBegin, n, value));
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline void vector<T, Allocator>::DoInit(InputIterator first, InputIterator last, false_type)
    {
        using IC = typename corsac::iterator_traits<InputIterator>:: iterator_category;
        DoInitFromIterator(first, last, IC());
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline void vector<T, Allocator>::DoInitFromIterator(InputIterator first, InputIterator last, corsac::input_iterator_tag)
    {
        // Todo: используйте emplace_back вместо push_back(). Наш emplace_back будет работать ниже без каких-либо ifdef.
        for(; first != last; ++first) // InputIterators по определению фактически позволяют выполнять итерацию только один раз.
            push_back(*first);        // Таким образом, стандарт требует, чтобы мы выполняли эту (неэффективную) реализацию.
    }                                 // К счастью, InputIterators на практике почти не используются, поэтому этот код, скорее всего, никогда не будет выполнен.

    template <typename T, typename Allocator>
    template <typename ForwardIterator>
    inline void vector<T, Allocator>::DoInitFromIterator(ForwardIterator first, ForwardIterator last, corsac::forward_iterator_tag)
    {
        const size_type n = static_cast<size_type>(corsac::distance(first, last));
        mpBegin    = DoAllocate(n);
        internalCapacityPtr() = mpBegin + n;
        mpEnd      = internalCapacityPtr();

        // Если T является константным типом (например, const int), нам нужно инициализировать его, как если бы он не был константным.
        using non_const_value_type = typename corsac::remove_const<T>::type;
        corsac::uninitialized_copy_ptr(first, last, static_cast<non_const_value_type*>(mpBegin));
    }

    template <typename T, typename Allocator>
    template <typename Integer, bool bMove>
    inline void vector<T, Allocator>::DoAssign(Integer n, Integer value, true_type)
    {
        DoAssignValues(static_cast<size_type>(n), static_cast<value_type>(value));
    }

    template <typename T, typename Allocator>
    template <typename InputIterator, bool bMove>
    inline void vector<T, Allocator>::DoAssign(InputIterator first, InputIterator last, false_type)
    {
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;
        DoAssignFromIterator<InputIterator, bMove>(first, last, IC());
    }


    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoAssignValues(size_type n, const value_type& value)
    {
        if(n > size_type(internalCapacityPtr() - mpBegin)) // Если n > емкость ...
        {
            this_type temp(n, value, internalAllocator()); // У нас нет другого выбора, кроме как перераспределить новую память.
            swap(temp);
        }
        else if(n > size_type(mpEnd - mpBegin)) // Если n > размер ...
        {
            corsac::fill(mpBegin, mpEnd, value);
            corsac::uninitialized_fill_n_ptr(mpEnd, n - size_type(mpEnd - mpBegin), value);
            mpEnd += n - size_type(mpEnd - mpBegin);
        }
        else // иначе 0 <= n <= размер
        {
            corsac::fill_n(mpBegin, n, value);
            erase(mpBegin + n, mpEnd);
        }
    }

    template <typename T, typename Allocator>
    template <typename InputIterator, bool bMove>
    void vector<T, Allocator>::DoAssignFromIterator(InputIterator first, InputIterator last, corsac::input_iterator_tag)
    {
        iterator position(mpBegin);

        while((position != mpEnd) && (first != last))
        {
            *position = *first;
            ++first;
            ++position;
        }
        if(first == last)
            erase(position, mpEnd);
        else
            insert(mpEnd, first, last);
    }

    template <typename T, typename Allocator>
    template <typename RandomAccessIterator, bool bMove>
    void vector<T, Allocator>::DoAssignFromIterator(RandomAccessIterator first, RandomAccessIterator last, corsac::random_access_iterator_tag)
    {
        const size_type n = static_cast<size_type>(corsac::distance(first, last));

        if(n > size_type(internalCapacityPtr() - mpBegin)) // Если n > емкость ...
        {
            pointer const pNewData = DoRealloc(n, first, last, should_move_or_copy_tag<bMove>());
            corsac::destruct(mpBegin, mpEnd);
            DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

            mpBegin    = pNewData;
            mpEnd      = mpBegin + n;
            internalCapacityPtr() = mpEnd;
        }
        else if(n <= size_type(mpEnd - mpBegin)) // Если n <= size ...
        {
            // Поскольку мы копируем в mpBegin, нам не нужно беспокоиться о необходимости copy_backward или копии, подобной memmove (в отличие от копии, подобной memcpy).
            pointer const pNewEnd = corsac::copy(first, last, mpBegin);
            corsac::destruct(pNewEnd, mpEnd);
            mpEnd = pNewEnd;
        }
        else // иначе размер < n <= емкость
        {
            RandomAccessIterator position = first + (mpEnd - mpBegin);
            // Поскольку мы копируем в mpBegin, нам не нужно беспокоиться о необходимости copy_backward или копии, подобной memmove (в отличие от копии, подобной memcpy).
            corsac::copy(first, position, mpBegin);
            mpEnd = corsac::uninitialized_copy_ptr(position, last, mpEnd);
        }
    }

    template <typename T, typename Allocator>
    template <typename Integer>
    inline void vector<T, Allocator>::DoInsert(const_iterator position, Integer n, Integer value, true_type)
    {
        DoInsertValues(position, static_cast<size_type>(n), static_cast<value_type>(value));
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline void vector<T, Allocator>::DoInsert(const_iterator position, InputIterator first, InputIterator last, false_type)
    {
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;
        DoInsertFromIterator(position, first, last, IC());
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    inline void vector<T, Allocator>::DoInsertFromIterator(const_iterator position, InputIterator first, InputIterator last, corsac::input_iterator_tag)
    {
        for(; first != last; ++first, ++position)
            position = insert(position, *first);
    }


    template <typename T, typename Allocator>
    template <typename BidirectionalIterator>
    void vector<T, Allocator>::DoInsertFromIterator(const_iterator position, BidirectionalIterator first, BidirectionalIterator last, corsac::bidirectional_iterator_tag)
    {
        #if CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY((position < mpBegin) || (position > mpEnd)))
                        CORSAC_FAIL_MSG("vector::insert -- invalid position");
        #endif
        iterator destPosition = const_cast<value_type*>(position);

        if(first != last)
        {
            const size_type n = static_cast<size_type>(corsac::distance(first, last));  // n - количество вставляемых элементов.

            if(n <= size_type(internalCapacityPtr() - mpEnd)) // Если n соответствует существующей емкости ...
            {
                const size_type nExtra = static_cast<size_type>(mpEnd - destPosition);

                if(n < nExtra) // Если вставленные значения полностью находятся в инициализированной памяти (т.е. перед mpEnd) ...
                {
                    corsac::uninitialized_move_ptr(mpEnd - n, mpEnd, mpEnd);
                    corsac::move_backward(destPosition, mpEnd - n, mpEnd); // Нам нужен move_backward из-за возможных проблем с перекрытием.
                    corsac::copy(first, last, destPosition);
                }
                else
                {
                    BidirectionalIterator iTemp = first;
                    corsac::advance(iTemp, nExtra);
                    corsac::uninitialized_copy_ptr(iTemp, last, mpEnd);
                    corsac::uninitialized_move_ptr(destPosition, mpEnd, mpEnd + n - nExtra);
                    corsac::copy_backward(first, iTemp, destPosition + nExtra);
                }

                mpEnd += n;
            }
            else // иначе нам нужно расширить наши возможности.
            {
                const size_type nPrevSize = size_type(mpEnd - mpBegin);
                const size_type nGrowSize = GetNewCapacity(nPrevSize);
                const size_type nNewSize  = nGrowSize > (nPrevSize + n) ? nGrowSize : (nPrevSize + n);
                pointer const   pNewData  = DoAllocate(nNewSize);

                #if CORSAC_EXCEPTIONS_ENABLED
                    pointer pNewEnd = pNewData;
                    try
                    {
                        pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                        pNewEnd = corsac::uninitialized_copy_ptr(first, last, pNewEnd);
                        pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, pNewEnd);
                    }
                    catch(...)
                    {
                        corsac::destruct(pNewData, pNewEnd);
                        DoFree(pNewData, nNewSize);
                        throw;
                    }
                #else
                    pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                    pNewEnd         = corsac::uninitialized_copy_ptr(first, last, pNewEnd);
                    pNewEnd         = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, pNewEnd);
                #endif

                corsac::destruct(mpBegin, mpEnd);
                DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

                mpBegin    = pNewData;
                mpEnd      = pNewEnd;
                internalCapacityPtr() = pNewData + nNewSize;
            }
        }
    }


    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoInsertValues(const_iterator position, size_type n, const value_type& value)
    {
    #if CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY((position < mpBegin) || (position > mpEnd)))
                    CORSAC_FAIL_MSG("vector::insert -- invalid position");
    #endif
        iterator destPosition = const_cast<value_type*>(position);

        if(n <= size_type(internalCapacityPtr() - mpEnd)) // Если n <= вместимость ...
        {
            if(n > 0) // Todo: посмотрите, есть ли способ устранить это выражение «если».
            {
                // Для рассмотрения: сделайте этот алгоритм более похожим на DoInsertValue, в котором используется указатель на значение.
                const value_type temp  = value;
                const size_type nExtra = static_cast<size_type>(mpEnd - destPosition);

                if(n < nExtra)
                {
                    corsac::uninitialized_move_ptr(mpEnd - n, mpEnd, mpEnd);
                    corsac::move_backward(destPosition, mpEnd - n, mpEnd); // Нам нужен move_backward из-за возможных проблем с перекрытием.
                    corsac::fill(destPosition, destPosition + n, temp);
                }
                else
                {
                    corsac::uninitialized_fill_n_ptr(mpEnd, n - nExtra, temp);
                    corsac::uninitialized_move_ptr(destPosition, mpEnd, mpEnd + n - nExtra);
                    corsac::fill(destPosition, mpEnd, temp);
                }

                mpEnd += n;
            }
        }
        else // иначе n > емкость
        {
            const size_type nPrevSize = size_type(mpEnd - mpBegin);
            const size_type nGrowSize = GetNewCapacity(nPrevSize);
            const size_type nNewSize  = nGrowSize > (nPrevSize + n) ? nGrowSize : (nPrevSize + n);
            pointer const pNewData    = DoAllocate(nNewSize);

            #if CORSAC_EXCEPTIONS_ENABLED
                pointer pNewEnd = pNewData;
                try
                {
                    pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                    corsac::uninitialized_fill_n_ptr(pNewEnd, n, value);
                    pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, pNewEnd + n);
                }
                catch(...)
                {
                    corsac::destruct(pNewData, pNewEnd);
                    DoFree(pNewData, nNewSize);
                    throw;
                }
                #else
                pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                corsac::uninitialized_fill_n_ptr(pNewEnd, n, value);
                pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, pNewEnd + n);
            #endif

            corsac::destruct(mpBegin, mpEnd);
            DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

            mpBegin    = pNewData;
            mpEnd      = pNewEnd;
            internalCapacityPtr() = pNewData + nNewSize;
        }
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoClearCapacity() // Эта функция существует, потому что set_capacity() в настоящее время косвенно требует, чтобы value_type был конструктивным по умолчанию,
    {                                            // и некоторые функции, которые должны очистить нашу емкость (например, operator =), не должны требовать возможности построения по умолчанию.
        clear();
        this_type temp(corsac::move(*this));  // Это самый простой способ добиться этого,
        swap(temp);                       // и он так же эффективен, как и любой другой.
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoGrow(size_type n)
    {
        pointer const pNewData = DoAllocate(n);

        pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);

        corsac::destruct(mpBegin, mpEnd);
        DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

        mpBegin    = pNewData;
        mpEnd      = pNewEnd;
        internalCapacityPtr() = pNewData + n;
    }

    template <typename T, typename Allocator>
    inline void vector<T, Allocator>::DoSwap(this_type& x)
    {
        corsac::swap(mpBegin,       x.mpBegin);
        corsac::swap(mpEnd,         x.mpEnd);
        corsac::swap(mCapacity,     x.mCapacity); // Мы делаем это, даже если CORSAC_ALLOCATOR_COPY_ENABLED равен 0.
        corsac::swap(mAllocator,    x.mAllocator);
    }

    // Дублирование кода между этой версией и версией, которая не принимает аргумент значения и по умолчанию создает значения
    // К сожалению, его нелегко решить, не полагаясь на идеальную пересылку C ++ 11.
    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoInsertValuesEnd(size_type n, const value_type& value)
    {
        if(n > size_type(internalCapacityPtr() - mpEnd))
        {
            const size_type nPrevSize = size_type(mpEnd - mpBegin);
            const size_type nGrowSize = GetNewCapacity(nPrevSize);
            const size_type nNewSize = corsac::max(nGrowSize, nPrevSize + n);
            pointer const pNewData = DoAllocate(nNewSize);

            #if CORSAC_EXCEPTIONS_ENABLED
                pointer pNewEnd = pNewData; // Присвойте pNewEnd значение здесь на случай, если копия сработает.
                try
                {
                    pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
                }
                catch(...)
                {
                    corsac::destruct(pNewData, pNewEnd);
                    DoFree(pNewData, nNewSize);
                    throw;
                }
            #else
                pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
            #endif

        corsac::uninitialized_fill_n_ptr(pNewEnd, n, value);
        pNewEnd += n;

        corsac::destruct(mpBegin, mpEnd);
        DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

        mpBegin    = pNewData;
        mpEnd      = pNewEnd;
        internalCapacityPtr() = pNewData + nNewSize;
        }
        else
        {
            corsac::uninitialized_fill_n_ptr(mpEnd, n, value);
            mpEnd += n;
        }
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::DoInsertValuesEnd(size_type n)
    {
        if (n > size_type(internalCapacityPtr() - mpEnd))
        {
            const size_type nPrevSize = size_type(mpEnd - mpBegin);
            const size_type nGrowSize = GetNewCapacity(nPrevSize);
            const size_type nNewSize = corsac::max(nGrowSize, nPrevSize + n);
            pointer const pNewData = DoAllocate(nNewSize);

            #if CORSAC_EXCEPTIONS_ENABLED
                pointer pNewEnd = pNewData;  // Присвойте pNewEnd значение здесь на случай, если копия сработает.
                try { pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData); }
                catch (...)
                {
                    corsac::destruct(pNewData, pNewEnd);
                    DoFree(pNewData, nNewSize);
                    throw;
                }
            #else
                pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
            #endif

        corsac::uninitialized_default_fill_n(pNewEnd, n);
        pNewEnd += n;

        corsac::destruct(mpBegin, mpEnd);
        DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

        mpBegin = pNewData;
        mpEnd = pNewEnd;
        internalCapacityPtr() = pNewData + nNewSize;
        }
        else
        {
            corsac::uninitialized_default_fill_n(mpEnd, n);
            mpEnd += n;
        }
    }

    template <typename T, typename Allocator>
    template<typename... Args>
    void vector<T, Allocator>::DoInsertValue(const_iterator position, Args&&... args)
    {
        // Для рассмотрения: вполне возможно, что аргументы из value_type поступают из самой текущей
        // последовательности, и поэтому мы должны быть уверены, что обработаем этот случай. Это
        // отличается от insert (position, const value_type &), потому что в этом случае значение потенциально изменяется.

        #if CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY((position < mpBegin) || (position > mpEnd)))
                        CORSAC_FAIL_MSG("vector::insert/emplace -- invalid position");
        #endif

        iterator destPosition = const_cast<value_type*>(position);

        if(mpEnd != internalCapacityPtr()) // Если размер < вместимость ...
        {
            // Нам нужно принять во внимание возможность того, что args - это value_type, который
            // поступает из самого вектора. создание временного значения в стеке здесь не является
            // tоптимальным способом решения этой проблемы, потому что sizeof (value_type) может быть
            // слишком большим для данной платформы. Альтернативным решением может быть специализация этой
            // функции для случая, когда аргумент имеет значение const value_type & или value_type &&.

            // Хотя вставка в end() допустима, наш дизайн таков, что вызывающий код должен обрабатывать этот случай, прежде чем попасть сюда,
            // поскольку наша оптимизированная логика напрямую не обрабатывает этот конкретный случай из-за результирующих отрицательных диапазонов.
            CORSAC_ASSERT(position < mpEnd);
            // Это нужно сделать до move_backward ниже, потому что, возможно, args относится к чему-то в пределах диапазона перемещения.
            value_type  value(corsac::forward<Args>(args)...);
            // mpEnd - это неинициализированная память, поэтому мы должны встраивать в нее, а не переходить в нее, как мы делаем с другими элементами ниже.
            ::new(static_cast<void*>(mpEnd)) value_type(corsac::move(*(mpEnd - 1)));\
            // Нам нужно вернуться назад из-за возможных проблем с дублированием.
            corsac::move_backward(destPosition, mpEnd - 1, mpEnd);
            corsac::destruct(destPosition);
            // Переместите аргумент значения в указанную позицию.
            ::new(static_cast<void*>(destPosition)) value_type(corsac::move(value));
            ++mpEnd;
        }
        else // else (size == capacity)
        {
            const size_type nPosSize  = size_type(destPosition - mpBegin); // Индекс позиции вставки.
            const size_type nPrevSize = size_type(mpEnd - mpBegin);
            const size_type nNewSize  = GetNewCapacity(nPrevSize);
            pointer const   pNewData  = DoAllocate(nNewSize);

            #if CORSAC_EXCEPTIONS_ENABLED
                pointer pNewEnd = pNewData;
                try
                {   // To do: ниже мы не обрабатываем исключения должным образом. В частности, мы не хотим
                    // вызовать corsac::destruct для всего диапазона, если была построена только первая часть диапазона.
                    // Поскольку старые данные потенциально перемещаются, а не копируются, нам нужно переместить.
                    ::new((void*)(pNewData + nPosSize)) value_type(corsac::forward<Args>(args)...);
                    // Установите значение NULL, чтобы в catch мы могли сказать, что исключение произошло во время следующего вызова.
                    pNewEnd = NULL;
                    // значение сначала, потому что это может быть ссылка на старые перемещаемые данные.
                    pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                    pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, ++pNewEnd);
                }
                catch(...)
                {
                    if(pNewEnd)
                        corsac::destruct(pNewData, pNewEnd);    // Уничтожьте то, что было построено до сих пор.
                    else
                        corsac::destruct(pNewData + nPosSize);  // Исключение произошло во время первого неинициализированного перемещения, поэтому уничтожьте только значение nPosSize.
                    DoFree(pNewData, nNewSize);
                    throw;
                }
            #else
                // Поскольку старые данные потенциально перемещаются, а не копируются, нам нужно переместить
                ::new((void*)(pNewData + nPosSize)) value_type(corsac::forward<Args>(args)...);
                // значение сначала, потому что это может быть ссылка на старые перемещаемые данные.
                pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, destPosition, pNewData);
                // Вопрос: с отключенными исключениями, предполагаем ли мы, что все операции не являются исключениями,
                // и поэтому нет необходимости в uninitialized_move_ptr_if_noexcept?
                pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(destPosition, mpEnd, ++pNewEnd);
            #endif

            corsac::destruct(mpBegin, mpEnd);
            DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

            mpBegin    = pNewData;
            mpEnd      = pNewEnd;
            internalCapacityPtr() = pNewData + nNewSize;
        }
    }

    template <typename T, typename Allocator>
    template<typename... Args>
    void vector<T, Allocator>::DoInsertValueEnd(Args&&... args)
    {
        const size_type nPrevSize = size_type(mpEnd - mpBegin);
        const size_type nNewSize  = GetNewCapacity(nPrevSize);
        pointer const   pNewData  = DoAllocate(nNewSize);

        #if CORSAC_EXCEPTIONS_ENABLED
            pointer pNewEnd = pNewData; // Присвойте pNewEnd значение здесь на случай, если копия сработает.
            try
            {
                pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
                ::new((void*)pNewEnd) value_type(corsac::forward<Args>(args)...);
                pNewEnd++;
            }
            catch(...)
            {
                corsac::destruct(pNewData, pNewEnd);
                DoFree(pNewData, nNewSize);
                throw;
            }
        #else
        pointer pNewEnd = corsac::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
        ::new((void*)pNewEnd) value_type(corsac::forward<Args>(args)...);
        pNewEnd++;
        #endif

        corsac::destruct(mpBegin, mpEnd);
        DoFree(mpBegin, static_cast<size_type>(internalCapacityPtr() - mpBegin));

        mpBegin    = pNewData;
        mpEnd      = pNewEnd;
        internalCapacityPtr() = pNewData + nNewSize;
    }

    template <typename T, typename Allocator>
    inline bool vector<T, Allocator>::validate() const noexcept
    {
        if(mpEnd < mpBegin)
            return false;
        if(internalCapacityPtr() < mpEnd)
            return false;
        return true;
    }

    template <typename T, typename Allocator>
    inline int vector<T, Allocator>::validate_iterator(const_iterator i) const noexcept
    {
        if(i >= mpBegin)
        {
            if(i < mpEnd)
                return (isf_valid | isf_current | isf_can_dereference);

            if(i <= mpEnd)
                return (isf_valid | isf_current);
        }
        return isf_none;
    }

    ///////////////////////////////////////////////////////////////////////
    // global operators
    ///////////////////////////////////////////////////////////////////////

    template <typename T, typename Allocator>
    inline bool operator==(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return ((a.size() == b.size()) && corsac::equal(a.begin(), a.end(), b.begin()));
    }

    template <typename T, typename Allocator>
    inline bool operator!=(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return ((a.size() != b.size()) || !corsac::equal(a.begin(), a.end(), b.begin()));
    }

    template <typename T, typename Allocator>
    inline bool operator<(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return corsac::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    template <typename T, typename Allocator>
    inline bool operator>(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return b < a;
    }

    template <typename T, typename Allocator>
    inline bool operator<=(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return !(b < a);
    }

    template <typename T, typename Allocator>
    inline bool operator>=(const vector<T, Allocator>& a, const vector<T, Allocator>& b)
    {
        return !(a < b);
    }

    template <typename T, typename Allocator>
    inline void swap(vector<T, Allocator>& a, vector<T, Allocator>& b) noexcept
    {
        a.swap(b);
    }

    ///////////////////////////////////////////////////////////////////////
    // erase / erase_if
    ///////////////////////////////////////////////////////////////////////
    template <class T, class Allocator, class U>
    void erase(vector<T, Allocator>& c, const U& value)
    {
        // Erases all elements that compare equal to value from the container.
        c.erase(corsac::remove(c.begin(), c.end(), value), c.end());
    }

    template <class T, class Allocator, class Predicate>
    void erase_if(vector<T, Allocator>& c, Predicate predicate)
    {
        // Erases all elements that satisfy the predicate pred from the container.
        c.erase(corsac::remove_if(c.begin(), c.end(), predicate), c.end());
    }
}

#endif

