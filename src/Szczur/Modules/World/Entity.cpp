#include "Entity.hpp"

#include "Scene.hpp"
#include "WorldManager.hpp"

namespace rat::wrd
{

Entity::Entity(Scene& scene, Registry_t& registry, const EntityID_t id)
    : _scene { scene }
    , _registry { registry }
    , _id { id }
{

}

Scene& Entity::getScene() const
{
    return _scene;
}

Registry_t& Entity::getRegistry() const
{
    return _registry;
}

EntityID_t Entity::getID() const
{
    return _id;
}

void Entity::addComponent(HashedID hid) const
{
    getScene().getManager().getComponentRegistry().assign(*this, hid);
}

void Entity::removeComponent(HashedID hid) const
{
    getScene().getManager().getComponentRegistry().remove(*this, hid);
}

bool Entity::isValid() const
{
    return _registry.valid(_id);
}

}
