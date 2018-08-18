#pragma once

#include <entt/entt.hpp>

#include <unordered_map>
#include <utility>

#include "Entity.hpp"

namespace rat::wrd
{

class ComponentRegistry
{
public:

    using Key_t        = Hash_t;
    using Functor_t    = void(*)(entt::DefaultRegistry&, const entt::DefaultRegistry::entity_type);
    using FunctorMap_t = std::unordered_map<Key_t, Functor_t>;

    ///
    ComponentRegistry() = default;

    ///
    ComponentRegistry(const ComponentRegistry&) = delete;

    ///
    ComponentRegistry& operator = (const ComponentRegistry&) = delete;

    ///
    ComponentRegistry(ComponentRegistry&&) = delete;

    ///
    ComponentRegistry& operator = (ComponentRegistry&&) = delete;

    ///
    ~ComponentRegistry() = default;

    ///
    template <typename T>
    void registerComponent(HashedID hid);

    ///
    void assign(const Entity& entity, HashedID hid) const;

    ///
    void remove(const Entity& entity, HashedID hid) const;

private:

    ///
    void _call(const FunctorMap_t& _map, const Entity& entity, HashedID hid) const;

    FunctorMap_t _assign;
    FunctorMap_t _remove;

};

template <typename T>
void ComponentRegistry::registerComponent(HashedID hid)
{
    const auto key = hid.hash;

    _assign.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        if (reg.has<T>(id))
        {
            // TODO log attempt of adding already existing component
        }
        else
        {
            reg.assign<T>(id);
        }
    });

    _remove.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        if (reg.has<T>(id))
        {
            reg.remove<T>(id);
        }
        else
        {
            // TODO log attempt of deleting non-existing component
        }
    });
}

}
