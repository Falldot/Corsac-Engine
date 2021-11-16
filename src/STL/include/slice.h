// Copyright (c) 2021 Corsac Team
// License: MIT License
// MIT License web page: https://opensource.org/licenses/MIT
#ifndef CORSAC_SLICE_H
#define CORSAC_SLICE_H

#pragma once

#include <iostream>
#include <utility>
#include <cstring>

#include <vector>

#include "allocator.h"

namespace corsac {

    template <typename T, typename Allocator = corsac_allocator_type>
    class slice
    {
        using allocator_type        = Allocator;
        using value_type            = T;
        using value_referens        = T&;
        using value_point           = T*;
        using iterator              = T*;
        using value_size            = size_t;

        using this_type = slice<T, Allocator>;

        const value_size init_size = 2;

        value_point         mpBegin;
        value_point         mpEnd;
        value_point         mpCapacity;
        allocator_type      mAllocator;

    public:
        explicit slice() noexcept;
        explicit slice(value_size value);
        explicit slice(const allocator_type& allocator) noexcept;
        explicit slice(value_size value, const allocator_type& allocator);

        slice(const this_type& value);
        explicit slice(this_type&& value);
        slice(const this_type& value, const allocator_type& allocator);
        slice(this_type&& value, const allocator_type& allocator);

        template <typename InputIterator>
        slice(InputIterator first, InputIterator last, const allocator_type& allocator);

        value_referens operator[](value_size value) noexcept;
        value_referens operator[](value_size value) const noexcept;

        ~slice();

        bool        empty()    const noexcept;
        value_size  size()     const noexcept;
        value_size  capacity() const noexcept;

        void resize(value_size value);
        void reserve(value_size value);
        void shrink();

        value_referens  last() noexcept;
        void            swap(this_type& value) noexcept;

        value_referens  append();
        void            append(const value_type& value);
        void            append(value_type&& value);

        template <class... Args>
        void append(Args&&... args);

        // value_referens  insert(int index);
        // void            insert(int index, const value_type& value);
        // void            insert(int index, value_type&& value);

        void  pop();
        //void            remove(int index);
        void            disorder_remove(int index);

        allocator_type& get_allocator() noexcept;
        void            set_allocator(const allocator_type& allocator) noexcept;

        void clear();

    protected:

        template<typename... Args>
        void Push(Args&&... args);

        void            Init(value_size value);
        value_point     Allocate(value_size value);
        void            Swap(this_type& value);
        void            Grow(value_size value);
        void            Free(value_point point, value_size value);
        value_size      GetNewCapacity(value_size currentCapacity);

        template <typename Integer>
        void Init(Integer first, Integer second);
    };

    ////////////////////////////////////////
    // CONSTRUCTOR
    ////////////////////////////////////////

    template <typename T, typename Allocator>
    inline slice<T, Allocator>::slice() noexcept
        :   mpBegin(NULL),
            mpEnd(NULL),
            mpCapacity(NULL),
            mAllocator(allocator_type())
    {
        Init(init_size);
    }

    template <typename T, typename Allocator>
    inline slice<T, Allocator>::slice(const allocator_type& allocator) noexcept
        :   mpBegin(NULL),
            mpEnd(NULL),
            mpCapacity(NULL),
            mAllocator(allocator)
    {
        Init(init_size);
    }

    template <typename T, typename Allocator>
    inline slice<T, Allocator>::slice(value_size value)
        :   mAllocator(allocator_type())
    {
        mpBegin     = Allocate(value);
        mpEnd       = mpBegin;
        mpCapacity  = mpBegin + value;
    }

    template <typename T, typename Allocator>
    inline slice<T, Allocator>::slice(value_size value, const allocator_type& allocator)
        :   mAllocator(allocator)
    {
        mpBegin     = Allocate(value);
        mpEnd       = mpBegin;
        mpCapacity  = mpBegin + value;
    }

    template<typename T, typename Allocator>
    slice<T, Allocator>::slice(const this_type &value)
        :   this_type(value.size(), value.mAllocator)
    {
        // TODO: Заменить std::memcpy на кастомный
        std::memcpy(mpBegin, value.mpBegin, sizeof(value_type)*value.size());
        mpEnd = mpBegin + value.size();
    }

