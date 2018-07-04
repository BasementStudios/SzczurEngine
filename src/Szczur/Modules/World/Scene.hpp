#pragma once

#include <fstream>
#include <memory>
#include <utility>
#include <unordered_map>

#include <boost/container/flat_map.hpp>

#include <Szczur/Utility/SFML3D/Drawable.hpp>
#include <Szczur/Utility/SFML3D/RenderTarget.hpp>
#include <Szczur/Utility/SFML3D/RenderStates.hpp>

#include "Entity.hpp"

namespace rat
{

// FWD
class ScenesManager;
class Script;

class Scene : public sf3d::Drawable
{
public:

	struct Entrance
	{
		size_t ID;
		std::string name;
		glm::vec3 position;
	};

	using EntitiesHolder_t            = std::vector<std::unique_ptr<Entity>>;
  	using CollectingHolder_t          = std::vector<std::pair<std::string, EntitiesHolder_t>>; 
	//using CollectingHolder_t          = boost::container::flat_map<std::string, EntitiesHolder_t>; @todo
	using SpriteDisplayDataHolder_t   = std::vector<SpriteDisplayData>;

	///
	Scene(ScenesManager* _parent);

	///
	Scene(const Scene&) = delete;

	///
	Scene& operator = (const Scene&) = delete;

	///
	Scene(Scene&&) = delete;

	///
	Scene& operator = (Scene&&) = delete;

	///
	~Scene() = default;

	///
	void update(float deltaTime);

	///
	virtual void draw(sf3d::RenderTarget& target, sf3d::RenderStates states = sf3d::RenderStates::Default) const override;

	///
	size_t getID() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	ScenesManager* getScenes();

	///
	const ScenesManager* getScenes() const;

	///
	Entity* addEntity(const std::string& group);

	///
	Entity* addRawEntity(const std::string& group);

	///
	Entity* duplicateEntity(size_t id);

	///
	bool removeEntity(size_t id);

	///
	bool removeEntity(const std::string& group, size_t id);

	///
	void removeAllEntities(const std::string& group);

	///
	void removeAllEntities();

	///
	Entity* getEntity(size_t id);

	///
	const Entity* getEntity(size_t id) const;

	///
	Entity* getEntity(const std::string& group, size_t id);

	///
	Entity* getEntity(const std::string& name);

	///
	const Entity* getEntity(const std::string& group, size_t id) const;

	///
	bool hasEntity(size_t id);

	///
	bool hasEntity(const std::string& group, size_t id);

	///
	EntitiesHolder_t& getEntities(const std::string& group);

	///
	const EntitiesHolder_t& getEntities(const std::string& group) const;

	///
	CollectingHolder_t& getAllEntities();

	///
	const CollectingHolder_t& getAllEntities() const;

	///
	SpriteDisplayDataHolder_t& getSpriteDisplayDataHolder();

	///
	const SpriteDisplayDataHolder_t& getSpriteDisplayDataHolder() const;

	/// Provides access to player object
	Entity* getPlayer();
	const Entity* getPlayer() const;
	void setPlayer(Entity* player);

	/// Provides access to current camera
	Entity* getCurrentCamera();
	const Entity* getCurrentCamera() const;
	void setCurrentCamera(Entity* camera);

	/// Get any camera if no current present
	Entity* getCamera();
	
	///
	void loadFromConfig(Json& config, bool withNewID = false);

	///
	void saveToConfig(Json& config) const;

	///
	template <typename F>
	void forEach(F&& function)
	{
		for (auto& group : _collectingHolder)
		{
			for (auto& entity : group.second)
			{
				std::invoke(std::forward<F>(function), group.first, *entity);
			}
		}
	}

	///
	void updateIDs();

	///
	static void initScript(Script& script);

private:

	///
	typename EntitiesHolder_t::iterator _find(const std::string& group, size_t id);

	///
	typename EntitiesHolder_t::const_iterator _find(const std::string& group, size_t id) const;

	size_t _id;
	std::string _name;
	ScenesManager* _parent;
	CollectingHolder_t _collectingHolder;
	SpriteDisplayDataHolder_t _spriteDisplayDataHolder;
	
	size_t _playerID {0u};
	Entity* _player {nullptr};

	Entity* _currentCamera {nullptr};
};

}
