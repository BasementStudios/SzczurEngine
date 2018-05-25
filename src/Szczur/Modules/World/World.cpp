#include "World.hpp"

namespace rat
{

World::World()
{
	LOG_INFO("Initializing World module");
	LOG_INFO("Module World initialized");

	// test1.world
	// _scenes.addScene()->addEntity("single")->setName("Karion");
	// _scenes.saveToFile("test1.world");

	// test2.world
	// _scenes.addScene()->addEntity("path")->setName("Droga");
	// _scenes.saveToFile("test2.world");

	// test3.world
	// _scenes.addScene()->addEntity("background")->setName("Domek");
	// _scenes.saveToFile("test3.world");

	// _scenes.loadFromFile("test1.world");
	// _scenes.appendScenesFromFile("test2.world");
	// _scenes.appendScenesFromFile("test3.world");

	// _scenes.saveToFile("out.world");
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
