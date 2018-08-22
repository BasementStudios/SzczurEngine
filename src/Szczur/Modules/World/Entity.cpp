#include "Entity.hpp"

#include "EntityManager.hpp"
#include "Scene.hpp"

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

EntityManager& Entity::getEntityManager() const
{
    return _scene.getEntityManager();
}

EntityID_t Entity::getID() const
{
    return _id;
}

bool Entity::addComponent(HashedID hid) const
{
    return getEntityManager().addComponent(_id, hid);
}

bool Entity::hasComponent(HashedID hid) const
{
    return getEntityManager().hasComponent(_id, hid);
}

bool Entity::removeComponent(HashedID hid) const
{
    return getEntityManager().removeComponent(_id, hid);
}

bool Entity::destroy() const
{
    if (getEntityManager().getRegistry().valid(_id))
    {
        getEntityManager().getRegistry().destroy(_id);

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
    return getEntityManager().getRegistry().valid(_id);
}

}
