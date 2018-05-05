#pragma once

#include <fstream>

#include <boost/container/flat_map.hpp>

#include "Entity.hpp"

namespace rat
{

class Scene
{
public:

	using EntitiesHolder_t   = std::vector<Entity>;
	using CollectingHolder_t = boost::container::flat_map<std::string, EntitiesHolder_t>;

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
	void update();

	///
	void render();

	///
	size_t getID() const;

	///
	void setName(const std::string& name);

	///
	const std::string& getName() const;

	///
	Entity* addEntity(const std::string group);

	///
	bool removeEntity(const std::string group, size_t id);

	///
	void removeAllEntities(const std::string group);

	///
	void removeAllEntities();

	///
	Entity* getEntity(const std::string group, size_t id);

    ///
	const Entity* getEntity(const std::string group, size_t id) const;

	///
	bool hasEntity(const std::string group, size_t id);

	///
	void loadFromFile(const std::string& filepath);

	///
	void saveToFile(const std::string& filepath) const;

private:

	// #ifdef EDITOR
	// 	void _renderBar();
	// 	//void _renderDisplayDataManager();
	// 	void _renderFocusedObjectsParams();
	// 	void _renderObjectsList();
	// 	void _renderModulesManager();
    //
	// 	bool _ifRenderObjectsList{true};
	// 	//bool _ifRenderDisplayDataManager{false};
	// 	bool _anySelected{false};
	// 	bool _ifRenderModulesManager{false};
	// 	EntitiesHolder_t::iterator _focusedObject;
    //
	// #endif

    ///
	static size_t _getUniqueID();

    ///
	EntitiesHolder_t& _getSubHolder(const std::string& group);

    ///
	const EntitiesHolder_t& _getSubHolder(const std::string& group) const;

    ///
	typename EntitiesHolder_t::iterator _find(const std::string group, size_t id);

    ///
	typename EntitiesHolder_t::const_iterator _find(const std::string group, size_t id) const;

    size_t _id;
    std::string _name;
	CollectingHolder_t _collectingHolder;

};

}
