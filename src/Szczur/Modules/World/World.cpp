#include "World.hpp"

namespace rat
{

World::World()
	: _levelEditor { _scenes }
{
	LOG_INFO("Initializing World module");

	ComponentTraits::initScript(getModule<Script>());
	Entity::initScript(getModule<Script>());

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
	if (_getScenes().isCurrentSceneValid())
	{
		_getScenes().getCurrentScene()->update(deltaTime);
	}

	#ifdef EDITOR
		if(_doEditor)
			_levelEditor.update(getModule<Input>().getManager(), getModule<Camera>());
	#endif
}

void World::render()
{
	auto& window = getModule<Window>().getWindow();

	if (_getScenes().isCurrentSceneValid())
	{
		_getScenes().getCurrentScene()->render(window);
		// _getScenes().getCurrentScene()->forEach([&window](const std::string&, Entity& entity) {
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

ScenesManager& World::_getScenes()
{
	return _scenes;
}

const ScenesManager& World::_getScenes() const
{
	return _scenes;
}

}
