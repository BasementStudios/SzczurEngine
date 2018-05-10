#include "World.hpp"

namespace rat
{

World::World()
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
	if (_getScenes().isCurrentSceneValid())
	{
		_getScenes().getCurrentScene()->update(deltaTime);
	}
}

void World::render()
{
	if (_getScenes().isCurrentSceneValid())
	{
		auto& window = getModule<Window>();

		_getScenes().getCurrentScene()->forEach([&window](const std::string&, Entity& entity) {
			if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr)
			{
				window.draw(*ptr);
			}
		});
	}
}

SceneManager& World::_getScenes()
{
	return _scenes;
}

const SceneManager& World::_getScenes() const
{
	return _scenes;
}

}
