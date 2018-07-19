#include "BattleTrigger.hpp"

#include "BattleScene.hpp"
#include "BattlePawn.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat
{

BattleTrigger::BattleTrigger(BattleScene* scene)
	: _scene(scene)
{

}

void BattleTrigger::setPosition(glm::vec2 position)
{
	_position = position;
}

glm::vec2 BattleTrigger::getPosition()
{
	return _position;
}

void BattleTrigger::setRadius(float radius)
{
	_radius = radius;
}

float BattleTrigger::getRadius()
{
	return _radius;
}

void BattleTrigger::run()
{
	if(_onOverlap.valid()) {
		for(auto& pawn : _scene->getPawns()) {
			if(check(pawn.get())) {
				_onOverlap(this, pawn.get());
			}
		}
	}
}

void BattleTrigger::kill()
{
	_isLife = false;
}

bool BattleTrigger::isAlive()
{
	return _isLife;
}

void BattleTrigger::runAndKill()
{
	run();
	kill();
}

bool BattleTrigger::check(BattlePawn* pawn)
{
	float dis = glm::distance(_position, pawn->getPosition());
	return dis < _radius + pawn->getRadius();
}

void BattleTrigger::initScript(Script& script)
{
	auto object = script.newClass<BattleTrigger>("BattleTrigger", "Battle");

	// Callbacks
	object.set("onOverlap", &BattleTrigger::_onOverlap);

	// Position
	object.set("setPosition", &BattleTrigger::setPosition);
	object.set("getPosition", &BattleTrigger::getPosition);

	// Radius
	object.set("setRadius", &BattleTrigger::setRadius);
	object.set("getRadius", &BattleTrigger::getRadius);

	// Check
	object.set("check", &BattleTrigger::check);

	// Run
	object.set("run", &BattleTrigger::run);
	object.set("kill", &BattleTrigger::kill);
	object.set("isAlive", &BattleTrigger::isAlive);
	object.set("runAndKill", &BattleTrigger::runAndKill);

	// Data object
	object.set(sol::meta_function::index, &BattleTrigger::_getScriptDataObject);
	object.set(sol::meta_function::new_index, &BattleTrigger::_setScriptDataObject);

	object.init();
}

// ----------------- Data object -----------------

void BattleTrigger::_setScriptDataObject(std::string key, sol::stack_object value) {
	auto obj = _scriptData.find(key);

	if (obj == _scriptData.cend()) {
		_scriptData.insert(obj, { std::move(key), std::move(value) });
	}
	else {
		obj->second = sol::object(std::move(value));
	}
}

sol::object BattleTrigger::_getScriptDataObject(const std::string& key) {
	auto obj = _scriptData.find(key);
	return (obj == _scriptData.cend() ? sol::lua_nil : obj->second);
}

}