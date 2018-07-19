#include "BattleEffect.hpp"

#include "BattlePawn.hpp"
#include "BattleSkill.hpp"

#include <Szczur/Modules/Script/Script.hpp>

namespace rat
{

// ----------------- State -----------------

void BattleEffect::activate()
{
	_isActive = true;
	_clock.restart();
}

bool BattleEffect::isActive()
{
	return _isActive && _isLife;
}

// ----------------- Data object -----------------

void BattleEffect::clearPawnFromData(BattlePawn* pawnToDelete)
{
	for(auto itr = _scriptData.begin(); itr != _scriptData.end(); ) {
		
		auto obj = itr->second;
		if(obj.is<BattlePawn*>()) {

			BattlePawn* pawn = obj.as<BattlePawn*>();
			if(pawn == pawnToDelete) {
				//LOG_INFO("Pawn deleted from effect!")
				itr = _scriptData.erase(itr);
			}
			else {
				++itr;
			}			
		}
		else {
			++itr;
		}
	}
}

// ----------------- Lifetime -----------------

bool BattleEffect::isAlive()
{
	return _isLife;
}

void BattleEffect::kill()
{
	if(_isLife) {
		if(_onFinish.valid()) {
			_onFinish(this);
		}
	}
	_isLife = false;
}

void BattleEffect::setDuration(float duration)
{
	_duration = duration;
}

float BattleEffect::getDuration()
{
	return _duration;
}

float BattleEffect::getCurrentDuration()
{
	return _clock.getElapsedTime().asSeconds();
}

bool BattleEffect::checkLifetime(float duration)
{
	return _clock.getElapsedTime().asSeconds() < duration;
}

// ----------------- Parent skill -----------------

void BattleEffect::setSkill(BattleSkill* skill)
{
	_skill = skill;
}	

BattleSkill* BattleEffect::getSkill()
{
	return _skill;
}

// ----------------- Main -----------------

void BattleEffect::update()
{
	if(_isActive) {
		
		// Update
		if(_onUpdate.valid()) {
			_onUpdate(this);
		}

		// Kill
		if(!checkLifetime(_duration)) {
			kill();
		}
	}
}

// ----------------- Scripts -----------------

void BattleEffect::initScript(Script& script)
{
	auto object = script.newClass<BattleEffect>("BattleEffect", "Battle");

	// Callbacks
	object.set("onUpdate", &BattleEffect::_onUpdate);
	object.set("onFinish", &BattleEffect::_onFinish);

	// State
	object.set("activate", &BattleEffect::activate);
	object.set("kill", &BattleEffect::kill);
	object.set("isActive", &BattleEffect::isActive);
	object.set("isAlive", &BattleEffect::isActive);

	// Lifetime
	object.set("setDuration", &BattleEffect::setDuration);
	object.set("getDuration", &BattleEffect::getDuration);
	object.set("getCurrentDuration", &BattleEffect::getCurrentDuration);
	object.set("checkLifetime", &BattleEffect::checkLifetime);

	// SKill
	object.set("getSkill", &BattleEffect::getSkill);

	// Data object
	object.set(sol::meta_function::index, &BattleEffect::_getScriptDataObject);
	object.set(sol::meta_function::new_index, &BattleEffect::_setScriptDataObject);

	object.init();
}

// ----------------- Data object -----------------

void BattleEffect::_setScriptDataObject(std::string key, sol::stack_object value)
{
	auto obj = _scriptData.find(key);

	if (obj == _scriptData.cend()) {
		_scriptData.insert(obj, { std::move(key), std::move(value) });
	}
	else {
		obj->second = sol::object(std::move(value));
	}
}
sol::object BattleEffect::_getScriptDataObject(const std::string& key)
{
	auto obj = _scriptData.find(key);
	return (obj == _scriptData.cend() ? sol::lua_nil : obj->second);
}

} // namespace rat