/**
 * corsac::STL
 *
 * compressed_pair.h
 *
 * Created by Falldot on 25.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_COMPRESSED_PAIR_H
#define CORSAC_COMPRESSED_PAIR_H

#pragma once
/**
 * Описание (Falldot 25.11.2021)
 *
 * Сжатый класс пары очень похож на std :: pair, но если какой-либо из аргументов шаблона является
 * пустым классом, то для сжатия размера пары применяется «оптимизация пустого базового класса».
 *
 * Дизайн compressed_pair здесь очень похож на тот, который можно найти в библиотеках
 * метапрограммирования шаблонов, таких как Boost, GCC и Metrowerks, учитывая, что эти
 * библиотеки установили этот интерфейс как стандарт де-факто для решения этой проблемы.
 * Кроме того, они описаны в различных книгах по теме метапрограммирования шаблонов,
 * таких как «Современный дизайн C ++».
 *
 * Доступ к двум членам пары можно получить с помощью функций-членов first() и second().
 * Обратите внимание, что не все функции-члены могут быть созданы для всех типов параметров
 * шаблона. В частности, compressed_pair может быть создан для ссылочных типов и типов массивов,
 * однако в этих случаях диапазон конструкторов, которые можно использовать, ограничен. Если
 * типы T1 и T2 относятся к одному типу, то существует только одна версия конструктора с одним
 * аргументом, и этот конструктор инициализирует оба значения в паре переданным значением.
 *
 * Обратите внимание, что compressed_pair не может быть создан, если какой-либо из аргументов
 * шаблона является типом объединения, если компилятор не поддерживает is_union, или если
 * is_union специализируется на типе объединения.
 */

#include "Corsac/internal/config.h"
#include "Corsac/type_traits.h"
#include "Corsac/call_traits.h"

namespace corsac
{
    template <typename T1, typename T2>
    class compressed_pair;

    template <typename T1, typename T2, bool isSame, bool firstEmpty, bool secondEmpty>
    struct compressed_pair_switch;

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, false, false, false>{ static const int value = 0; };

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, false, true, false> { static const int value = 1; };

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, false, false, true> { static const int value = 2; };

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, false, true, true>  { static const int value = 3; };

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, true, true, true>   { static const int value = 4; };

    template <typename T1, typename T2>
    struct compressed_pair_switch<T1, T2, true, false, false> { static const int value = 5; };

    template <typename T1, typename T2, int version>
    class compressed_pair_imp;

