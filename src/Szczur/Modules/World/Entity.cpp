#include "Entity.hpp"

#include "EntityManager.hpp"
#include "Scene.hpp"

namespace rat::world
{

Entity::Entity(EntityManager& entityManager, const EntityID_t id)
	: _entityManager { entityManager }
	, _id { id }
{

}

Scene& Entity::getScene() const
{
	return _entityManager.getScene();
}

EntityManager& Entity::getEntityManager() const
{
	return _entityManager;
}

EntityID_t Entity::getID() const
{
	return _id;
}

bool Entity::addComponent(HashedID hid) const
{
	return _entityManager.addComponent(_id, hid);
}

bool Entity::hasComponent(HashedID hid) const
{
	return _entityManager.hasComponent(_id, hid);
}

bool Entity::removeComponent(HashedID hid) const
{
	return _entityManager.removeComponent(_id, hid);
}

bool Entity::destroy() const
{
	if (isValid())
	{
		_entityManager.getRegistry().destroy(_id);

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
	return _entityManager.getRegistry().valid(_id);
}

}
