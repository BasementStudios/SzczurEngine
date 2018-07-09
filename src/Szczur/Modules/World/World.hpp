#pragma once

#include "Szczur/Utility/SFML3D/RenderLayer.hpp"

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "SceneManager.hpp"

namespace rat
{

class World : public Module<Window, Input>
{
public:

	///
	World();

	///
	World(const World&) = delete;

	///
	World& operator = (const World&) = delete;

	///
	World(World&&) = delete;

	///
	World& operator = (World&&) = delete;

	///
	~World();

	///
	void update(float deltaTime);

	///
	void render();

private:

	///
	SceneManager& _getScenes();

	///
	const SceneManager& _getScenes() const;

	SceneManager _scenes;

	/// Layer used to render objects before postprocess shader
	sf3d::RenderLayer _layer;

	/// Shader program used to render object to the layer
	std::unique_ptr<sf3d::ShaderProgram> _shader;
};

}
