#include "World.hpp"

#include <Szczur/Modules/World/Data/TextureDataHolder.hpp>

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
	if(getModule<Input>().getManager().isReleased(Keyboard::F10)) {
		_doEditor = !_doEditor;
	}
	if (getScenes().isCurrentSceneValid())
	{
		getScenes().getCurrentScene()->update(deltaTime);
	}
	#ifdef EDITOR
		if(_doEditor)
			_levelEditor.update(getModule<Input>().getManager(), getModule<Window>());
	#endif
}

void World::render()
{
	auto& target = getModule<Window>().getWindow();

	if (getScenes().isCurrentSceneValid())
	{
		getScenes().getCurrentScene()->draw(target);
		// getScenes().getCurrentScene()->forEach([&window](const std::string&, Entity& entity) {
		// 	if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr != nullptr)
		// 	{
		// 		window.draw(*ptr);
		// 	}
		// });
	}

	#ifdef EDITOR
		if(_doEditor)
			_levelEditor.render(target);
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

bool World::isEditor() {
	return _doEditor;
}

void World::setEditor(bool flag) {
	_doEditor = flag;
}

void World::initScript() {
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

	module.set_function("getTextureDataHolder", [&](){return std::ref(getScenes().getTextureDataHolder());});

	script.initClasses<Entity, Scene, TextureDataHolder>();
}

}
