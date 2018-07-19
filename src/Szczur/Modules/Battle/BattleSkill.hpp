#pragma once

#include <unordered_map>
#include <string>

#include <sol.hpp>

#include <Szczur/Utility/SFML3D/RenderTarget.hpp>

namespace rat 
{

class Battle;
class BattlePawn;

class Script;

class BattleSkill
{
public:

// Constructors

	BattleSkill();

// Using skill

	void update();

	void selectedUpdate();

	void use(bool zeroTime);

	void setCostPP(int value);

	int getCostPP();

// Identification

	///
	void setName(const std::string& name);

	///
	const std::string& getName();

	///
	void setPawn(BattlePawn* pawn);
	///
	BattlePawn* getPawn();

// Scripting

	static void initScript(Script& script); 

// Rendering

	void renderSpellIndicator();

private:

// Data object

	///
	void _setScriptDataObject(std::string key, sol::stack_object value);
	///
	sol::object _getScriptDataObject(const std::string& key);

private:

// PP
	int _costPP = 0;

// Identification

	std::string _name;
	BattlePawn* _pawn = nullptr;

// Data object

	std::unordered_map<std::string, sol::object> _scriptData;

// Callbacks

	sol::function _onUpdate;
	sol::function _onSelectedUpdate;
	sol::function _onSpellIndicator;
	sol::function _onUsed;

// Modules

	Battle* _battleModule;

};

}
