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

bool ComponentRegistry::assingComponentFromJson(Registry_t& registry, EntityID_t id, const nlohmann::json& j)
{
    return _callWithJson(_assingComponentFromJson, registry, id, j);
}

bool ComponentRegistry::_call(const FunctorMap_t& map, Registry_t& registry, EntityID_t id, HashedID hid)
{
    // TODO log attempt of invoking nor-registered component/tag function, 99% probability to crash if so

    return std::invoke(map.at(hid.hash), registry, id);
}

bool ComponentRegistry::_callWithJson(const JsonFunctorMap_t& map, Registry_t& registry, EntityID_t id, const nlohmann::json& j)
{
    // TODO log attempt of invoking nor-registered component/tag function, 99% probability to crash if so

    return std::invoke(map.at(HashedID{ j["name"].get<Name_t>() }.hash), registry, id, j["data"]);
}

}
