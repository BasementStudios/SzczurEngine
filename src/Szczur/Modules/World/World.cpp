#include "World.hpp"

namespace rat
{

World::World() :
_levelEditor(_scenes)
{
	LOG_INFO("Initializing World module");

	ComponentTraits::initScript(getModule<Script>());
	Entity::initScript(getModule<Script>());

	_scenes.setCurrentScene(_scenes.addScene()->getID());

	#ifdef EDITOR
		//_levelEditor.setScene(_scenes.getCurrentScene(), camera->getID());
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
	if (_getScenes().isCurrentSceneValid())
	{
		_getScenes().getCurrentScene()->update(deltaTime, _scenes);
	}

	#ifdef EDITOR
		_levelEditor.update(getModule<Input>().getManager(), getModule<Camera>());
	#endif
}

void World::render()
{
	auto& window = getModule<Window>().getWindow();
	if (_getScenes().isCurrentSceneValid())
	{
		_getScenes().getCurrentScene()->forEach([&window](const std::string&, Entity& entity) {
			if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr)
			{
				window.draw(*ptr);
			}
		});
	}

	#ifdef EDITOR
		_levelEditor.render(window);
	#endif
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
