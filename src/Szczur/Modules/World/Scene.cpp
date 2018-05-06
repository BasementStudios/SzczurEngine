#include "Scene.hpp"

#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Utility/SFML3D/Drawable.hpp"
#include "Szczur/Utility/SFML3D/Sprite.hpp"
#include "Szczur/Utility/SFML3D/Texture.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

namespace rat
{

Scene::Scene()
	: _id { _getUniqueID() }
	, _name { "unnamed_" + std::to_string(_id) }
	, _collectingHolder { { "background", {} }, { "foreground", {} }, { "path", {} }, { "single", {} } }
{
	for (auto& holder : _collectingHolder)
	{
		holder.second.reserve(100);
	}
}

void Scene::update(float deltaTime)
{
	for (auto& holder : _collectingHolder)
	{
		for (auto& entity : holder.second)
		{
			entity.update(deltaTime);
		}
	}
}

void Scene::render()
{
	for (auto& holder : _collectingHolder)
	{
		for (auto& entity : holder.second)
		{
			entity.render();
		}
	}
}

size_t Scene::getID() const
{
	return _id;
}

void Scene::setName(const std::string& name)
{
	_name = name;
}

const std::string& Scene::getName() const
{
	return _name;
}

Entity* Scene::addEntity(const std::string group)
{
	return &_getSubHolder(group).emplace_back(this);
}

bool Scene::removeEntity(const std::string group, size_t id)
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		_getSubHolder(group).erase(it);

		return true;
	}

	return false;
}

void Scene::removeAllEntities(const std::string group)
{
	_getSubHolder(group).clear();
}

void Scene::removeAllEntities()
{
	for (auto& holder : _collectingHolder)
	{
		holder.second.clear();
	}
}

Entity* Scene::getEntity(const std::string group, size_t id)
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

const Entity* Scene::getEntity(const std::string group, size_t id) const
{
	if (auto it = _find(group, id); it != _getSubHolder(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

bool Scene::hasEntity(const std::string group, size_t id)
{
	return _find(group, id) != _getSubHolder(group).end();
}

void Scene::loadFromConfig(const Json& config)
{
	_id = config["id"];
	_name = config["name"];

	const Json& groups = config["groups"];

	for (auto it = groups.begin(); it != groups.end(); ++it)
	{
		for (const Json& current : it.value())
		{
			addEntity(it.key())->loadFromConfig(current);
		}
	}
}

void Scene::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();

	Json& groups = config["groups"] = Json::object();

	for (auto& holder : _collectingHolder)
	{
		Json& group = groups[holder.first] = Json::array();

		for (auto& entity : holder.second)
		{
			group.push_back(Json::object());
			Json& current = group.back();

			entity.saveToConfig(current);
		}
	}
}

size_t Scene::_getUniqueID()
{
	static size_t id = 0;

	return ++id;
}

Scene::EntitiesHolder_t& Scene::_getSubHolder(const std::string& group)
{
	return _collectingHolder.at(group);
}

const Scene::EntitiesHolder_t& Scene::_getSubHolder(const std::string& group) const
{
	return _collectingHolder.at(group);
}

typename Scene::EntitiesHolder_t::iterator Scene::_find(const std::string group, size_t id)
{
	auto& subHolder = _getSubHolder(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

typename Scene::EntitiesHolder_t::const_iterator Scene::_find(const std::string group, size_t id) const
{
	const auto& subHolder = _getSubHolder(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

}
