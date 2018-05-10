#pragma once

#include "SceneManager.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"

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

};

}
