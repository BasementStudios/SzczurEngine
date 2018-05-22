#pragma once 

#include <SFML/Graphics.hpp>

#include "BattleTexture.hpp"

namespace rat {

class Input;
class BattleScene;
class BattleSkill;
class Script;

class BattlePawn {
	using RenderTarget = sf::RenderTarget;
public:

// Constructors

	///
	BattlePawn(BattleScene& battleScene);

// Main

	///
	void update(float deltaTime);

	///
	void render(RenderTarget& canvas);

// Getters

	///
	BattleScene* getScene();

	///
	const sf::Vector2f& getPosition() const;

	///
	float getColliderRadius() const;

	/// Check collision with point
	bool hitTest(const sf::Vector2f& point) const;
	
	/// (angle in format -pi..pi), 1:left, 0:right
	bool getDirection(float angle) const;

	///
	const std::vector<std::unique_ptr<BattleSkill>>& getSkills() const;

	///
	sf::Sprite getIconSprite(int iconFrame) const;

	///
	const std::vector<std::pair<BattleSkill*, int>>& getUsableSkills() const;

// Manipulations

	/// Set position with collision callculation
	void setPosition(const sf::Vector2f& position);
	void setPositionX(float x);
	void setPositionY(float y);

	/// Set position without collision callculation
	void setRawPosition(const sf::Vector2f& position);
	void setRawPositionX(float x);
	void setRawPositionY(float y);

	///
	void move(const sf::Vector2f& vector);

	///
	void moveRaw(const sf::Vector2f& vector);

	///
	float getAngleTo(const sf::Vector2f& pos);

	///
	float getAngleToPointer();

	/// 
	void moveInDirection(float angle, float distance);

	/// Load pawn from folder
	void loadPawn(const std::string& dirPath);

	/// 
	void setCollisionStatus(bool flag);

	/// 
	bool isCollision();

// Battle

	/// 
	void addHp(float value);

	/// 
	void setHp(float value);

	/// 
	void setMaxHp(float value);

	/// 
	float getHp() const;

	/// 
	float getMaxHp() const;

// Skills

	/// Add new skill for pawn
	BattleSkill* newSkill(const std::string& skillName);
	
	/// [NEED FIX!] Create and use skill wihout name
	// BattleSkill* newInstaSkill();

	/// Get skill by name
	BattleSkill* getSkill(const std::string& skillName);

	/// Use skill 
	BattleSkill* useSkill(BattleSkill* skill);

	/// Use skill by name (add skill to queue)
	BattleSkill* useSkill(const std::string& skillName);

	///
	void addUsable(BattleSkill* skill, int icon);

	///
	void removeUsable(BattleSkill* skill);

	/// 
	void clearUsable();

// Controller

	void renderController(BattlePawn::RenderTarget& canvas, bool selected) const;

// Visual

	///
	void setFrame(int frame);

	///
	void setFlip(bool flag);

	/// (offset y)
	void renderHpBar(sf::RenderTarget& canvas, float oy);

// Scripts

	///
	static void initScript(Script& script);
	
	/// Run script for this pawn
	void runScript(const std::string& filePath);

private:

// Main

	BattleScene& battleScene;
	sf::Vector2f pos{0,0};
	float colliderRadius = 20.f;
	bool collision = false;

// Stats

	float health = 50;
	float maxHealth = 100;

// Visual
	
	BattleTexture texturePose;
	BattleTexture textureSkillIcons;
	int frame = 0;
	bool flip = false;

// Controller
	
	Input& input;

// Skills
	
	std::vector<std::pair<BattleSkill*, int>> usableSkills;
	std::vector<std::unique_ptr<BattleSkill>> skills;

};

}