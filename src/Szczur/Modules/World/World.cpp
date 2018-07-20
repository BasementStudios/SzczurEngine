#include "World.hpp"

#include <SFML/Window/Event.hpp>
#include "Szczur/Utility/SFML3D/Shader.hpp"
#include "Szczur/Utility/SFML3D/ShaderProgram.hpp"
#include "Szczur/Utility/SFML3D/RenderLayer.hpp"
#include <Szczur/Modules/World/Data/TextureDataHolder.hpp>
#include "ScenesManager.hpp"
#include "Scene.hpp"

#include "Szczur/Modules/World/Data/TextureDataHolder.hpp"

namespace rat
{

World::World()
: 	_levelEditor { _scenes }, 
	_blackScreen(sf::Vector2f(9999.f, 9999.f)) // XD
{
	LOG_INFO("Initializing World module");
	this->init();
	LOG_INFO("Module World initialized");
}

World::~World()
{
	LOG_INFO("Module World destructed");
}

void World::init()
{
	// ComponentTraits::initScript(getModule<Script>());

	// Init scenes manager
	_scenes.init(getModule<Window>());

	// Init scripts
	initScript();

	// Create and select default scene
	_scenes.setCurrentScene(_scenes.addScene()->getID());

#ifdef EDITOR
	//_levelEditor.setScene(_scenes.getCurrentScene(), camera->getID());
#endif

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

	// kurwa mac
	try {
		_thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit.linkShaders(
			sf3d::Shader {sf3d::Shader::Vertex, 	"Assets/Shaders/assemble.vert"},
			sf3d::Shader {sf3d::Shader::Fragment, 	"Assets/Shaders/fuck.frag"}
		);
		_thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit.loadConfig("Assets/Shaders/Configs/none.json");
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Shader couldn't been loaded."));
	}	
}

void World::update(float deltaTime)
{
	_elapsedTime += deltaTime;
	_thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit.setUniform("time", _elapsedTime);
	
	// Editor toggle
	if (getModule<Input>().getManager().isReleased(Keyboard::F10)) {
		_doEditor = !_doEditor;
	}

	// Scene update
	if (getScenes().isCurrentSceneValid()) {
		getScenes().getCurrentScene()->update(deltaTime);
	}

	// Editor update
#ifdef EDITOR
	if (_doEditor) {
		_levelEditor.update(getModule<Input>().getManager(), getModule<Window>(), getScenes().getHelperRenderLayer());
	} 
	else {
		_levelEditor.updateDisabledEditor(getModule<Input>().getManager());
	}
#endif

	// Changing scene fade update
	if (_isChangingScene) {
		float progress = _fadeStart.getElapsedTime().asSeconds() / _fadeTime;

		if (_fadeStage == 1) {
			if (progress >= 1.f) {
				progress = 1.f;
				_scenes.setCurrentScene(_sceneToChange);
				_fadeStage = 2;
				_fadeStart.restart();
			}
		}
		else if (_fadeStage == 2) {
			if (progress < 1.f) {
				progress = 1.f - progress;
			}
			else if (progress >= 1.f) {
				progress = 0.f;
				_isChangingScene = false;
			}
		}

		_blackScreen.setFillColor(sf::Color(0.f, 0.f, 0.f, progress * 255));
	}
}

void World::render()
{
	sf3d::RenderWindow& windowTarget = getModule<Window>().getWindow();
	sf3d::RenderLayer& layer = getScenes().getHelperRenderLayer();

	layer.clear({0.f, 0.f, 0.f, 0.f}, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Scene 
	getScenes().render(layer);

	// Fade
	if (_isChangingScene) {
		windowTarget.pushGLStates();
		windowTarget.draw(_blackScreen);
		windowTarget.popGLStates();
	}

	// Editor
#ifdef EDITOR
	if (_doEditor) {
		// sf3d::RenderLayer& layer = getScenes().getHelperRenderLayer();
		
		// glEnable(GL_DEPTH_TEST);
		_levelEditor.render(layer);
		
		// glDisable(GL_DEPTH_TEST);
		// windowTarget.draw(layer);
	}
#endif

	glDisable(GL_DEPTH_TEST); //
	windowTarget.draw(getScenes().getHelperRenderLayer(), sf3d::RenderStates{sf3d::Transform{}, &_thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit});
}

void World::processEvent(sf::Event event)
{
	getScenes().processEvent(event);
}

void World::loadFuckingShaderConfig(const std::string& paff)
{
	_thisWholeCodeWillBeDeletedAndReplacedWithBetterCodeWhichWouldIncludePlanningAndTestingInsteadOfJustWritingShit.loadConfig(paff);
}

const ScenesManager& World::getScenes() const
{
	return _scenes;
}

ScenesManager& World::getScenes()
{
	return _scenes;
}

bool World::isEditor()
{
	return _doEditor;
}

void World::setEditor(bool flag)
{
	_doEditor = flag;
}

void World::fadeIntoScene(size_t id, float fadeTime)
{
	_fadeStage = 1;
	_isChangingScene = true;
	_sceneToChange = id;
	_fadeTime = fadeTime;
	_fadeStart.restart();
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
	module.set_function("fadeIntoScene", sol::overload(
		[&] (Scene* scene, float fadeTime = 1.f) {fadeIntoScene(scene->getID(), fadeTime); },
		[&] (const std::string& name, float fadeTime = 1.f) {fadeIntoScene(_scenes.getScene(name)->getID(), fadeTime); }
	));

	module.set_function("getTextureDataHolder", [&](){return std::ref(getScenes().getTextureDataHolder());});

	module.set_function("loadFuckingShaderConfig", &World::loadFuckingShaderConfig, this);

	script.initClasses<Entity, Scene, TextureDataHolder>();
}

}
