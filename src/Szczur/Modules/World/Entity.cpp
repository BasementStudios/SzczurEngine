#include "Entity.hpp"

#include "Scene.hpp"
#include "WorldManager.hpp"

namespace rat::wrd
{

Entity::Entity(Scene& scene, const EntityID_t id)
    : _scene { scene }
    , _id { id }
{

}

Scene& Entity::getScene() const
{
    return _scene;
}

EntityID_t Entity::getID() const
{
    return _id;
}

bool Entity::addComponent(HashedID hid) const
{
    return getScene().getManager().getComponentRegistry().assignComponent(*this, hid);
}

bool Entity::hasComponent(HashedID hid) const
{
    return getScene().getManager().getComponentRegistry().hasComponent(*this, hid);
}

bool Entity::removeComponent(HashedID hid) const
{
    return getScene().getManager().getComponentRegistry().removeComponent(*this, hid);
}

bool Entity::addTag(HashedID hid) const
{
    return getScene().getManager().getComponentRegistry().assignTag(*this, hid);
}

bool Entity::hasTag(HashedID hid) const
{
    return getScene().getManager().getComponentRegistry().hasTag(*this, hid);
}

bool Entity::destroy() const
{
    if (getScene().getRegistry().valid(_id))
    {
        getScene().getRegistry().destroy(_id);

        return true;
    }
    else
    {
        // TODO log attempt of deleting invalid entity

        return false;
    }
}

bool Entity::isValid() const
{
    return getScene().getRegistry().valid(_id);
}

}
