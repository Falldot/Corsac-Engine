//
// Created by Falldot on 17.12.2021.
//

#ifndef CORSAC_ECS_COMPONENT_H
#define CORSAC_ECS_COMPONENT_H

#include "Corsac/sparse_set.h"
#include "Corsac/type_traits.h"

namespace corsac
{
    using EntityType = uint32_t;

    /**
     * ComponentContainerType
     *
     * Способы храния данных в компоненатных контейнерах:
     *      UNIQUE  - Только одна сущность может владеть этим компонентом.
     *      DYNAMIC - Память под данные выделяться динамически в heap.
     *      FIXED   - Память под данные выделяеться заранее в stack, но преодоление лимита будет увеличена емкость в heap.
     *      STATIC  - Память под данные выделяеться заранее в stack, расширение не возможно.
     */
    enum ComponentContainerType
    {
        SINGLE,
        DYNAMIC,
        FIXED,
        STATIC
    };

    namespace internal
    {
        enum ComponentType
        {
            AOS,
            SOA,
            TAG
        };

        template<ComponentContainerType C, size_t nodeCount, typename... Ts>
        struct Container {
            static_assert(sizeof...(Ts) == 0, "Invalid template argument!");

            static constexpr ComponentType type()
            {
                if constexpr(sizeof...(Ts) == 0)
                    return TAG;
                else if constexpr(sizeof...(Ts) == 1)
                    return SOA;
                else
                    return AOS;
            };

            using container = corsac::conditional_t<sizeof...(Ts) == 1, corsac::vector<Ts...>, corsac::tuple_vector<Ts...>>;

            template<size_t count, bool bEnableOverflow = true>
            using fixed_container = corsac::conditional_t<sizeof...(Ts) == 1, corsac::fixed_vector<Ts..., count, bEnableOverflow>,
                    corsac::fixed_tuple_vector<count, bEnableOverflow, Ts...>>;

