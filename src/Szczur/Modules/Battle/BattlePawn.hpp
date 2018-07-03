#pragma once

#include <memory>

#include <Szczur/Modules/World/Entity.hpp>

#include <Szczur/Utility/SFML3D/CircleShape.hpp>

namespace rat {

class BattleScene;
class Script;

class BattlePawn {
public:

// Constructors

	///
	BattlePawn(BattleScene* scene);

// Getters : General

	///
	BattleScene* getScene();

	///
	const std::string& getNameID();

	///
	float getScale();

	///
	const std::string& getArmaturePath();

	///
	glm::vec2 getOrigin();

	///
	const std::string& getScriptPath();

// Getters : Battle

	///
	glm::vec2 getPosition();

	///
	float getRadius();

	///
	ArmatureComponent* getArmature();

// Manipulations : General

	///
	void setNameID(const std::string& name);

	///
	void setScale(float scale);

	///
	void setArmature(const std::string& armDir);

	///
	void setOrigin(const glm::vec2& origin);

// Manipulations : Battle

	///
	void setRadius(float radius);

	///
	void setPosition(glm::vec2 position);

	///
	void move(glm::vec2 delta);

	///
	void loadScript(const std::string& scriptPath);

	/// Get _moved flag and set it on false
	bool popMovedFlag();

// Main

	///
	void update(float deltaTime); 

	///
	void render(sf3d::RenderTarget& target);

	///
	void updateEditor(bool& removeSignal);

private:

// General

	std::string _nameID;
	float _scale = 1.00;
	std::string _scriptPath;
	
// Battle

	glm::vec2 _position;
	float _radius = 50.0;
	bool _moved = false;
	ArmatureComponent* _armature = nullptr;

// Render

	Entity _entity;
	BattleScene* _scene;
	bool _movedRender = false;
	sf3d::CircleShape _radiusShape;

// Modules

	Script* _scriptModule;
};

}