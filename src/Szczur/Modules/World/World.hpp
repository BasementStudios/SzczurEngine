#pragma once

#include <memory>

#include "Scene.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

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
	SceneManager _scenes;
};

}