    template<typename T, typename Allocator>
    slice<T, Allocator>::slice(slice::this_type &&value)
            :   this_type(std::move(value.mAllocator))
    // TODO: Заменить std::move на кастомный (см. eastl::move)
    {
        Swap(value);
    }

    template<typename T, typename Allocator>
    slice<T, Allocator>::slice(const slice::this_type &value, const allocator_type &allocator)
            :   this_type(value.size(), allocator)
    {
        // TODO: Заменить std::memcpy на кастомный
        std::memcpy(mpBegin, value.mpBegin, sizeof(value_type)*value.size());
        mpEnd = mpBegin + value.size();
    }

    template<typename T, typename Allocator>
    slice<T, Allocator>::slice(slice::this_type &&value, const allocator_type &allocator)
        :   mAllocator(allocator)
    {
        if (mAllocator == value.mAllocator)
        {
            Swap(value);
        }
        else
        {
            this_type temp(std::move(*this));
            // TODO: Заменить std::move на кастомный (см. eastl::move)
            temp.swap(value);
        }
    }

    template<typename T, typename Allocator>
    template<typename InputIterator>
    slice<T, Allocator>::slice(InputIterator first, InputIterator last, const allocator_type &allocator)
            :   mAllocator(allocator)
    {
        //Init(first, last, is_integral<InputIterator>());
    }

    ////////////////////////////////////////
    // DISTRUCTOR
    ////////////////////////////////////////

    template <typename T, typename Allocator>
    inline slice<T, Allocator>::~slice()
    {
        if(mpBegin != nullptr)
            corsac_free(mAllocator, mpBegin, static_cast<value_size>(mpCapacity - mpBegin) * sizeof(T));
    }

