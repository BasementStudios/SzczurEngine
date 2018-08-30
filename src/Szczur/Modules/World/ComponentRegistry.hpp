#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

#include <entt/entt.hpp>

#include <nlohmann/json.hpp>

#include "Scene.hpp"

namespace rat::world
{

class ComponentRegistry
{
public:

	struct ComponentData
	{
		using Functor_t         = bool(*)(const char*, Registry_t&, EntityID_t);
		using ToJsonFunctor_t   = bool(*)(const char*, Registry_t&, EntityID_t, nlohmann::json&);
		using FromJsonFunctor_t = bool(*)(const char*, Registry_t&, EntityID_t, const nlohmann::json&);

		const char* name;

		Functor_t assign;
		Functor_t has;
		Functor_t remove;

		ToJsonFunctor_t toJson;
		FromJsonFunctor_t fromJson;
	};

	using Key_t              = Hash_t;
	using ComponentDataMap_t = std::unordered_map<Key_t, ComponentData>;

	///
	template <typename T>
	bool registerComponent(const char* name);

	///
	const char* getComponentName(HashedID hid) const;

	///
	bool assignComponent(Registry_t& registry, EntityID_t id, HashedID hid);

	///
	bool hasComponent(Registry_t& registry, EntityID_t id, HashedID hid);

	///
	bool removeComponent(Registry_t& registry, EntityID_t id, HashedID hid);

	///
	bool componentToJson(Registry_t& registry, EntityID_t id, HashedID hid, nlohmann::json& j);

	///
	bool allComponentsToJson(Registry_t& registry, EntityID_t id, nlohmann::json& j);

	///
	bool componentFromJson(Registry_t& registry, EntityID_t id, const char* name, const nlohmann::json& j);

private:

	ComponentDataMap_t _componentDataMap;

};

template <typename T>
bool ComponentRegistry::registerComponent(const char* name)
{
	const auto key = HashedID{ name }.hash;

	if (_componentDataMap.count(key))
	{
		// TODO log attempt of registering component more than once

		return false;
	}

	_componentDataMap.emplace(key, ComponentData{
		// name
		name,
		// assign
		[](const char*, Registry_t& reg, EntityID_t id) {
			if (reg.has<T>(id))
			{
				// TODO log attempt of adding already existing component

				return false;
			}
			else
			{
				reg.assign<T>(id);

				return true;
			}
		},
		// has
		[](const char*, Registry_t& reg, EntityID_t id) {
			return reg.has<T>(id);
		},
		// remove
		[](const char*, Registry_t& reg, EntityID_t id) {
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
		},
		// toJson
		[](const char* name, Registry_t& reg, EntityID_t id, nlohmann::json& j) {
			if (reg.has<T>(id))
			{
				j[name] = reg.get<T>(id);

				return true;
			}
			else
			{
				// TODO log attempt of adding to config non-existing component
				// TODO do not do this yet

				return false;
			}
		},
		// fromJson
		[](const char*, Registry_t& reg, EntityID_t id, const nlohmann::json& j) {
			if (reg.has<T>(id))
			{
				// TODO log attempt of adding from config already existing component

				return false;
			}
			else
			{
				reg.assign<T>(id) = j;

				return true;
			}
		}
	});

	return true;
}

}
