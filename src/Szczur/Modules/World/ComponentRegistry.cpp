#include "ComponentRegistry.hpp"

namespace rat::wrd
{

bool ComponentRegistry::assignComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
    return _call(_assignComponent, registry, id, hid);
}

bool ComponentRegistry::hasComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
    return _call(_hasComponent, registry, id, hid);
}

bool ComponentRegistry::removeComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
    return _call(_removeComponent, registry, id, hid);
}

bool ComponentRegistry::componentToJson(Registry_t& registry, EntityID_t id, HashedID hid, nlohmann::json& j)
{
    // TODO log attempt of invoking nor-registered component/tag function, 99% probability to crash if so

    return std::invoke(_componentToJson.at(hid.hash), registry, id, j);
}

bool ComponentRegistry::allComponentsToJson(Registry_t& registry, EntityID_t id, nlohmann::json& j)
{
    nlohmann::json k;

    for (const Hash_t hash : _registeredIDs)
    {
        componentToJson(registry, id, hash, j);
    }

    if (!k.is_null())
    {
        j.push_back(k);
    }

    return true; // TODO return something meaningful
}

bool ComponentRegistry::componentFromJson(Registry_t& registry, EntityID_t id, const nlohmann::json& j)
{
    // TODO log attempt of invoking nor-registered component/tag function, 99% probability to crash if so

    return std::invoke(_componentFromJson.at(HashedID{ j["name"].get<Name_t>() }.hash), registry, id, j["data"]);
}

bool ComponentRegistry::_call(const FunctorMap_t& map, Registry_t& registry, EntityID_t id, HashedID hid)
{
    // TODO log attempt of invoking nor-registered component/tag function, 99% probability to crash if so

    return std::invoke(map.at(hid.hash), registry, id);
}

}
