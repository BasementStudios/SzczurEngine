#include "Scene.hpp"

#include "WorldManager.hpp"

namespace rat::wrd
{

Scene::Scene(WorldManager_t& manager, ID_t id, NameView_t name)
    : _manager { manager }
    , _id { id }
    , _name { name }
    , _registry {}
{

}

Scene::WorldManager_t& Scene::getManager() const
{
    return _manager;
}

ID_t Scene::getID() const
{
    return _id;
}

void Scene::setName(const Name_t& name)
{
    _name = name;
}

const Name_t& Scene::getName() const
{
    return _name;
}

Registry_t& Scene::getRegistry()
{
    return _registry;
}

const Registry_t& Scene::getRegistry() const
{
    return _registry;
}

Entity Scene::addEntity()
{
    return { *this, _registry, _registry.create() };
}

bool Scene::removeEntity(EntityID_t id)
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

}
