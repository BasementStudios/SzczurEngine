#include "World.hpp"

namespace rat
{

World::World()
{
	LOG_INFO("Initializing World module");

	_scenes.setCurrentScene(_scenes.addScene()->getID());
	auto* camera = _scenes.getCurrentScene()->addEntity("single");
	camera->setName("Camera");
	camera->setPosition({0.f, 1160.f, 3085.f}); 
    camera->setRotation({15.f, 0.f, 0.f}); 
	#ifdef EDITOR
		_levelEditor.setScene(_scenes.getCurrentScene(), camera->getID());
	#endif

	//getCurrentScene()->addEntity("single")->setName("Cedmin");
	//auto* ptr = getCurrentScene()->getEntity(1)->addComponent<SpriteComponent>();
	//ptr->getEntity()->setName("Karion");
	//getCurrentScene()->addEntity("background")->setName("Tlo");
	//getCurrentScene()->addEntity("foreground")->setName("Kamyk");
	//getCurrentScene()->addEntity("path")->setName("Droga");
	//getCurrentScene()->removeEntity(1);

	// loadFromFile("test.json");
	//saveToFile("test.json");

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
	#ifdef EDITOR
		_levelEditor.update(getModule<Input>().getManager(), getModule<Camera>());
	#endif
}

void World::render()
{
	auto& window = getModule<Window>().getWindow();
	if (_scenes.isCurrentSceneValid())
	{
		auto& window = getModule<Window>();
		_scenes.getCurrentScene()->forEach([&window](const std::string&, Entity& entity) { 
			if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr) { 
				window.draw(*ptr); 
			} 
		}); 
	}
	#ifdef EDITOR
		_levelEditor.render(window);
	#endif
}

}
