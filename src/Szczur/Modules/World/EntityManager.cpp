#include "EntityManager.hpp"

#include <iostream>

#include "ComponentRegistry.hpp"

namespace rat::wrd
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
    return { _scene, _registry.create() };
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
        return { _scene, id };
    }
    else
    {
        // TODO log attempt of obtaining invalid entity

        return { _scene, entt::null };
    }
}

bool EntityManager::addComponent(EntityID_t id, HashedID hid)
{
    return ComponentRegistry::assignComponent(_registry, id, hid);
}

bool EntityManager::hasComponent(EntityID_t id, HashedID hid)
{
    return ComponentRegistry::hasComponent(_registry, id, hid);
}

bool EntityManager::removeComponent(EntityID_t id, HashedID hid)
{
    return ComponentRegistry::removeComponent(_registry, id, hid);
}

void EntityManager::reset()
{
    _registry.reset();
}

nlohmann::json EntityManager::saveToConfig()
{
    nlohmann::json config = nlohmann::json::array_t{};

    _registry.each([this, &j = config](const EntityID_t id) {
        nlohmann::json k = nlohmann::json::array_t{};

        ComponentRegistry::allComponentsToJson(_registry, id, k);

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

        if (j.is_array())
        {
            for (const nlohmann::json& k : j)
            {
                // TODO log possible invalid json?

                std::cout << "load: -- " << k << '\n';

                ComponentRegistry::componentFromJson(_registry, id, k);
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
