#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"

#include "Pawn.hpp"

namespace rat
{
using namespace battle_field;

struct BattleField : public Module<Canvas, Script, Input>
{
	std::vector<std::unique_ptr<Pawn>> pawns;
	
	template <typename Tuple>
	BattleField(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_CONSTRUCTOR();
		initScript();
		init();
	}
	
	auto addPawn(Pawn* newPawn) {		
		pawns.emplace_back(newPawn);
		return pawns.back().get();
	}
	
	void reset() {
		pawns.clear();
	}
		

	void render() {
		for(auto& obj : pawns) {
			obj->render(getModule<Canvas>());
		}
	}

	void update() {
		auto& input = getModule<Input>();
		if(input.isPressed(Keyboard::Space)) {
			reset();
			init();
		}
	}	

	void testInit() {
		// SCRIPT_NEW_MODULE(BattleField,pos,size,setPos,setSize,instance)
	}
	
	void init() {
		sol::state& lua = getModule<Script>().get();
		lua.script_file("../src/Szczur/Modules/BattleField/test.lua");
	}
	
	void initScript() {
		// sol::state& lua = getModule<Script>().get();
		// auto mod = lua.create_table("BattleField");
		// SCRIPT_NEW_MODULE(BattleField, "../src/Szczur/Modules/BattleField/BattleField.lua")
		auto module = getModule<Script>().newModule("BattleField", "../src/Szczur/Modules/BattleField/BattleField.lua");
		SCRIPT_SET_MODULE(BattleField, addPawn)
		
	// Module script
		// temp.set_function("addPawn", &BattleField::addPawn, this);
		// SCRIPT_NEW_MODULE(BattleField,addPawn)
		
	// Class script
		getModule<Script>().initClasses<Pawn>();
		// Pawn::initScript(getModule<Script>());
	}
};

}