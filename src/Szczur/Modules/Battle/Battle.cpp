#include "Battle.hpp"

#include "BattleConfig.hpp"
#include "BattleScene.hpp"

#include <Szczur/Modules/World/Scene.hpp>

namespace rat {

// ----------------- Constructors -----------------

Battle::Battle()
{
    LOG_INFO("Initializing Battle module");

    _pawnManager.loadAllPawns("Assets/Pawns/pawns.json");

    LOG_INFO("Module Battle initialized");
}

Battle::~Battle()
{
    LOG_INFO("Module Battle destructed");
}

// ----------------- Getters -----------------

BattlePawnManager& Battle::getPawnManager()
{
	return _pawnManager;
}

// ----------------- Scene -----------------

BattleScene* Battle::activateScene(Scene* scene)
{
	_currentScene.reset(new BattleScene(scene));
	return _currentScene.get();
}

void Battle::deactivateScene() 
{
	_currentScene = nullptr;
}

bool Battle::isActiveScene()
{
	return _currentScene.get() != nullptr;
}

// ----------------- Main -----------------

void Battle::render(sf3d::RenderTarget& target)
{
	if(_currentScene) {
		_currentScene->render(target);
	}
}

void Battle::update(float deltaTime)
{
	if(_currentScene) {
		_currentScene->update(deltaTime);
	}
}

void Battle::updateEditor()
{
	if(_currentScene) {
		if(ImGui::Begin("Battle editor")) {
		
			// Close editor button
			bool closeFlag = false;
			if(ImGui::Button("Close")) {
				closeFlag = true;
			}

			// Save config button
			if(ImGui::Button("Update config")) {
				_currentScene->updateConfig();
			}
		

			// Reload pawns.json file
			ImGui::SameLine();
			if(ImGui::Button("Reload pawns")) {
				getPawnManager().reloadAllPawns();
				_currentScene->reloadAllPawns();
			}

			// Update editor for current scene
			ImGui::Separator();
			_currentScene->updateEditor();

			if(closeFlag) {
				deactivateScene();
			}
		}
		ImGui::End();
	}
}

} // namespace rat
