//
// Created by Falldot on 18.12.2021.
//

#ifndef CORSAC_ECS_SPARSE_SET_H
#define CORSAC_ECS_SPARSE_SET_H

#include "Corsac/type_traits.h"
#include "Corsac/vector.h"
#include "Corsac/fixed_vector.h"
#include "Corsac/tuple_vector.h"
#include "Corsac/fixed_tuple_vector.h"

namespace corsac
{
    template<typename T, size_t nodeCount = 0, bool bEnableOverflow = true>
    class sparse_set
    {
        static_assert(corsac::is_unsigned_v<T>,
                      "sparse_set can only store integers numbers");

        using base_type = corsac::conditional_t<
                nodeCount == 0,
                corsac::vector<T>,
                corsac::fixed_vector<T, nodeCount, bEnableOverflow>
        >;

        // Base types
        using value_type                = T;
        using pointer                   = T*;
        using const_pointer             = const T*;
        using reference                 = T&;
        using const_reference           = const T&;

        // Iterators
        using iterator                  = T*;
        using const_iterator            = const T*;
        using reverse_iterator          = corsac::reverse_iterator<iterator>;
        using const_reverse_iterator    = corsac::reverse_iterator<const_iterator>;

    public:
        using size_type = typename base_type::size_type;

    protected:
        base_type packed;
        base_type sparse;

    public:
        sparse_set() noexcept;
        explicit sparse_set(size_type n) noexcept;

        iterator       begin() noexcept;
        const_iterator begin() const noexcept;

        iterator       end() noexcept;
        const_iterator end() const noexcept;

        reverse_iterator       rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;

        reverse_iterator       rend() noexcept;
        const_reverse_iterator rend() const noexcept;

        reference       front();
        const_reference front() const;

        reference       back();
        const_reference back() const;

        reference       at(size_type n);
        const_reference at(size_type n) const;

        reference       operator[](size_type n);
        const_reference operator[](size_type n) const;

        void resize(size_type n);
        void reserve(size_type n);

        void set_capacity(size_type n = base_type::npos);
        void shrink_to_fit();

        pointer       data() noexcept;
        const_pointer data() const noexcept;

        [[nodiscard]] bool      empty() const noexcept;
        [[nodiscard]] size_type size() const noexcept;
        [[nodiscard]] size_type capacity() const noexcept;

        [[nodiscard]] bool has(const_reference value) const;
        [[nodiscard]] bool has(reference& value) const;

        void add(const_reference value) noexcept;
        void add(reference& value) noexcept;

        void remove(const_reference value) noexcept;
        void remove(reference& value) noexcept;

        void clear() noexcept;

        virtual void reset_lose_memory() noexcept;

