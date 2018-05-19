#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat {

class BattlePawn;

class BattleSkill {
public:

// Constructors

	///
	BattleSkill(BattlePawn* pawn, const std::string& name);

// Main

	/// Init when skill is created
	virtual void init();

	/// Update when skill is activated
	virtual void update(float deltaTime);
	
// Getters

	///
	bool isFinished();

	///
	void getPawn();

	///
	const std::string& getName() const;
	
// Manipulations

	///
	void kill();

	/// Example: SELECT_ENEMY|SELECT_OBJECT
	void setType(size_t type);

// Controller

	///
	void renderCircle(float radius);

// Scripts

	///
	static void initScript(Script& script);

protected:

	BattlePawn* pawn = nullptr;

	/// Binary mask
	size_t selectType = SELECT_AUTO;

	bool updateStatus = false;
	bool killed = false;
	std::string name;

	Input& input;

public:

	sol::function onInit;
	sol::function onUpdate;
	sol::function onActive;

	enum SELECT_ {
		SELECT_AUTO = 1,
		SELECT_CURSOR = 1<<1,
		SELECT_ENEMY = 1<<2,
		SELECT_FRIEND = 1<<3,
		SELECT_OBJECT = 1<<4,
		SELECT_ANY = SELECT_ENEMY | SELECT_FRIEND | SELECT_OBJECT
	};

};

/*
	Script:
	> onSignal
*/

// class BattleSkillMove : public BattleSkill {
// public:

// 	///
// 	void init() override;
	
// 	///
// 	void update(float deltaTime) override;

// private:

// 	sf::Clock clock;
// 	sf::Vector2f posBegin;
// 	sf::Vector2f posEnd;
// 	float angle;
// 	float distance;
	
// public:

// 	// Signal after finish skill
// 	sol::function onSignal;

// };

}