#pragma once

#include <memory>

#include "SceneManager.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Camera/Camera.hpp"
#include "Szczur/Utility/Modules/Module.hpp"

#ifdef EDITOR
	#include "LevelEditor.hpp"
#endif

namespace rat
{

class World : public Module<Window, Input, Camera>
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
	#ifdef EDITOR
	LevelEditor _levelEditor;
	#endif
};

}
