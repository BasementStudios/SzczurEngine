#include "World.hpp"

namespace rat
{

World::World()
	: _currentSceneID { 0 }
{
	LOG_INFO("Initializing World module");

	LOG_INFO("Module World initialized");
}

World::~World()
{
	LOG_INFO("Module World destructed");
}

void World::update(float deltaTime)
{
	if (isCurrentSceneValid())
	{
		getCurrentScene()->update(deltaTime);
	}
}

void World::render()
{
	if (isCurrentSceneValid())
	{
		auto& window = getModule<Window>();
		getCurrentScene()->forEach([&window](const std::string&, Entity& entity) { 
			if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) { 
				window.draw(*ptr); 
			} 
		}); 
	}
}

Scene* World::addScene()
{
	return _holder.emplace_back(std::make_unique<Scene>()).get();
}

bool World::removeScene(size_t id)
{
	if (auto it = _find(id); it != _holder.end())
	{
		_holder.erase(it);

		if (_currentSceneID == id)
		{
			_currentSceneID = 0;
		}

		return true;
	}

	return false;
}

void World::removeAllScenes()
{
	_holder.clear();

	_currentSceneID = 0;
}

Scene* World::getScene(size_t id) const
{
	if (auto it = _find(id); it != _holder.end())
	{
		return it->get();
	}

	return nullptr;
}

Scene* World::getCurrentScene() const
{
	return getScene(_currentSceneID);
}

size_t World::getCurrentSceneID() const
{
	return _currentSceneID;
}

bool World::hasScene(size_t id) const
{
	return _find(id) != _holder.end();
}

bool World::isCurrentSceneValid() const
{
	return _currentSceneID != 0;
}

World::ScenesHolder_t& World::getScenes()
{
	return _holder;
}

const World::ScenesHolder_t& World::getScenes() const
{
	return _holder;
}

void World::loadFromFile(const std::string& filepath)
{
	std::ifstream file{ filepath };
	Json config;

	file >> config;

	_currentSceneID = config["currentSceneID"];

	const Json& scenes = config["scenes"];

	for (auto& current : scenes)
	{
		addScene()->loadFromConfig(current);
	}
}

void World::saveToFile(const std::string& filepath) const
{
	std::ofstream file{ filepath };
	Json config;

	config["currentSceneID"] = getCurrentSceneID();
	Json& scenes = config["scenes"] = Json::array();

	for (auto& scene : _holder)
	{
		scenes.push_back(Json::object());
		Json& current = scenes.back();

		scene->saveToConfig(current);
	}

	file << std::setw(4) << config << std::endl;
}

typename World::ScenesHolder_t::iterator World::_find(size_t id)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

typename World::ScenesHolder_t::const_iterator World::_find(size_t id) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

}
