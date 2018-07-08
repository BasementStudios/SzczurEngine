#include "World.hpp"

namespace rat
{

World::World()
{
	LOG_INFO("Initializing World module");

	// Initialize layer
	_layer.create(); // @todo . register window resize hook
	
	// Load shader
	try {
		sf3d::Shader VShader;
		VShader.loadFromFile(sf3d::Shader::Vertex, "Assets/Shaders/projection.vert");
		sf3d::Shader FShader;
		FShader.loadFromFile(sf3d::Shader::Fragment, "Assets/Shaders/world.frag");

		_shader = std::make_unique<sf3d::ShaderProgram>();
		_shader->linkShaders(VShader, FShader);
		LOG_INFO("Shader loaded, compiled and linked.");
	}
	catch (...) {
		std::throw_with_nested(std::runtime_error("Shader couldn't been loaded."));
	}

	_shader.setDefaultShaderProgram(_shader.get());

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
		Scene* scene = _getScenes().getCurrentScene();

		// Render background entities
		_layer.clear(0, 0, 0, 0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			for (const auto& entity : scene.getEntities("background")) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr) {
					layer.draw(*ptr);
				}
			}
		}
		window.draw(_layer);
		
		// Render path and single entities
		_layer.clear(0, 0, 0, 0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			for (const auto& entity : scene.getEntities("path")) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr) {
					layer.draw(*ptr);
				}
			}
			for (const auto& entity : scene.getEntities("single")) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr) {
					layer.draw(*ptr);
				}
			}
		}
		window.draw(_layer);

		// Render foreground entities
		_layer.clear(0, 0, 0, 0, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			for (const auto& entity : scene.getEntities("foreground")) {
				if (auto ptr = entity.getFeature<sf3d::Drawable>(); ptr) {
					layer.draw(*ptr);
				}
			}
		}
		window.draw(_layer);
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
