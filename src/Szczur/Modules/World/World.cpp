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
	if (_scenes.isCurrentSceneValid())
	{
		_scenes.getCurrentScene()->update(deltaTime);
	}
}

void World::render()
{
	if (_scenes.isCurrentSceneValid())
	{
		auto& window = getModule<Window>();
		_scenes.getCurrentScene()->forEach([&window](const std::string&, Entity& entity) { 
			if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) { 
				window.draw(*ptr); 
			} 
		}); 
	}
}

}