            using value = corsac::conditional_t<
                    C == DYNAMIC,
                    container,
                    corsac::conditional_t<
                            C == FIXED,
                            fixed_container<nodeCount, true>,
                            corsac::conditional_t<
                                    C == STATIC,
                                    fixed_container<nodeCount, false>,
                                    corsac::false_type>>
            >;
            static_assert(!corsac::is_same_v<value, corsac::false_type>, "Invalid template argument!");
        };
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    class ComponentAoS : public sparse_set<EntityType, nodeCount, C != STATIC>
    {
        using Values = corsac::conditional_t<
                C == DYNAMIC,
                corsac::vector<T>,
                corsac::conditional_t<
                        C == FIXED,
                        corsac::fixed_vector<T, nodeCount, true>,
                        corsac::conditional_t<
                                C == STATIC,
                                corsac::fixed_vector<T, nodeCount, false>,
                                corsac::false_type
                        >
                >
        >;
        static_assert(
                !is_same_v<Values, corsac::false_type>,
                "ComponentAoS<ComponentContainerType> - invalid template argument"
        );

        using base_type                 = sparse_set<EntityType, nodeCount, C != STATIC>;
        using size_type                 = typename base_type::size_type;
        using pointer                   = T*;
        using const_pointer             = const T*;
        using value_type                = T;
        using reference                 = T&;
        using const_reference           = const T&;
        using iterator                  = T*;
        using const_iterator            = const T*;
        using reverse_iterator          = corsac::reverse_iterator<iterator>;
        using const_reverse_iterator    = corsac::reverse_iterator<const_iterator>;

    public:
        using base_type::packed;
        using base_type::sparse;
        using base_type::has;

    protected:
        Values values;

    public:
        ComponentAoS() noexcept;

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

        reference operator[](size_type n);
        const_reference operator[](size_type n) const;

        void resize(size_type n);
        void reserve(size_type n);

        void set_capacity(size_type n = Values::npos);
        void shrink_to_fit();

        pointer       data() noexcept;
        const_pointer data() const noexcept;

        reference       get(const EntityType& value);
        reference       get(EntityType&& value);
        const_reference get(const EntityType& value) const;
        const_reference get(EntityType&& value) const;

        void add(const EntityType& value) noexcept;
        void add(EntityType&& value) noexcept;
        void add(const EntityType& value, const value_type& data) noexcept;
        void add(EntityType&& value, value_type&& data) noexcept;

        void set(const EntityType& value) noexcept;
        void set(EntityType&& value) noexcept;
        void set(const EntityType& value, const value_type& data) noexcept;
        void set(EntityType&& value, value_type&& data) noexcept;

        void fit(const EntityType& value) noexcept;
        void fit(EntityType&& value) noexcept;
        void fit(const EntityType& value, const value_type& data) noexcept;
        void fit(EntityType&& value, value_type&& data) noexcept;

        void remove(const EntityType& value) noexcept;
        void remove(EntityType&& value) noexcept;

        void clear() noexcept;
        void reset_lose_memory() noexcept;
    };

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline ComponentAoS<C, nodeCount, T>::ComponentAoS() noexcept
        = default;

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::iterator
    ComponentAoS<C, nodeCount, T>::begin() noexcept
    {
        return values.begin();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_iterator
    ComponentAoS<C, nodeCount, T>::begin() const noexcept
    {
        return values.begin();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::iterator
    ComponentAoS<C, nodeCount, T>::end() noexcept
    {
        return values.end();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_iterator
    ComponentAoS<C, nodeCount, T>::end() const noexcept
    {
        return values.end();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reverse_iterator
    ComponentAoS<C, nodeCount, T>::rbegin() noexcept
    {
        return values.rbegin();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reverse_iterator
    ComponentAoS<C, nodeCount, T>::rbegin() const noexcept
    {
        return values.rbegin();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reverse_iterator
    ComponentAoS<C, nodeCount, T>::rend() noexcept
    {
        return values.rend();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reverse_iterator
    ComponentAoS<C, nodeCount, T>::rend() const noexcept
    {
        return values.rend();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::front()
    {
        return values.front();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::front() const
    {
        return values.front();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::back()
    {
        return values.back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::back() const
    {
        return values.back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::at(size_type n)
    {
        return n < packed.size() ? values[n] : nullptr;
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::at(size_type n) const
    {
        return n < packed.size() ? values[n] : nullptr;
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::operator[](size_type n)
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(n < packed.size()))
                    throw std::out_of_range("ComponentAoS::[] -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(n < packed.size()))
                CORSAC_FAIL_MSG("ComponentAoS::[] -- out of range");
        #endif
        return values[n];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::operator[](size_type n) const
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(n < packed.size()))
                        throw std::out_of_range("ComponentAoS::[] -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(n < packed.size()))
                CORSAC_FAIL_MSG("ComponentAoS::[] -- out of range");
        #endif
        return values[n];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::resize(size_type n)
    {
        packed.resize(n);
        sparse.resize(n);
        values.resize(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::reserve(size_type n)
    {
        packed.reserve(n);
        sparse.reserve(n);
        values.reserve(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::set_capacity(size_type n)
    {
        packed.set_capacity(n);
        sparse.set_capacity(n);
        values.set_capacity(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::shrink_to_fit()
    {
        packed.shrink_to_fit();
        sparse.shrink_to_fit();
        values.shrink_to_fit();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::pointer
    ComponentAoS<C, nodeCount, T>::data() noexcept
    {
        return values.data();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_pointer
    ComponentAoS<C, nodeCount, T>::data() const noexcept
    {
        return values.data();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::get(const EntityType& value)
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                throw std::out_of_range("ComponentAoS::get -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                CORSAC_FAIL_MSG("ComponentAoS::get -- out of range");
        #endif
        return values[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::reference
    ComponentAoS<C, nodeCount, T>::get(EntityType&& value)
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                throw std::out_of_range("ComponentAoS::get -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                CORSAC_FAIL_MSG("ComponentAoS::get -- out of range");
        #endif
        return values[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::get(const EntityType& value) const
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                throw std::out_of_range("ComponentAoS::get -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                CORSAC_FAIL_MSG("ComponentAoS::get -- out of range");
        #endif
        return values[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline typename ComponentAoS<C, nodeCount, T>::const_reference
    ComponentAoS<C, nodeCount, T>::get(EntityType&& value) const
    {
        #if CORSAC_EXCEPTIONS_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                throw std::out_of_range("ComponentAoS::get -- out of range");
        #elif CORSAC_ASSERT_ENABLED
            if(CORSAC_UNLIKELY(value >= sparse.size()))
                CORSAC_FAIL_MSG("ComponentAoS::get -- out of range");
        #endif
        return values[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::add(const EntityType &value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::add(EntityType &&value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::add(const EntityType &value, const value_type &data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back(data);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::add(EntityType &&value, value_type &&data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back(corsac::move(data));
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::set(const EntityType &value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            get(value) = T();
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::set(EntityType &&value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            get(corsac::move(value)) = T();
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::set(const EntityType &value, const value_type &data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            get(value) = T(data);
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back(data);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::set(EntityType &&value, value_type &&data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            get(corsac::move(value)) = T(corsac::move(data));
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back(corsac::move(data));
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::fit(const EntityType &value) noexcept
    {
        get(value) = T();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::fit(EntityType &&value) noexcept
    {
        get(corsac::move(value)) = T();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::fit(const EntityType &value, const value_type &data) noexcept
    {
        get(value) = T(data);
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::fit(EntityType &&value, value_type &&data) noexcept
    {
        get(corsac::move(value)) = T(corsac::move(data));
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::remove(const EntityType &value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            values[sparse[value]] = values.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
            values.pop_back();
        }
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::remove(EntityType &&value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            values[sparse[value]] = values.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
            values.pop_back();
        }
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::clear() noexcept
    {
        packed.clear();
        sparse.clear();
        values.clear();
    }

    template<ComponentContainerType C, size_t nodeCount, typename T>
    inline void ComponentAoS<C, nodeCount, T>::reset_lose_memory() noexcept
    {
        packed.reset_lose_memory();
        values.reset_lose_memory();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    class ComponentSoA : public sparse_set<EntityType, nodeCount, !(C == STATIC)>
    {
        using Values = corsac::conditional_t<
            C == DYNAMIC,
            corsac::tuple_vector<Ts...>,
            corsac::conditional_t<
                C == FIXED,
                corsac::fixed_tuple_vector<nodeCount, true, Ts...>,
                corsac::conditional_t<
                    C == STATIC,
                    corsac::fixed_tuple_vector<nodeCount, false, Ts...>,
                    corsac::false_type
                >
            >
        >;
        static_assert(!is_same_v<Values, corsac::false_type>,
                      "ComponentSoA<ComponentContainerType> - invalid template argument");

        using base_type                 = sparse_set<EntityType, nodeCount, !(C == STATIC)>;
        using size_type                 = typename base_type::size_type;

    public:
        using base_type::packed;
        using base_type::sparse;
        using base_type::has;
        Values values;

    protected:


    public:
        ComponentSoA() noexcept;

        auto front();
        auto front() const;

        auto back();
        auto back() const;

        template<size_t I>
        auto get();

        template<size_t I>
        auto get() const;

        template<size_t I>
        auto get(const EntityType& value);

        template<size_t I>
        auto get(EntityType&& value);

        template<size_t I>
        auto get(const EntityType& value) const;

        template<size_t I>
        auto get(EntityType&& value) const;

        auto operator[](size_type n);
        auto operator[](size_type n) const;

        void resize(size_type n);
        void reserve(size_type n);

        void set_capacity(size_type n = Values::npos);
        void shrink_to_fit();

        auto data() noexcept;
        auto data() const noexcept;

        void add(const EntityType& value) noexcept;
        void add(EntityType&& value) noexcept;

        template<typename ...Args>
        void add(const EntityType& value, Args&&... data) noexcept;

        template<typename ...Args>
        void add(EntityType&& value, Args&&... data) noexcept;

        void set(const EntityType& value) noexcept;
        void set(EntityType&& value) noexcept;

        template<typename ...Args>
        void set(const EntityType& value, Args&&... data) noexcept;

        template<typename ...Args>
        void set(EntityType&& value, Args&&... data) noexcept;

        void fit(const EntityType& value) noexcept;
        void fit(EntityType&& value) noexcept;

        template<typename ...Args>
        void fit(const EntityType& value, Args&&... data) noexcept;

        template<typename ...Args>
        void fit(EntityType&& value, Args&&... data) noexcept;

        void remove(const EntityType& value) noexcept;
        void remove(EntityType&& value) noexcept;

        void clear() noexcept;
        void reset_lose_memory() noexcept;
    };

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline ComponentSoA<C, nodeCount, Ts...>::ComponentSoA() noexcept
            = default;

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::front()
    {
        return values.front();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::front() const
    {
        return values.front();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::back()
    {
        return values.back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::back() const
    {
        return values.back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get()
    {
        return values.template get<I>();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get() const
    {
        return values.template get<I>();
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get(const EntityType& value)
    {
        return values.template get<I>()[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get(const EntityType& value) const
    {
        return values.template get<I>()[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get(EntityType&& value)
    {
        return values.template get<I>()[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    template<size_t I>
    auto ComponentSoA<C, nodeCount, Ts...>::get(EntityType&& value) const
    {
        return values.template get<I>()[sparse[value]];
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::operator[](size_type n)
    {
        return values[n];
    }

    template<ComponentContainerType C, size_t nodeCount, typename... Ts>
    auto ComponentSoA<C, nodeCount, Ts...>::operator[](size_type n) const
    {
        return values[n];
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::resize(size_type n)
    {
        packed.resize(n);
        sparse.resize(n);
        values.resize(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::reserve(size_type n)
    {
        packed.reserve(n);
        sparse.reserve(n);
        values.reserve(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::set_capacity(size_type n)
    {
        packed.set_capacity(n);
        sparse.set_capacity(n);
        values.set_capacity(n);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::shrink_to_fit()
    {
        packed.shrink_to_fit();
        sparse.shrink_to_fit();
        values.shrink_to_fit();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline auto ComponentSoA<C, nodeCount, Ts...>::data() noexcept
    {
        return values.mpBegin;
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline auto ComponentSoA<C, nodeCount, Ts...>::data() const noexcept
    {
        return values.mpBegin;
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::add(const EntityType &value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::add(EntityType &&value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::add(const EntityType &value, Args&&... data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back(data...);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::add(EntityType &&value, Args&&... data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            return;
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back(data...);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::set(const EntityType &value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            values.at(sparse[value]) = {};
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::set(EntityType &&value) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            values.at(sparse[value]) = {};
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::set(const EntityType &value, Args&&... data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            values.at(sparse[value]) = {data...};
        sparse[value] = packed.size();
        packed.push_back(value);
        values.push_back(data...);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::set(EntityType &&value, Args&&... data) noexcept
    {
        if (value >= sparse.size())
            sparse.resize(value * 2);
        else if (has(value))
            values.at(sparse[value]) = {data...};
        sparse[value] = packed.size();
        packed.push_back(corsac::move(value));
        values.push_back(data...);
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::fit(const EntityType &value) noexcept
    {
        values.at(sparse[value]) = {};
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::fit(EntityType &&value) noexcept
    {
        values.at(sparse[value]) = {};
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::fit(const EntityType &value, Args&&... data) noexcept
    {
        values.at(sparse[value]) = {data...};
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    template<typename ...Args>
    inline void ComponentSoA<C, nodeCount, Ts...>::fit(EntityType &&value, Args&&... data) noexcept
    {
        values.at(sparse[value]) = {data...};
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::remove(const EntityType &value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            values[sparse[value]] = values.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
            values.pop_back();
        }
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::remove(EntityType &&value) noexcept
    {
        if (has(value))
        {
            packed[sparse[value]] = packed.back();
            values[sparse[value]] = values.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
            values.pop_back();
        }
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::clear() noexcept
    {
        packed.clear();
        sparse.clear();
        values.clear();
    }

    template<ComponentContainerType C, size_t nodeCount, typename ...Ts>
    inline void ComponentSoA<C, nodeCount, Ts...>::reset_lose_memory() noexcept
    {
        packed.reset_lose_memory();
        values.reset_lose_memory();
    }

    template<ComponentContainerType C, size_t nodeCount>
    class ComponentTag : public sparse_set<EntityType, nodeCount, C != STATIC>
    {};

    template<typename... Ts>
    class SingleComponentSoA
    {

    };

    template<typename T>
    class SingleComponentAoS
    {

    };

    class SingleComponentTag
    {

    };

    template<typename... Ts>
    struct Component : public ComponentSoA<DYNAMIC, 0, Ts...>
    {
        template<ComponentContainerType C, size_t nodeCount = 0>
        using Config = corsac::conditional_t<C == SINGLE, SingleComponentSoA<Ts...>, ComponentSoA<C, nodeCount, Ts...>>;
    };

    template<typename T>
    struct Component<T> : public ComponentAoS<DYNAMIC, 0, T>
    {
        template<ComponentContainerType C, size_t nodeCount = 0>
        using Config = corsac::conditional_t<C == SINGLE, SingleComponentAoS<T>, ComponentAoS<C, nodeCount, T>>;
    };

    template<>
    struct Component<> : public ComponentTag<DYNAMIC, 0>
    {
        template<ComponentContainerType C, size_t nodeCount = 0>
        using Config = corsac::conditional_t<C == SINGLE, SingleComponentTag, ComponentTag<C, nodeCount>>;
    };
}

#endif //CORSAC_ECS_COMPONENT_H
