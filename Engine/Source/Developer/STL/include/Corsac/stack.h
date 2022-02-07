/**
 * corsac::STL
 *
 * stack.h
 *
 * Created by Falldot on 03.12.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_STACK_H
#define CORSAC_STL_STACK_H

#pragma once

#include "Corsac/vector.h"

namespace corsac
{
    // CORSAC_STACK_DEFAULT_NAME
    #ifndef CORSAC_STACK_DEFAULT_NAME
        #define CORSAC_STACK_DEFAULT_NAME CORSAC_DEFAULT_NAME_PREFIX " stack"
    #endif

    // CORSAC_STACK_DEFAULT_ALLOCATOR
    #ifndef CORSAC_STACK_DEFAULT_ALLOCATOR
        #define CORSAC_STACK_DEFAULT_ALLOCATOR allocator_type(CORSAC_STACK_DEFAULT_NAME)
    #endif

    /**
    * stack
    *
    * stack - это класс адаптера, обеспечивающий интерфейс LIFO (последний пришел, первый ушел)
    * путем упаковки последовательности, которая обеспечивает как минимум следующие операции:
    *     push_back
    *     pop_back
    *     back
    */
    template <typename T, typename Container = corsac::vector<T> >
    class stack
    {
    public:
        using this_type         = stack<T, Container>;
        using container_type    = Container;
        using value_type        = typename Container::value_type;
        using reference         = typename Container::reference;
        using const_reference   = typename Container::const_reference;
        using size_type         = typename Container::size_type;

    public:               // Мы объявляем общедоступными, чтобы операторы глобального сравнения могли быть реализованы без добавления встроенного уровня и без возникновения ошибок объявления друга в GCC 2.x. GCC (по крайней мере, до версии 4.0) плохо справляется с встраиванием, и производительность побеждает правильность.
        container_type c; // Стандарт C++ указывает, что вы объявляете защищенную переменную-член типа Контейнер с именем 'c'.

    public:
        stack();

        // Здесь используется шаблон Allocator, потому что нам не разрешено выводить
        // allocator_type из контейнера, поскольку некоторые контейнеры (например, массив) не
        // имеют распределителей. Для контейнеров, у которых нет типов распределителя, вы
        // можете использовать void или char в качестве типа шаблона распределителя.

        template <class Allocator>
        explicit stack(const Allocator& allocator, typename corsac::enable_if<corsac::uses_allocator<container_type, Allocator>::value>::type* = NULL)
                : c(allocator)
        {}

        template <class Allocator>
        stack(const this_type& x, const Allocator& allocator, typename corsac::enable_if<corsac::uses_allocator<container_type, Allocator>::value>::type* = NULL)
                : c(x.c, allocator)
        {}

        template <class Allocator>
        stack(this_type&& x, const Allocator& allocator, typename corsac::enable_if<corsac::uses_allocator<container_type, Allocator>::value>::type* = NULL)
                : c(corsac::move(x.c), allocator)
        {}

        explicit stack(const container_type& x);
        explicit stack(container_type&& x);

        stack(std::initializer_list<value_type> ilist); // Первый элемент в списке инициализаторов помещается первым. C++ 11 не указывает, что std::stack поддерживает список инициализаторов.

        bool      empty() const;
        size_type size() const;

        reference       top();
        const_reference top() const;

        void push(const value_type& value);
        void push(value_type&& x);

        template <class... Args> decltype(auto) emplace(Args&&... args);

        void pop();

        container_type&       get_container();
        const container_type& get_container() const;

        void swap(this_type& x) noexcept;

        bool validate() const;

    }; // class stack

    template <typename T, typename Container>
    inline stack<T, Container>::stack()
            : c() // Для рассмотрения: используйте здесь c (CORSAC_STACK_DEFAULT_ALLOCATOR), хотя это добавило бы требование, чтобы контейнер, предоставленный пользователем, поддерживал это.
    {}

    template <typename T, typename Container>
    inline stack<T, Container>::stack(const Container& x)
            : c(x)
    {}

    template <typename T, typename Container>
    inline stack<T, Container>::stack(Container&& x)
            : c(corsac::move(x))
    {}

    template <typename T, typename Container>
    inline stack<T, Container>::stack(std::initializer_list<value_type> ilist)
            : c() // В качестве альтернативы мы могли бы использовать здесь c (ilist), но для этого у c должен быть конструктор ilist.
    {
        // Лучшее решение, но требует функции вставки.
        // c.insert(ilist.begin(), ilist.end());

        // Возможно более медленное решение, но не требует функции вставки.
        for(const auto& value : ilist)
        {
            c.push_back(value);
        }
    }

    template <typename T, typename Container>
    inline bool stack<T, Container>::empty() const
    {
        return c.empty();
    }

    template <typename T, typename Container>
    inline typename stack<T, Container>::size_type
    stack<T, Container>::size() const
    {
        return c.size();
    }

    template <typename T, typename Container>
    inline typename stack<T, Container>::reference
    stack<T, Container>::top()
    {
        return c.back();
    }

    template <typename T, typename Container>
    inline typename stack<T, Container>::const_reference
    stack<T, Container>::top() const
    {
        return c.back();
    }

    template <typename T, typename Container>
    inline void stack<T, Container>::push(const value_type& value)
    {
        c.push_back(const_cast<value_type&>(value)); // const_cast чтобы intrusive_list мог работать. Мы можем вернуться к этому.
    }

    template <typename T, typename Container>
    inline void stack<T, Container>::push(value_type&& x)
    {
        c.push_back(corsac::move(x));
    }

    template <typename T, typename Container>
    template <class... Args>
    inline decltype(auto) stack<T, Container>::emplace(Args&&... args)
    {
        return c.emplace_back(corsac::forward<Args>(args)...);
    }

    template <typename T, typename Container>
    inline void stack<T, Container>::pop()
    {
        c.pop_back();
    }

    template <typename T, typename Container>
    inline typename stack<T, Container>::container_type&
    stack<T, Container>::get_container()
    {
        return c;
    }

    template <typename T, typename Container>
    inline const typename stack<T, Container>::container_type&
    stack<T, Container>::get_container() const
    {
        return c;
    }


    template <typename T, typename Container>
    void stack<T, Container>::swap(this_type& x) noexcept
    {
            using corsac::swap;
            swap(c, x.c);
    }

    template <typename T, typename Container>
    bool stack<T, Container>::validate() const
    {
        return c.validate();
    }

    template <typename T, typename Container>
    inline bool operator==(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return (a.c == b.c);
    }

    template <typename T, typename Container>
    inline bool operator!=(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return !(a.c == b.c);
    }

    template <typename T, typename Container>
    inline bool operator<(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return (a.c < b.c);
    }

    template <typename T, typename Container>
    inline bool operator>(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return (b.c < a.c);
    }

    template <typename T, typename Container>
    inline bool operator<=(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return !(b.c < a.c);
    }

    template <typename T, typename Container>
    inline bool operator>=(const stack<T, Container>& a, const stack<T, Container>& b)
    {
        return !(a.c < b.c);
    }

    template <typename T, typename Container>
    inline void swap(stack<T, Container>& a, stack<T, Container>& b) noexcept
    {
            a.swap(b);
    }
} // corsac

#endif //CORSAC_STL_STACK_H
