/**
 * corsac::STL
 *
 * sparse_set.h
 *
 * Created by Falldot on 17.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_SPARSE_SET_H
#define CORSAC_SPARSE_SET_H
/**
 * Описание (Falldot 17.11.2021)
 *
 * Sparse Set - разреженное множество — это структура данных с быстрой итерацией
 * и получением данных из разреженного идентификатора. Разреженное множество имеет 2 массива,
 * разреженный массив 'sparse' и плотный массив 'packed'. Хранит только целые числа.
 *
 * Компиляторы:
 *      - g++       v10.3.0
 *      - clang++   v12.0.1
 *      - MSVC      v16.11.5
 */
#pragma once

#include "vector.h"

namespace corsac
{
    template <typename T, typename Allocator = CORSAC_AllocatorType, bool is_unsigned = corsac::is_unsigned_v<T>>
    class sparse_set {};

    template <typename T, typename Allocator>
    class sparse_set<T, Allocator, true>
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
        explicit sparse_set(const this_type& x);
        sparse_set(const this_type& x, const allocator_type& allocator);
        sparse_set(const this_type& x, const allocator_type& packed_allocator, const allocator_type& sparse_allocator);
        explicit sparse_set(this_type&& x) noexcept;
        sparse_set(this_type&& x, const allocator_type& allocator);
        sparse_set(this_type&& x, const allocator_type& packed_allocator, const allocator_type& sparse_allocator);
        sparse_set(std::initializer_list<value_type> packed_list, std::initializer_list<value_type> sparse_list, const allocator_type& allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);
        sparse_set(std::initializer_list<value_type> packed_list, std::initializer_list<value_type> sparse_list, const allocator_type& packed_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR, const allocator_type& sparse_allocator = CORSAC_VECTOR_DEFAULT_ALLOCATOR);

        this_type& operator=(const this_type& x) noexcept;
        this_type& operator=(this_type&& x) noexcept;

        reference       operator[](size_type n);
        const_reference operator[](size_type n) const;

        reference       front();
        const_reference front() const;

        reference       back();
        const_reference back() const;

        [[nodiscard]] bool empty() const noexcept;
        void set_capacity(size_type n = npos);

        bool has(const value_type& value);
        bool has(value_type&& value);

        void insert(const value_type& value);
        void insert(value_type&& value);

        void erase(const value_type& value);
        void erase(value_type&& value);

        void clear() noexcept;
        void reset_lose_memory() noexcept;
    };

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set() noexcept
        :   packed(container()),
            sparse(container())
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(const allocator_type &allocator) noexcept
        :   packed(container(allocator)),
            sparse(container(allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator) noexcept
        :   packed(container(packed_allocator)),
            sparse(container(sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(sparse_set::size_type n, const allocator_type &allocator)
        :   packed(container(n, allocator)),
            sparse(container(n, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(
            sparse_set::size_type n, const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(n, packed_allocator)),
            sparse(container(n, sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(const sparse_set::this_type &x)
        :   packed(container(x.packed)),
            sparse(container(x.sparse))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(const sparse_set::this_type &x, const allocator_type &allocator)
        :   packed(container(x.packed, allocator)),
            sparse(container(x.sparse, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(const sparse_set::this_type &x,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(x.packed, packed_allocator)),
            sparse(container(x.sparse, sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(sparse_set::this_type &&x) noexcept
        :   packed(container(corsac::move(x.packed))),
            sparse(container(corsac::move(x.sparse)))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(sparse_set::this_type &&x,
            const allocator_type &allocator)
        :   packed(container(corsac::move(x.packed), allocator)),
            sparse(container(corsac::move(x.sparse), allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(sparse_set::this_type &&x,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(corsac::move(x.packed), packed_allocator)),
            sparse(container(corsac::move(x.sparse), sparse_allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(
            std::initializer_list<value_type> packed_list,
            std::initializer_list<value_type> sparse_list,
            const allocator_type &allocator)
        :   packed(container(packed_list, allocator)),
            sparse(container(sparse_list, allocator))
    {}

    template<typename T, typename Allocator>
    sparse_set<T, Allocator, true>::sparse_set(
            std::initializer_list<value_type> packed_list,
            std::initializer_list<value_type> sparse_list,
            const allocator_type &packed_allocator,
            const allocator_type &sparse_allocator)
        :   packed(container(packed_list, packed_allocator)),
            sparse(container(sparse_list, sparse_allocator))
    {}

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::this_type&
    sparse_set<T, Allocator, true>::operator=(const this_type& x) noexcept
    {
        packed.swap(x.packed);
        sparse.swap(x.sparse);
        return *this;
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::this_type&
    sparse_set<T, Allocator, true>::operator=(this_type&& x) noexcept
    {
        packed.swap(x.packed);
        sparse.swap(x.sparse);
        return *this;
    }

    template<typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::reference
    sparse_set<T, Allocator, true>::operator[](sparse_set::size_type n)
    {
        return packed[n];
    }

    template<typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::const_reference
    sparse_set<T, Allocator, true>::operator[](sparse_set::size_type n) const
    {
        return packed[n];
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::reference
    sparse_set<T, Allocator, true>::front()
    {
        return packed.front();
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::const_reference
    sparse_set<T, Allocator, true>::front() const
    {
        return packed.front();
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::reference
    sparse_set<T, Allocator, true>::back()
    {
        return packed.back();
    }

    template <typename T, typename Allocator>
    inline typename sparse_set<T, Allocator, true>::const_reference
    sparse_set<T, Allocator, true>::back() const
    {
        return packed.back();
    }

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator, true>::empty() const noexcept
    {
        return packed.empty();
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::set_capacity(size_type n)
    {
        packed.set_capacity(n);
        sparse.set_capacity(n);
    }

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator, true>::has(const value_type& value)
    {
        return sparse[value] < packed.size() && packed[sparse[value]] == value;
    }

    template<typename T, typename Allocator>
    inline bool sparse_set<T, Allocator, true>::has(value_type &&value)
    {
        return sparse[value] < packed.size() && packed[sparse[value]] == value;
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::insert(const value_type &value)
    {
        if (value >= sparse.size()) sparse.resize(value * 2);
        if (!has(value))
        {
            sparse[value] = static_cast<value_type>(packed.size());
            packed.push_back(value);
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::insert(value_type &&value)
    {
        if (value >= sparse.size()) sparse.resize(value * 2);
        if (!has(value))
        {
            sparse[value] = static_cast<value_type>(packed.size());
            packed.push_back(value);
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::erase(const value_type &value)
    {
        if (has(value))
        {
            packed[sparse[value]] = packed[packed.size() - 1];
            sparse[packed[packed.size() - 1]] = sparse[value];
            packed.pop_back();
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::erase(value_type &&value)
    {
        // TODO: Тут необходима оптимизация
        if (has(value))
        {
            packed[sparse[value]] = packed[packed.size() - 1];
            sparse[packed[packed.size() - 1]] = sparse[value];
            packed.pop_back();
        }
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::clear() noexcept
    {
        packed.clear();
        sparse.clear();
    }

    template<typename T, typename Allocator>
    inline void sparse_set<T, Allocator, true>::reset_lose_memory() noexcept
    {
        packed.reset_lose_memory();
    }
}

#endif //CORSAC_SPARSE_SET_H
