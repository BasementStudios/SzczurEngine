#include "World.hpp"

namespace rat
{

World::World()
	: _levelEditor { _scenes }
{
	LOG_INFO("Initializing World module");

	// ComponentTraits::initScript(getModule<Script>());

	initScript();

	_scenes.setCurrentScene(_scenes.addScene()->getID());

	#ifdef EDITOR
		//_levelEditor.setScene(_scenes.getCurrentScene(), camera->getID());
	#endif

	LOG_INFO("Module World initialized");
}

World::~World()
{
	LOG_INFO("Module World destructed");
}

void World::update(float deltaTime)
{
	if(getModule<Input>().getManager().isReleased(Keyboard::F10)) {
		if(_doEditor)
			_doEditor = false;
		else
			_doEditor = true;
	}
	if (getScenes().isCurrentSceneValid())
	{
		getScenes().getCurrentScene()->update(deltaTime);
	}

	#ifdef EDITOR
		if(_doEditor)
			_levelEditor.update(getModule<Input>().getManager(), getModule<Camera>());
	#endif
}

void World::render()
{
	auto& window = getModule<Window>().getWindow();

	if (getScenes().isCurrentSceneValid())
	{
		getScenes().getCurrentScene()->render(window);
		// getScenes().getCurrentScene()->forEach([&window](const std::string&, Entity& entity) {
		// 	if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr)
		// 	{
		// 		window.draw(*ptr);
		// 	}
		// });
	}

	#ifdef EDITOR
		if(_doEditor)
			_levelEditor.render(window);
	#endif
}

ScenesManager& World::getScenes()
{
	return _scenes;
}

const ScenesManager& World::getScenes() const
{
	return _scenes;
}

void World::	initScript() {
	auto& script = getModule<Script>();

	auto module = script.newModule("World");

	module.set_function("getScene", sol::overload(
		[&](){return _scenes.getCurrentScene();},
		[&](const std::string& name){return _scenes.getScene(name);}
	));
	module.set_function("setCurrentScene", sol::overload(
		[&](Scene* scene){_scenes.setCurrentScene(scene->getID());},
		[&](const std::string& name){_scenes.setCurrentScene(_scenes.getScene(name)->getID());}
	));

	script.initClasses<Entity, Scene>();
}

}
