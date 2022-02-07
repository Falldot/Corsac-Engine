/**
 * corsac::STL
 *
 * chrono.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_TUPLE_H
#define CORSAC_STL_TUPLE_H

#pragma once

#include "Corsac/STL/config.h"
#include "Corsac/functional.h"
#include "Corsac/type_traits.h"
#include "Corsac/utility.h"

#include "Corsac/STL/tuple_fwd_decls.h"

CORSAC_DISABLE_VC_WARNING(4623) // warning C4623: default constructor was implicitly defined as deleted
CORSAC_DISABLE_VC_WARNING(4625) // warning C4625: copy constructor was implicitly defined as deleted
CORSAC_DISABLE_VC_WARNING(4510) // warning C4510: default constructor could not be generated

namespace corsac
{
    template <typename... Ts> struct TupleTypes {};

    template <typename T> class tuple_size {};
    template <typename T> class tuple_size<const T>          : public tuple_size<T> {};
    template <typename T> class tuple_size<volatile T>       : public tuple_size<T> {};
    template <typename T> class tuple_size<const volatile T> : public tuple_size<T> {};

    template <typename... Ts> class tuple_size<TupleTypes<Ts...>> : public integral_constant<size_t, sizeof...(Ts)> {};
    template <typename... Ts> class tuple_size<tuple<Ts...>>      : public integral_constant<size_t, sizeof...(Ts)> {};

    template <class T>
        constexpr size_t tuple_size_v = tuple_size<T>::value;

    namespace internal
    {
        template <typename TupleIndices, typename... Ts>
        struct TupleImpl;
    } // namespace internal

    template <typename Indices, typename... Ts>
    class tuple_size<internal::TupleImpl<Indices, Ts...>> : public integral_constant<size_t, sizeof...(Ts)>
    {};

    // tuple_element helper чтобы иметь возможность изолировать тип по индексу
    template <size_t I, typename T>
    class tuple_element
    {};

    template <size_t I>
    class tuple_element<I, TupleTypes<>>
    {
    public:
        static_assert(I != I, "tuple_element index out of range");
    };

    template <typename H, typename... Ts>
    class tuple_element<0, TupleTypes<H, Ts...>>
    {
    public:
        using type = H;
    };

    template <size_t I, typename H, typename... Ts>
    class tuple_element<I, TupleTypes<H, Ts...>>
    {
    public:
        using type = tuple_element_t<I - 1, TupleTypes<Ts...>>;
    };

    template <size_t I, typename... Ts>
    class tuple_element<I, tuple<Ts...>>
    {
    public:
        using type = tuple_element_t<I, TupleTypes<Ts...>>;
    };

    template <size_t I, typename... Ts>
    class tuple_element<I, const tuple<Ts...>>
    {
    public:
        using type = typename add_const<tuple_element_t<I, TupleTypes<Ts...>>>::type;
    };

    template <size_t I, typename... Ts>
    class tuple_element<I, volatile tuple<Ts...>>
    {
    public:
        using type = typename add_volatile<tuple_element_t<I, TupleTypes<Ts...>>>::type;
    };

    template <size_t I, typename... Ts>
    class tuple_element<I, const volatile tuple<Ts...>>
    {
    public:
        using type = typename add_cv<tuple_element_t<I, TupleTypes<Ts...>>>::type;
    };

    template <size_t I, typename Indices, typename... Ts>
    class tuple_element<I, internal::TupleImpl<Indices, Ts...>> : public tuple_element<I, tuple<Ts...>>
    {};

    template <size_t I, typename Indices, typename... Ts>
    class tuple_element<I, const internal::TupleImpl<Indices, Ts...>> : public tuple_element<I, const tuple<Ts...>>
    {};

    template <size_t I, typename Indices, typename... Ts>
    class tuple_element<I, volatile internal::TupleImpl<Indices, Ts...>> : public tuple_element<I, volatile tuple<Ts...>>
    {};

    template <size_t I, typename Indices, typename... Ts>
    class tuple_element<I, const volatile internal::TupleImpl<Indices, Ts...>> : public tuple_element<
            I, const volatile tuple<Ts...>>
    {};

    template <typename T, typename Tuple>
    struct tuple_index
    {};

    template <typename T>
    struct tuple_index<T, TupleTypes<>>
    {
        using DuplicateTypeCheck = void;
        tuple_index() = delete; // tuple_index следует использовать только для помощи во время компиляции и никогда не создавать экземпляры
        static const size_t index = 0;
    };

    template <typename T, typename... TsRest>
    struct tuple_index<T, TupleTypes<T, TsRest...>>
    {
        using DuplicateTypeCheck = int;
        // найдя тип T в списке типов, попробуйте найти тип T в TsRest.
        // Если мы снова наткнемся на эту версию tuple_index, то есть тип T появится дважды в списке типов, тогда DuplicateTypeCheck будет иметь тип int, а static_assert завершится ошибкой.
        // Если мы этого не сделаем, мы рассмотрим версию tuple_index выше, где все типы были исчерпаны, и DuplicateTypeCheck будет недействителен.
        static_assert(is_void<typename tuple_index<T, TupleTypes<TsRest...>>::DuplicateTypeCheck>::value, "duplicate type T in tuple_vector::get<T>(); unique types must be provided in declaration, or only use get<size_t>()");
        static const size_t index = 0;
    };

    template <typename T, typename TsHead, typename... TsRest>
    struct tuple_index<T, TupleTypes<TsHead, TsRest...>>
    {
        using DuplicateTypeCheck = typename tuple_index<T, TupleTypes<TsRest...>>::DuplicateTypeCheck;
        static const size_t index = tuple_index<T, TupleTypes<TsRest...>>::index + 1;
    };

    template <typename T, typename Indices, typename... Ts>
    struct tuple_index<T, internal::TupleImpl<Indices, Ts...>> : public tuple_index<T, TupleTypes<Ts...>>
    {};


    namespace internal
    {
        /**
        * swallow
        *
        * Предоставляет сосуд для развертывания вариативных упаковок.
        */
        template <typename... Ts>
        void swallow(Ts&&...)
        {}

        // TupleLeaf
        template <size_t I, typename ValueType, bool IsEmpty = is_empty_v<ValueType>>
        class TupleLeaf;

        template <size_t I, typename ValueType, bool IsEmpty>
        inline void swap(TupleLeaf<I, ValueType, IsEmpty>& a, TupleLeaf<I, ValueType, IsEmpty>& b)
        {
            corsac::swap(a.getinternal(), b.getinternal());
        }

        template <size_t I, typename ValueType, bool IsEmpty>
        class TupleLeaf
        {
        public:
            TupleLeaf() : mValue() {}
            TupleLeaf(const TupleLeaf&) = default;
            TupleLeaf& operator=(const TupleLeaf&) = delete;

            // Нам не нужен этот явный конструктор, так как он должен обрабатываться шаблоном ниже, но OSX clang
            // Признак типа is_constructible  неправильно дает false для is_constructible<T&&, T&&>::value
            explicit TupleLeaf(ValueType&& v) : mValue(corsac::move(v)) {}

            template <typename T, typename = typename enable_if<is_constructible<ValueType, T&&>::value>::type>
            explicit TupleLeaf(T&& t)
                    : mValue(corsac::forward<T>(t))
            {}

            template <typename T>
            explicit TupleLeaf(const TupleLeaf<I, T>& t)
                    : mValue(t.getinternal())
            {}

            template <typename T>
            TupleLeaf& operator=(T&& t)
            {
                mValue = corsac::forward<T>(t);
                return *this;
            }

            int swap(TupleLeaf& t)
            {
                corsac::internal::swap(*this, t);
                return 0;
            }

            ValueType& getinternal() { return mValue; }
            const ValueType& getinternal() const { return mValue; }

        private:
            ValueType mValue;
        };

        // TupleLeaf: Specialize for when ValueType is a reference
        template <size_t I, typename ValueType, bool IsEmpty>
        class TupleLeaf<I, ValueType&, IsEmpty>
        {
        public:
            TupleLeaf(const TupleLeaf&) = default;
            TupleLeaf& operator=(const TupleLeaf&) = delete;

            template <typename T, typename = typename enable_if<is_constructible<ValueType, T&&>::value>::type>
            explicit TupleLeaf(T&& t)
                    : mValue(corsac::forward<T>(t))
            {}

            explicit TupleLeaf(ValueType& t) : mValue(t)
            {}

            template <typename T>
            explicit TupleLeaf(const TupleLeaf<I, T>& t)
                    : mValue(t.getinternal())
            {}

            template <typename T>
            TupleLeaf& operator=(T&& t)
            {
                mValue = corsac::forward<T>(t);
                return *this;
            }

            int swap(TupleLeaf& t)
            {
                corsac::internal::swap(*this, t);
                return 0;
            }

            ValueType& getinternal() { return mValue; }
            const ValueType& getinternal() const { return mValue; }

        private:
            ValueType& mValue;
        };

        // TupleLeaf: partial specialization for when we can use the Empty Base Class Optimization
        template <size_t I, typename ValueType>
        class TupleLeaf<I, ValueType, true> : private ValueType
        {
        public:
            // true_type / false_type constructors for case where ValueType is default constructible and should be value
            // initialized and case where it is not
            TupleLeaf(const TupleLeaf&) = default;

            template <typename T, typename = typename enable_if<is_constructible<ValueType, T&&>::value>::type>
            explicit TupleLeaf(T&& t)
                    : ValueType(corsac::forward<T>(t))
            {}

            template <typename T>
            explicit TupleLeaf(const TupleLeaf<I, T>& t)
                    : ValueType(t.getinternal())
            {}

            template <typename T>
            TupleLeaf& operator=(T&& t)
            {
                ValueType::operator=(corsac::forward<T>(t));
                return *this;
            }

            int swap(TupleLeaf& t)
            {
                corsac::internal::swap(*this, t);
                return 0;
            }

            ValueType& getinternal() { return static_cast<ValueType&>(*this); }
            const ValueType& getinternal() const { return static_cast<const ValueType&>(*this); }

        private:
            TupleLeaf& operator=(const TupleLeaf&) = delete;
        };

        // MakeTupleTypes
        template <typename TupleTypes, typename Tuple, size_t Start, size_t End>
        struct MakeTupleTypesImpl;

        template <typename... Types, typename Tuple, size_t Start, size_t End>
        struct MakeTupleTypesImpl<TupleTypes<Types...>, Tuple, Start, End>
        {
            using TupleType = typename remove_reference<Tuple>::type;
            using type = typename MakeTupleTypesImpl<
                    TupleTypes<Types..., typename conditional<is_lvalue_reference<Tuple>::value,
                            // добавить ссылку, если кортеж - ссылка
                            tuple_element_t<Start, TupleType>&,
                            // добавить non-ref в противном случае
                            tuple_element_t<Start, TupleType>>::type>,
            Tuple, Start + 1, End>::type;
        };

        template <typename... Types, typename Tuple, size_t End>
        struct MakeTupleTypesImpl<TupleTypes<Types...>, Tuple, End, End>
    {
        using type = TupleTypes<Types...>;
    };

    template <typename Tuple>
    using MakeTupleTypes_t = typename MakeTupleTypesImpl<TupleTypes<>, Tuple, 0,
            tuple_size<typename remove_reference<Tuple>::type>::value>::type;


    // TupleImpl
    template <size_t I, typename Indices, typename... Ts>
    tuple_element_t<I, TupleImpl<Indices, Ts...>>& get(TupleImpl<Indices, Ts...>& t);

    template <size_t I, typename Indices, typename... Ts>
    const_tuple_element_t<I, TupleImpl<Indices, Ts...>>& get(const TupleImpl<Indices, Ts...>& t);

    template <size_t I, typename Indices, typename... Ts>
    tuple_element_t<I, TupleImpl<Indices, Ts...>>&& get(TupleImpl<Indices, Ts...>&& t);

    template <typename T, typename Indices, typename... Ts>
    T& get(TupleImpl<Indices, Ts...>& t);

    template <typename T, typename Indices, typename... Ts>
    const T& get(const TupleImpl<Indices, Ts...>& t);

    template <typename T, typename Indices, typename... Ts>
    T&& get(TupleImpl<Indices, Ts...>&& t);

    template <size_t... Indices, typename... Ts>
    struct TupleImpl<integer_sequence<size_t, Indices...>, Ts...> : public TupleLeaf<Indices, Ts>...
    {
    constexpr TupleImpl() = default;

    template <typename... Us, typename... ValueTypes>
    explicit TupleImpl(integer_sequence<size_t, Indices...>, TupleTypes<Us...>, ValueTypes&&... values)
    : TupleLeaf<Indices, Ts>(corsac::forward<ValueTypes>(values))...
    {}

    template <typename OtherTuple>
    TupleImpl(OtherTuple&& t)
    : TupleLeaf<Indices, Ts>(corsac::forward<tuple_element_t<Indices, MakeTupleTypes_t<OtherTuple>>>(get<Indices>(t)))...
    {}

    template <typename OtherTuple>
    TupleImpl& operator=(OtherTuple&& t)
    {
        swallow(TupleLeaf<Indices, Ts>::operator=(
                corsac::forward<tuple_element_t<Indices, MakeTupleTypes_t<OtherTuple>>>(get<Indices>(t)))...);
        return *this;
    }

    TupleImpl& operator=(const TupleImpl& t)
    {
        swallow(TupleLeaf<Indices, Ts>::operator=(static_cast<const TupleLeaf<Indices, Ts>&>(t).getinternal())...);
        return *this;
    }

    void swap(TupleImpl& t) { swallow(TupleLeaf<Indices, Ts>::swap(static_cast<TupleLeaf<Indices, Ts>&>(t))...); }
    };

    template <size_t I, typename Indices, typename... Ts>
    inline tuple_element_t<I, TupleImpl<Indices, Ts...>>& get(TupleImpl<Indices, Ts...>& t)
    {
        using Type = tuple_element_t<I, TupleImpl<Indices, Ts...>>;
        return static_cast<internal::TupleLeaf<I, Type>&>(t).getinternal();
    }

    template <size_t I, typename Indices, typename... Ts>
    inline const_tuple_element_t<I, TupleImpl<Indices, Ts...>>& get(const TupleImpl<Indices, Ts...>& t)
    {
        using Type = tuple_element_t<I, TupleImpl<Indices, Ts...>>;
        return static_cast<const internal::TupleLeaf<I, Type>&>(t).getinternal();
    }

    template <size_t I, typename Indices, typename... Ts>
    inline tuple_element_t<I, TupleImpl<Indices, Ts...>>&& get(TupleImpl<Indices, Ts...>&& t)
    {
        using Type = tuple_element_t<I, TupleImpl<Indices, Ts...>>;
        return static_cast<Type&&>(static_cast<internal::TupleLeaf<I, Type>&>(t).getinternal());
    }

    template <typename T, typename Indices, typename... Ts>
    inline T& get(TupleImpl<Indices, Ts...>& t)
    {
        using Index = tuple_index<T, TupleImpl<Indices, Ts...>>;
        return static_cast<internal::TupleLeaf<Index::index, T>&>(t).getinternal();
    }

    template <typename T, typename Indices, typename... Ts>
    inline const T& get(const TupleImpl<Indices, Ts...>& t)
    {
        using Index = tuple_index<T, TupleImpl<Indices, Ts...>>;
        return static_cast<const internal::TupleLeaf<Index::index, T>&>(t).getinternal();
    }

    template <typename T, typename Indices, typename... Ts>
    inline T&& get(TupleImpl<Indices, Ts...>&& t)
    {
        using Index = tuple_index<T, TupleImpl<Indices, Ts...>>;
        return static_cast<T&&>(static_cast<internal::TupleLeaf<Index::index, T>&>(t).getinternal());
    }


    /**
    * TupleLike
    *
    * type-trait, который определяет, является ли тип corsac::tuple или corsac::pair.
    */
    template <typename T> struct TupleLike                   : public false_type {};
    template <typename T> struct TupleLike<const T>          : public TupleLike<T> {};
    template <typename T> struct TupleLike<volatile T>       : public TupleLike<T> {};
    template <typename T> struct TupleLike<const volatile T> : public TupleLike<T> {};

    template <typename... Ts>
    struct TupleLike<tuple<Ts...>> : public true_type {};

    template <typename First, typename Second>
    struct TupleLike<corsac::pair<First, Second>> : public true_type {};

    // TupleConvertible
    template <bool IsSameSize, typename From, typename To>
    struct TupleConvertibleImpl : public false_type
    {};

    template <typename... FromTypes, typename... ToTypes>
    struct TupleConvertibleImpl<true, TupleTypes<FromTypes...>,	TupleTypes<ToTypes...>>
            : public integral_constant<bool, conjunction<is_convertible<FromTypes, ToTypes>...>::value>
    {};

    template <typename From, typename To,
            bool = TupleLike<typename remove_reference<From>::type>::value,
            bool = TupleLike<typename remove_reference<To>::type>::value>
    struct TupleConvertible : public false_type
    {};

    template <typename From, typename To>
    struct TupleConvertible<From, To, true, true>
            : public TupleConvertibleImpl<tuple_size<typename remove_reference<From>::type>::value ==
                                          tuple_size<typename remove_reference<To>::type>::value,
                    MakeTupleTypes_t<From>, MakeTupleTypes_t<To>>
    {};


    // TupleAssignable
    template <bool IsSameSize, typename Target, typename From>
    struct TupleAssignableImpl : public false_type
    {};

    template <typename... TargetTypes, typename... FromTypes>
    struct TupleAssignableImpl<true, TupleTypes<TargetTypes...>, TupleTypes<FromTypes...>>
            : public bool_constant<conjunction<is_assignable<TargetTypes, FromTypes>...>::value>
    {};

    template <typename Target, typename From,
            bool = TupleLike<typename remove_reference<Target>::type>::value,
            bool = TupleLike<typename remove_reference<From>::type>::value>
    struct TupleAssignable : public false_type
    {};

    template <typename Target, typename From>
    struct TupleAssignable<Target, From, true, true>
            : public TupleAssignableImpl<
                    tuple_size<typename remove_reference<Target>::type>::value ==
                    tuple_size<typename remove_reference<From>::type>::value,
                    MakeTupleTypes_t<Target>, MakeTupleTypes_t<From>>
    {};


    // TupleImplicitlyConvertible and TupleExplicitlyConvertible
    template <bool IsSameSize, typename TargetType, typename... FromTypes>
    struct TupleImplicitlyConvertibleImpl : public false_type
    {};

    template <typename... TargetTypes, typename... FromTypes>
    struct TupleImplicitlyConvertibleImpl<true, TupleTypes<TargetTypes...>, FromTypes...>
            : public conjunction<
                    is_constructible<TargetTypes, FromTypes>...,
              is_convertible<FromTypes, TargetTypes>...>
    {};

    template <typename TargetTupleType, typename... FromTypes>
    struct TupleImplicitlyConvertible
            : public TupleImplicitlyConvertibleImpl<
                    tuple_size<TargetTupleType>::value == sizeof...(FromTypes),
                    MakeTupleTypes_t<TargetTupleType>, FromTypes...>::type
    {};

    template<typename TargetTupleType, typename... FromTypes>
    using TupleImplicitlyConvertible_t = enable_if_t<TupleImplicitlyConvertible<TargetTupleType, FromTypes...>::value, bool>;

    template <bool IsSameSize, typename TargetType, typename... FromTypes>
    struct TupleExplicitlyConvertibleImpl : public false_type
    {};

    template <typename... TargetTypes, typename... FromTypes>
    struct TupleExplicitlyConvertibleImpl<true, TupleTypes<TargetTypes...>, FromTypes...>
            : public conjunction<
                    is_constructible<TargetTypes, FromTypes>...,
              negation<conjunction<is_convertible<FromTypes, TargetTypes>...>>>
    {};

    template <typename TargetTupleType, typename... FromTypes>
    struct TupleExplicitlyConvertible
            : public TupleExplicitlyConvertibleImpl<
                    tuple_size<TargetTupleType>::value == sizeof...(FromTypes),
                    MakeTupleTypes_t<TargetTupleType>, FromTypes...>::type
    {};

    template<typename TargetTupleType, typename... FromTypes>
    using TupleExplicitlyConvertible_t = enable_if_t<TupleExplicitlyConvertible<TargetTupleType, FromTypes...>::value, bool>;

    // TupleEqual
    template <size_t I>
    struct TupleEqual
    {
        template <typename Tuple1, typename Tuple2>
        bool operator()(const Tuple1& t1, const Tuple2& t2)
        {
            static_assert(tuple_size<Tuple1>::value == tuple_size<Tuple2>::value, "comparing tuples of different sizes.");
            return TupleEqual<I - 1>()(t1, t2) && get<I - 1>(t1) == get<I - 1>(t2);
        }
    };

    template <>
    struct TupleEqual<0>
    {
        template <typename Tuple1, typename Tuple2>
        bool operator()(const Tuple1&, const Tuple2&)
        {
            return true;
        }
    };

    // TupleLess
    template <size_t I>
    struct TupleLess
    {
        template <typename Tuple1, typename Tuple2>
        bool operator()(const Tuple1& t1, const Tuple2& t2)
        {
            static_assert(tuple_size<Tuple1>::value == tuple_size<Tuple2>::value, "comparing tuples of different sizes.");
            return TupleLess<I - 1>()(t1, t2) || (!TupleLess<I - 1>()(t2, t1) && get<I - 1>(t1) < get<I - 1>(t2));
        }
    };

    template <>
    struct TupleLess<0>
    {
        template <typename Tuple1, typename Tuple2>
        bool operator()(const Tuple1&, const Tuple2&)
        {
            return false;
        }
    };

    // MakeTupleReturnImpl
    template <typename T> struct MakeTupleReturnImpl                       { using type = T; };
    template <typename T> struct MakeTupleReturnImpl<reference_wrapper<T>> { typedef T& type; };

    template <typename T>
    using MakeTupleReturn_t = typename MakeTupleReturnImpl<decay_t<T>>::type;


    // tuple_cat helpers

    // TupleCat2Impl
    template <typename Tuple1, typename Is1, typename Tuple2, typename Is2>
    struct TupleCat2Impl;

    template <typename... T1s, size_t... I1s, typename... T2s, size_t... I2s>
    struct TupleCat2Impl<tuple<T1s...>, index_sequence<I1s...>, tuple<T2s...>, index_sequence<I2s...>>
    {
        using ResultType = tuple<T1s..., T2s...>;

        template <typename Tuple1, typename Tuple2>
        static inline ResultType DoCat2(Tuple1&& t1, Tuple2&& t2)
        {
            return ResultType(get<I1s>(corsac::forward<Tuple1>(t1))..., get<I2s>(corsac::forward<Tuple2>(t2))...);
        }
    };

    // TupleCat2
    template <typename Tuple1, typename Tuple2>
    struct TupleCat2;

    template <typename... T1s, typename... T2s>
    struct TupleCat2<tuple<T1s...>, tuple<T2s...>>
    {
        using Is1        = make_index_sequence<sizeof...(T1s)>;
        using Is2        = make_index_sequence<sizeof...(T2s)>;
        using TCI        = TupleCat2Impl<tuple<T1s...>, Is1, tuple<T2s...>, Is2>;
        using ResultType = typename TCI::ResultType;

        template <typename Tuple1, typename Tuple2>
        static inline ResultType DoCat2(Tuple1&& t1, Tuple2&& t2)
        {
            return TCI::DoCat2(corsac::forward<Tuple1>(t1), corsac::forward<Tuple2>(t2));
        }
    };

    // TupleCat
    template <typename... Tuples>
    struct TupleCat;

    template <typename Tuple1, typename Tuple2, typename... TuplesRest>
    struct TupleCat<Tuple1, Tuple2, TuplesRest...>
    {
        using FirstResultType = typename TupleCat2<Tuple1, Tuple2>::ResultType;
        using ResultType      = typename TupleCat<FirstResultType, TuplesRest...>::ResultType;

        template <typename TupleArg1, typename TupleArg2, typename... TupleArgsRest>
        static inline ResultType DoCat(TupleArg1&& t1, TupleArg2&& t2, TupleArgsRest&&... ts)
        {
            return TupleCat<FirstResultType, TuplesRest...>::DoCat(
                    TupleCat2<TupleArg1, TupleArg2>::DoCat2(corsac::forward<TupleArg1>(t1), corsac::forward<TupleArg2>(t2)),
                    corsac::forward<TupleArgsRest>(ts)...);
        }
    };

    template <typename Tuple1, typename Tuple2>
    struct TupleCat<Tuple1, Tuple2>
    {
        using TC2 = TupleCat2<Tuple1, remove_reference_t<Tuple2>>;
        using ResultType = typename TC2::ResultType;

        template <typename TupleArg1, typename TupleArg2>
        static inline ResultType DoCat(TupleArg1&& t1, TupleArg2&& t2)
        {
            return TC2::DoCat2(corsac::forward<TupleArg1>(t1), corsac::forward<TupleArg2>(t2));
        }
    };
    }  // namespace internal

    /**
    * tuple
    *
    * corsac :: tuple - это контейнер разнородных значений фиксированного размера.
    * Это обобщение corsac :: pair, которое содержит только два разнородных значения.
    */
    template <typename... Ts>
    class tuple;

    template <typename T, typename... Ts>
    class tuple<T, Ts...>
    {
    public:
        constexpr tuple() = default;

        template <typename T2 = T,
                internal::TupleImplicitlyConvertible_t<tuple, const T2&, const Ts&...> = 0>
        constexpr tuple(const T& t, const Ts&... ts)
                : mImpl(make_index_sequence<sizeof...(Ts) + 1>{}, internal::MakeTupleTypes_t<tuple>{}, t, ts...)
        {}

        template <typename T2 = T,
                internal::TupleExplicitlyConvertible_t<tuple, const T2&, const Ts&...> = 0>
        explicit constexpr tuple(const T& t, const Ts&... ts)
                : mImpl(make_index_sequence<sizeof...(Ts) + 1>{}, internal::MakeTupleTypes_t<tuple>{}, t, ts...)
        {}

        template <typename U, typename... Us,
                internal::TupleImplicitlyConvertible_t<tuple, U, Us...> = 0>
        constexpr tuple(U&& u, Us&&... us)
                : mImpl(make_index_sequence<sizeof...(Us) + 1>{}, internal::MakeTupleTypes_t<tuple>{}, corsac::forward<U>(u),
                        corsac::forward<Us>(us)...)
        {}

        template <typename U, typename... Us,
                internal::TupleExplicitlyConvertible_t<tuple, U, Us...> = 0>
        explicit constexpr tuple(U&& u, Us&&... us)
                : mImpl(make_index_sequence<sizeof...(Us) + 1>{}, internal::MakeTupleTypes_t<tuple>{}, corsac::forward<U>(u),
                        corsac::forward<Us>(us)...)
        {}

        template <typename OtherTuple,
                typename enable_if<internal::TupleConvertible<OtherTuple, tuple>::value, bool>::type = false>
        tuple(OtherTuple&& t)
                : mImpl(corsac::forward<OtherTuple>(t))
        {}

        template <typename OtherTuple,
                typename enable_if<internal::TupleAssignable<tuple, OtherTuple>::value, bool>::type = false>
        tuple& operator=(OtherTuple&& t)
        {
            mImpl.operator=(corsac::forward<OtherTuple>(t));
            return *this;
        }

        void swap(tuple& t) { mImpl.swap(t.mImpl); }

    private:
        using Impl = internal::TupleImpl<make_index_sequence<sizeof...(Ts) + 1>, T, Ts...>;
        Impl mImpl;

        template <size_t I, typename... Ts_>
        friend tuple_element_t<I, tuple<Ts_...>>& get(tuple<Ts_...>& t);

        template <size_t I, typename... Ts_>
        friend const_tuple_element_t<I, tuple<Ts_...>>& get(const tuple<Ts_...>& t);

        template <size_t I, typename... Ts_>
        friend tuple_element_t<I, tuple<Ts_...>>&& get(tuple<Ts_...>&& t);

        template <typename T_, typename... ts_>
        friend T_& get(tuple<ts_...>& t);

        template <typename T_, typename... ts_>
        friend const T_& get(const tuple<ts_...>& t);

        template <typename T_, typename... ts_>
        friend T_&& get(tuple<ts_...>&& t);
    };

    // template specialization for an empty tuple
    template <>
    class tuple<>
    {
    public:
        void swap(tuple&) {}
    };

    template <size_t I, typename... Ts>
    inline tuple_element_t<I, tuple<Ts...>>& get(tuple<Ts...>& t)
    {
        return get<I>(t.mImpl);
    }

    template <size_t I, typename... Ts>
    inline const_tuple_element_t<I, tuple<Ts...>>& get(const tuple<Ts...>& t)
    {
        return get<I>(t.mImpl);
    }

    template <size_t I, typename... Ts>
    inline tuple_element_t<I, tuple<Ts...>>&& get(tuple<Ts...>&& t)
    {
        return get<I>(corsac::move(t.mImpl));
    }

    template <typename T, typename... Ts>
    inline T& get(tuple<Ts...>& t)
    {
        return get<T>(t.mImpl);
    }

    template <typename T, typename... Ts>
    inline const T& get(const tuple<Ts...>& t)
    {
        return get<T>(t.mImpl);
    }

    template <typename T, typename... Ts>
    inline T&& get(tuple<Ts...>&& t)
    {
        return get<T>(corsac::move(t.mImpl));
    }

    template <typename... Ts>
    inline void swap(tuple<Ts...>& a, tuple<Ts...>& b)
    {
        a.swap(b);
    }

    // tuple operators
    template <typename... T1s, typename... T2s>
    inline bool operator==(const tuple<T1s...>& t1, const tuple<T2s...>& t2)
    {
        return internal::TupleEqual<sizeof...(T1s)>()(t1, t2);
    }

    template <typename... T1s, typename... T2s>
    inline bool operator<(const tuple<T1s...>& t1, const tuple<T2s...>& t2)
    {
        return internal::TupleLess<sizeof...(T1s)>()(t1, t2);
    }

    template <typename... T1s, typename... T2s> inline bool operator!=(const tuple<T1s...>& t1, const tuple<T2s...>& t2) { return !(t1 == t2); }
    template <typename... T1s, typename... T2s> inline bool operator> (const tuple<T1s...>& t1, const tuple<T2s...>& t2) { return t2 < t1; }
    template <typename... T1s, typename... T2s> inline bool operator<=(const tuple<T1s...>& t1, const tuple<T2s...>& t2) { return !(t2 < t1); }
    template <typename... T1s, typename... T2s> inline bool operator>=(const tuple<T1s...>& t1, const tuple<T2s...>& t2) { return !(t1 < t2); }

    // tuple_cat
    template <typename... Tuples>
    inline typename internal::TupleCat<Tuples...>::ResultType tuple_cat(Tuples&&... ts)
    {
        return internal::TupleCat<Tuples...>::DoCat(corsac::forward<Tuples>(ts)...);
    }

    // make_tuple
    template <typename... Ts>
    inline constexpr tuple<internal::MakeTupleReturn_t<Ts>...> make_tuple(Ts&&... values)
    {
    return tuple<internal::MakeTupleReturn_t<Ts>...>(corsac::forward<Ts>(values)...);
    }

    // forward_as_tuple
    template <typename... Ts>
    inline constexpr tuple<Ts&&...> forward_as_tuple(Ts&&... ts) noexcept
    {
        return tuple<Ts&&...>(corsac::forward<Ts&&>(ts)...);
    }


    /*
    * ignore
    *
    * Объект неопределенного типа, которому может быть присвоено любое значение без какого-либо эффекта.
    */
    namespace internal
    {
        struct ignore_t
        {
            ignore_t() = default;

            template <typename T>
            const ignore_t& operator=(const T&) const
            {
                return *this;
            }
        };
    }// namespace internal

    static const internal::ignore_t ignore;

    /**
    * tie
    *
    * Создает кортеж lvalue-ссылок на свои аргументы или экземпляры corsac::ignore.
    */
    template <typename... Ts>
    inline constexpr tuple<Ts&...> tie(Ts&... ts) noexcept
    {
        return tuple<Ts&...>(ts...);
    }

    /**
    * apply
    *
    * Вызвать вызываемый объект, используя кортеж для предоставления аргументов.
    */
    namespace detail
    {
        template <class F, class Tuple, size_t... I>
        constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, index_sequence<I...>)
        {
            return invoke(corsac::forward<F>(f), get<I>(corsac::forward<Tuple>(t))...);
        }
    } // namespace detail

    template <class F, class Tuple>
    constexpr decltype(auto) apply(F&& f, Tuple&& t)
    {
        return detail::apply_impl(corsac::forward<F>(f), corsac::forward<Tuple>(t),
                                  make_index_sequence<tuple_size_v<remove_reference_t<Tuple>>>{});
    }
}

CORSAC_RESTORE_VC_WARNING()
CORSAC_RESTORE_VC_WARNING()
CORSAC_RESTORE_VC_WARNING()
#endif //CORSAC_STL_TUPLE_H
