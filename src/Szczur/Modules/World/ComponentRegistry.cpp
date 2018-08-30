#include "ComponentRegistry.hpp"

namespace rat::world
{

const char* ComponentRegistry::getComponentName(HashedID hid) const
{
	// TODO log attempt of invoking nor-registered component function

	return _componentDataMap.at(hid.hash).name;
}

bool ComponentRegistry::assignComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
	// TODO log attempt of invoking nor-registered component function

	return std::invoke(_componentDataMap.at(hid.hash).assign, _componentDataMap.at(hid.hash).name, registry, id);
}

bool ComponentRegistry::hasComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
	// TODO log attempt of invoking nor-registered component function

	return std::invoke(_componentDataMap.at(hid.hash).has, _componentDataMap.at(hid.hash).name, registry, id);
}

bool ComponentRegistry::removeComponent(Registry_t& registry, EntityID_t id, HashedID hid)
{
	// TODO log attempt of invoking nor-registered component function

	return std::invoke(_componentDataMap.at(hid.hash).remove, _componentDataMap.at(hid.hash).name, registry, id);
}

bool ComponentRegistry::componentToJson(Registry_t& registry, EntityID_t id, HashedID hid, nlohmann::json& j)
{
	// TODO log attempt of invoking nor-registered component function

	return std::invoke(_componentDataMap.at(hid.hash).toJson, _componentDataMap.at(hid.hash).name, registry, id, j);
}

bool ComponentRegistry::allComponentsToJson(Registry_t& registry, EntityID_t id, nlohmann::json& j)
{
	for (const auto& [ k, v ] : _componentDataMap)
	{
		componentToJson(registry, id, k, j);
	}

	return true; // TODO return something meaningful
}

bool ComponentRegistry::componentFromJson(Registry_t& registry, EntityID_t id, const char* name, const nlohmann::json& j)
{
	// TODO log attempt of invoking nor-registered component/tag function

	return std::invoke(_componentDataMap.at(HashedID{ name }.hash).fromJson, name, registry, id, j);
}

}
