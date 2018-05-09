#include "Scene.hpp"

#include <functional>

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
	for (auto& holder : getAllEntities())
	{
		holder.second.reserve(100);
	}

	_spriteDisplayDataHolder.reserve(100);
	_armatureDisplayDataHolder.reserve(100);
}

void Scene::update(float deltaTime)
{
	for (auto& holder : getAllEntities())
	{
		for (auto& entity : holder.second)
		{
			entity.update(deltaTime);
		}
	}
}

void Scene::render()
{
	for (auto& holder : getAllEntities())
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

Entity* Scene::addEntity(const std::string& group)
{
	return &getEntities(group).emplace_back(this);
}

bool Scene::removeEntity(const std::string& group, size_t id)
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		getEntities(group).erase(it);

		return true;
	}

	return false;
}

bool Scene::removeEntity(size_t id)
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		getEntities("single").erase(it);

		return true;
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		getEntities("path").erase(it);

		return true;
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		getEntities("foreground").erase(it);

		return true;
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		getEntities("background").erase(it);

		return true;
	}

	return false;
}

void Scene::removeAllEntities(const std::string& group)
{
	getEntities(group).clear();
}

void Scene::removeAllEntities()
{
	for (auto& holder : getAllEntities())
	{
		holder.second.clear();
	}
}

Entity* Scene::getEntity(size_t id)
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		return &(*it);
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		return &(*it);
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		return &(*it);
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		return &(*it);
	}

	return nullptr;
}

const Entity* Scene::getEntity(size_t id) const
{
	if (auto it = _find("single", id); it != getEntities("single").end())
	{
		return &(*it);
	}

	if (auto it = _find("path", id); it != getEntities("path").end())
	{
		return &(*it);
	}

	if (auto it = _find("foreground", id); it != getEntities("foreground").end())
	{
		return &(*it);
	}

	if (auto it = _find("background", id); it != getEntities("background").end())
	{
		return &(*it);
	}

	return nullptr;
}

Entity* Scene::getEntity(const std::string& group, size_t id)
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

const Entity* Scene::getEntity(const std::string& group, size_t id) const
{
	if (auto it = _find(group, id); it != getEntities(group).end())
	{
		return &(*it);
	}

	return nullptr;
}

bool Scene::hasEntity(const std::string& group, size_t id)
{
	return _find(group, id) != getEntities(group).end();
}

Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group)
{
	return getAllEntities().at(group);
}

const Scene::EntitiesHolder_t& Scene::getEntities(const std::string& group) const
{
	return getAllEntities().at(group);
}

Scene::CollectingHolder_t& Scene::getAllEntities()
{
	return _collectingHolder;
}

const Scene::CollectingHolder_t& Scene::getAllEntities() const
{
	return _collectingHolder;
}

Scene::SpriteDisplayDataHolder_t& Scene::getSpriteDisplayData()
{
	return _spriteDisplayDataHolder;
}

const Scene::SpriteDisplayDataHolder_t& Scene::getSpriteDisplayData() const
{
	return _spriteDisplayDataHolder;
}

Scene::ArmatureDisplayDataHolder_t& Scene::getArmatureDisplayData()
{
	return _armatureDisplayDataHolder;
}

const Scene::ArmatureDisplayDataHolder_t& Scene::getArmatureDisplayData() const
{
	return _armatureDisplayDataHolder;
}

void Scene::setCameraID(size_t id)
{
	_cameraID = id;
}

size_t Scene::getCameraID() const
{
	return _cameraID;
}

void Scene::loadFromConfig(const Json& config)
{
	_id = config["id"];
	_name = config["name"].get<std::string>();
	_cameraID = config["camera"];

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
	config["camera"] = getCameraID();

	Json& groups = config["groups"] = Json::object();

	for (auto& holder : getAllEntities())
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

void Scene::forEach(const std::function<void(const std::string& group, Entity& entity)>& function)
{
	for (auto& group : _collectingHolder)
	{
		for (auto& entity : group.second)
		{
			std::invoke(function, group.first, entity);
		}
	}
}

size_t Scene::_getUniqueID()
{
	static size_t id = 0;

	return ++id;
}

typename Scene::EntitiesHolder_t::iterator Scene::_find(const std::string& group, size_t id)
{
	auto& subHolder = getEntities(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

typename Scene::EntitiesHolder_t::const_iterator Scene::_find(const std::string& group, size_t id) const
{
	const auto& subHolder = getEntities(group);

	return std::find_if(subHolder.begin(), subHolder.end(), [=](const auto& arg) {
		return arg.getID() == id;
	});
}

}
