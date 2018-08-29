#include "Scene.hpp"

#include "WorldManager.hpp"

namespace rat::world
{

Scene::Scene(WorldManager& worldManager, Hash_t id, const Name_t& name)
	: _worldManager { worldManager }
	, _entityManager { *this }
	, _id { id }
	, _name { name }
{

}

WorldManager& Scene::getWorldManager() const
{
	return _worldManager;
}

EntityManager& Scene::getEntityManager()
{
	return _entityManager;
}

const EntityManager& Scene::getEntityManager() const
{
	return _entityManager;
}

Hash_t Scene::getID() const
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

bool Scene::isValid() const
{
	return _id != 0;
}

}
