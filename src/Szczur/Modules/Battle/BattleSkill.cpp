#include "BattleSkill.hpp"

#include "Battle.hpp"
#include "BattlePawn.hpp"

#include <Szczur/Modules/Battle/Battle.hpp>
#include <Szczur/Modules/Battle/BattleScene.hpp>

#include <Szczur/Modules/Script/Script.hpp>

namespace rat
{

// ----------------- Using skill -----------------

BattleSkill::BattleSkill()
{
	_battleModule = detail::globalPtr<Battle>;
}

void BattleSkill::update()
{
	if(_onUpdate.valid()) {
		_onUpdate(this);
	}
}

void BattleSkill::selectedUpdate()
{
	if(_onSelectedUpdate.valid()) {
		_onSelectedUpdate(this);
	}
}

void BattleSkill::use(bool zeroTime)
{	
	auto* battle = detail::globalPtr<Battle>;

	if(_pawn != nullptr) {
	
		if(!_pawn->isTimeFull()) 
			return;
	
		if(getPawn() == battle->getPlayer() && battle->getCurrentScene()->getPP()<getCostPP())
			return;

		if(zeroTime) {
			_pawn->setTime(0);
		}
	}

	if(getPawn() == battle->getPlayer())
		battle->getCurrentScene()->addPP(-getCostPP());

	if(_onUsed.valid()) {
		_onUsed(this);
	}
}

void BattleSkill::renderSpellIndicator()
{
	if(_onSpellIndicator.valid()) {
		_onSpellIndicator(this, _battleModule->getSpellIndicatorManager());
	}
}

void BattleSkill::setCostPP(int value)
{
	_costPP = value;
}

int BattleSkill::getCostPP()
{
	return _costPP;
}

// ----------------- Identification -----------------

void BattleSkill::setName(const std::string& name)
{
	_name = name;
}

const std::string& BattleSkill::getName()
{
	return _name;
}

void BattleSkill::setPawn(BattlePawn* pawn)
{
	_pawn = pawn;
}

BattlePawn* BattleSkill::getPawn()
{
	return _pawn;
}

// ----------------- Scripting -----------------

void BattleSkill::initScript(Script& script)
{
	auto object = script.newClass<BattleSkill>("BattleSkill", "Battle");

	// Using skill
	object.set("use", &BattleSkill::use);

	// Callbacks
	object.set("getPawn", &BattleSkill::getPawn);
	object.set("onUpdate", &BattleSkill::_onUpdate);
	object.set("getName", &BattleSkill::getName);
	object.set("onSelectedUpdate", &BattleSkill::_onSelectedUpdate);
	object.set("onSpellIndicator", &BattleSkill::_onSpellIndicator);
	object.set("onUsed", &BattleSkill::_onUsed);
	object.set("setCostPP", &BattleSkill::setCostPP);
	object.set("getCostPP", &BattleSkill::getCostPP);

	// Data object
	object.set(sol::meta_function::index, &BattleSkill::_getScriptDataObject);
	object.set(sol::meta_function::new_index, &BattleSkill::_setScriptDataObject);

	object.init();
}

// ----------------- Data object -----------------

void BattleSkill::_setScriptDataObject(std::string key, sol::stack_object value) {
	auto obj = _scriptData.find(key);

	if (obj == _scriptData.cend()) {
		_scriptData.insert(obj, { std::move(key), std::move(value) });
	}
	else {
		obj->second = sol::object(std::move(value));
	}
}

sol::object BattleSkill::_getScriptDataObject(const std::string& key) {
	auto obj = _scriptData.find(key);
	return (obj == _scriptData.cend() ? sol::lua_nil : obj->second);
}

}