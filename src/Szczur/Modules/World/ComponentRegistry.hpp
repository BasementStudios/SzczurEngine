#pragma once

#include <entt/entt.hpp>

#include <functional>
#include <unordered_map>
#include <utility>

#include "Scene.hpp"

namespace rat::wrd
{

class ComponentRegistry
{
public:

    using Key_t        = Hash_t;
    template <typename Ret>
    using Functor_t    = Ret(*)(entt::DefaultRegistry&, const entt::DefaultRegistry::entity_type);
    template <typename Ret>
    using FunctorMap_t = std::unordered_map<Key_t, Functor_t<Ret>>;

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
    bool assignComponent(const Entity& entity, HashedID hid) const;

    ///
    bool hasComponent(const Entity& entity, HashedID hid) const;

    ///
    bool removeComponent(const Entity& entity, HashedID hid) const;

    ///
    template <typename T>
    void registerTag(HashedID hid);

    ///
    bool assignTag(const Entity& entity, HashedID hid) const;

    ///
    bool hasTag(const Entity& entity, HashedID hid) const;

private:

    ///
    template <typename Ret>
    Ret _call(const FunctorMap_t<Ret>& _map, const Entity& entity, HashedID hid) const;

    FunctorMap_t<bool> _assignComponent;
    FunctorMap_t<bool> _hasComponent;
    FunctorMap_t<bool> _removeComponent;

    FunctorMap_t<bool> _assignTag;
    FunctorMap_t<bool> _hasTag;

};

template <typename T>
void ComponentRegistry::registerComponent(HashedID hid)
{
    const auto key = hid.hash;

    _assignComponent.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        if (reg.has<T>(id))
        {
            return false;

            // TODO log attempt of adding already existing component
        }
        else
        {
            reg.assign<T>(id);

            return true;
        }
    });

    _hasComponent.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        return reg.has<T>(id);
    });

    _removeComponent.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        if (reg.has<T>(id))
        {
            reg.remove<T>(id);

            return true;
        }
        else
        {
            // TODO log attempt of deleting non-existing component

            return false;
        }
    });
}

template <typename T>
void ComponentRegistry::registerTag(HashedID hid)
{
    const auto key = hid.hash;

    _assignTag.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        if (reg.has<T>())
        {
            // TODO log attempt of transfering tag ownership

            reg.move<T>(id);

            return true;
        }
        else
        {
            reg.assign<T>(entt::tag_t{}, id);

            return true;
        }
    });

    _hasTag.emplace(key, [](entt::DefaultRegistry& reg, const entt::DefaultRegistry::entity_type id) {
        return reg.has<T>(entt::tag_t{}, id);
    });
}

template <typename Ret>
Ret ComponentRegistry::_call(const FunctorMap_t<Ret>& _map, const Entity& entity, HashedID hid) const
{
    const auto it = _map.find(hid.hash);

    if (it != _map.end())
    {
        return std::invoke(it->second, entity.getScene().getRegistry(), entity.getID());
    }

    // TODO log attempt of invoking nor-registered component/tag function

    if constexpr (!std::is_void_v<Ret>)
    {
        return {};
    }
}

}
