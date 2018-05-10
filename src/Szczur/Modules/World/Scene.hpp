#pragma once

#include <fstream>

#include <boost/container/flat_map.hpp>

#include "Entity.hpp"

namespace rat
{

class Scene
{
public:

	using EntitiesHolder_t            = std::vector<Entity>;
	using CollectingHolder_t          = boost::container::flat_map<std::string, EntitiesHolder_t>;
	using SpriteDisplayDataHolder_t   = std::vector<SpriteDisplayData>;
	using ArmatureDisplayDataHolder_t = std::vector<ArmatureDisplayData>;

	///
	Scene();

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
	void render();

	///
	size_t getID() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	Entity* addEntity(const std::string& group);

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

	///
	ArmatureDisplayDataHolder_t& getArmatureDisplayDataHolder();

	///
	const ArmatureDisplayDataHolder_t& getArmatureDisplayDataHolder() const;

	///
	void loadFromConfig(const Json& config);

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
				std::invoke(std::forward<F>(function), group.first, entity);
			}
		}
	}

private:

    ///
	typename EntitiesHolder_t::iterator _find(const std::string& group, size_t id);

    ///
	typename EntitiesHolder_t::const_iterator _find(const std::string& group, size_t id) const;

    size_t _id;
    std::string _name;
	CollectingHolder_t _collectingHolder;
	SpriteDisplayDataHolder_t _spriteDisplayDataHolder;
	ArmatureDisplayDataHolder_t _armatureDisplayDataHolder;

};

}
