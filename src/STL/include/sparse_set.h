//
// Created by Falldot on 17.11.2021.
//

#ifndef CORSAC_SPARSE_SET_H
#define CORSAC_SPARSE_SET_H

#pragma once

#include "vector.h"

namespace corsac
{
    template <typename T, typename Allocator = CORSAC_AllocatorType>
    class sparse_set
    {
        using this_type = sparse_set<T, Allocator>;
        using container = corsac::vector<T, Allocator>;
        using size_type = size_t;

        // 'npos' означает недопустимую позицию или просто непозицию.
        static const size_type npos     = static_cast<size_type>(-1);

    protected:
        container packed;
        container sparse;

    public:
        using allocator_type    = Allocator;
        using value_type        = T;
        using pointer           = T*;
        using const_pointer     = const T*;
        using reference         = T&;
        using const_reference   = const T&;

    public:
        sparse_set() noexcept;
        explicit sparse_set(const allocator_type& allocator) noexcept;
        explicit sparse_set(const allocator_type& packed_allocator, const allocator_type& sparse_allocator) noexcept;
        explicit sparse_set(size_type n, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        explicit sparse_set(size_type n, const allocator_type& packed_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR, const allocator_type& sparse_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        sparse_set(const this_type& x);
        sparse_set(const this_type& x, const allocator_type& allocator);
        sparse_set(const this_type& x, const allocator_type& packed_allocator, const allocator_type& sparse_allocator);
        sparse_set(this_type&& x) noexcept;
        sparse_set(this_type&& x, const allocator_type& allocator);
        sparse_set(this_type&& x, const allocator_type& packed_allocator, const allocator_type& sparse_allocator);
        sparse_set(std::initializer_list<value_type> packed_list, std::initializer_list<value_type> sparse_list, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        sparse_set(std::initializer_list<value_type> packed_list, std::initializer_list<value_type> sparse_list, const allocator_type& packed_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR, const allocator_type& sparse_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);

        this_type& operator=(const this_type& x) noexcept;
        this_type& operator=(this_type&& x) noexcept;

        reference       operator[](size_type n);
        const_reference operator[](size_type n) const;

        bool empty() const noexcept;
        void set_capacity(size_type n = npos);

        bool Has(const value_type& value);
        bool Has(value_type&& value);

        void Add(const value_type& value);
        void Add(value_type&& value);

        void Remove(const value_type& value);
        void Remove(value_type&& value);

        void Clear();
    };

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set() noexcept
        :   packed(container()),
            sparse(container())
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(const allocator_type &allocator) noexcept
        :   packed(container(allocator)),
            sparse(container(allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator) noexcept
        :   packed(container(packed_allocator)),
            sparse(container(sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(sparse_set::size_type n, const allocator_type &allocator)
        :   packed(container(n, allocator)),
            sparse(container(n, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(
            sparse_set::size_type n, const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(n, packed_allocator)),
            sparse(container(n, sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(const sparse_set::this_type &x)
        :   packed(container(x.packed)),
            sparse(container(x.sparse))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(const sparse_set::this_type &x, const allocator_type &allocator)
        :   packed(container(x.packed, allocator)),
            sparse(container(x.sparse, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(const sparse_set::this_type &x,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(x.packed, packed_allocator)),
            sparse(container(x.sparse, sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(sparse_set::this_type &&x) noexcept
        :   packed(container(corsac::move(x.packed))),
            sparse(container(corsac::move(x.sparse)))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(sparse_set::this_type &&x,
            const allocator_type &allocator)
        :   packed(container(corsac::move(x.packed), allocator)),
            sparse(container(corsac::move(x.sparse), allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(sparse_set::this_type &&x,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(corsac::move(x.packed), packed_allocator)),
            sparse(container(corsac::move(x.sparse), sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(
            std::initializer_list<value_type> packed_list,
            std::initializer_list<value_type> sparse_list,
            const allocator_type &allocator)
        :   packed(container(packed_list, allocator)),
            sparse(container(sparse_list, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator>::sparse_set(
            std::initializer_list<value_type> packed_list,
            std::initializer_list<value_type> sparse_list,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(packed_list, packed_allocator)),
            sparse(container(sparse_list, sparse_allocator))
    {}

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator>::Has(const value_type& value)
    {
        return sparse[value] < packed.size() && packed[sparse[value]] == value;
    }

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator>::Has(value_type &&value)
    {
        return sparse[corsac::move(value)] < packed.size() && packed[sparse[corsac::move(value)]] == corsac::move(value);
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::Add(const value_type &value)
    {
        if (value >= sparse.size()) sparse.resize(value * 2);
        if (!Has(value))
        {
            sparse[value] = packed.size();
            packed.push_back(value);
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::Add(value_type &&value)
    {
        if (corsac::move(value) >= sparse.size()) sparse.resize(corsac::move(value) * 2);
        if (!Has(corsac::move(value)))
        {
            sparse[corsac::move(value)] = packed.size();
            packed.push_back(corsac::move(value));
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::Remove(const value_type &value)
    {
        if (Has(value))
        {
            sparse[value] = *packed.erase_unsorted(&sparse[value]);
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::Remove(value_type &&value)
    {
        if (Has(corsac::move(value)))
        {
            sparse[corsac::move(value)] = *packed.erase_unsorted(&sparse[corsac::move(value)]);
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::Clear()
    {
        packed.reset_lose_memory();
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator>::this_type&
    sparse_set<T, Allocator>::operator=(const this_type& x) noexcept
    {
        if(this != &x)
        {
            packed = x.packed;
            sparse = x.sparse;
        }
        return *this;
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator>::this_type&
    sparse_set<T, Allocator>::operator=(this_type&& x) noexcept
    {
        if(this != &x)
        {
            packed = corsac::move(x.packed);
            sparse = corsac::move(x.sparse);
        }
        return *this;
    }

    template<typename T, typename Allocator>
    inline typename sparse_set<T, Allocator>::reference
    sparse_set<T, Allocator>::operator[](sparse_set::size_type n)
    {
        return packed[n];
    }

    template<typename T, typename Allocator>
    inline typename sparse_set<T, Allocator>::const_reference
    sparse_set<T, Allocator>::operator[](sparse_set::size_type n) const
    {
        return packed[n];
    }

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator>::empty() const noexcept
    {
        return packed.empty();
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator>::set_capacity(sparse_set::size_type n)
    {
        packed.set_capacity(n);
        sparse.set_capacity(n);
    }
}

#endif //CORSAC_SPARSE_SET_H