    ////////////////////////////////////////
    // GLOBAL OPERATORS OVERLOAD
    ////////////////////////////////////////

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_referens
    slice<T, Allocator>::operator[](value_size value) noexcept
    {
        return *(mpBegin + value);
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_referens
    slice<T, Allocator>::operator[](value_size value) const noexcept
    {
        return *(mpBegin + value);
    }

    ////////////////////////////////////////
    // METHODS
    ////////////////////////////////////////

    template <typename T, typename Allocator>
    inline bool slice<T, Allocator>::empty() const noexcept
    {
        return (mpBegin == mpEnd);
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_size
    slice<T, Allocator>::size() const noexcept
    {
        return static_cast<value_size>(mpEnd - mpBegin);
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_size
    slice<T, Allocator>::capacity() const noexcept
    {
        return static_cast<value_size>(mpCapacity - mpBegin);
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::resize(value_size value) {
        if (value > static_cast<value_size>(mpEnd - mpBegin))
        {
            Push(value - (static_cast<value_size>(mpEnd - mpBegin)));
        }
		else
		{
			corsac::destruct<value_type>(mpBegin + value, mpEnd);
			mpEnd = mpBegin + value;
		}
    }

    template<typename T, typename Allocator>
    void slice<T, Allocator>::reserve(slice::value_size value)
    {
        if (value > value_size(mpCapacity - mpBegin))
            Grow(value);
    }

    template<typename T, typename Allocator>
    void slice<T, Allocator>::shrink()
    {
        this_type temp = this_type(mpBegin, mpEnd, mAllocator);
        Swap(temp);
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_referens
    slice<T, Allocator>::last() noexcept
    {
            return *(mpEnd - 1);
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::swap(this_type& value) noexcept
    {
        Swap(value);
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_referens
    slice<T, Allocator>::append()
    {
        if (mpEnd < mpCapacity)
            ::new(static_cast<void*>(mpEnd++)) value_type();
        else
            Push(value_type());
        return *(mpEnd - 1);
    }

    template <typename T, typename Allocator>
    template <class... Args>
    inline void slice<T, Allocator>::append(Args&&... args)
    {
        if (mpEnd < mpCapacity)
        {
            ::new(static_cast<void*>(mpEnd)) value_type(std::forward<Args>(args)...);
            ++mpEnd;
        }
        else
            Push(std::forward<Args>(args)...);
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::append(value_type&& value)
    {
        if (mpEnd < mpCapacity)
            ::new(static_cast<void*>(mpEnd++)) value_type(value);
        else
            Push(std::move(value));
        // TODO: Заменить std::move на кастомный (см. eastl::move)
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::append(const value_type& value)
    {
        if (mpEnd < mpCapacity)
            ::new(static_cast<void*>(mpEnd++)) value_type(value);
        else
            Push(value);
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::pop()
    {
        --mpEnd;
        mpEnd->~value_type();
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::allocator_type&
    slice<T, Allocator>::get_allocator() noexcept
    {
        return mAllocator;
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::set_allocator(const allocator_type& allocator) noexcept
    {
        mAllocator = allocator;
    }

    template <typename T, typename Allocator>
	inline typename slice<T, Allocator>::value_size
	slice<T, Allocator>::GetNewCapacity(value_size currentCapacity)
	{
		return (currentCapacity > 0) ? (2 * currentCapacity) : 1;
	}

    template <typename T, typename Allocator>
    template<typename... Args>
    void slice<T, Allocator>::Push(Args&&... args)
    {
        // Получаем текущий размер
        const value_size nPrevSize = value_size(mpEnd - mpBegin);
        // Получаем новый размер
        const value_size nNewSize = GetNewCapacity(nPrevSize);
        // Выдеяем новый увеличенный массив
        value_point const pNewData = Allocate(nNewSize);

        // Копируем данные старого массива в новый
        std::memcpy(pNewData, mpBegin, sizeof(value_type)*nPrevSize);
        // TODO: Заменить std::memcpy на кастомный (см. на функцию ниже)
        // value_point pNewEnd = eastl::uninitialized_move_ptr_if_noexcept(mpBegin, mpEnd, pNewData);
        value_point pNewEnd = pNewData + nPrevSize;


        // Выделяем память под новые элементы
        ::new(static_cast<void*>(pNewEnd)) value_type(std::forward<Args>(args)...);
        // Передвигаем ссылку на последний элемент на 1 в право
        pNewEnd++;

        // Освобождаем память старого массива
        corsac::destruct<value_type>(mpBegin, mpEnd);
        Free(mpBegin, static_cast<value_size>(mpCapacity - mpBegin));
        // Переопределям мета данные
		mpBegin    = pNewData;
		mpEnd      = pNewEnd;
		mpCapacity = pNewData + nNewSize;
    }

    template <typename T, typename Allocator>
    inline typename slice<T, Allocator>::value_point
    slice<T, Allocator>::Allocate(value_size value)
    {
        return static_cast<value_point>(allocate_memory(mAllocator, value * sizeof(T), alignof(T), 0));
    }

    template<typename T, typename Allocator>
    inline void slice<T, Allocator>::Swap(this_type& value)
    {
        std::swap(mpBegin, value.mpBegin);
        std::swap(mpEnd, value.mpEnd);
        std::swap(mpCapacity, value.mpCapacity);
        // TODO: Заменить std::swap на кастомный (см. eastl::swap)
    }

    template<typename T, typename Allocator>
    inline void slice<T, Allocator>::Grow(value_size value)
    {
        const value_size nPrevSize = mpEnd - mpBegin;
        value_point const pNewData = Allocate(value);

        std::memcpy(pNewData, mpBegin, sizeof(value_type)*nPrevSize);
        value_point pNewEnd = pNewData + nPrevSize;

        corsac::destruct<value_type>(mpBegin, mpEnd);
        Free(mpBegin, static_cast<value_size>(mpCapacity - mpBegin));

        mpBegin    = pNewData;
        mpEnd      = pNewEnd;
        mpCapacity = pNewData + value;
    }

    template <typename T, typename Allocator>
    inline void slice<T, Allocator>::Free(value_point point, value_size value)
    {
        if(point != nullptr)
	        corsac_free(mAllocator, point, value * sizeof(T));
    }

    template<typename T, typename Allocator>
    inline void slice<T, Allocator>::Init(slice::value_size value)
    {
        mpBegin = Allocate(value);
        mpCapacity = mpBegin + value;
        mpEnd = mpBegin;
    }

    template<typename T, typename Allocator>
    template <typename Integer>
    inline void slice<T, Allocator>::Init(Integer first, Integer second)
    {
        mpBegin     = Allocate(static_cast<value_size>(first));
        mpCapacity  = mpBegin + first;
        mpEnd       = mpCapacity;
    }

    template <typename T, typename Allocator>
    inline void swap(slice<T, Allocator>& a, slice<T, Allocator>& b) noexcept
    {
        a.swap(b);
    }



}

#endif // Header include guard