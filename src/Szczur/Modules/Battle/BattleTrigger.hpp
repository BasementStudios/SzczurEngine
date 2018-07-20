#pragma once

#include <string>
#include <unordered_map>

#include <glm/vec2.hpp>

#include <sol.hpp>

namespace rat
{

class BattleScene;
class BattlePawn;

class Script;

class BattleTrigger
{

public:

	BattleTrigger(BattleScene* scene);
	
	///
	void setPosition(glm::vec2 position);
	///
	glm::vec2 getPosition();

	///
	void setRadius(float radius);
	///
	float getRadius();

	///
	void run();

	///
	void kill();
	///
	bool isAlive();
	///
	void runAndKill();

	///
	bool check(BattlePawn* pawn);

	///
	static void initScript(Script& script);

private:

// Data object

	///
	void _setScriptDataObject(std::string key, sol::stack_object value);
	///
	sol::object _getScriptDataObject(const std::string& key);

private:

	glm::vec2 _position;
	float _radius;

	sol::function _onOverlap;

	BattleScene* _scene;

	bool _isLife = true;

// Data object

	std::unordered_map<std::string, sol::object> _scriptData;

};

}