    template <typename T>
    inline void cp_swap(T& t1, T& t2)
    {
        T tTemp = t1;
        t1 = t2;
        t2 = tTemp;
    }

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 0>
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type y)
                : mFirst(x), mSecond(y) {}

        explicit compressed_pair_imp(first_param_type x)
                : mFirst(x) {}

        explicit compressed_pair_imp(second_param_type y)
                : mSecond(y) {}

        first_reference       first()       { return mFirst; }
        first_const_reference first() const { return mFirst; }

        second_reference       second()       { return mSecond; }
        second_const_reference second() const { return mSecond; }

        void swap(compressed_pair<T1, T2>& y) noexcept
        {
            cp_swap(mFirst, y.first());
            cp_swap(mSecond, y.second());
        }

    private:
        first_type  mFirst;
        second_type mSecond;
    };

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 1> : private T1
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type y)
                : first_type(x), mSecond(y) {}

        explicit compressed_pair_imp(first_param_type x)
                : first_type(x) {}

        explicit compressed_pair_imp(second_param_type y)
                : mSecond(y) {}

        first_reference       first()       { return *this; }
        first_const_reference first() const { return *this; }

        second_reference       second()       { return mSecond; }
        second_const_reference second() const { return mSecond; }

        void swap(compressed_pair<T1,T2>& y)
        {
            // Нет необходимости менять пустой базовый класс
            cp_swap(mSecond, y.second());
        }

    private:
        second_type mSecond;
    };

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 2> : private T2
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type y)
                : second_type(y), mFirst(x) {}

        explicit compressed_pair_imp(first_param_type x)
                : mFirst(x) {}

        explicit compressed_pair_imp(second_param_type y)
                : second_type(y) {}

        first_reference       first()       { return mFirst; }
        first_const_reference first() const { return mFirst; }

        second_reference       second()       { return *this; }
        second_const_reference second() const { return *this; }

        void swap(compressed_pair<T1,T2>& y) noexcept
        {
            // Нет необходимости менять пустой базовый класс
            cp_swap(mFirst, y.first());
        }

    private:
        first_type mFirst;
    };

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 3> : private T1, private T2
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type y)
                : first_type(x), second_type(y) {}

        explicit compressed_pair_imp(first_param_type x)
                : first_type(x) {}

        explicit compressed_pair_imp(second_param_type y)
                : second_type(y) {}

        first_reference       first()       { return *this; }
        first_const_reference first() const { return *this; }

        second_reference       second()       { return *this; }
        second_const_reference second() const { return *this; }

        // Не нужно менять
        void swap(compressed_pair<T1, T2>&) noexcept
        { /* Умышленно нереализовано... */ }
    };

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 4> : private T1
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type)
                : first_type(x) {}

        explicit compressed_pair_imp(first_param_type x)
                : first_type(x) {}

        first_reference       first()       { return *this; }
        first_const_reference first() const { return *this; }

        second_reference       second()       { return *this; }
        second_const_reference second() const { return *this; }

        void swap(compressed_pair<T1, T2>&) noexcept
        { /* Умышленно нереализовано... */ }
    };

    template <typename T1, typename T2>
    class compressed_pair_imp<T1, T2, 5>
    {
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair_imp() = default;

        compressed_pair_imp(first_param_type x, second_param_type y)
                : mFirst(x), mSecond(y) {}

        explicit compressed_pair_imp(first_param_type x)
                : mFirst(x), mSecond(x) {}

        first_reference       first()       { return mFirst; }
        first_const_reference first() const { return mFirst; }

        second_reference       second()       { return mSecond; }
        second_const_reference second() const { return mSecond; }

        void swap(compressed_pair<T1, T2>& y) noexcept
        {
            cp_swap(mFirst, y.first());
            cp_swap(mSecond, y.second());
        }

    private:
        first_type  mFirst;
        second_type mSecond;
    };

    template <typename T1, typename T2>
    class compressed_pair
            : private compressed_pair_imp<T1, T2,
                    compressed_pair_switch<
                            T1,
                            T2,
                            is_same<typename remove_cv<T1>::type, typename remove_cv<T2>::type>::value,
                            is_empty<T1>::value,
                            is_empty<T2>::value>::value>
    {
    private:
        using base = compressed_pair_imp<T1, T2,
                compressed_pair_switch<
                        T1,
                        T2,
                        is_same<typename remove_cv<T1>::type, typename remove_cv<T2>::type>::value,
                        is_empty<T1>::value,
                        is_empty<T2>::value>::value>;
    public:
        using first_type                = T1;
        using second_type               = T2;
        using first_param_type          = typename corsac::call_traits<first_type>::param_type;
        using second_param_type         = typename corsac::call_traits<second_type>::param_type;
        using first_reference           = typename corsac::call_traits<first_type>::reference;
        using second_reference          = typename corsac::call_traits<second_type>::reference;
        using first_const_reference     = typename corsac::call_traits<first_type>::const_reference;
        using second_const_reference    = typename corsac::call_traits<second_type>::const_reference;

        compressed_pair() : base() {}
        compressed_pair(first_param_type x, second_param_type y) : base(x, y) {}
        explicit compressed_pair(first_param_type x) : base(x) {}
        explicit compressed_pair(second_param_type y) : base(y) {}

        first_reference       first()       { return base::first(); }
        first_const_reference first() const { return base::first(); }

        second_reference       second()       { return base::second(); }
        second_const_reference second() const { return base::second(); }

        void swap(compressed_pair& y) noexcept { base::swap(y); }
    };

    template <typename T>
    class compressed_pair<T, T>
            : private compressed_pair_imp<T, T,
                    compressed_pair_switch<
                            T,
                            T,
                            is_same<typename remove_cv<T>::type, typename remove_cv<T>::type>::value,
                            is_empty<T>::value,
                            is_empty<T>::value>::value>
    {
    private:
        using base = compressed_pair_imp<T, T,
                compressed_pair_switch<
                        T,
                        T,
                        is_same<typename remove_cv<T>::type, typename remove_cv<T>::type>::value,
                        is_empty<T>::value,
                        is_empty<T>::value>::value>;
    public:
        using first_type                = T;
        using second_type               = T;
        using first_param_type          = typename call_traits<first_type>::param_type;
        using second_param_type         = typename call_traits<second_type>::param_type;
        using first_reference           = typename call_traits<first_type>::reference;
        using second_reference          = typename call_traits<second_type>::reference;
        using first_const_reference     = typename call_traits<first_type>::const_reference;
        using second_const_reference    = typename call_traits<second_type>::const_reference;

        compressed_pair() : base() {}
        compressed_pair(first_param_type x, second_param_type y) : base(x, y) {}
        explicit compressed_pair(first_param_type x) : base(x) {}

        first_reference       first()       { return base::first(); }
        first_const_reference first() const { return base::first(); }

        second_reference       second()       { return base::second(); }
        second_const_reference second() const { return base::second(); }

        void swap(compressed_pair<T, T>& y) noexcept { base::swap(y); }
    };


    template <typename T1, typename T2>
    inline void swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y) noexcept
    {
        x.swap(y);
    }
}

#endif //CORSAC_COMPRESSED_PAIR_H