        // only fixed sparse_set
        size_type max_size() const;
        [[nodiscard]] bool full() const;
        [[nodiscard]] bool has_overflowed() const;
        [[nodiscard]] bool can_overflow() const;
    };

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    sparse_set<T, nodeCount, bEnableOverflow>::sparse_set() noexcept = default;

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    sparse_set<T, nodeCount, bEnableOverflow>::sparse_set(size_type n) noexcept
            : packed(n), sparse()
    {}

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::iterator
    sparse_set<T, nodeCount, bEnableOverflow>::begin() noexcept
    {
        return packed.mpBegin;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::begin() const noexcept
    {
        return packed.mpBegin;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::iterator
    sparse_set<T, nodeCount, bEnableOverflow>::end() noexcept
    {
        return packed.mpEnd;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::end() const noexcept
    {
        return packed.mpEnd;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reverse_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::rbegin() noexcept
    {
        return reverse_iterator(packed.mpEnd);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reverse_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::rbegin() const noexcept
    {
        return const_reverse_iterator(packed.mpEnd);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reverse_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::rend() noexcept
    {
        return reverse_iterator(packed.mpBegin);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reverse_iterator
    sparse_set<T, nodeCount, bEnableOverflow>::rend() const noexcept
    {
        return const_reverse_iterator(packed.mpBegin);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reference
    sparse_set<T, nodeCount, bEnableOverflow>::front()
    {
    #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
        // Мы не разрешаем пользователю ссылаться на пустой контейнер.
        if (CORSAC_UNLIKELY((packed.mpBegin == nullptr) || (packed.mpEnd <= packed.mpBegin)))
            CORSAC_FAIL_MSG("sparse_set::front -- empty sparse_set");
    #else
        // Мы позволяем пользователю ссылаться на пустой контейнер.
    #endif
        return packed.front();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reference
    sparse_set<T, nodeCount, bEnableOverflow>::front() const
    {
    #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
        // Мы не разрешаем пользователю ссылаться на пустой контейнер.
        if (CORSAC_UNLIKELY((packed.mpBegin == nullptr) || (packed.mpEnd <= packed.mpBegin)))
            CORSAC_FAIL_MSG("sparse_set::front -- empty sparse_set");
    #else
        // Мы позволяем пользователю ссылаться на пустой контейнер.
    #endif
        return packed.front();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reference
    sparse_set<T, nodeCount, bEnableOverflow>::back()
    {
    #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
        // Мы не разрешаем пользователю ссылаться на пустой контейнер.
        if (CORSAC_UNLIKELY((packed.mpBegin == nullptr) || (packed.mpEnd <= packed.mpBegin)))
            CORSAC_FAIL_MSG("sparse_set::back -- empty sparse_set");
    #else
        // Мы позволяем пользователю ссылаться на пустой контейнер.
    #endif
        return packed.back();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reference
    sparse_set<T, nodeCount, bEnableOverflow>::back() const
    {
    #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
        // Мы не разрешаем пользователю ссылаться на пустой контейнер.
        if (CORSAC_UNLIKELY((packed.mpBegin == nullptr) || (packed.mpEnd <= packed.mpBegin)))
            CORSAC_FAIL_MSG("sparse_set::back -- empty sparse_set");
    #else
        // Мы позволяем пользователю ссылаться на пустой контейнер.
    #endif
        return packed.back();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reference
    sparse_set<T, nodeCount, bEnableOverflow>::at(size_type n)
    {
        return n < packed.size() ? packed[n] : nullptr;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reference
    sparse_set<T, nodeCount, bEnableOverflow>::at(size_type n) const
    {
        return n < packed.size() ? packed[n] : nullptr;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::reference
    sparse_set<T, nodeCount, bEnableOverflow>::operator[](size_type n)
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(n < packed.size()))
            throw std::out_of_range("sparse_set::[] -- out of range");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(n < packed.size()))
            CORSAC_FAIL_MSG("sparse_set::[] -- out of range");
    #endif
        return packed[n];
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_reference
    sparse_set<T, nodeCount, bEnableOverflow>::operator[](size_type n) const
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(n < packed.size()))
            throw std::out_of_range("sparse_set::[] -- out of range");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(n < packed.size()))
            CORSAC_FAIL_MSG("sparse_set::[] -- out of range");
    #endif
        return packed[n];
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::resize(size_type n)
    {
        packed.resize(n);
        sparse.resize(n);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::reserve(size_type n)
    {
        packed.reserve(n);
        sparse.reserve(n);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::set_capacity(size_type n)
    {
        packed.set_capacity(n);
        sparse.set_capacity(n);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::shrink_to_fit()
    {
        packed.shrink_to_fit();
        sparse.shrink_to_fit();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::pointer
    sparse_set<T, nodeCount, bEnableOverflow>::data() noexcept
    {
        return packed.mpBegin;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::const_pointer
    sparse_set<T, nodeCount, bEnableOverflow>::data() const noexcept
    {
        return packed.mpBegin;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::empty() const noexcept
    {
        return packed.empty();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::size_type
    sparse_set<T, nodeCount, bEnableOverflow>::size() const noexcept
    {
        return packed.size();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::size_type
    sparse_set<T, nodeCount, bEnableOverflow>::capacity() const noexcept
    {
        return packed.capacity();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::has(const_reference value) const
    {
        return sparse[value] < packed.size() && packed[sparse[value]] == value;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::has(reference& value) const
    {
        return sparse[value] < packed.size() && packed[sparse[value]] == value;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::add(const_reference value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(value);
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::add(reference& value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::remove(const_reference value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
        }
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::remove(reference& value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
        }
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::clear() noexcept
    {
        packed.clear();
        sparse.clear();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline void sparse_set<T, nodeCount, bEnableOverflow>::reset_lose_memory() noexcept
    {
        packed.reset_lose_memory();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline typename sparse_set<T, nodeCount, bEnableOverflow>::size_type
    sparse_set<T, nodeCount, bEnableOverflow>::max_size() const
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
                throw std::out_of_range("sparse_set::[] -- not fixed");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            CORSAC_FAIL_MSG("sparse_set::[] -- not fixed");
    #endif
        return packed.kMaxSize;
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::full() const
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            throw std::out_of_range("sparse_set::[] -- not fixed");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            CORSAC_FAIL_MSG("sparse_set::[] -- not fixed");
    #endif
        return packed.full();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::has_overflowed() const
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            throw std::out_of_range("sparse_set::[] -- not fixed");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            CORSAC_FAIL_MSG("sparse_set::[] -- not fixed");
    #endif
        return packed.has_overflowed();
    }

    template <typename T, size_t nodeCount, bool bEnableOverflow>
    inline bool sparse_set<T, nodeCount, bEnableOverflow>::can_overflow() const
    {
    #if CORSAC_EXCEPTIONS_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            throw std::out_of_range("sparse_set::[] -- not fixed");
    #elif CORSAC_ASSERT_ENABLED
        if(CORSAC_UNLIKELY(nodeCount == 0))
            CORSAC_FAIL_MSG("sparse_set::[] -- not fixed");
    #endif
        return bEnableOverflow;
    }
}

#endif //CORSAC_ECS_SPARSE_SET_H
