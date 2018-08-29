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

	using Key_t                = Hash_t;
	using RegisteredIDs_t      = std::vector<Hash_t>;
	using Functor_t            = bool(*)(Registry_t&, EntityID_t);
	using FunctorMap_t         = std::unordered_map<Key_t, Functor_t>;
	using ToJsonFunctor_t      = bool(*)(Registry_t&, EntityID_t, nlohmann::json&);
	using ToJsonFunctorMap_t   = std::unordered_map<Key_t, ToJsonFunctor_t>;
	using FromJsonFunctor_t    = bool(*)(Registry_t&, EntityID_t, const nlohmann::json&);
	using FromJsonFunctorMap_t = std::unordered_map<Key_t, FromJsonFunctor_t>;

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
	static bool componentToJson(Registry_t& registry, EntityID_t id, HashedID hid, nlohmann::json& j);

	///
	static bool allComponentsToJson(Registry_t& registry, EntityID_t id, nlohmann::json& j);

	///
	static bool componentFromJson(Registry_t& registry, EntityID_t id, const nlohmann::json& j);

private:

	///
	static bool _call(const FunctorMap_t& map, Registry_t& registry, EntityID_t id, HashedID hid);

	inline static RegisteredIDs_t _registeredIDs = {};

	inline static FunctorMap_t _assignComponent = {};
	inline static FunctorMap_t _hasComponent = {};
	inline static FunctorMap_t _removeComponent = {};

	inline static ToJsonFunctorMap_t _componentToJson = {};
	inline static FromJsonFunctorMap_t _componentFromJson = {};

};

template <typename T>
void ComponentRegistry::registerComponent(const char* name)
{
	const auto key = HashedID{ name }.hash;

	_registeredIDs.emplace_back(key);

	_assignComponent.emplace(key, [](Registry_t& reg, EntityID_t id) {
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

	_componentToJson.emplace(key, [](Registry_t& reg, EntityID_t id, nlohmann::json& j) {
		if (reg.has<T>(id))
		{
			j.push_back(reg.get<T>(id));

			return true;
		}
		else
		{
			// TODO log attempt of adding to config non-existing component
			// TODO do not do this yet

			return false;
		}
	});

	_componentFromJson.emplace(key, [](Registry_t& reg, EntityID_t id, const nlohmann::json& j) {
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
	});
}

}
