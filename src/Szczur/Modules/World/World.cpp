#include "World.hpp"

namespace rat
{

World::World()
	: _currentSceneID { static_cast<size_t>(-1) }
{
	LOG_INFO("Initializing World module");
	LOG_INFO("Module World initialized");

	// _currentSceneID = addScene()->getID();
	// getCurrentScene()->addEntity("single")->setName("Cedmin");
	// getCurrentScene()->getEntity("single", 0)->addComponent(ComponentTraits::createFromName("SpriteComponent"));
	// getCurrentScene()->addEntity("background")->setName("Tlo");
	// getCurrentScene()->addEntity("foreground")->setName("Kamyk");
	// getCurrentScene()->addEntity("path")->setName("Droga");
}

World::~World()
{
	LOG_INFO("Module World destructed");
}

void World::update()
{
	if (isCurrentSceneValid())
	{
		getCurrentScene()->update();
	}
}

void World::render()
{
	if (isCurrentSceneValid())
	{
		getCurrentScene()->render();
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
			_currentSceneID = static_cast<size_t>(-1);
		}

		return true;
	}

	return false;
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

bool World::hasScene(size_t id) const
{
	return _find(id) != _holder.end();
}

bool World::isCurrentSceneValid() const
{
	return _currentSceneID != static_cast<size_t>(-1);
}

void World::removeAllScenes()
{
	_holder.clear();

	_currentSceneID = static_cast<size_t>(-1);
}

const World::ScenesHolder_t& World::getScenes() const
{
	return _holder;
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
