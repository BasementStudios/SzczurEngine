#include "EntityManager.hpp"

#include <iostream>

#include "WorldManager.hpp"

namespace rat::world
{

EntityManager::EntityManager(Scene& scene)
	: _scene { scene }
	, _registry {}
{

}

Scene& EntityManager::getScene() const
{
	return _scene;
}

ComponentRegistry& EntityManager::getComponentRegistry() const
{
	return _scene.getWorldManager().getComponentRegistry();
}

Registry_t& EntityManager::getRegistry()
{
	return _registry;
}

const Registry_t& EntityManager::getRegistry() const
{
	return _registry;
}

Entity EntityManager::addEntity()
{
	return { *this, _registry.create() };
}

bool EntityManager::hasEntity(EntityID_t id) const
{
	return _registry.valid(id);
}

bool EntityManager::removeEntity(EntityID_t id)
{
	if (_registry.valid(id))
	{
		_registry.destroy(id);

		return true;
	}
	else
	{
		// TODO log attempt of deleting invalid entity

		return false;
	}
}

Entity EntityManager::getEntity(EntityID_t id)
{
	if (_registry.valid(id))
	{
		return { *this, id };
	}
	else
	{
		// TODO log attempt of obtaining invalid entity

		return { *this, entt::null };
	}
}

bool EntityManager::addComponent(EntityID_t id, HashedID hid)
{
	return getComponentRegistry().assignComponent(_registry, id, hid);
}

bool EntityManager::hasComponent(EntityID_t id, HashedID hid)
{
	return getComponentRegistry().hasComponent(_registry, id, hid);
}

bool EntityManager::removeComponent(EntityID_t id, HashedID hid)
{
	return getComponentRegistry().removeComponent(_registry, id, hid);
}

void EntityManager::reset()
{
	_registry.reset();
}

nlohmann::json EntityManager::saveToConfig()
{
	nlohmann::json config = nlohmann::json::array_t{};

	_registry.each([this, &j = config](const EntityID_t id) {
		nlohmann::json k = nlohmann::json::object_t{};

		getComponentRegistry().allComponentsToJson(_registry, id, k);

		// Do not save empty entities
		if (!k.empty())
		{
			j.push_back(k);
		}
	});

	return config;
}

void EntityManager::loadFromConfig(const nlohmann::json& config)
{
	for (const nlohmann::json& j : config)
	{
		if (j.is_null())
		{
			// Do not load empty entities

			continue;
		}

		const auto id = _registry.create();

		if (j.is_object())
		{
			// for (const nlohmann::json& k : j)
			for (auto it = j.begin(); it != j.end(); ++it)
			{
				// TODO log possible invalid json?

				getComponentRegistry().componentFromJson(_registry, id, it.key().data(), it.value());
			}
		}
		else
		{
			// TODO log invalid json

			continue;
		}
	}
}

}
