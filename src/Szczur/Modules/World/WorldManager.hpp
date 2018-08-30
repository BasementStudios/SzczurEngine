#pragma once

#include <entt/entt.hpp>

#include "ComponentRegistry.hpp"

namespace rat::world
{

class WorldManager
{
public:

	using ScenesHolder_t = std::vector<std::unique_ptr<Scene>>;

	///
	WorldManager();

	///
	WorldManager(const WorldManager&) = delete;

	///
	WorldManager& operator = (const WorldManager&) = delete;

	///
	WorldManager(WorldManager&&) = default;

	///
	WorldManager& operator = (WorldManager&&) = default;

	///
	~WorldManager() = default;

	///
	void reset();

	///
	Scene* addScene(const Name_t& name);

	///
	bool hasScene(HashedID hid);

	///
	bool removeScene(HashedID hid);

	///
	Scene* getScene(HashedID hid) const;

	///
	bool setCurrentScene(HashedID hid);

	///
	Scene* getCurrentScene() const;

	///
	Hash_t getCurrentSceneID() const;

	///
	bool isCurrentSceneValid() const;

	///
	ComponentRegistry& getComponentRegistry();

	///
	const ComponentRegistry& getComponentRegistry() const;

private:

	///
	typename ScenesHolder_t::iterator _find(HashedID hid);

	///
	typename ScenesHolder_t::const_iterator _find(HashedID hid) const;

	ScenesHolder_t _scenes;
	Hash_t _currentSceneID;
	ComponentRegistry _componentRegistry;

};

}
