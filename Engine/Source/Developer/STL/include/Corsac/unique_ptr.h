/**
 * corsac::STL
 *
 * unique_ptr.h
 *
 * Created by Falldot on 18.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_UNIQUE_PTR_H
#define CORSAC_STL_UNIQUE_PTR_H

#pragma once

#include "Corsac/STL/config.h"
#include "Corsac/STL/help_move.h"
#include "Corsac/STL/smart_ptr.h"
#include "Corsac/type_traits.h"
#include "Corsac/utility.h"
#include "Corsac/functional.h"
#include "Corsac/compressed_pair.h"

namespace corsac
{
    /**
    * class unique_ptr
    *
    * Этот класс реализует шаблон unique_ptr. Это класс, похожий на шаблон C ++ auto_ptr,
    * за исключением того, что в целях безопасности он запрещает копирование самого себя.
    *
    * В частности, шаблон класса unique_ptr хранит указатель на динамически выделяемый
    * объект. Указанный объект автоматически удаляется деструктором unique_ptr или может
    * быть удален вручную с помощью функции unique_ptr::reset.
    *
    * Примечания по распределению памяти:
    * unique_ptr не выделяет память; все выделенные указатели выводятся извне. unique_ptr
    * освобождает память, но всегда с помощью средства удаления, предоставленного
    * пользователем. Вам нужно убедиться, что вы последовательно предоставляете средство
    * удаления, которое освобождает память в соответствии с тем, как она была
    * первоначально выделена. Удалители имеют информацию об экземплярах и перемещаются
    * между контейнерами так же, как выделенные указатели. Таким образом, вы можете
    * выделить память через некоторую кучу и предоставить средство удаления, которое
    * содержит указатель на ту же самую кучу, и независимо от того, что вы делаете с
    * unique_ptr, включая перемещение его на другой unique_ptr, при удалении будет
    * использоваться первоначально предоставленная куча.
    *
    * Пример использования:
    *     unique_ptr<int> p(new int);
    *     *p = 4;
    *
    *     unique_ptr<int[]> pArray(new int[4]);
    *     p[0] = 4;
    */
    template <typename T, typename Deleter = corsac::default_delete<T> >
    class unique_ptr
    {
        static_assert(!is_rvalue_reference<Deleter>::value, "The supplied Deleter cannot be a r-value reference.");
    public:
        using deleter_type  = Deleter;
        using element_type  = T;
        using this_type     = unique_ptr<element_type, deleter_type>;
        using pointer       = typename internal::unique_pointer_type<element_type, deleter_type>::type;

    public:
        constexpr unique_ptr() noexcept
                : mPair(pointer())
        {
            static_assert(!corsac::is_pointer<deleter_type>::value, "unique_ptr deleter default-constructed with null pointer. Use a different constructor or change your deleter to a class.");
        }

        constexpr unique_ptr(std::nullptr_t) noexcept
                : mPair(pointer())
        {
            static_assert(!corsac::is_pointer<deleter_type>::value, "unique_ptr deleter default-constructed with null pointer. Use a different constructor or change your deleter to a class.");
        }

        explicit unique_ptr(pointer pValue) noexcept
                : mPair(pValue)
        {
            static_assert(!corsac::is_pointer<deleter_type>::value, "unique_ptr deleter default-constructed with null pointer. Use a different constructor or change your deleter to a class.");
        }

        unique_ptr(pointer pValue, typename corsac::conditional<corsac::is_lvalue_reference<deleter_type>::value, deleter_type, typename corsac::add_lvalue_reference<const deleter_type>::type>::type deleter) noexcept
                : mPair(pValue, deleter) {}


        unique_ptr(pointer pValue, typename corsac::remove_reference<deleter_type>::type&& deleter) noexcept
                : mPair(pValue, corsac::move(deleter))
        {
            static_assert(!corsac::is_lvalue_reference<deleter_type>::value, "deleter_type reference refers to an rvalue deleter. The reference will probably become invalid before used. Change the deleter_type to not be a reference or construct with permanent deleter.");
        }

        unique_ptr(this_type&& x) noexcept
                : mPair(x.release(), corsac::forward<deleter_type>(x.get_deleter())) {}

        template <typename U, typename E>
        unique_ptr(unique_ptr<U, E>&& u, typename enable_if<!is_array<U>::value && is_convertible<typename unique_ptr<U, E>::pointer, pointer>::value && is_convertible<E, deleter_type>::value && (is_same<deleter_type, E>::value || !is_lvalue_reference<deleter_type>::value)>::type* = 0) noexcept
                : mPair(u.release(), corsac::forward<E>(u.get_deleter())) {}

        this_type& operator=(this_type&& x) noexcept
        {
            reset(x.release());
            mPair.second() = corsac::move(corsac::forward<deleter_type>(x.get_deleter()));
            return *this;
        }

        template <typename U, typename E>
        typename enable_if<!is_array<U>::value && is_convertible<typename unique_ptr<U, E>::pointer, pointer>::value && is_assignable<deleter_type&, E&&>::value, this_type&>::type
        operator=(unique_ptr<U, E>&& u) noexcept
        {
            reset(u.release());
            mPair.second() = corsac::move(corsac::forward<E>(u.get_deleter()));
            return *this;
        }

        // operator=(nullptr_t)
        this_type& operator=(std::nullptr_t) noexcept
        {
            reset();
            return *this;
        }

        ~unique_ptr() noexcept
        {
            reset();
        }

        void reset(pointer pValue = pointer()) noexcept
        {
            if (pValue != mPair.first())
            {
                if (auto first = corsac::exchange(mPair.first(), pValue))
                    get_deleter()(first);
            }
        }

        pointer release() noexcept
        {
            pointer const pTemp = mPair.first();
            mPair.first() = pointer();
            return pTemp;
        }

        void swap(this_type& x) noexcept
        {
            mPair.swap(x.mPair);
        }

        typename add_lvalue_reference<T>::type operator*() const // Не noexcept, потому что указатель может быть NULL.
        {
            return *mPair.first();
        }

        pointer operator->() const noexcept
        {
            return mPair.first();
        }

        pointer get() const noexcept
        {
            return mPair.first();
        }

        deleter_type& get_deleter() noexcept
        {
            return mPair.second();
        }

        const deleter_type& get_deleter() const noexcept
        {
            return mPair.second();
        }

        explicit operator bool() const noexcept
        {
            return (mPair.first() != pointer());
        }

        /// Эти функции удалены, чтобы предотвратить копирование.
        unique_ptr(const this_type&) = delete;
        unique_ptr& operator=(const this_type&) = delete;
        unique_ptr& operator=(pointer pValue) = delete;

    protected:
        corsac::compressed_pair<pointer, deleter_type> mPair;
    }; // class unique_ptr

    /**
    * unique_ptr для неограниченных массивов.
    *
    * Отличия от unique_ptr<T>:
    *     - Преобразования между различными типами unique_ptr<T[], D> или в или из
    *       не-массивных форм unique_ptr создают плохо сформированную программу.
    *     - Указатели на типы, производные от T, отклоняются конструкторами и сбрасываются.
    *     - Операторы наблюдателей и оператор-> не предусмотрены.
    *     - Предоставляется оператор[] наблюдателя индексации.
    *     - Средство удаления по умолчанию будет вызывать delete[].
    *
    * Невозможно создать unique_ptr для массивов с известной границей (например, int[4] вместо int[]).
    *
    * Пример использования:
    *     unique_ptr<int[]> ptr(new int[10]);
    *     ptr[4] = 4;
    */
    template <typename T, typename Deleter>
    class unique_ptr<T[], Deleter>
    {
    public:
        using deleter_type  = Deleter;
        using element_type  = T;
        using this_type     = unique_ptr<element_type[], deleter_type>;
        using pointer       = typename internal::unique_pointer_type<element_type, deleter_type>::type;

    public:
        constexpr unique_ptr() noexcept
                : mPair(pointer())
        {
            static_assert(!corsac::is_pointer<deleter_type>::value, "unique_ptr deleter default-constructed with null pointer. Use a different constructor or change your deleter to a class.");
        }

        constexpr unique_ptr(std::nullptr_t) noexcept
                : mPair(pointer())
        {
            static_assert(!corsac::is_pointer<deleter_type>::value, "unique_ptr deleter default-constructed with null pointer. Use a different constructor or change your deleter to a class.");
        }

        template <typename P,
                typename = corsac::enable_if_t<internal::is_array_cv_convertible<P, pointer>::value>> // Указатели на типы, производные от T, отклоняются конструкторами и сбрасываются.
        explicit unique_ptr(P pArray) noexcept
                : mPair(pArray)
        {
            static_assert(!corsac::is_pointer<deleter_type>::value,
                          "unique_ptr deleter default-constructed with null pointer. Use a different constructor or "
                          "change your deleter to a class.");
        }

        template <typename P>
        unique_ptr(P pArray, typename corsac::conditional<corsac::is_lvalue_reference<deleter_type>::value, deleter_type,
                typename corsac::add_lvalue_reference<const deleter_type>::type>::type deleter,
                   typename corsac::enable_if<internal::is_array_cv_convertible<P, pointer>::value>::type* = 0) noexcept
                : mPair(pArray, deleter) {}

        template <typename P>
        unique_ptr(P pArray, typename corsac::remove_reference<deleter_type>::type&& deleter, corsac::enable_if_t<internal::is_array_cv_convertible<P, pointer>::value>* = 0) noexcept
                : mPair(pArray, corsac::move(deleter))
        {
            static_assert(!corsac::is_lvalue_reference<deleter_type>::value, "deleter_type reference refers to an rvalue deleter. The reference will probably become invalid before used. Change the deleter_type to not be a reference or construct with permanent deleter.");
        }

        unique_ptr(this_type&& x) noexcept
                : mPair(x.release(), corsac::forward<deleter_type>(x.get_deleter())) {}

        template <typename U, typename E>
        unique_ptr(unique_ptr<U, E>&& u, typename enable_if<internal::is_safe_array_conversion<T, pointer, U, typename unique_ptr<U, E>::pointer>::value &&
                                                            corsac::is_convertible<E, deleter_type>::value &&
                                                            (!corsac::is_lvalue_reference<deleter_type>::value || corsac::is_same<E, deleter_type>::value)>::type* = 0) noexcept
                : mPair(u.release(), corsac::forward<E>(u.get_deleter())) {}

        this_type& operator=(this_type&& x) noexcept
        {
            reset(x.release());
            mPair.second() = corsac::move(corsac::forward<deleter_type>(x.get_deleter()));
            return *this;
        }

        template <typename U, typename E>
        typename enable_if<internal::is_safe_array_conversion<T, pointer, U, typename unique_ptr<U, E>::pointer>::value && is_assignable<deleter_type&, E&&>::value, this_type&>::type
        operator=(unique_ptr<U, E>&& u) noexcept
        {
            reset(u.release());
            mPair.second() = corsac::move(corsac::forward<E>(u.get_deleter()));
            return *this;
        }

        this_type& operator=(std::nullptr_t) noexcept
        {
            reset();
            return *this;
        }

        ~unique_ptr() noexcept
        {
            reset();
        }

        void reset(pointer pArray = pointer()) noexcept
        {
            if(pArray != mPair.first())
            {
                if (auto first = corsac::exchange(mPair.first(), pArray))
                    get_deleter()(first);
            }
        }

        pointer release() noexcept
        {
            pointer const pTemp = mPair.first();
            mPair.first() = pointer();
            return pTemp;
        }

        void swap(this_type& x) noexcept
        {
            mPair.swap(x.mPair);
        }

        typename add_lvalue_reference<T>::type operator[](ptrdiff_t i) const
        {
            // assert(mpArray && (i >= 0));
            return mPair.first()[i];
        }

        pointer get() const noexcept
        {
            return mPair.first();
        }

        deleter_type& get_deleter() noexcept
        {
            return mPair.second();
        }

        const deleter_type& get_deleter() const noexcept
        {
            return mPair.second();
        }

        explicit operator bool() const noexcept
        {
            return (mPair.first() != pointer());
        }

        // Эти функции удалены, чтобы предотвратить копирование.
        unique_ptr(const this_type&) = delete;
        unique_ptr& operator=(const this_type&) = delete;
        unique_ptr& operator=(pointer pArray) = delete;

    protected:
        corsac::compressed_pair<pointer, deleter_type> mPair;
    };

    /// make_unique
    ///
    /// В стандарте C ++ 11 нет make_unique, но нет согласованной причины, почему.
    /// Мы делаем то же самое, что и libc++, и делаем специализацию make_unique для массивов.
    ///
    /// У make_unique есть два случая, когда вы не можете его использовать и вам нужно напрямую использовать unique_ptr:
    ///     - Вам нужно создать unique_ptr с необработанным указателем.
    ///     - Вам необходимо указать настраиваемое средство удаления.
    ///
    /// Note: Эта функция по умолчанию использует глобальный new T для создания
    /// экземпляра ptr в соответствии со стандартом C++ 11 make_shared_ptr.
    ///
    /// Пример использования:
    ///     struct Test{ Test(int, int){} };
    ///     auto p = make_unique<Test>(1, 2);
    ///
    ///     auto pArray = make_unique<Test[]>(4);
    ///
    namespace internal
    {
        template <typename T>
        struct unique_type
        { using unique_type_single = unique_ptr<T>; };

        template <typename T>
        struct unique_type<T[]>
        { using unique_type_unbounded_array = unique_ptr<T[]>; };

        template <typename T, size_t N>
        struct unique_type<T[N]>
        { using unique_type_bounded_array = void; };
    }

    template <typename T, typename... Args>
    inline typename internal::unique_type<T>::unique_type_single make_unique(Args&&... args)
    { return unique_ptr<T>(new T(corsac::forward<Args>(args)...)); }

    template <typename T>
    inline typename internal::unique_type<T>::unique_type_unbounded_array make_unique(size_t n)
    {
        using TBase = typename corsac::remove_extent<T>::type;
        return unique_ptr<T>(new TBase[n]);
    }

    // Невозможно создать unique_ptr для массивов с известной границей (например, int[4] вместо int[]).
    template <typename T, typename... Args>
    typename internal::unique_type<T>::unique_type_bounded_array
    make_unique(Args&&...) = delete;

    // хеш-специализация для unique_ptr.
    // Он просто возвращает corsac::hash (x.get()). Если ваш тип указателя unique_ptr (возвращаемое значение unique_ptr<T>::get)
    // является настраиваемым типом, а не типом встроенного указателя, вам нужно будет независимо определить corsac::hash для этого типа.
    template <typename T, typename D>
    struct hash< unique_ptr<T, D> >
    {
        size_t operator()(const unique_ptr<T, D>& x) const noexcept
        { return corsac::hash<typename unique_ptr<T, D>::pointer>()(x.get()); }
    };

    // swap
    // Обменивает принадлежащий указатель между двумя объектами unique_ptr. Эта версия,
    // не являющаяся членом, полезна для совместимости объектов unique_ptr со стандартной
    // библиотекой C++ и другими библиотеками.
    template <typename T, typename D>
    inline void swap(unique_ptr<T, D>& a, unique_ptr<T, D>& b) noexcept
    {
        a.swap(b);
    }

    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator==(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        return (a.get() == b.get());
    }

    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator!=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        return !(a.get() == b.get());
    }

    /// Возвращает значение unique_ptr «меньше» другого. Полезно при хранении
    /// отсортированных контейнеров объектов unique_ptr.
    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator<(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        using P1        = typename corsac::unique_ptr<T1, D1>::pointer;
        using P2        = typename corsac::unique_ptr<T2, D2>::pointer;
        using PCommon   = typename corsac::common_type<P1, P2>::type;
        PCommon pT1 = a.get();
        PCommon pT2 = b.get();
        return less<PCommon>()(pT1, pT2);
    }

    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator>(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        return (b < a);
    }

    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator<=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        return !(b < a);
    }

    template <typename T1, typename D1, typename T2, typename D2>
    inline bool operator>=(const unique_ptr<T1, D1>& a, const unique_ptr<T2, D2>& b)
    {
        return !(a < b);
    }

    template <typename T, typename D>
    inline bool operator==(const unique_ptr<T, D>& a, std::nullptr_t) noexcept
    {
        return !a;
    }

    template <typename T, typename D>
    inline bool operator==(std::nullptr_t, const unique_ptr<T, D>& a) noexcept
    {
        return !a;
    }

    template <typename T, typename D>
    inline bool operator!=(const unique_ptr<T, D>& a, std::nullptr_t) noexcept
    {
        return static_cast<bool>(a);
    }

    template <typename T, typename D>
    inline bool operator!=(std::nullptr_t, const unique_ptr<T, D>& a) noexcept
    {
        return static_cast<bool>(a);
    }

    template <typename T, typename D>
    inline bool operator<(const unique_ptr<T, D>& a, std::nullptr_t)
    {
        using pointer = typename unique_ptr<T, D>::pointer;
        return less<pointer>()(a.get(), nullptr);
    }

    template <typename T, typename D>
    inline bool operator<(std::nullptr_t, const unique_ptr<T, D>& b)
    {
        using pointer = typename unique_ptr<T, D>::pointer;
        pointer pT = b.get();
        return less<pointer>()(nullptr, pT);
    }

    template <typename T, typename D>
    inline bool operator>(const unique_ptr<T, D>& a, std::nullptr_t)
    {
        return (nullptr < a);
    }

    template <typename T, typename D>
    inline bool operator>(std::nullptr_t, const unique_ptr<T, D>& b)
    {
        return (b < nullptr);
    }

    template <typename T, typename D>
    inline bool operator<=(const unique_ptr<T, D>& a, std::nullptr_t)
    {
        return !(nullptr < a);
    }

    template <typename T, typename D>
    inline bool operator<=(std::nullptr_t, const unique_ptr<T, D>& b)
    {
        return !(b < nullptr);
    }

    template <typename T, typename D>
    inline bool operator>=(const unique_ptr<T, D>& a, std::nullptr_t)
    {
        return !(a < nullptr);
    }

    template <typename T, typename D>
    inline bool operator>=(std::nullptr_t, const unique_ptr<T, D>& b)
    {
        return !(nullptr < b);
    }
}


#endif //CORSAC_STL_UNIQUE_PTR_H
