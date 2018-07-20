#pragma once

#include <memory>

#include <Szczur/Modules/World/Entity.hpp>

#include <Szczur/Utility/SFML3D/Geometry/CircleShape.hpp>

namespace rat {

class Battle;
class BattleScene;
class BattleSkill;

class Script;
class World;

class BattlePawn {
public:

// Constructors

	///
	BattlePawn(BattleScene* scene);

	///
	~BattlePawn();

// Positioning
	
	/// Set position
	void setPosition(glm::vec2 position);
	/// Get position
	glm::vec2 getPosition();

	/// Move
	void move(glm::vec2 delta);
	void move(glm::vec2 normalizedDirection, float distance);

	/// Get _moved flag and set it on false
	bool popMovedFlag();

	/// Return normalized vector of direction from pawn to specific position
	glm::vec2 getNormalDirection(glm::vec2 position);

	///
	float getDistanceTo(glm::vec2 position);

// Angle

	float getAngleTo(glm::vec2 position);

// Time

	///
	bool isTimeFull();

// Collider
	
	/// Set/Get radius of collider
	void setRadius(float radius);
	float getRadius();

	///
	void collision(BattlePawn* target);
	
// Model

	/// Load and set armature
	void setArmature(const std::string& armDir);
	/// Get loaded armature
	ArmatureComponent* getArmature();
	/// Get path o armature
	const std::string& getArmaturePath();

	///
	void setFlipForPosition(glm::vec2 position);

	/// Set origin
	void setOrigin(const glm::vec2& origin);
	/// Get origin
	glm::vec2 getOrigin();

	/// Set scale of pawn
	void setScale(float scale);
	/// Get scale of pawn
	float getScale();

	///
	void setHeight(float height);
	///
	float getHeight();

	///
	void renderBars(sf3d::RenderTarget& canvas);

// Skills

	///
	BattleSkill* newSkill(const std::string& name);

	///
	void renderSkills(sf3d::RenderTarget& canvas);

	///
	void updateSkills();

	///
	BattleSkill* getSkill(int index);

	///
	int getSkillAmount();

	///
	void setSkills(const std::vector<std::string>& _skillNames);

// Statuses

	///
	void setStatus(int index);

	///
	bool checkStatus(int index);

	///
	void clearStatuses();

// Script

	///
	void loadScript(const std::string& scriptPath);

	///
	const std::string& getScriptPath();

	///
	static void initScript(Script& script);

// Identification

	///
	void setNameID(const std::string& name);
	///
	const std::string& getNameID();
	
	///
	BattleScene* getScene();

// Health

	void setHealth(float value);
	void setMaxHealth(float value);
	void addHealth(float value);

	float getHealth();
	float getMaxHealth();

	void addHealthHit(float value);
	void setHealthHit(float value);

// Time

	void setTime(float value);
	void setMaxTime(float value);
	void addTime(float value);
	float getTime();
	float getMaxTime();

// Damage animation

	void setDamageAnimation(const std::string& name, float fade, bool wait, float speed);

// Main

	///
	void update(float deltaTime); 

	///
	void render(sf3d::RenderTarget& target);

	/// 
	void bottomRender(sf3d::RenderTarget& target);

	///
	// void updateEditor(bool& removeSignal);

private:

// Data object

	///
	void _setScriptDataObject(std::string key, sol::stack_object value);
	///
	sol::object _getScriptDataObject(const std::string& key);

private:

// Health
	float _health = 100.f;
	float _maxHealth = 100.f;

// Time
	float _time = 0.f;
	float _maxTime = 100.f;

// Data object
	std::unordered_map<std::string, sol::object> _scriptData;

// Positioning
	glm::vec2 _position;
	bool _moved = false;

// Callbacks
	sol::function _onCollision;
	sol::function _onUpdate;

// Collider
	float _radius = 50.0;
	sf3d::CircleShape _radiusShape;

// Model
	float _scale = 1.00;
	ArmatureComponent* _armature = nullptr;
	Entity _entity;
	float _height = 0.f;

// Modules
	Script* _scriptModule;
	World* _worldModule;
	Battle* _battleModule;

// Skills
	std::vector<std::unique_ptr<BattleSkill>> _skills;

// Statuses
	std::vector<int> _statuses;

// Script
	std::string _scriptPath;

// Identification
	std::string _nameID;
	BattleScene* _scene;

// Damage animation
	bool _isDamageAnimation = false;
	std::string _damageAnimationName = "";
	float _damageAnimationFade = 0.1;
	bool _damageAnimationWait = false;
	float _damageAnimationSpeed = 1.0;
};

}
