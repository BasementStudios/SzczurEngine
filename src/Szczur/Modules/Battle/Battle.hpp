#pragma once

#include <memory>

#include "BattlePawnManager.hpp"
#include "BattleSpellIndicatorManager.hpp"
#include "BattleTextureManager.hpp"

#include <Szczur/Utility/Modules/Module.hpp>
#include <Szczur/Modules/Window/Window.hpp>
#include <Szczur/Modules/Input/Input.hpp>
#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Player/Player.hpp>

namespace rat {

class BattleScene;
class BattlePawn;
class BattleEffect;
class BattleTrigger;
class BattleSprite;
class BattleAnimationSprite;
class BattleSkill;

class Scene;
class Entity;

class Battle : public Module<Window, Input, Script, Player> {
public:

// Constructors

	///
	Battle();

	///
	~Battle();

// Pawn manager

	///
	BattlePawnManager& getPawnManager();

// Spell indicator

	///
	BattleSpellIndicatorManager& getSpellIndicatorManager();

// Texture Manager

	BattleTextureManager& getTextureManager();

// Effects

	///
	BattleEffect* newEffect();

	///
	BattleEffect* newEffect(BattleSkill* skill);

// Triggers

	///
	BattleTrigger* newTrigger();

// Pawns

	///
	BattlePawn* getPlayer();

	///
	void setPlayerSkills(const std::vector<std::string>& _skillNames);

// Sprites

	///
	BattleSprite* newSprite(const std::string& textureName);

	///
	BattleAnimationSprite* newAnimationSprite(const std::string& textureName);

// Scene

	///
	BattleScene* createScene(const glm::vec3& position, const glm::vec2& size, float scale);

	///
	void deactivateScene();

	///
	bool isActiveScene();

	///
	BattleScene* getCurrentScene();

// Mouse

	///
	bool isSkillButtonPressed();
	///
	bool isSkillButtonKept();
	///
	bool isSkillButtonReleased();

	/// Get Y-plane projection of mouse position on scene
	glm::vec3 getCursorPosition(float height);

	///
	glm::vec2 getCursorPosition();

// Math

	///
	glm::vec2 getMovedPosition(glm::vec2 position, float angle, float distance);

	///
	bool checkCollisionCC(glm::vec2 pos1, float r1, glm::vec2 pos2, float r2);

	///
	glm::vec2 getNormalDirection(glm::vec2 from, glm::vec2 to);

	///
	float getDistance(glm::vec2 pos1, glm::vec2 pos2);

// Main

	///
	void render(sf3d::RenderTarget& target);

	///
	void update(float deltaTime);

// Script

	///
	void initScript();

private:

	BattlePawnManager _pawnManager;
	BattleSpellIndicatorManager _spellIndicatorManager;
	BattleTextureManager _textureManager;

	std::unique_ptr<BattleScene> _currentScene;
	bool _battleActive = false;

	glm::vec2 _defaultWindowSize;

};

}
