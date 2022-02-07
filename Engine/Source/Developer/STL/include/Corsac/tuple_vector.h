/**
 * corsac::STL
 *
 * chrono.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_TUPLE_VECTOR_H
#define CORSAC_STL_TUPLE_VECTOR_H

#include "Corsac/compressed_pair.h"
#include "Corsac/STL/config.h"
#include "Corsac/iterator.h"
#include "Corsac/memory.h"
#include "Corsac/tuple.h"
#include "Corsac/utility.h"

CORSAC_DISABLE_VC_WARNING(4244) // warning C4244: 'conversion from '___' to '___', possible loss of data
CORSAC_DISABLE_VC_WARNING(4623) // warning C4623: default constructor was implicitly defined as deleted
CORSAC_DISABLE_VC_WARNING(4625) // warning C4625: copy constructor was implicitly defined as deleted
CORSAC_DISABLE_VC_WARNING(4510) // warning C4510: default constructor could not be generated

namespace corsac
{
    // CORSAC_TUPLE_VECTOR_DEFAULT_NAME
    #ifndef CORSAC_TUPLE_VECTOR_DEFAULT_NAME
        #define CORSAC_TUPLE_VECTOR_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " tuple-vector"
    #endif


    // CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR
    #ifndef CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR
        #define CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR allocator_type(CORSAC_TUPLE_VECTOR_DEFAULT_NAME)
    #endif

    namespace TupleVecInternal
    {
        template <size_t I, typename... Ts>
        struct tuplevec_element;

        template <size_t I, typename... Ts>
        using tuplevec_element_t = typename tuplevec_element<I, Ts...>::type;

        template <typename... Ts>
        struct TupleTypes {};

        template <typename Allocator, typename Indices, typename... Ts>
        class TupleVecImpl;

        template <typename... Ts>
        struct TupleRecurser;

        template <size_t I, typename... Ts>
        struct TupleIndexRecurser;

        template <size_t I, typename T>
        struct TupleVecLeaf;

        template <typename Indices, typename... Ts>
        struct TupleVecIter;

        template <size_t I>
        struct tuplevec_element<I>
        {
            static_assert(I != I, "tuplevec_element index out of range");
        };

        template <typename T, typename... Ts>
        struct tuplevec_element<0, T, Ts...>
        {
            tuplevec_element() = delete; // tuplevec_element следует использовать только для помощи во время компиляции и никогда не создавать экземпляры
            using type = T;
        };

        template <size_t I, typename T, typename... Ts>
        struct tuplevec_element<I, T, Ts...>
        {
            using type = tuplevec_element_t<I - 1, Ts...>;
        };

        // попытаться изолировать индекс по типу
        template <typename T, typename TupleVector>
        struct tuplevec_index
        {};

        template <typename T>
        struct tuplevec_index<T, TupleTypes<>>
        {
            typedef void DuplicateTypeCheck;
            tuplevec_index() = delete; // tuplevec_index следует использовать только для помощи во время компиляции и никогда не создавать экземпляры
            static const size_t index = 0;
        };

        template <typename T, typename... TsRest>
        struct tuplevec_index<T, TupleTypes<T, TsRest...>>
        {
            using DuplicateTypeCheck = int;
            static_assert(is_void<typename tuplevec_index<T, TupleTypes<TsRest...>>::DuplicateTypeCheck>::value, "duplicate type T in tuple_vector::get<T>(); unique types must be provided in declaration, or only use get<size_t>()");

            static const size_t index = 0;
        };

        template <typename T, typename Ts, typename... TsRest>
        struct tuplevec_index<T, TupleTypes<Ts, TsRest...>>
        {
            using DuplicateTypeCheck = typename tuplevec_index<T, TupleTypes<TsRest...>>::DuplicateTypeCheck;
            static const size_t index = tuplevec_index<T, TupleTypes<TsRest...>>::index + 1;
        };

        template <typename Allocator, typename T, typename Indices, typename... Ts>
        struct tuplevec_index<T, TupleVecImpl<Allocator, Indices, Ts...>> : public tuplevec_index<T, TupleTypes<Ts...>>
        {};

        // helper для расчета макета распределения для кортежа типов (особенно для учета выравнивания)
        template <>
        struct TupleRecurser<>
        {
            using size_type = size_t;

            // Этот класс никогда не должен создаваться. Это просто помощник для работы со статическими функциями,
            // когда анонимные функции не работают и предоставляют некоторые другие утилиты
            TupleRecurser() = delete;

            static constexpr size_type GetTotalAlignment()
            {
                return 0;
            }

            static constexpr size_type GetTotalAllocationSize(size_type capacity, size_type offset)
            {
                CORSAC_UNUSED(capacity);
                return offset;
            }

            template<typename Allocator, size_type I, typename Indices, typename... VecTypes>
            static pair<void*, size_type> DoAllocate(TupleVecImpl<Allocator, Indices, VecTypes...> &vec, void** ppNewLeaf, size_type capacity, size_type offset)
            {
                CORSAC_UNUSED(ppNewLeaf);

                // Если n равно нулю, мы не выделяем память и просто возвращаем NULL.
                // Это нормально, поскольку наш ctor по умолчанию инициализируется указателями NULL.
                size_type alignment = TupleRecurser<VecTypes...>::GetTotalAlignment();
                void* ptr = capacity ? allocate_memory(vec.get_allocator(), offset, alignment, 0) : nullptr;

                #if CORSAC_ASSERT_ENABLED
                    if (CORSAC_UNLIKELY((size_t)ptr & (alignment - 1)) != 0)
                    {
                        CORSAC_FAIL_MSG("tuple_vector::DoAllocate -- memory not alignment at requested alignment");
                    }
                #endif

                return make_pair(ptr, offset);
            }

            template<typename TupleVecImplType, size_type I>
            static void SetNewData(TupleVecImplType &vec, void* pData, size_type capacity, size_type offset)
            {
                CORSAC_UNUSED(vec);
                CORSAC_UNUSED(pData);
                CORSAC_UNUSED(capacity);
                CORSAC_UNUSED(offset);
            }
        };

        template <typename T, typename... Ts>
        struct TupleRecurser<T, Ts...> : TupleRecurser<Ts...>
        {
            using size_type = size_t;

            static constexpr size_type GetTotalAlignment()
            {
                return max(alignof(T), TupleRecurser<Ts...>::GetTotalAlignment());
            }

            static constexpr size_type GetTotalAllocationSize(size_type capacity, size_type offset)
            {
                return TupleRecurser<Ts...>::GetTotalAllocationSize(capacity, CalculateAllocationSize(offset, capacity));
            }

            template<typename Allocator, size_type I, typename Indices, typename... VecTypes>
            static pair<void*, size_type> DoAllocate(TupleVecImpl<Allocator, Indices, VecTypes...> &vec, void** ppNewLeaf, size_type capacity, size_type offset)
            {
                size_type allocationOffset = CalculatAllocationOffset(offset);
                size_type allocationSize = CalculateAllocationSize(offset, capacity);
                pair<void*, size_type> allocation = TupleRecurser<Ts...>::template DoAllocate<Allocator, I + 1, Indices, VecTypes...>(
                        vec, ppNewLeaf, capacity, allocationSize);
                ppNewLeaf[I] = (void*)((uintptr_t)(allocation.first) + allocationOffset);
                return allocation;
            }

            template<typename TupleVecImplType, size_type I>
            static void SetNewData(TupleVecImplType &vec, void* pData, size_type capacity, size_type offset)
            {
                size_type allocationOffset = CalculatAllocationOffset(offset);
                size_type allocationSize = CalculateAllocationSize(offset, capacity);
                vec.TupleVecLeaf<I, T>::mpData = (T*)((uintptr_t)pData + allocationOffset);
                TupleRecurser<Ts...>::template SetNewData<TupleVecImplType, I + 1>(vec, pData, capacity, allocationSize);
            }

        private:
            static constexpr size_type CalculateAllocationSize(size_type offset, size_type capacity)
            {
                return CalculatAllocationOffset(offset) + sizeof(T) * capacity;
            }

            static constexpr size_type CalculatAllocationOffset(size_type offset) { return (offset + alignof(T) - 1) & (~alignof(T) + 1); }
        };

        template <size_t I, typename T>
        struct TupleVecLeaf
        {
            using size_type = size_t;

            void DoUninitializedMoveAndDestruct(const size_type begin, const size_type end, T* pDest)
            {
                T* pBegin = mpData + begin;
                T* pEnd = mpData + end;
                corsac::uninitialized_move_ptr_if_noexcept(pBegin, pEnd, pDest);
                corsac::destruct(pBegin, pEnd);
            }

            void DoInsertAndFill(size_type pos, size_type n, size_type numElements, const T& arg)
            {
                T* pDest = mpData + pos;
                T* pDataEnd = mpData + numElements;
                const T temp = arg;
                const size_type nExtra = (numElements - pos);
                if (n < nExtra) // Если вставленные значения полностью находятся в инициализированной памяти (т.е. перед mpEnd) ...
                {
                    corsac::uninitialized_move_ptr(pDataEnd - n, pDataEnd, pDataEnd);
                    corsac::move_backward(pDest, pDataEnd - n, pDataEnd); // Нам нужен move_backward из-за возможных проблем с перекрытием.
                    corsac::fill(pDest, pDest + n, temp);
                }
                else
                {
                    corsac::uninitialized_fill_n_ptr(pDataEnd, n - nExtra, temp);
                    corsac::uninitialized_move_ptr(pDest, pDataEnd, pDataEnd + n - nExtra);
                    corsac::fill(pDest, pDataEnd, temp);
                }
            }

            void DoInsertRange(T* pSrcBegin, T* pSrcEnd, T* pDestBegin, size_type numDataElements)
            {
                size_type pos = pDestBegin - mpData;
                size_type n = pSrcEnd - pSrcBegin;
                T* pDataEnd = mpData + numDataElements;
                const size_type nExtra = numDataElements - pos;
                if (n < nExtra) // Если вставленные значения полностью находятся в инициализированной памяти (т.е. перед mpEnd) ...
                {
                    corsac::uninitialized_move_ptr(pDataEnd - n, pDataEnd, pDataEnd);
                    corsac::move_backward(pDestBegin, pDataEnd - n, pDataEnd); // Нам нужен move_backward из-за возможных проблем с перекрытием.
                    corsac::copy(pSrcBegin, pSrcEnd, pDestBegin);
                }
                else
                {
                    corsac::uninitialized_copy(pSrcEnd - (n - nExtra), pSrcEnd, pDataEnd);
                    corsac::uninitialized_move_ptr(pDestBegin, pDataEnd, pDataEnd + n - nExtra);
                    corsac::copy(pSrcBegin, pSrcEnd - (n - nExtra), pDestBegin);
                }
            }

            void DoInsertValue(size_type pos, size_type numElements, T&& arg)
            {
                T* pDest = mpData + pos;
                T* pDataEnd = mpData + numElements;

                corsac::uninitialized_move_ptr(pDataEnd - 1, pDataEnd, pDataEnd);
                corsac::move_backward(pDest, pDataEnd - 1, pDataEnd); // Нам нужен move_backward из-за возможных проблем с перекрытием.
                corsac::destruct(pDest);
                ::new (pDest) T(corsac::forward<T>(arg));
            }

            T* mpData = nullptr;
        };

        // swallow позволяет расширение пакета параметров аргументов в качестве средства расширения операций,
        // выполняемых, если для раскрытия операции используется функция void, она должна быть заключена в (..., 0),
        // чтобы компилятор считал, что у него есть параметр для передачи в функцию
        template <typename... Ts>
        void swallow(Ts&&...)
        {}

        inline bool variadicAnd(bool cond) { return cond; }

        inline bool variadicAnd(bool cond, bool conds...) { return cond && variadicAnd(conds); }

        // Helper структура для проверки строгой совместимости между двумя итераторами,
        // в то же время позволяющая преобразование между TupleVecImpl<Ts...>::iterator и TupleVecImpl<Ts...>::const_iterator.
        template <bool IsSameSize, typename From, typename To>
        struct TupleVecIterCompatibleImpl : public false_type {};

        template<>
        struct TupleVecIterCompatibleImpl<true, TupleTypes<>, TupleTypes<>> : public true_type {};

        template <typename From, typename... FromRest, typename To, typename... ToRest>
        struct TupleVecIterCompatibleImpl<true, TupleTypes<From, FromRest...>, TupleTypes<To, ToRest...>> : public integral_constant<bool,
                TupleVecIterCompatibleImpl<true, TupleTypes<FromRest...>, TupleTypes<ToRest...>>::value &&
                is_same<typename remove_const<From>::type, typename remove_const<To>::type>::value >
        {};

        template <typename From, typename To>
        struct TupleVecIterCompatible;

        template<typename... Us, typename... Ts>
        struct TupleVecIterCompatible<TupleTypes<Us...>, TupleTypes<Ts...>> :
                public TupleVecIterCompatibleImpl<sizeof...(Us) == sizeof...(Ts), TupleTypes<Us...>, TupleTypes<Ts...>>
        {};

        // Iterator работает, сохраняя постоянный индекс внутри и разрешая кортеж указателей на
        // различные части исходного tupleVec при разыменовании. Хотя разрешение кортежа является
        // ненулевой операцией, оно последовательно генерирует лучший код, чем альтернатива хранения
        // - и гармоничного обновления при каждой модификации - полного кортежа указателей на данные tupleVec.
        template <size_t... Indices, typename... Ts>
        struct TupleVecIter<index_sequence<Indices...>, Ts...>
        : public iterator<random_access_iterator_tag, tuple<Ts...>, size_t, tuple<Ts*...>, tuple<Ts&...>>
        {
        private:
            using this_type = TupleVecIter<index_sequence<Indices...>, Ts...>;
            using size_type = size_t;

            using iter_type = iterator<random_access_iterator_tag, tuple<Ts...>, size_t, tuple<Ts*...>, tuple<Ts&...>>;

            template<typename U, typename... Us>
            friend struct TupleVecIter;

            template<typename U, typename V, typename... Us>
            friend class TupleVecImpl;

            template<typename U>
            friend class move_iterator;
        public:
            using iterator_category = typename iter_type::iterator_category;
            using value_type        = typename iter_type::value_type;
            using difference_type   = typename iter_type::difference_type;
            using pointer           = typename iter_type::pointer;
            using reference         = typename iter_type::reference;

            TupleVecIter() = default;

            template<typename VecImplType>
            TupleVecIter(VecImplType* tupleVec, size_type index)
            : mIndex(index), mpData{(void*)tupleVec->TupleVecLeaf<Indices, Ts>::mpData...}
            {}

            template <typename OtherIndicesType, typename... Us,
                    typename = typename enable_if<TupleVecIterCompatible<TupleTypes<Us...>, TupleTypes<Ts...>>::value, bool>::type>
            TupleVecIter(const TupleVecIter<OtherIndicesType, Us...>& other)
                    : mIndex(other.mIndex)
                    , mpData{other.mpData[Indices]...}
            {}

            bool operator==(const TupleVecIter& other) const { return mIndex == other.mIndex && mpData[0] == other.mpData[0]; }
            bool operator!=(const TupleVecIter& other) const { return mIndex != other.mIndex || mpData[0] != other.mpData[0]; }
            reference operator*() const { return MakeReference(); }

            this_type& operator++() { ++mIndex; return *this; }
            this_type operator++(int)
            {
                this_type temp = *this;
                ++mIndex;
                return temp;
            }

            this_type& operator--() { --mIndex; return *this; }
            this_type operator--(int)
            {
                this_type temp = *this;
                --mIndex;
                return temp;
            }

            this_type& operator+=(difference_type n) { mIndex += n; return *this; }
            this_type operator+(difference_type n) const
            {
                this_type temp = *this;
                return temp += n;
            }

            friend this_type operator+(difference_type n, const this_type& rhs)
            {
                this_type temp = rhs;
                return temp += n;
            }

            this_type& operator-=(difference_type n) { mIndex -= n; return *this; }
            this_type operator-(difference_type n) const
            {
                this_type temp = *this;
                return temp -= n;
            }
            friend this_type operator-(difference_type n, const this_type& rhs)
            {
                this_type temp = rhs;
                return temp -= n;
            }

            difference_type operator-(const this_type& rhs) const { return mIndex - rhs.mIndex; }
            bool operator<(const this_type& rhs) const { return mIndex < rhs.mIndex; }
            bool operator>(const this_type& rhs) const { return mIndex > rhs.mIndex; }
            bool operator>=(const this_type& rhs) const { return mIndex >= rhs.mIndex; }
            bool operator<=(const this_type& rhs) const { return mIndex <= rhs.mIndex; }

            reference operator[](const size_type n) const
            {
                return *(*this + n);
            }

            private:

            value_type MakeValue() const
            {
                return value_type(((Ts*)mpData[Indices])[mIndex]...);
            }

            reference MakeReference() const
            {
                return reference(((Ts*)mpData[Indices])[mIndex]...);
            }

            pointer MakePointer() const
            {
                return pointer(&((Ts*)mpData[Indices])[mIndex]...);
            }

            size_type mIndex = 0;
            const void* mpData[sizeof...(Ts)];
        };

    // TupleVecImpl
    template <typename Allocator, size_t... Indices, typename... Ts>
    class TupleVecImpl<Allocator, index_sequence<Indices...>, Ts...> : public TupleVecLeaf<Indices, Ts>...
    {
        using allocator_type        = Allocator;
        using index_sequence_type   = index_sequence<Indices...>;
        using this_type             = TupleVecImpl<Allocator, index_sequence_type, Ts...>;
        using const_this_type       = TupleVecImpl<Allocator, index_sequence_type, const Ts...>;

    public:
        using iterator                  = TupleVecInternal::TupleVecIter<index_sequence_type, Ts...>;
        using const_iterator            = TupleVecInternal::TupleVecIter<index_sequence_type, const Ts...>;
        using reverse_iterator          = corsac::reverse_iterator<iterator>;
        using const_reverse_iterator    = corsac::reverse_iterator<const_iterator>;
        using size_type                 = size_t;
        using value_tuple               = corsac::tuple<Ts...>;
        using reference_tuple           = corsac::tuple<Ts&...>;
        using const_reference_tuple     = corsac::tuple<const Ts&...>;
        using ptr_tuple                 = corsac::tuple<Ts*...>;
        using const_ptr_tuple           = corsac::tuple<const Ts*...>;
        using rvalue_tuple              = corsac::tuple<Ts&&...>;

        TupleVecImpl(): mDataSizeAndAllocator(0, CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        {}

        explicit TupleVecImpl(const allocator_type& allocator): mDataSizeAndAllocator(0, allocator)
        {}

        TupleVecImpl(this_type&& x) noexcept
        : mDataSizeAndAllocator(0, corsac::move(x.get_allocator()))
        {
            swap(x);
        }

        TupleVecImpl(this_type&& x, const Allocator& allocator)
        : mDataSizeAndAllocator(0, allocator)
        {
            if (get_allocator() == x.get_allocator()) // Если allocators эквивалентны, тогда мы можем безопасно поменять местами по очереди
            {
                swap(x);
            }
            else
            {
                this_type temp(corsac::move(*this));
                temp.swap(x);
            }
        }

        TupleVecImpl(const this_type& x)
                : mDataSizeAndAllocator(0, x.get_allocator())
        {
            DoInitFromIterator(x.begin(), x.end());
        }

        template<typename OtherAllocator>
        TupleVecImpl(const TupleVecImpl<OtherAllocator, index_sequence_type, Ts...>& x, const Allocator& allocator)
                : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFromIterator(x.begin(), x.end());
        }

        template<typename MoveIterBase>
        TupleVecImpl(move_iterator<MoveIterBase> begin, move_iterator<MoveIterBase> end, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFromIterator(begin, end);
        }

        TupleVecImpl(const_iterator begin, const_iterator end, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator )
        {
            DoInitFromIterator(begin, end);
        }

        TupleVecImpl(size_type n, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitDefaultFill(n);
        }

        TupleVecImpl(size_type n, const Ts&... args)
        : mDataSizeAndAllocator(0, CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        {
            DoInitFillArgs(n, args...);
        }

        TupleVecImpl(size_type n, const Ts&... args, const allocator_type& allocator)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFillArgs(n, args...);
        }

        TupleVecImpl(size_type n, const_reference_tuple tup, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFillTuple(n, tup);
        }

        TupleVecImpl(const value_tuple* first, const value_tuple* last, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFromTupleArray(first, last);
        }

        TupleVecImpl(std::initializer_list<value_tuple> iList, const allocator_type& allocator = CORSAC_TUPLE_VECTOR_DEFAULT_ALLOCATOR)
        : mDataSizeAndAllocator(0, allocator)
        {
            DoInitFromTupleArray(iList.begin(), iList.end());
        }

    protected:
        // ctor, чтобы предоставить заранее выделенное поле данных, которым будет владеть контейнер, особенно для fixed_tuple_vector
        TupleVecImpl(const allocator_type& allocator, void* pData, size_type capacity, size_type dataSize)
                : mpData(pData), mNumCapacity(capacity), mDataSizeAndAllocator(dataSize, allocator)
        {
            TupleRecurser<Ts...>::template SetNewData<this_type, 0>(*this, mpData, mNumCapacity, 0);
        }

        public:
        ~TupleVecImpl()
        {
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData, TupleVecLeaf<Indices, Ts>::mpData + mNumElements), 0)...);
            if (mpData)
                CORSAC_Free(get_allocator(), mpData, internalDataSize());
        }

        void assign(size_type n, const Ts&... args)
        {
            if (n > mNumCapacity)
            {
                this_type temp(n, args..., get_allocator()); // У нас нет другого выбора, кроме как перераспределить новую память.
                swap(temp);
            }
            else if (n > mNumElements) // Если n > mNumElements ...
            {
                size_type oldNumElements = mNumElements;
                swallow((corsac::fill(TupleVecLeaf<Indices, Ts>::mpData, TupleVecLeaf<Indices, Ts>::mpData + oldNumElements, args), 0)...);
                swallow((corsac::uninitialized_fill_ptr(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                                       TupleVecLeaf<Indices, Ts>::mpData + n, args), 0)...);
                mNumElements = n;
            }
            else // иначе 0 <= n <= mNumElements
            {
                swallow((corsac::fill(TupleVecLeaf<Indices, Ts>::mpData, TupleVecLeaf<Indices, Ts>::mpData + n, args), 0)...);
                erase(begin() + n, end());
            }
        }

        void assign(const_iterator first, const_iterator last)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(!validate_iterator_pair(first, last)))
                        CORSAC_FAIL_MSG("tuple_vector::assign -- invalid iterator pair");
            #endif
            size_type newNumElements = last - first;
            if (newNumElements > mNumCapacity)
            {
                this_type temp(first, last, get_allocator());
                swap(temp);
            }
            else
            {
                const void* ppOtherData[sizeof...(Ts)] = {first.mpData[Indices]...};
                size_type firstIdx = first.mIndex;
                size_type lastIdx = last.mIndex;
                if (newNumElements > mNumElements) // Если n > mNumElements ...
                {
                    size_type oldNumElements = mNumElements;
                    swallow((corsac::copy((Ts*)(ppOtherData[Indices]) + firstIdx,
                                         (Ts*)(ppOtherData[Indices]) + firstIdx + oldNumElements,
                                         TupleVecLeaf<Indices, Ts>::mpData), 0)...);
                    swallow((corsac::uninitialized_copy_ptr((Ts*)(ppOtherData[Indices]) + firstIdx + oldNumElements,
                                                           (Ts*)(ppOtherData[Indices]) + lastIdx,
                                                           TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
                    mNumElements = newNumElements;
                }
                else // иначе 0 <= n <= mNumElements
                {
                    swallow((corsac::copy((Ts*)(ppOtherData[Indices]) + firstIdx, (Ts*)(ppOtherData[Indices]) + lastIdx,
                             TupleVecLeaf<Indices, Ts>::mpData), 0)...);
                    erase(begin() + newNumElements, end());
                }
            }
        }

        void assign(const value_tuple* first, const value_tuple* last)
        {
            #if CORSAC_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(first > last || first == nullptr || last == nullptr))
                        CORSAC_FAIL_MSG("tuple_vector::assign from tuple array -- invalid ptrs");
            #endif
            size_type newNumElements = last - first;
            if (newNumElements > mNumCapacity)
            {
                this_type temp(first, last, get_allocator());
                swap(temp);
            }
            else
            {
                if (newNumElements > mNumElements) // Если n > mNumElements ...
                {
                    size_type oldNumElements = mNumElements;

                    DoCopyFromTupleArray(begin(), begin() + oldNumElements, first);
                    DoUninitializedCopyFromTupleArray(begin() + oldNumElements, begin() + newNumElements, first + oldNumElements);
                    mNumElements = newNumElements;
                }
                else // иначе 0 <= n <= mNumElements
                {
                    DoCopyFromTupleArray(begin(), begin() + newNumElements, first);
                    erase(begin() + newNumElements, end());
                }
            }
        }

        reference_tuple push_back()
        {
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + 1;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            DoGrow(oldNumElements, oldNumCapacity, newNumElements);
            swallow(::new(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements) Ts()...);
            return back();
        }

        void push_back(const Ts&... args)
        {
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + 1;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            DoGrow(oldNumElements, oldNumCapacity, newNumElements);
            swallow(::new(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements) Ts(args)...);
        }

        void push_back_uninitialized()
        {
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + 1;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            DoGrow(oldNumElements, oldNumCapacity, newNumElements);
        }

        reference_tuple emplace_back(Ts&&... args)
        {
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + 1;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            DoGrow(oldNumElements, oldNumCapacity, newNumElements);
            swallow(::new(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements) Ts(corsac::forward<Ts>(args))...);
            return back();
        }

        template<typename Function>
        void each(Function function)
        {
            for (int i = 0; i < size(); ++i)
                function(i, *(TupleVecLeaf<Indices, Ts>::mpData + i)...);
        }

        iterator emplace(const_iterator pos, Ts&&... args)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                        CORSAC_FAIL_MSG("tuple_vector::emplace -- invalid iterator");
            #endif
            size_type firstIdx = pos - cbegin();
            size_type oldNumElements = mNumElements;
            size_type newNumElements = mNumElements + 1;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            if (newNumElements > oldNumCapacity || firstIdx != oldNumElements)
            {
                if (newNumElements > oldNumCapacity)
                {
                    const size_type newCapacity = corsac::max(GetNewCapacity(oldNumCapacity), newNumElements);

                    void* ppNewLeaf[sizeof...(Ts)];
                    pair<void*, size_type> allocation =	TupleRecurser<Ts...>::template DoAllocate<allocator_type, 0, index_sequence_type, Ts...>(
                            *this, ppNewLeaf, newCapacity, 0);

                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            0, firstIdx, (Ts*)ppNewLeaf[Indices]), 0)...);
                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            firstIdx, oldNumElements, (Ts*)ppNewLeaf[Indices] + firstIdx + 1), 0)...);
                    swallow(::new ((Ts*)ppNewLeaf[Indices] + firstIdx) Ts(corsac::forward<Ts>(args))...);
                    swallow(TupleVecLeaf<Indices, Ts>::mpData = (Ts*)ppNewLeaf[Indices]...);

                    CORSAC_Free(get_allocator(), mpData, internalDataSize());
                    mpData = allocation.first;
                    mNumCapacity = newCapacity;
                    internalDataSize() = allocation.second;
                }
                else
                {
                    swallow((TupleVecLeaf<Indices, Ts>::DoInsertValue(firstIdx, oldNumElements, corsac::forward<Ts>(args)), 0)...);
                }
            }
            else
            {
                swallow(::new (TupleVecLeaf<Indices, Ts>::mpData + oldNumElements) Ts(corsac::forward<Ts>(args))...);
            }
            return begin() + firstIdx;
        }

        iterator insert(const_iterator pos, size_type n, const Ts&... args)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                        {CORSAC_FAIL_MSG("tuple_vector::insert -- invalid iterator")}
            #endif
            size_type firstIdx = pos - cbegin();
            size_type lastIdx = firstIdx + n;
            size_type oldNumElements = mNumElements;
            size_type newNumElements = mNumElements + n;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            if (newNumElements > oldNumCapacity || firstIdx != oldNumElements)
            {
                if (newNumElements > oldNumCapacity)
                {
                    const size_type newCapacity = corsac::max(GetNewCapacity(oldNumCapacity), newNumElements);

                    void* ppNewLeaf[sizeof...(Ts)];
                    pair<void*, size_type> allocation = TupleRecurser<Ts...>::template DoAllocate<allocator_type, 0, index_sequence_type, Ts...>(
                            *this, ppNewLeaf, newCapacity, 0);

                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            0, firstIdx, (Ts*)ppNewLeaf[Indices]), 0)...);
                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            firstIdx, oldNumElements, (Ts*)ppNewLeaf[Indices] + lastIdx), 0)...);
                    swallow((corsac::uninitialized_fill_ptr((Ts*)ppNewLeaf[Indices] + firstIdx, (Ts*)ppNewLeaf[Indices] + lastIdx, args), 0)...);
                    swallow(TupleVecLeaf<Indices, Ts>::mpData = (Ts*)ppNewLeaf[Indices]...);

                    CORSAC_Free(get_allocator(), mpData, internalDataSize());
                    mpData = allocation.first;
                    mNumCapacity = newCapacity;
                    internalDataSize() = allocation.second;
                }
                else
                {
                    swallow((TupleVecLeaf<Indices, Ts>::DoInsertAndFill(firstIdx, n, oldNumElements, args), 0)...);
                }
            }
            else
            {
                swallow((corsac::uninitialized_fill_ptr(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                                   TupleVecLeaf<Indices, Ts>::mpData + newNumElements, args), 0)...);
            }
            return begin() + firstIdx;
        }

        iterator insert(const_iterator pos, const_iterator first, const_iterator last)
        {
            #if CORSAC_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                    {CORSAC_FAIL_MSG("tuple_vector::insert -- invalid iterator")}
                if (CORSAC_UNLIKELY(!validate_iterator_pair(first, last)))
                    {CORSAC_FAIL_MSG("tuple_vector::insert -- invalid iterator pair")}
            #endif
            size_type posIdx = pos - cbegin();
            size_type firstIdx = first.mIndex;
            size_type lastIdx = last.mIndex;
            size_type numToInsert = last - first;
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + numToInsert;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            const void* ppOtherData[sizeof...(Ts)] = {first.mpData[Indices]...};
            if (newNumElements > oldNumCapacity || posIdx != oldNumElements)
            {
                if (newNumElements > oldNumCapacity)
                {
                    const size_type newCapacity = corsac::max(GetNewCapacity(oldNumCapacity), newNumElements);

                    void* ppNewLeaf[sizeof...(Ts)];
                    pair<void*, size_type> allocation = TupleRecurser<Ts...>::template DoAllocate<allocator_type, 0, index_sequence_type, Ts...>(
                            *this, ppNewLeaf, newCapacity, 0);

                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            0, posIdx, (Ts*)ppNewLeaf[Indices]), 0)...);
                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            posIdx, oldNumElements, (Ts*)ppNewLeaf[Indices] + posIdx + numToInsert), 0)...);
                    swallow((corsac::uninitialized_copy_ptr((Ts*)(ppOtherData[Indices]) + firstIdx,
                                                           (Ts*)(ppOtherData[Indices]) + lastIdx,
                                                           (Ts*)ppNewLeaf[Indices] + posIdx), 0)...);
                    swallow(TupleVecLeaf<Indices, Ts>::mpData = (Ts*)ppNewLeaf[Indices]...);

                    CORSAC_Free(get_allocator(), mpData, internalDataSize());
                    mpData = allocation.first;
                    mNumCapacity = newCapacity;
                    internalDataSize() = allocation.second;
                }
                else
                {
                    swallow((TupleVecLeaf<Indices, Ts>::DoInsertRange(
                            (Ts*)(ppOtherData[Indices]) + firstIdx, (Ts*)(ppOtherData[Indices]) + lastIdx,
                            TupleVecLeaf<Indices, Ts>::mpData + posIdx, oldNumElements), 0)...);
                }
            }
            else
            {
                swallow((corsac::uninitialized_copy_ptr((Ts*)(ppOtherData[Indices]) + firstIdx,
                                               (Ts*)(ppOtherData[Indices]) + lastIdx,
                                               TupleVecLeaf<Indices, Ts>::mpData + posIdx), 0)...);
            }
            return begin() + posIdx;
        }

        iterator insert(const_iterator pos, const value_tuple* first, const value_tuple* last)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                        {CORSAC_FAIL_MSG("tuple_vector::insert -- invalid iterator")}
                    if (CORSAC_UNLIKELY(first > last || first == nullptr || last == nullptr))
                        {CORSAC_FAIL_MSG("tuple_vector::insert -- invalid source pointers")}
            #endif
            size_type posIdx = pos - cbegin();
            size_type numToInsert = last - first;
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements + numToInsert;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = newNumElements;
            if (newNumElements > oldNumCapacity || posIdx != oldNumElements)
            {
                if (newNumElements > oldNumCapacity)
                {
                    const size_type newCapacity = corsac::max(GetNewCapacity(oldNumCapacity), newNumElements);

                    void* ppNewLeaf[sizeof...(Ts)];
                    pair<void*, size_type> allocation = TupleRecurser<Ts...>::template DoAllocate<allocator_type, 0, index_sequence_type, Ts...>(
                            *this, ppNewLeaf, newCapacity, 0);

                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            0, posIdx, (Ts*)ppNewLeaf[Indices]), 0)...);
                    swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(
                            posIdx, oldNumElements, (Ts*)ppNewLeaf[Indices] + posIdx + numToInsert), 0)...);

                    swallow(TupleVecLeaf<Indices, Ts>::mpData = (Ts*)ppNewLeaf[Indices]...);

                    // Do this after mpData is updated so that we can use new iterators
                    DoUninitializedCopyFromTupleArray(begin() + posIdx, begin() + posIdx + numToInsert, first);

                    CORSAC_Free(get_allocator(), mpData, internalDataSize());
                    mpData = allocation.first;
                    mNumCapacity = newCapacity;
                    internalDataSize() = allocation.second;
                }
                else
                {
                    const size_type nExtra = oldNumElements - posIdx;
                    void* ppDataEnd[sizeof...(Ts)] = { (void*)(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements)... };
                    void* ppDataBegin[sizeof...(Ts)] = { (void*)(TupleVecLeaf<Indices, Ts>::mpData + posIdx)... };
                    if (numToInsert < nExtra) // Если вставленные значения полностью находятся в инициализированной памяти (т.е. перед mpEnd) ...
                    {
                        swallow((corsac::uninitialized_move_ptr((Ts*)ppDataEnd[Indices] - numToInsert,
                                                               (Ts*)ppDataEnd[Indices], (Ts*)ppDataEnd[Indices]), 0)...);
                        // Нам нужен move_backward из-за возможных проблем с перекрытием.
                        swallow((corsac::move_backward((Ts*)ppDataBegin[Indices],
                                                      (Ts*)ppDataEnd[Indices] - numToInsert, (Ts*)ppDataEnd[Indices]), 0)...);

                        DoCopyFromTupleArray(pos, pos + numToInsert, first);
                    }
                    else
                    {
                        size_type numToInitialize = numToInsert - nExtra;
                        swallow((corsac::uninitialized_move_ptr((Ts*)ppDataBegin[Indices],
                                                               (Ts*)ppDataEnd[Indices], (Ts*)ppDataEnd[Indices] + numToInitialize), 0)...);

                        DoCopyFromTupleArray(pos, begin() + oldNumElements, first);
                        DoUninitializedCopyFromTupleArray(begin() + oldNumElements, pos + numToInsert, first + nExtra);
                    }
                }
            }
            else
            {
                DoUninitializedCopyFromTupleArray(pos, pos + numToInsert, first);
            }
            return begin() + posIdx;
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(validate_iterator(first) == isf_none || validate_iterator(last) == isf_none))
                {CORSAC_FAIL_MSG("tuple_vector::erase -- invalid iterator")}
            if (CORSAC_UNLIKELY(!validate_iterator_pair(first, last)))
                {CORSAC_FAIL_MSG("tuple_vector::erase -- invalid iterator pair")}
            #endif
            if (first != last)
            {
            size_type firstIdx = first - cbegin();
            size_type lastIdx = last - cbegin();
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements - (lastIdx - firstIdx);
            mNumElements = newNumElements;
            swallow((corsac::move(TupleVecLeaf<Indices, Ts>::mpData + lastIdx,
                                 TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                 TupleVecLeaf<Indices, Ts>::mpData + firstIdx), 0)...);
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + newNumElements,
                                     TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
            }
            return begin() + first.mIndex;
        }

        iterator erase_unsorted(const_iterator pos)
        {
        #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                    {CORSAC_FAIL_MSG("tuple_vector::erase_unsorted -- invalid iterator")}
        #endif
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements - 1;
            mNumElements = newNumElements;
            swallow((corsac::move(TupleVecLeaf<Indices, Ts>::mpData + newNumElements,
                                 TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                 TupleVecLeaf<Indices, Ts>::mpData + (pos - begin())), 0)...);
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + newNumElements,
                                     TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
            return begin() + pos.mIndex;
        }

        iterator erase_unsorted(size_type n)
        {
            const_iterator pos = const_iterator((const_this_type*)(this), n);
            #if CORSAC_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(validate_iterator(pos) == isf_none))
                    {CORSAC_FAIL_MSG("tuple_vector::erase_unsorted -- invalid iterator")}
            #endif
            size_type oldNumElements = mNumElements;
            size_type newNumElements = oldNumElements - 1;
            mNumElements = newNumElements;
            swallow((corsac::move(TupleVecLeaf<Indices, Ts>::mpData + newNumElements,
                                  TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                  TupleVecLeaf<Indices, Ts>::mpData + (pos - begin())), 0)...);
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + newNumElements,
                                      TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
            return begin() + pos.mIndex;
        }

        void resize(size_type n)
        {
            size_type oldNumElements = mNumElements;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = n;
            if (n > oldNumElements)
            {
                if (n > oldNumCapacity)
                {
                DoReallocate(oldNumElements, corsac::max<size_type>(GetNewCapacity(oldNumCapacity), n));
                }
                swallow((corsac::uninitialized_default_fill_n(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements, n - oldNumElements), 0)...);
            }
            else
            {
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + n,
                                     TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
            }
        }

        void resize(size_type n, const Ts&... args)
        {
            size_type oldNumElements = mNumElements;
            size_type oldNumCapacity = mNumCapacity;
            mNumElements = n;
            if (n > oldNumElements)
            {
                if (n > oldNumCapacity)
                {
                    DoReallocate(oldNumElements, corsac::max<size_type>(GetNewCapacity(oldNumCapacity), n));
                }
                swallow((corsac::uninitialized_fill_ptr(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements,
                                                   TupleVecLeaf<Indices, Ts>::mpData + n, args), 0)...);
            }
            else
            {
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + n,
                                     TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
            }
        }

        void reserve(size_type n)
        {
            DoConditionalReallocate(mNumElements, mNumCapacity, n);
        }

        void shrink_to_fit()
        {
            this_type temp(move_iterator<iterator>(begin()), move_iterator<iterator>(end()), get_allocator());
            swap(temp);
        }

        void clear() noexcept
        {
            size_type oldNumElements = mNumElements;
            mNumElements = 0;
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData, TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
        }

        void reset_lose_memory() noexcept
        {
            size_type oldNumElements = mNumElements;
            mNumElements = 0;
        }

        void pop_back()
        {
            #if CORSAC_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(mNumElements <= 0))
                        CORSAC_FAIL_MSG("tuple_vector::pop_back -- container is empty");
            #endif
            size_type oldNumElements = mNumElements--;
            swallow((corsac::destruct(TupleVecLeaf<Indices, Ts>::mpData + oldNumElements - 1,
                                     TupleVecLeaf<Indices, Ts>::mpData + oldNumElements), 0)...);
        }

        void swap(this_type& x)
        {
            swallow((corsac::swap(TupleVecLeaf<Indices, Ts>::mpData, x.TupleVecLeaf<Indices, Ts>::mpData), 0)...);
            corsac::swap(mpData, x.mpData);
            corsac::swap(mNumElements, x.mNumElements);
            corsac::swap(mNumCapacity, x.mNumCapacity);
            corsac::swap(get_allocator(), x.get_allocator());
            corsac::swap(internalDataSize(), x.internalDataSize());
        }

        void assign(size_type n, const_reference_tuple tup) { assign(n, corsac::get<Indices>(tup)...); }
        void assign(std::initializer_list<value_tuple> iList) { assign(iList.begin(), iList.end()); }

        void push_back(Ts&&... args) { emplace_back(corsac::forward<Ts>(args)...); }
        void push_back(const_reference_tuple tup) { push_back(corsac::get<Indices>(tup)...); }
        void push_back(rvalue_tuple tup) { emplace_back(corsac::forward<Ts>(corsac::get<Indices>(tup))...); }

        void emplace_back(rvalue_tuple tup) { emplace_back(corsac::forward<Ts>(corsac::get<Indices>(tup))...); }
        void emplace(const_iterator pos, rvalue_tuple tup) { emplace(pos, corsac::forward<Ts>(corsac::get<Indices>(tup))...); }

        iterator insert(const_iterator pos, const Ts&... args) { return insert(pos, 1, args...); }
        iterator insert(const_iterator pos, Ts&&... args) { return emplace(pos, corsac::forward<Ts>(args)...); }
        iterator insert(const_iterator pos, rvalue_tuple tup) { return emplace(pos, corsac::forward<Ts>(corsac::get<Indices>(tup))...); }
        iterator insert(const_iterator pos, const_reference_tuple tup) { return insert(pos, corsac::get<Indices>(tup)...); }
        iterator insert(const_iterator pos, size_type n, const_reference_tuple tup) { return insert(pos, n, corsac::get<Indices>(tup)...); }
        iterator insert(const_iterator pos, std::initializer_list<value_tuple> iList) { return insert(pos, iList.begin(), iList.end()); }

        iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
        reverse_iterator erase(const_reverse_iterator pos) { return reverse_iterator(erase((pos + 1).base(), (pos).base())); }
        reverse_iterator erase(const_reverse_iterator first, const_reverse_iterator last) { return reverse_iterator(erase((last).base(), (first).base())); }
        reverse_iterator erase_unsorted(const_reverse_iterator pos) { return reverse_iterator(erase_unsorted((pos + 1).base())); }

        void resize(size_type n, const_reference_tuple tup) { resize(n, corsac::get<Indices>(tup)...); }

        bool empty() const noexcept { return mNumElements == 0; }
        size_type size() const noexcept { return mNumElements; }
        size_type capacity() const noexcept { return mNumCapacity; }

        iterator begin() noexcept { return iterator(this, 0); }
        const_iterator begin() const noexcept { return const_iterator((const_this_type*)(this), 0); }
        const_iterator cbegin() const noexcept { return const_iterator((const_this_type*)(this), 0); }

        iterator end() noexcept { return iterator(this, size()); }
        const_iterator end() const noexcept { return const_iterator((const_this_type*)(this), size()); }
        const_iterator cend() const noexcept { return const_iterator((const_this_type*)(this), size()); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const  noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

        reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

        ptr_tuple data() noexcept { return ptr_tuple(TupleVecLeaf<Indices, Ts>::mpData...); }
        const_ptr_tuple data() const noexcept { return const_ptr_tuple(TupleVecLeaf<Indices, Ts>::mpData...); }

        reference_tuple at(size_type n)
        {
            #if CORSAC_EXCEPTIONS_ENABLED
            if (CORSAC_UNLIKELY(n >= mNumElements))
                        throw std::out_of_range("tuple_vector::at -- out of range");
            #elif CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(n >= mNumElements))
                        {CORSAC_FAIL_MSG("tuple_vector::at -- out of range")}
            #endif
            return reference_tuple(*(TupleVecLeaf<Indices, Ts>::mpData + n)...);
        }

        const_reference_tuple at(size_type n) const
        {
            #if CORSAC_EXCEPTIONS_ENABLED
            if (CORSAC_UNLIKELY(n >= mNumElements))
                        throw std::out_of_range("tuple_vector::at -- out of range");
            #elif CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(n >= mNumElements))
                        {CORSAC_FAIL_MSG("tuple_vector::at -- out of range")}
            #endif
            return const_reference_tuple(*(TupleVecLeaf<Indices, Ts>::mpData + n)...);
        }

        reference_tuple operator[](size_type n) { return at(n); }
        const_reference_tuple operator[](size_type n) const { return at(n); }

        reference_tuple front()
        {
            #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(mNumElements == 0)) // Мы не разрешаем пользователю ссылаться на пустой контейнер.
                            {CORSAC_FAIL_MSG("tuple_vector::front -- empty vector")};
            #else
                // Мы позволяем пользователю ссылаться на пустой контейнер.
            #endif
                return at(0);
        }

        const_reference_tuple front() const
        {
            #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(mNumElements == 0)) // Мы не разрешаем пользователю ссылаться на пустой контейнер.
                            {CORSAC_FAIL_MSG("tuple_vector::front -- empty vector")};
            #else
                // Мы позволяем пользователю ссылаться на пустой контейнер.
            #endif

                return at(0);
        }

        reference_tuple back()
        {
            #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(mNumElements == 0)) // Мы не разрешаем пользователю ссылаться на пустой контейнер.
                            {CORSAC_FAIL_MSG("tuple_vector::back -- empty vector")}
            #else
                // Мы позволяем пользователю ссылаться на пустой контейнер.
            #endif
                return at(size() - 1);
        }

        const_reference_tuple back() const
        {
            #if CORSAC_ASSERT_ENABLED && CORSAC_EMPTY_REFERENCE_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(mNumElements == 0)) // Мы не разрешаем пользователю ссылаться на пустой контейнер.
                            {CORSAC_FAIL_MSG("tuple_vector::back -- empty vector")}
            #else
                // Мы позволяем пользователю ссылаться на пустой контейнер.
            #endif
                return at(size() - 1);
        }

        template <size_type I>
        tuplevec_element_t<I, Ts...>* get()
        {
            using Element = tuplevec_element_t<I, Ts...>;
            return TupleVecLeaf<I, Element>::mpData;
        }

        template <size_type I>
        const tuplevec_element_t<I, Ts...>* get() const
        {
            using Element = tuplevec_element_t<I, Ts...>;
            return TupleVecLeaf<I, Element>::mpData;
        }

        template <typename T>
        T* get()
        {
            using Index = tuplevec_index<T, TupleTypes<Ts...>>;
            return TupleVecLeaf<Index::index, T>::mpData;
        }

        template <typename T>
        const T* get() const
        {
            using Index = tuplevec_index<T, TupleTypes<Ts...>>;
            return TupleVecLeaf<Index::index, T>::mpData;
        }

        this_type& operator=(const this_type& other)
        {
            if (this != &other)
            {
                clear();
                assign(other.begin(), other.end());
            }
            return *this;
        }

        this_type& operator=(this_type&& other)
        {
            if (this != &other)
            {
                swap(other);
            }
            return *this;
        }

        this_type& operator=(std::initializer_list<value_tuple> iList)
        {
            assign(iList.begin(), iList.end());
            return *this;
        }

        bool validate() const noexcept
        {
            if (mNumElements > mNumCapacity)
                return false;
            if (!(variadicAnd(mpData <= TupleVecLeaf<Indices, Ts>::mpData...)))
                return false;
            void* pDataEnd = (void*)((uintptr_t)mpData + internalDataSize());
            if (!(variadicAnd(pDataEnd >= TupleVecLeaf<Indices, Ts>::mpData...)))
                return false;
            return true;
        }

        int validate_iterator(const_iterator iter) const noexcept
        {
            if (!(variadicAnd(iter.mpData[Indices] == TupleVecLeaf<Indices, Ts>::mpData...)))
                return isf_none;
            if (iter.mIndex < mNumElements)
                return (isf_valid | isf_current | isf_can_dereference);
            if (iter.mIndex <= mNumElements)
                return (isf_valid | isf_current);
            return isf_none;
        }

        static bool validate_iterator_pair(const_iterator first, const_iterator last) noexcept
        {
            return (first.mIndex <= last.mIndex) && variadicAnd(first.mpData[Indices] == last.mpData[Indices]...);
        }

        template <typename Iterator, typename = typename enable_if<is_iterator_wrapper<Iterator>::value, bool>::type>
        int validate_iterator(Iterator iter) const noexcept { return validate_iterator(unwrap_iterator(iter)); }

        template <typename Iterator, typename = typename enable_if<is_iterator_wrapper<Iterator>::value, bool>::type>
        static bool validate_iterator_pair(Iterator first, Iterator last) noexcept { return validate_iterator_pair(unwrap_iterator(first), unwrap_iterator(last)); }

        allocator_type& get_allocator() noexcept { return mDataSizeAndAllocator.second(); }
        const allocator_type& get_allocator() const noexcept { return mDataSizeAndAllocator.second(); }

        void set_allocator(const allocator_type& alloc) { mDataSizeAndAllocator.second() = alloc; }

    protected:

        void* mpData = nullptr;
        size_type mNumElements = 0;
        size_type mNumCapacity = 0;

        compressed_pair<size_type, allocator_type> mDataSizeAndAllocator;

        size_type& internalDataSize() noexcept { return mDataSizeAndAllocator.first(); }
        size_type const& internalDataSize() const noexcept { return mDataSizeAndAllocator.first(); }

        friend struct TupleRecurser<>;
        template<typename... Us>
        friend struct TupleRecurser;

        template <typename MoveIterBase>
        void DoInitFromIterator(move_iterator<MoveIterBase> begin, move_iterator<MoveIterBase> end)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(!validate_iterator_pair(begin, end)))
                        {CORSAC_FAIL_MSG("tuple_vector::erase -- invalid iterator pair")}
            #endif
            size_type newNumElements = (size_type)(end - begin);
            const void* ppOtherData[sizeof...(Ts)] = { begin.base().mpData[Indices]... };
            size_type beginIdx = begin.base().mIndex;
            size_type endIdx = end.base().mIndex;
            DoConditionalReallocate(0, mNumCapacity, newNumElements);
            mNumElements = newNumElements;
            swallow((corsac::uninitialized_move_ptr(corsac::move_iterator<Ts*>((Ts*)(ppOtherData[Indices]) + beginIdx),
                                                   corsac::move_iterator<Ts*>((Ts*)(ppOtherData[Indices]) + endIdx),
                                                   TupleVecLeaf<Indices, Ts>::mpData), 0)...);
        }

        void DoInitFromIterator(const_iterator begin, const_iterator end)
        {
            #if CORSAC_ASSERT_ENABLED
            if (CORSAC_UNLIKELY(!validate_iterator_pair(begin, end)))
                        {CORSAC_FAIL_MSG("tuple_vector::erase -- invalid iterator pair")}
            #endif
            size_type newNumElements = (size_type)(end - begin);
            const void* ppOtherData[sizeof...(Ts)] = { begin.mpData[Indices]... };
            size_type beginIdx = begin.mIndex;
            size_type endIdx = end.mIndex;
            DoConditionalReallocate(0, mNumCapacity, newNumElements);
            mNumElements = newNumElements;
            swallow((corsac::uninitialized_copy_ptr((Ts*)(ppOtherData[Indices]) + beginIdx,
                                                   (Ts*)(ppOtherData[Indices]) + endIdx,
                                                   TupleVecLeaf<Indices, Ts>::mpData), 0)...);
        }

        void DoInitFillTuple(size_type n, const_reference_tuple tup) { DoInitFillArgs(n, corsac::get<Indices>(tup)...); }

        void DoInitFillArgs(size_type n, const Ts&... args)
        {
            DoConditionalReallocate(0, mNumCapacity, n);
            mNumElements = n;
            swallow((corsac::uninitialized_fill_ptr(TupleVecLeaf<Indices, Ts>::mpData, TupleVecLeaf<Indices, Ts>::mpData + n, args), 0)...);
        }

        void DoInitDefaultFill(size_type n)
        {
            DoConditionalReallocate(0, mNumCapacity, n);
            mNumElements = n;
            swallow((corsac::uninitialized_default_fill_n(TupleVecLeaf<Indices, Ts>::mpData, n), 0)...);
        }

        void DoInitFromTupleArray(const value_tuple* first, const value_tuple* last)
        {
            #if CORSAC_ASSERT_ENABLED
                if (CORSAC_UNLIKELY(first > last || first == nullptr || last == nullptr))
                        CORSAC_FAIL_MSG("tuple_vector::ctor from tuple array -- invalid ptrs");
            #endif
            size_type newNumElements = last - first;
            DoConditionalReallocate(0, mNumCapacity, newNumElements);
            mNumElements = newNumElements;
            DoUninitializedCopyFromTupleArray(begin(), end(), first);
        }

        void DoCopyFromTupleArray(iterator destPos, iterator destEnd, const value_tuple* srcTuple)
        {
            while (destPos < destEnd)
            {
                *destPos = *srcTuple;
                ++destPos;
                ++srcTuple;
            }
        }

        void DoUninitializedCopyFromTupleArray(iterator destPos, iterator destEnd, const value_tuple* srcTuple)
        {
            while (destPos < destEnd)
            {
                swallow(::new(corsac::get<Indices>(destPos.MakePointer())) Ts(corsac::get<Indices>(*srcTuple))...);
                ++destPos;
                ++srcTuple;
            }
        }

        // Постарайтесь увеличить размер контейнера "естественным образом" с учетом количества используемых элементов.
        void DoGrow(size_type oldNumElements, size_type oldNumCapacity, size_type requiredCapacity)
        {
            if (requiredCapacity > oldNumCapacity)
                DoReallocate(oldNumElements, GetNewCapacity(requiredCapacity));
        }

        // Перераспределите на newCapacity (хотя на самом деле он больше на IFF)
        void DoConditionalReallocate(size_type oldNumElements, size_type oldNumCapacity, size_type requiredCapacity)
        {
            if (requiredCapacity > oldNumCapacity)
                DoReallocate(oldNumElements, requiredCapacity);
        }

        void DoReallocate(size_type oldNumElements, size_type requiredCapacity)
        {
            void* ppNewLeaf[sizeof...(Ts)];
            pair<void*, size_type> allocation = TupleRecurser<Ts...>::template DoAllocate<allocator_type, 0, index_sequence_type, Ts...>(
            *this, ppNewLeaf, requiredCapacity, 0);
            swallow((TupleVecLeaf<Indices, Ts>::DoUninitializedMoveAndDestruct(0, oldNumElements, (Ts*)ppNewLeaf[Indices]), 0)...);
            swallow(TupleVecLeaf<Indices, Ts>::mpData = (Ts*)ppNewLeaf[Indices]...);

            CORSAC_Free(get_allocator(), mpData, internalDataSize());
            mpData = allocation.first;
            mNumCapacity = requiredCapacity;
            internalDataSize() = allocation.second;
        }

        size_type GetNewCapacity(size_type oldNumCapacity)
        {
            return (oldNumCapacity > 0) ? (2 * oldNumCapacity) : 1;
        }
    };
}  // namespace TupleVecInternal

    // Специализация Move_iterator для TupleVecIter.
    // Ссылка rvalue для move_iterator обычно будет "tuple <Ts ...> &&", тогда как
    // на самом деле нам нужно "tuple<Ts&&...>". Эта специализация дает нам это.
    template <size_t... Indices, typename... Ts>
    class move_iterator<TupleVecInternal::TupleVecIter<index_sequence<Indices...>, Ts...>>
    {
    public:
        using iterator_type         = TupleVecInternal::TupleVecIter<index_sequence<Indices...>, Ts...>;
        using wrapped_iterator_type = iterator_type; // Этого нет в стандарте C ++; он используется, чтобы идентифицировать его как
        // тип итератора-оболочки.
        using traits_type           = iterator_traits<iterator_type>;
        using iterator_category     = typename traits_type::iterator_category;
        using value_type            = typename traits_type::value_type;
        using difference_type       = typename traits_type::difference_type;
        using pointer               = typename traits_type::pointer;
        using reference             = tuple<Ts&&...>;
        using this_type             = move_iterator<iterator_type>;

    protected:
        iterator_type mIterator;

    public:
        move_iterator() : mIterator() {}
        explicit move_iterator(iterator_type mi) : mIterator(mi) {}

        template <typename U>
        move_iterator(const move_iterator<U>& mi) : mIterator(mi.base()) {}

        iterator_type base() const { return mIterator; }
        reference operator*() const { return corsac::move(MakeReference()); }
        pointer operator->() const { return mIterator; }

        this_type& operator++() { ++mIterator; return *this; }
        this_type operator++(int) {
            this_type tempMoveIterator = *this;
            ++mIterator;
            return tempMoveIterator;
        }

        this_type& operator--() { --mIterator; return *this; }
        this_type operator--(int)
        {
            this_type tempMoveIterator = *this;
            --mIterator;
            return tempMoveIterator;
        }

        this_type operator+(difference_type n) const { return move_iterator(mIterator + n); }
        this_type& operator+=(difference_type n)
        {
            mIterator += n;
            return *this;
        }

        this_type operator-(difference_type n) const { return move_iterator(mIterator - n); }
        this_type& operator-=(difference_type n)
        {
            mIterator -= n;
            return *this;
        }

        difference_type operator-(const this_type& rhs) const { return mIterator - rhs.mIterator; }
        bool operator<(const this_type& rhs) const { return mIterator < rhs.mIterator; }
        bool operator>(const this_type& rhs) const { return mIterator > rhs.mIterator; }
        bool operator>=(const this_type& rhs) const { return mIterator >= rhs.mIterator; }
        bool operator<=(const this_type& rhs) const { return mIterator <= rhs.mIterator; }

        reference operator[](difference_type n) const { return *(*this + n); }

    private:
        reference MakeReference() const
        {
            return reference(corsac::move(((Ts*)mIterator.mpData[Indices])[mIterator.mIndex])...);
        }
    };

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator==(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                           const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return ((a.size() == b.size()) && corsac::equal(a.begin(), a.end(), b.begin()));
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator!=(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                           const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return ((a.size() != b.size()) || !corsac::equal(a.begin(), a.end(), b.begin()));
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator<(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                          const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return corsac::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator>(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                          const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return b < a;
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator<=(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                           const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return !(b < a);
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline bool operator>=(const TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                           const TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        return !(a < b);
    }

    template <typename AllocatorA, typename AllocatorB, typename Indices, typename... Ts>
    inline void swap(TupleVecInternal::TupleVecImpl<AllocatorA, Indices, Ts...>& a,
                     TupleVecInternal::TupleVecImpl<AllocatorB, Indices, Ts...>& b)
    {
        a.swap(b);
    }

    template<typename... Ts>
    inline
    typename enable_if<conjunction<is_swappable<Ts>...>::value>::type
            swap(tuple<Ts&...>&& a, tuple<Ts&...>&& b)
    {
        a.swap(b);
    }

    template<typename... Ts>
    inline
    typename enable_if<!conjunction<is_swappable<Ts>...>::value>::type
            swap(tuple<Ts&...>&& a, tuple<Ts&...>&& b) = delete;


    // Внешний интерфейс tuple_vector
    template <typename... Ts>
    class tuple_vector : public TupleVecInternal::TupleVecImpl<CORSAC_ALLOCATOR_TYPE, make_index_sequence<sizeof...(Ts)>, Ts...>
    {
        using this_type = tuple_vector<Ts...>;
        using base_type = TupleVecInternal::TupleVecImpl<CORSAC_ALLOCATOR_TYPE, make_index_sequence<sizeof...(Ts)>, Ts...>;
        using base_type::base_type;

    public:
        using value_type                = typename base_type::value_tuple;
        using pointer                   = typename base_type::ptr_tuple;
        using const_pointer             = typename base_type::const_ptr_tuple;
        using reference                 = typename base_type::reference_tuple;
        using const_reference           = typename base_type::const_reference_tuple;

        this_type& operator=(std::initializer_list<typename base_type::value_tuple> iList)
        {
            base_type::operator=(iList);
            return *this;
        }
    };

    // Вариант tuple_vector, который позволяет использовать определяемый пользователем тип распределителя
    // (нельзя смешивать параметры шаблона по умолчанию с вариативными)
    template <typename AllocatorType, typename... Ts>
    class tuple_vector_alloc
            : public TupleVecInternal::TupleVecImpl<AllocatorType, make_index_sequence<sizeof...(Ts)>, Ts...>
    {
        using this_type = tuple_vector_alloc<AllocatorType, Ts...>;
        using base_type = TupleVecInternal::TupleVecImpl<AllocatorType, make_index_sequence<sizeof...(Ts)>, Ts...>;
        using base_type::base_type;

    public:
        this_type& operator=(std::initializer_list<typename base_type::value_tuple> iList)
        {
            base_type::operator=(iList);
            return *this;
        }
    };

} // corsac

CORSAC_RESTORE_VC_WARNING()
CORSAC_RESTORE_VC_WARNING()
CORSAC_RESTORE_VC_WARNING()
CORSAC_RESTORE_VC_WARNING()

#endif //CORSAC_STL_TUPLE_VECTOR_H
