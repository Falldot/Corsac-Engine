//
// Created by Falldot on 19.12.2021.
//

#ifndef CORSAC_ECS_GROUP_H
#define CORSAC_ECS_GROUP_H

#pragma once

namespace corsac
{
    namespace internal
    {
        inline EntityType getNewEntityTypeID() noexcept
        {
            static EntityType lastID = 1;
            return lastID++;
        }

        template <class F, class... Args>
        inline void static_for(F&& f, Args&&... args)
        {
            (f(corsac::forward<Args>(args)), ...);
        }
    }

    template<auto& ...Group>
    class Entity
    {
        EntityType ID;
    public:
        Entity();

        explicit Entity(EntityType id);

        EntityType id();

        template<auto& Component>
        bool has();

        template<auto& Component, size_t I>
        decltype(auto) get();

        template<auto& Component>
        decltype(auto) get();

        template<auto& Component>
        decltype(auto) at();

        template<auto& Component>
        decltype(auto) add();

        template<auto& Component, typename ...Args>
        decltype(auto) add(Args&&... data);

        template<auto& Component>
        decltype(auto) set();

        template<auto& Component, typename ...Args>
        decltype(auto) set(Args&&... data);

        template<auto& Component>
        decltype(auto) fit();

        template<auto& Component, typename ...Args>
        decltype(auto) fit(Args&&... data);

        template<auto& Component>
        decltype(auto) remove();

        void destroy();
    };

    template<auto &...Group>
    Entity<Group...>::Entity()
    {
        ID = internal::getNewEntityTypeID();
        corsac::internal::static_for([this](auto& G)
        {
            G.add(ID);
        }, Group...);
    }

    template<auto &...Group>
    Entity<Group...>::Entity(EntityType id) : ID(id) {};

    template<auto &...Group>
    EntityType Entity<Group...>::id()
    {
        return ID;
    }

    template<auto &...Group>
    template<auto &Component>
    inline bool Entity<Group...>::has()
    {
        return Component.has(ID);
    }

    template<auto &...Group>
    template<auto &Component, size_t I>
    inline decltype(auto) Entity<Group...>::get()
    {
        return Component.get<I>(ID);
    }

    template<auto &...Group>
    template<auto &Component>
    inline decltype(auto) Entity<Group...>::get()
    {
        return Component.get(ID);
    }

    template<auto &...Group>
    template<auto &Component>
    inline decltype(auto) Entity<Group...>::at()
    {
        return Component.at(ID);
    }

    template<auto &...Group>
    template<auto &Component>
    inline decltype(auto) Entity<Group...>::add()
    {
        Component.add(ID);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto& Component, typename ...Args>
    inline decltype(auto) Entity<Group...>::add(Args&&... data)
    {
        Component.add(ID, data...);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto &Component>
    inline decltype(auto) Entity<Group...>::set()
    {
        Component.set(ID);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto& Component, typename ...Args>
    inline decltype(auto) Entity<Group...>::set(Args&&... data)
    {
        Component.set(ID, data...);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto &Component>
    inline decltype(auto) Entity<Group...>::fit()
    {
        Component.fit(ID);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto& Component, typename ...Args>
    inline decltype(auto) Entity<Group...>::fit(Args&&... data)
    {
        Component.fit(ID, data...);
        return Entity(ID);
    }

    template<auto &...Group>
    template<auto& Component>
    inline decltype(auto) Entity<Group...>::remove()
    {
        Component.remove(ID);
        return Entity(ID);
    }

    template<auto &...Group>
    inline void Entity<Group...>::destroy()
    {
        corsac::internal::static_for([this](auto& G)
        {
            G.remove(ID);
        }, Group...);
    }

    template<ComponentContainerType C, size_t nodeCount, auto&...Ts>
    struct ComponentGroup : public sparse_set<EntityType, nodeCount, C != STATIC>
    {
        using base_type = sparse_set<EntityType, nodeCount, C != STATIC>;

        using base_type::packed;
        using base_type::sparse;
        using base_type::has;

        inline void add(const EntityType& value) noexcept
        {
            if (value >= sparse.size())
                sparse.resize(value * 2);
            else if (has(value))
                return;
            sparse[value] = packed.size();
            packed.push_back(value);
            corsac::internal::static_for([this, &value](auto& v) {
                v.add(value);
            }, Ts...);
        }

        inline void remove(const EntityType& value)
        {
            if (!has(value))
                return;
            packed[sparse[value]] = packed.back();
            sparse[packed.back()] = sparse[value];
            packed.pop_back();
            corsac::internal::static_for([&value](auto& v) {
                v.remove(value);
            }, Ts...);
        }
    };

    template<auto&... Ts>
    struct Group : public ComponentGroup<DYNAMIC, 0, Ts...>
    {
        template<ComponentContainerType C, size_t nodeCount = 0>
        using Config = ComponentGroup<C, nodeCount, Ts...>;
    };
}

#endif //CORSAC_ECS_GROUP_H
