#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

#include <entt/entt.hpp>

#include <nlohmann/json.hpp>

#include "Scene.hpp"

namespace rat::wrd
{

class ComponentRegistry
{
public:

    using Key_t            = Hash_t;
    using Functor_t        = bool(*)(Registry_t&, EntityID_t);
    using FunctorMap_t     = std::unordered_map<Key_t, Functor_t>;
    using JsonFunctor_t    = bool(*)(Registry_t&, EntityID_t, const nlohmann::json&);
    using JsonFunctorMap_t = std::unordered_map<Key_t, JsonFunctor_t>;

    ///
    template <typename T>
    static void registerComponent(const char* name);

    ///
    static bool assignComponent(Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool hasComponent(Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool removeComponent(Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool assingComponentFromJson(Registry_t& registry, EntityID_t id, const nlohmann::json& j);

private:

    ///
    static bool _call(const FunctorMap_t& map, Registry_t& registry, EntityID_t id, HashedID hid);

    ///
    static bool _callWithJson(const JsonFunctorMap_t& map, Registry_t& registry, EntityID_t id, const nlohmann::json& j);

    inline static FunctorMap_t _assignComponent = {};
    inline static FunctorMap_t _hasComponent = {};
    inline static FunctorMap_t _removeComponent = {};
    inline static JsonFunctorMap_t _assingComponentFromJson = {};

};

template <typename T>
void ComponentRegistry::registerComponent(const char* name)
{
    const auto key = HashedID{ name }.hash;

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

    _assingComponentFromJson.emplace(key, [](Registry_t& reg, EntityID_t id, const nlohmann::json& j) {
        if (reg.has<T>(id))
        {
            return false;

            // TODO log attempt of adding already existing component
        }
        else
        {
            reg.assign<T>(id) = j;

            return true;
        }
    });
}

}
