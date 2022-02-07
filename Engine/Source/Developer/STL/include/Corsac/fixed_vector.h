/**
 * corsac::STL
 *
 * fixed_vector.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_FIXED_VECTOR_H
#define CORSAC_STL_FIXED_VECTOR_H

#pragma once

#include "Corsac/vector.h"
#include "Corsac/STL/fixed_pool.h"

namespace corsac
{
    // CORSAC_FIXED_VECTOR_DEFAULT_NAME
    #ifndef CORSAC_FIXED_VECTOR_DEFAULT_NAME
        #define CORSAC_FIXED_VECTOR_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " fixed_vector"
    #endif


    // CORSAC_FIXED_VECTOR_DEFAULT_ALLOCATOR
    #ifndef CORSAC_FIXED_VECTOR_DEFAULT_ALLOCATOR
        #define CORSAC_FIXED_VECTOR_DEFAULT_ALLOCATOR overflow_allocator_type(CORSAC_FIXED_VECTOR_DEFAULT_NAME)
    #endif

    /**
    * fixed_vector
    *
    * Fixed_vector с bEnableOverflow == true идентичен обычному вектору с точки зрения его поведения.
    * К нему относятся все ожидания регулярного вектора и никаких дополнительных ожиданий от него не возникает.
    * Когда bEnableOverflow имеет значение false, fixed_vector ведет себя как обычный вектор,
    * за исключением того, что его емкость никогда не может увеличиваться.
    * Все операции, которые вы выполняете с таким фиксированным_вектором, которые требуют увеличения емкости,
    * приведут к неопределенному поведению или исключению распределения C++, в зависимости от конфигурации.
    *
    * Параметры шаблона:
    *     T                      Тип объекта, который содержит вектор.
    *     nodeCount              Максимальное количество содержащихся объектов.
    *     bEnableOverflow        Должны ли мы использовать кучу переполнения, если наш пул объектов исчерпан.
    *     OverflowAllocator     Распределитель переполнения, который используется, только если bEnableOverflow == true. По умолчанию используется глобальная куча.
    *
    * Примечание. Значение nodeCount должно быть не менее 1.
    *
    * Пример использования:
    *    fixed_vector<Widget, 128, true> fixedVector);
    *    fixedVector.push_back(Widget());
    *    fixedVector.resize(200);
    *    fixedVector.clear();
    */
    template <typename T, size_t nodeCount, bool bEnableOverflow = true, typename OverflowAllocator = typename corsac::type_select<bEnableOverflow, CORSAC_ALLOCATOR_TYPE, CORSAC_ALLOCATOR_TYPE>::type>
    class fixed_vector : public vector<T, fixed_vector_allocator<sizeof(T), nodeCount, CORSAC_ALIGN_OF(T), 0, bEnableOverflow, OverflowAllocator> >
    {
    public:
        using fixed_allocator_type      = fixed_vector_allocator<sizeof(T), nodeCount, CORSAC_ALIGN_OF(T), 0, bEnableOverflow, OverflowAllocator>;
        using overflow_allocator_type   = OverflowAllocator;
        using base_type                 = vector<T, fixed_allocator_type>;
        using this_type                 = fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>;
        using size_type                 = typename base_type::size_type;
        using value_type                = typename base_type::value_type;
        using reference                 = typename base_type::reference;
        using iterator                  = typename base_type::iterator;
        using const_iterator            = typename base_type::const_iterator;
        using aligned_buffer_type       = aligned_buffer<nodeCount * sizeof(T), CORSAC_ALIGN_OF(T)>;

        enum { kMaxSize = nodeCount };

        using base_type::get_allocator;
        using base_type::mpBegin;
        using base_type::mpEnd;
        using base_type::internalCapacityPtr;
        using base_type::resize;
        using base_type::clear;
        using base_type::size;
        using base_type::assign;
        using base_type::npos;
        using base_type::DoAllocate;
        using base_type::DoFree;
        using base_type::DoAssign;
        using base_type::DoAssignFromIterator;

    protected:
        aligned_buffer_type mBuffer;

    public:
        fixed_vector();
        explicit fixed_vector(const overflow_allocator_type& overflowAllocator); // Применимо, только если bEnableOverflow истинно.
        explicit fixed_vector(size_type n);                                      // В настоящее время для простоты мы не поддерживаем спецификацию overflowAllocator для других конструкторов.
        fixed_vector(size_type n, const value_type& value);
        fixed_vector(const this_type& x);
        fixed_vector(this_type&& x);
        fixed_vector(this_type&& x, const overflow_allocator_type& overflowAllocator);
        fixed_vector(std::initializer_list<T> ilist, const overflow_allocator_type& overflowAllocator = CORSAC_FIXED_VECTOR_DEFAULT_ALLOCATOR);

        template <typename InputIterator>
        fixed_vector(InputIterator first, InputIterator last);

        this_type& operator=(const this_type& x);
        this_type& operator=(std::initializer_list<T> ilist);
        this_type& operator=(this_type&& x);

        void swap(this_type& x);

        void      set_capacity(size_type n);
        void      clear(bool freeOverflow);
        void      reset_lose_memory();          // Это односторонний сброс в исходное пустое состояние. Деструкторы не вызываются, освобождение памяти не происходит.
        size_type max_size() const;             // Возвращает максимальный фиксированный размер, который является параметром nodeCount, задаваемым пользователем.
        bool      full() const;                 // Возвращает истину, если фиксированное пространство было полностью выделено. Обратите внимание, что если переполнение включено, размер контейнера может быть больше, чем nodeCount, но full () может вернуть true, потому что фиксированное пространство может иметь недавно освобожденный слот.
        bool      has_overflowed() const;       // Возвращает истину, если выделения перетекают в распределитель переполнения. Имеет значение, только если разрешено переполнение.
        bool      can_overflow() const;         // Возвращает значение параметра шаблона bEnableOverflow.

        void*     push_back_uninitialized();
        void      push_back(const value_type& value);   // Мы реализуем здесь push_back, потому что у нас есть специализация, которая
        reference push_back();                          // меньше в случае отключения переполнения.
        void      push_back(value_type&& value);

        const overflow_allocator_type& get_overflow_allocator() const noexcept;
        overflow_allocator_type&       get_overflow_allocator() noexcept;
        void                           set_overflow_allocator(const overflow_allocator_type& allocator);

    protected:
        void*     DoPushBackUninitialized(true_type);
        void*     DoPushBackUninitialized(false_type);

        void      DoPushBack(true_type, const value_type& value);
        void      DoPushBack(false_type, const value_type& value);

        void      DoPushBackMove(true_type, value_type&& value);
        void      DoPushBackMove(false_type, value_type&& value);

        reference DoPushBack(false_type);
        reference DoPushBack(true_type);

    }; // fixed_vector

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector()
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(CORSAC_FIXED_VECTOR_DEFAULT_NAME);
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(const overflow_allocator_type& overflowAllocator)
            : base_type(fixed_allocator_type(mBuffer.buffer, overflowAllocator))
    {
        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(CORSAC_FIXED_VECTOR_DEFAULT_NAME);
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(size_type n)
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(CORSAC_FIXED_VECTOR_DEFAULT_NAME);
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        resize(n);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(size_type n, const value_type& value)
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(CORSAC_FIXED_VECTOR_DEFAULT_NAME);
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        resize(n, value);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(const this_type& x)
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        get_allocator().copy_overflow_allocator(x.get_allocator());

        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(x.get_allocator().get_name());
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        base_type::template DoAssign<const_iterator, false>(x.begin(), x.end(), false_type());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(this_type&& x)
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        get_allocator().copy_overflow_allocator(x.get_allocator());

        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(x.get_allocator().get_name());
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        base_type::template DoAssign<move_iterator<iterator>, true>(corsac::make_move_iterator(x.begin()), corsac::make_move_iterator(x.end()), false_type());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(this_type&& x, const overflow_allocator_type& overflowAllocator)
            : base_type(fixed_allocator_type(mBuffer.buffer, overflowAllocator))
    {
        get_allocator().copy_overflow_allocator(x.get_allocator());

        #if CORSAC_NAME_ENABLED
                get_allocator().set_name(x.get_allocator().get_name());
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        base_type::template DoAssign<iterator, true>(x.begin(), x.end(), false_type());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(std::initializer_list<T> ilist, const overflow_allocator_type& overflowAllocator)
            : base_type(fixed_allocator_type(mBuffer.buffer, overflowAllocator))
    {
        using InputIterator = typename std::initializer_list<value_type>::iterator;
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        base_type::template DoAssignFromIterator<InputIterator, false>(ilist.begin(), ilist.end(), IC());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    template <typename InputIterator>
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::fixed_vector(InputIterator first, InputIterator last)
            : base_type(fixed_allocator_type(mBuffer.buffer))
    {
        #if CORSAC_NAME_ENABLED
            get_allocator().set_name(CORSAC_FIXED_VECTOR_DEFAULT_NAME);
        #endif

        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
        base_type::template DoAssign<InputIterator, false>(first, last, is_integral<InputIterator>());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::this_type&
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::operator=(const this_type& x)
    {
        if(this != &x)
        {
            clear();
            get_allocator() = x.get_allocator(); // Основным результатом этого является копирование распределителя переполнения.
            base_type::template DoAssign<const_iterator, false>(x.begin(), x.end(), false_type());
        }
        return *this;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::this_type&
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::operator=(std::initializer_list<T> ilist)
    {
        using InputIterator = typename std::initializer_list<value_type>::iterator;
        using IC = typename corsac::iterator_traits<InputIterator>::iterator_category;

        clear();
        base_type::template DoAssignFromIterator<InputIterator, false>(ilist.begin(), ilist.end(), IC());
        return *this;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::this_type&
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::operator=(this_type&& x)
    {
        if (this != &x)
        {
            clear();
            get_allocator() = x.get_allocator(); // Основным результатом этого является копирование распределителя переполнения.
            base_type::template DoAssign<move_iterator<iterator>, true>(corsac::make_move_iterator(x.begin()), corsac::make_move_iterator(x.end()), false_type());
        }
        return *this;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::swap(this_type& x)
    {
        if((has_overflowed() && x.has_overflowed()) && (get_overflow_allocator() == x.get_overflow_allocator())) // Если оба контейнера используют кучу вместо локальной памяти тогда мы можем
        {                                                                                                        // выполнить быструю подкачку указателя вместо подкачки содержимого.
            corsac::swap(mpBegin,    x.mpBegin);
            corsac::swap(mpEnd,      x.mpEnd);
            corsac::swap(internalCapacityPtr(), x.internalCapacityPtr());
        }
        else
        {
            // Фиксированные контейнеры используют специальный своп, который может работать с чрезмерно большими буферами.
            corsac::fixed_swap(*this, x);
        }
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::set_capacity(size_type n)
    {
        const size_type nPrevSize     = (size_type)(mpEnd - mpBegin);
        const size_type nPrevCapacity = (size_type)(internalCapacityPtr() - mpBegin);

        if(n == npos) // Если пользователь хочет установить емкость так, чтобы она была равна размеру (т. Е. Свободная избыточная емкость) ...
            n = nPrevSize;

        if(n != nPrevCapacity)  // Если запрос приводит к изменению емкости ...
        {
            if(can_overflow() && (((uintptr_t)mpBegin != (uintptr_t)mBuffer.buffer) || (n > kMaxSize))) // Если мы используем или будем использовать динамически выделяемую память вместо нашего буфера-члена фиксированного размера ...
            {
                T* const pNewData = (n <= kMaxSize) ? (T*)&mBuffer.buffer[0] : DoAllocate(n);
                T* const pCopyEnd = (n < nPrevSize) ? (mpBegin + n) : mpEnd;
                corsac::uninitialized_move_ptr(mpBegin, pCopyEnd, pNewData); // Move [mpBegin, pCopyEnd).
                corsac::destruct(mpBegin, mpEnd);
                if((uintptr_t)mpBegin != (uintptr_t)mBuffer.buffer)
                    DoFree(mpBegin, (size_type)(internalCapacityPtr() - mpBegin));

                mpEnd      = pNewData + (pCopyEnd - mpBegin);
                mpBegin    = pNewData;
                internalCapacityPtr() = mpBegin + n;
            } // В противном случае новая емкость будет в пределах нашего фиксированного буфера.
            else if(n < nPrevSize) // Если вновь запрошенная емкость меньше нашего размера, мы делаем то же, что и vector::set_capacity, и изменяем размер, хотя на самом деле мы не уменьшаем емкость.
                resize(n);
        }
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename Allocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, Allocator>::clear(bool freeOverflow)
    {
        base_type::clear();
        if (freeOverflow && mpBegin != (value_type*)&mBuffer.buffer[0])
        {
            CORSACFree(get_allocator(), mpBegin, (internalCapacityPtr() - mpBegin) * sizeof(T));
            mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
            internalCapacityPtr() = mpBegin + nodeCount;
        }
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::reset_lose_memory()
    {
        mpBegin = mpEnd = (value_type*)&mBuffer.buffer[0];
        internalCapacityPtr() = mpBegin + nodeCount;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::size_type
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::max_size() const
    {
        return kMaxSize;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline bool fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::full() const
    {
        // Если size >= capacity, то мы точно полны.
        // Кроме того, если наш размер меньше, но мы отказались от mBuffer из-за предыдущего переполнения, то мы считаются заполненными.
        return ((size_t)(mpEnd - mpBegin) >= kMaxSize) || ((void*)mpBegin != (void*)mBuffer.buffer);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline bool fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::has_overflowed() const
    {
        return ((void*)mpBegin != (void*)mBuffer.buffer);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline bool fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::can_overflow() const
    {
        return bEnableOverflow;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void* fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::push_back_uninitialized()
    {
        return DoPushBackUninitialized(typename type_select<bEnableOverflow, true_type, false_type>::type());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void* fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBackUninitialized(true_type)
    {
        return base_type::push_back_uninitialized();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void* fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBackUninitialized(false_type)
    {
        CORSAC_ASSERT(mpEnd < internalCapacityPtr());
        return mpEnd++;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::push_back(const value_type& value)
    {
        DoPushBack(typename type_select<bEnableOverflow, true_type, false_type>::type(), value);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBack(true_type, const value_type& value)
    {
        base_type::push_back(value);
    }

    // Этот шаблон специализируется на отключении переполнения.
    // В этой конфигурации нет необходимости в тяжелом push_back(), который проверяет, должен ли контейнер расти (он никогда не будет)
    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBack(false_type, const value_type& value)
    {
        CORSAC_ASSERT(mpEnd < internalCapacityPtr());
        ::new((void*)mpEnd++) value_type(value);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::reference fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::push_back()
    {
        return DoPushBack(typename type_select<bEnableOverflow, true_type, false_type>::type());
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::reference fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBack(true_type)
    {
        return base_type::push_back();
    }

    // Этот шаблон специализируется на отключении переполнения.
    // В этой конфигурации нет необходимости в тяжелом push_back(), который проверяет, должен ли контейнер расти (он никогда не будет)
    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::reference fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBack(false_type)
    {
        CORSAC_ASSERT(mpEnd < internalCapacityPtr());
        ::new((void*)mpEnd++) value_type; // братите внимание, что это не value_type(), поскольку этот синтаксис работает не на всех компиляторах для типов POD.
        return *(mpEnd - 1); // То же, что и return back();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::push_back(value_type&& value)
    {
        DoPushBackMove(typename type_select<bEnableOverflow, true_type, false_type>::type(), corsac::move(value));
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBackMove(true_type, value_type&& value)
    {
        base_type::push_back(corsac::move(value)); // Это вызовет vector::push_back(value_type &&) и, возможно, поменяет значение на mpEnd.
    }

    // Этот шаблон специализируется на отключении переполнения.
    // В этой конфигурации нет необходимости в тяжелом push_back(), который проверяет, должен ли контейнер расти (он никогда не будет)
    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::DoPushBackMove(false_type, value_type&& value)
    {
        CORSAC_ASSERT(mpEnd < internalCapacityPtr());
        ::new((void*)mpEnd++) value_type(corsac::move(value)); // Это вызовет конструктор value_type(value_type &&) и, возможно, поменяет значение на mpEnd.
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline const typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::overflow_allocator_type&
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::get_overflow_allocator() const noexcept
    {
        return get_allocator().get_overflow_allocator();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline typename fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::overflow_allocator_type&
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::get_overflow_allocator() noexcept
    {
    return get_allocator().get_overflow_allocator();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void
    fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>::set_overflow_allocator(const overflow_allocator_type& allocator)
    {
        get_allocator().set_overflow_allocator(allocator);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow, typename OverflowAllocator>
    inline void swap(fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>& a,
                     fixed_vector<T, nodeCount, bEnableOverflow, OverflowAllocator>& b)
    {
        // Фиксированные контейнеры используют специальный своп, который может работать с чрезмерно большими буферами.
        corsac::fixed_swap(a, b);
    }
}

#endif //CORSAC_STL_FIXED_VECTOR_H
