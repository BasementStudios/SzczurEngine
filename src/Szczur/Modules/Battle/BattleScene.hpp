#pragma once

#include <vector>

#include <Szczur/Utility/SFML3D/RenderTarget.hpp>
#include <Szczur/Utility/SFML3D/RectangleShape.hpp>

namespace rat {

class BattlePawn;
class BattleConfig;
class Scene;
class Battle;

class BattleScene {
public:

// Constructors

	///
	BattleScene(Scene* scene);

	///
	~BattleScene();

// Getters

	///
	Scene* getScene();

	///
	glm::vec3 getPosition();

	///
	glm::vec3 getRealPosition();

	///
	glm::vec2 getSize();

	///
	float getScale();

// Config

	///
	void setupConfig(const BattleConfig& config);

	///
	void setupConfig(BattleConfig* config);

	///
	BattleConfig createConfig();

	///
	void updateConfig();


// Manipulations

	///
	void setSize(glm::vec2 size);

	///
	void setPosition(glm::vec3 position);

	///
	void setScale(float scale);

	///
	BattlePawn* addPawn(const std::string& nameID);

	///
	void reloadAllPawns();

// Main

	///
	void update(float deltaTime);

	///
	void render(sf3d::RenderTarget& target);

	///
	void updateEditor();

private:

// Render

	///
	void _updateAreaShape();

	///
	void _updateLeftCorner();

// Vars

	BattleConfig* _lastConfig = nullptr;

	sf3d::RectangleShape _areaShape;

	Battle* _battleModule;
	std::vector<std::unique_ptr<BattlePawn>> _pawns; 
	Scene* _scene;

	float _scale;
	glm::vec3 _position;
	glm::vec2 _size;
	glm::vec3 _leftCorner;

};

}