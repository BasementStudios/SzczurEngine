#pragma once

#include <sol.hpp>
#include <SFML/System/Clock.hpp>

namespace rat
{

class BattlePawn;
class BattleSkill;

class Script;

class BattleEffect
{

public:

// Constructors

// State

	///
	void activate();

	///
	bool isActive();

// Data object
	
	///
	void clearPawnFromData(BattlePawn* pawnToDelete);

// Lifetime

	///
	bool isAlive();

	///
	void kill();

	///
	void setDuration(float duration);

	///
	float getDuration();

	///
	float getCurrentDuration();

	///
	bool checkLifetime(float duration);

// Parent skill

	void setSkill(BattleSkill* skill);

	BattleSkill* getSkill();

// Main

	///
	void update();

// Scripts

	static void initScript(Script& script);

private:

// Data object

	///
	void _setScriptDataObject(std::string key, sol::stack_object value);
	///
	sol::object _getScriptDataObject(const std::string& key);

private:

// Parent skill
	BattleSkill* _skill = nullptr;

// Callbacks
	sol::function _onUpdate;
	sol::function _onFinish;

// Lifetime
	float _duration = 0.f;
	bool _isActive = false;
	bool _isLife = true;
	sf::Clock _clock;


// Data object
	std::unordered_map<std::string, sol::object> _scriptData;
};

}