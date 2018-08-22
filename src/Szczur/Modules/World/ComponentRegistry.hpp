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
    using Functor_t    = bool(*)(Registry_t&, EntityID_t);
    using FunctorMap_t = std::unordered_map<Key_t, Functor_t>;

    ///
    template <typename T>
    static void registerComponent(HashedID hid);

    ///
    static bool assignComponent(Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool hasComponent(Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool removeComponent(Registry_t& registry, EntityID_t id, HashedID hid);

private:

    ///
    static bool _call(const FunctorMap_t& map, Registry_t& registry, EntityID_t id, HashedID hid);

    inline static FunctorMap_t _assignComponent = {};
    inline static FunctorMap_t _hasComponent = {};
    inline static FunctorMap_t _removeComponent = {};

};

template <typename T>
void ComponentRegistry::registerComponent(HashedID hid)
{
    const auto key = hid.hash;

    _assignComponent.emplace(key, [](Registry_t& reg, EntityID_t id) {
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

    _hasComponent.emplace(key, [](Registry_t& reg, EntityID_t id) {
        return reg.has<T>(id);
    });

    _removeComponent.emplace(key, [](Registry_t& reg, EntityID_t id) {
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

}
