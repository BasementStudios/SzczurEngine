#pragma once

#include <memory>

#include "BattlePawnManager.hpp"

#include <Szczur/Utility/SFML3D/View.hpp>
#include <Szczur/Utility/Modules/Module.hpp>
#include <Szczur/Modules/Window/Window.hpp>

namespace rat {

class BattleConfig;
class BattleScene;
class Scene;

class Battle : public Module<Window> {
public:

// Constructors

	///
	Battle();

	///
	~Battle();

// Getters

	///
	BattlePawnManager& getPawnManager();

// Scene

	///
	BattleScene* activateScene(Scene* scene);

	///
	void deactivateScene();

	///
	bool isActiveScene();
	
// Main

	///
	void render(sf3d::RenderTarget& target);

	///
	void update(float deltaTime);

	///
	void updateEditor();

private:

	BattlePawnManager _pawnManager;
	std::unique_ptr<BattleScene> _currentScene;
};

}
