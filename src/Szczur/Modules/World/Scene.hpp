#pragma once

#include <functional>

#include <entt/entt.hpp>

#include "EntityManager.hpp"

namespace rat::world
{

// fwd decl
class ComponentRegistry;
class WorldManager;

class Scene
{
public:

	///
	Scene(WorldManager& worldManager, Hash_t id, const Name_t& name);

	///
	Scene(const Scene&) = delete;

	///
	Scene& operator = (const Scene&) = delete;

	///
	Scene(Scene&&) = default;

	///
	Scene& operator = (Scene&&) = default;

	///
	~Scene() = default;

	///
	WorldManager& getWorldManager() const;

	///
	EntityManager& getEntityManager();

	///
	const EntityManager& getEntityManager() const;

	///
	Hash_t getID() const;

	///
	void setName(const Name_t& name);

	///
	const Name_t& getName() const;

	///
	bool isValid() const;

private:

	WorldManager& _worldManager;
	EntityManager _entityManager;
	Hash_t _id;
	Name_t _name;

};

}
