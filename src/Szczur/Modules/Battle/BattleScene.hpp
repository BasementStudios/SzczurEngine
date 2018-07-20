#pragma once

#include <vector>
#include <string>
#include <memory>

#include <Szczur/Utility/SFML3D/RenderTarget.hpp>
#include <Szczur/Utility/SFML3D/Geometry/RectangleShape.hpp>
#include <Szczur/Utility/SFML3D/Geometry/CircleShape.hpp>

#include <sol.hpp>

namespace rat {

class Battle;
class BattlePawn;
class BattleSkill;
class BattleEffect;
class BattleTrigger;
class BattleSprite;
class BattleAnimationSprite;

class Scene;
class Window;
class Input;
class Script;

class BattleScene 
{
public:

// Constructors

	///
	BattleScene(const glm::vec3& position, const glm::vec2& size, float scale);
	
	///
	~BattleScene();

// Main parameters

	///
	void setParameters(const glm::vec3& position, const glm::vec2& size, float scale);
	
	///
	glm::vec3 getPosition();

	///
	glm::vec2 getSize();

	///
	float getScale();

// Callbacks

	void finish();

// Parent scene

	///
	Scene* getScene();

// Pawns

	///
	BattlePawn* addPawn(const std::string& nameID, const glm::vec2& position);

	///
	BattlePawn* addPlayer(const std::string& nameID, const glm::vec2& position);

	///
	void reloadAllPawns();

	///
	void updateRemovingPawns();

	///
	std::vector<std::unique_ptr<BattlePawn>>& getPawns();

	///
	BattlePawn* getPlayer();

// Effects

	///
	BattleEffect* newEffect();
	///
	void removePawnFromEffects(BattlePawn* pawn);

// Effects

	///
	BattleTrigger* newTrigger();

// Sprites

	///
	BattleSprite* newSprite(const std::string& textureName);

	///
	BattleAnimationSprite* newAnimationSprite(const std::string& textureName);

	///
	void removeSprite(BattleSprite* sprite);

// State

	///
	bool isActive();
	///
	void activate();

// Cursor

	///
	glm::vec2 getCursorPosition();

// Collision

	void updateCollision();

// GUI

	void updateGuiParameters();

// PP

	void setPP(int value);
	void addPP(int value);
	int getPP();

	void setMaxPP(int value);
	void addMaxPP(int value);
	int getMaxPP();

	void setBrokenPP(int value);
	void addBrokenPP(int value);
	int getBrokenPP();

// Main

	///
	void update(float deltaTime);

	///
	void render(sf3d::RenderTarget& target);

// Script

	///
	static void initScript(Script& script);

private:

// Callbacks
	sol::function _onFinish;

// Pawns
	std::vector<std::unique_ptr<BattlePawn>> _pawns;
	BattlePawn* _player = nullptr;

// Effects
	std::vector<std::unique_ptr<BattleEffect>> _effects;

// Sprites
	std::vector<std::unique_ptr<BattleSprite>> _sprites;

// Triggers
	std::vector<std::unique_ptr<BattleTrigger>> _triggers;

// Skills
	BattleSkill* _selectedSkill = nullptr;

// PP
	int _maxPP = 7;
	int _pp = 4;
	int _brokenPP = 0;

// View
	sf3d::RectangleShape _areaShape;
	float _scale;

// Size
	glm::vec2 _size;

// Positioning
	glm::vec3 _position;

// Parent scene
	Scene* _scene;

// State
	bool _activated = false;

// Modules
	Battle* _battleModule;
	//Window* _windowModule;
	Input* _inputModule;
};

}