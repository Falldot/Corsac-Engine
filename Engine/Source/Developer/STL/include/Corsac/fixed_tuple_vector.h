/**
 * corsac::STL
 *
 * fixed_tuple_vector.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_FIXED_TUPLE_VECTOR_H
#define CORSAC_STL_FIXED_TUPLE_VECTOR_H

#pragma once

#include "Corsac/tuple_vector.h"
#include "Corsac/STL/fixed_pool.h"

namespace corsac
{
    // CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_NAME
    #ifndef CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_NAME
        #define CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " fixed_tuple_vector"
    #endif

    // CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR
    #ifndef CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR
        #define CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR overflow_allocator_type(CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_NAME)
    #endif

    // Внешний интерфейс fixed_tuple_vector
    template <size_t nodeCount, bool bEnableOverflow, typename... Ts>
    class fixed_tuple_vector : public TupleVecInternal::TupleVecImpl<fixed_vector_allocator<
                                                                     TupleVecInternal::TupleRecurser<Ts...>::GetTotalAllocationSize(nodeCount, 0), 1,
            TupleVecInternal::TupleRecurser<Ts...>::GetTotalAlignment(), 0,
            bEnableOverflow, CORSAC_ALLOCATOR_TYPE>, make_index_sequence<sizeof...(Ts)>, Ts...>
    {
    public:
        using fixed_allocator_type = fixed_vector_allocator<
                TupleVecInternal::TupleRecurser<Ts...>::GetTotalAllocationSize(nodeCount, 0), 1,
                TupleVecInternal::TupleRecurser<Ts...>::GetTotalAlignment(), 0,
                bEnableOverflow, CORSAC_ALLOCATOR_TYPE>;
        using aligned_buffer_type       = aligned_buffer<fixed_allocator_type::kNodesSize, fixed_allocator_type::kNodeAlignment>;
        using this_type                 = fixed_tuple_vector<nodeCount, bEnableOverflow, Ts...>;
        using overflow_allocator_type   = CORSAC_ALLOCATOR_TYPE;
        using base_type                 = TupleVecInternal::TupleVecImpl<fixed_allocator_type, make_index_sequence<sizeof...(Ts)>, Ts...>;
        using size_type                 = typename base_type::size_type;

        using value_type                = typename base_type::value_tuple;
        using pointer                   = typename base_type::ptr_tuple;
        using const_pointer             = typename base_type::const_ptr_tuple;
        using reference                 = typename base_type::reference_tuple;
        using const_reference           = typename base_type::const_reference_tuple;

    private:
    aligned_buffer_type mBuffer;

    public:
        fixed_tuple_vector()
                : base_type(fixed_allocator_type(mBuffer.buffer), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        { }

        fixed_tuple_vector(const overflow_allocator_type& allocator)
                : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        { }

        fixed_tuple_vector(this_type&& x)
        : base_type(fixed_allocator_type(mBuffer.buffer), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::get_allocator().copy_overflow_allocator(x.get_allocator());
            base_type::DoInitFromIterator(make_move_iterator(x.begin()), make_move_iterator(x.end()));
            x.clear();
        }

        fixed_tuple_vector(this_type&& x, const overflow_allocator_type& allocator)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromIterator(make_move_iterator(x.begin()), make_move_iterator(x.end()));
            x.clear();
        }

        fixed_tuple_vector(const this_type& x)
                : base_type(fixed_allocator_type(mBuffer.buffer), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::get_allocator().copy_overflow_allocator(x.get_allocator());
            base_type::DoInitFromIterator(x.begin(), x.end());
        }

        fixed_tuple_vector(const this_type& x, const overflow_allocator_type& allocator)
                : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromIterator(x.begin(), x.end());
        }

        template <typename MoveIterBase>
        fixed_tuple_vector(move_iterator<MoveIterBase> begin, move_iterator<MoveIterBase> end, const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromIterator(begin, end);
        }

        template <typename Iterator>
        fixed_tuple_vector(Iterator begin, Iterator end, const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromIterator(begin, end);
        }

        fixed_tuple_vector(size_type n, const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitDefaultFill(n);
        }

        fixed_tuple_vector(size_type n, const Ts&... args)
        : base_type(fixed_allocator_type(mBuffer.buffer), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFillArgs(n, args...);
        }

        fixed_tuple_vector(size_type n, const Ts&... args, const overflow_allocator_type& allocator)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFillArgs(n, args...);
        }

        fixed_tuple_vector(size_type n,
        typename base_type::const_reference_tuple tup,
        const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFillTuple(n, tup);
        }

        fixed_tuple_vector(const typename base_type::value_tuple* first, const typename base_type::value_tuple* last,
                           const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromTupleArray(first, last);
        }

        fixed_tuple_vector(std::initializer_list<typename base_type::value_tuple> iList,
        const overflow_allocator_type& allocator = CORSAC_FIXED_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : base_type(fixed_allocator_type(mBuffer.buffer, allocator), mBuffer.buffer, nodeCount, fixed_allocator_type::kNodeSize)
        {
            base_type::DoInitFromTupleArray(iList.begin(), iList.end());
        }

        this_type& operator=(const this_type& other)
        {
            base_type::operator=(other);
            return *this;
        }

        this_type& operator=(this_type&& other)
        {
            base_type::clear();
            // ОК, чтобы вызвать DoInitFromIterator в сценарии без ctor, потому что clear () сбрасывает все, более или менее
            base_type::DoInitFromIterator(make_move_iterator(other.begin()), make_move_iterator(other.end()));
            other.clear();
            return *this;
        }

        this_type& operator=(std::initializer_list<typename base_type::value_tuple> iList)
        {
            base_type::operator=(iList);
            return *this;
        }

        void swap(this_type& x)
        {
            // Если оба контейнера используют кучу вместо локальной памяти,
            // мы можем выполнить быструю замену указателя вместо обмена содержимым.
            if ((has_overflowed() && x.has_overflowed()) && (get_overflow_allocator() == x.get_overflow_allocator()))
            {
                base_type::swap(x);
            }
            else
            {
                // Фиксированные контейнеры используют специальный своп, который может работать с чрезмерно большими буферами.
                corsac::fixed_swap(*this, x);
            }
        }

        // Возвращает максимальный фиксированный размер, который является параметром nodeCount, задаваемым пользователем.
        size_type max_size() const { return nodeCount; }
        bool full() const { return (base_type::mNumElements >= nodeCount) || ((void*)base_type::mpData != (void*)mBuffer.buffer);	}
        bool has_overflowed() const { return ((void*)base_type::mpData != (void*)mBuffer.buffer); }
        bool can_overflow() const { return bEnableOverflow; }

        const overflow_allocator_type& get_overflow_allocator() const { return base_type::get_allocator().get_overflow_allocator(); }
    };

    template <size_t nodeCount, bool bEnableOverflow, typename... Ts>
    inline void swap(fixed_tuple_vector<nodeCount, bEnableOverflow, Ts...>& a,
    fixed_tuple_vector<nodeCount, bEnableOverflow, Ts...>& b)
    {
        a.swap(b);
    }
}

#endif //CORSAC_STL_FIXED_TUPLE_VECTOR_H
