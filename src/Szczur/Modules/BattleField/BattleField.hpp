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
	
	
// BACKUP

// #define PRINT(...) _DO_RUN(_ARG_NUM(__VA_ARGS__))(__VA_ARGS__)
// #define _DO_RUN(N) DOG(N)
// #define _DO_NAME(N) PRINT ## N

// /* max supported args is 5 for now */
// #define _ARG_NUM(...) _ARG_NUM_VALS(__VA_ARGS__, 5, 4, 3, 2, 1)
// #define _ARG_NUM_VALS(N1, N2, N3, N4, N5, N, ...) N

// #define _DO_MY()

// #define _DO_1(F, A, B) printf("%d\n", A)
// #define _DO_2(F, A, B, ...) printf("%d\n", A), PRINT1(F, A, __VA_ARGS__)
// #define _DO_3(F, A, B, ...) printf("%d\n", A), PRINT2(F, A, __VA_ARGS__)
// #define _DO_4(F, A, B, ...) printf("%d\n", A), PRINT3(F, A, __VA_ARGS__)
// #define _DO_5(F, A, B, ...) printf("%d\n", A), PRINT4(F, A, __VA_ARGS__)

	
	
	// SCRIPT_NEW_CLASS(BattleField,Pawn)	
	// SCRIPT_SET_CLASS(size,pos,setPosition,setSize,instance)
	// SCRIPT_INIT_CLASS()
	
// CLASS
	
#define SCRIPT_NEW_CLASS(...) _SCRIPT_NEW_CLASS_DO(_SCRIPT_ARG_NUM(__VA_ARGS__))(__VA_ARGS__)
#define _SCRIPT_NEW_CLASS_DO(N) _SCRIPT_NEW_CLASS_N(N)
#define _SCRIPT_NEW_CLASS_N(N) _SCRIPT_NEW_CLASS_ ## N
	
#define _SCRIPT_NEW_CLASS_1(arg) std::cout<< #arg <<'\n'; 
#define _SCRIPT_NEW_CLASS_2(arg, ...) std::cout<< #arg <<'\n'; _SCRIPT_NEW_CLASS_1(__VA_ARGS__)
#define _SCRIPT_NEW_CLASS_3(arg, ...) std::cout<< #arg <<'\n'; _SCRIPT_NEW_CLASS_2(__VA_ARGS__)
#define _SCRIPT_NEW_CLASS_4(arg, ...) std::cout<< #arg <<'\n'; _SCRIPT_NEW_CLASS_3(__VA_ARGS__)
#define _SCRIPT_NEW_CLASS_5(arg, ...) std::cout<< #arg <<'\n'; _SCRIPT_NEW_CLASS_4(__VA_ARGS__)
	
// MODULE : SCRIPT_SET_MODULE

#define _SCRIPT_SET_MODULE(className, ...) _SCRIPT_SET_MODULE_DO(_SCRIPT_ARG_NUM(__VA_ARGS__))(className, __VA_ARGS__)
#define _SCRIPT_SET_MODULE_DO(N) _SCRIPT_SET_MODULE_N(N)
#define _SCRIPT_SET_MODULE_N(N) _SCRIPT_SET_MODULE_ ## N	

#define _SCRIPT_SET_MODULE_BODY(c,f) module.set_function( #f , & c :: f , this);
	
#define _SCRIPT_SET_MODULE_1(className, funcName) _SCRIPT_SET_MODULE_BODY(className,funcName) 
#define _SCRIPT_SET_MODULE_2(className, funcName, ...) _SCRIPT_SET_MODULE_BODY(className,funcName)  _SCRIPT_SET_MODULE_1(className, __VA_ARGS__)
#define _SCRIPT_SET_MODULE_3(className, funcName, ...) _SCRIPT_SET_MODULE_BODY(className,funcName)  _SCRIPT_SET_MODULE_2(className, __VA_ARGS__)
#define _SCRIPT_SET_MODULE_4(className, funcName, ...) _SCRIPT_SET_MODULE_BODY(className,funcName)  _SCRIPT_SET_MODULE_3(className, __VA_ARGS__)
#define _SCRIPT_SET_MODULE_5(className, funcName, ...) _SCRIPT_SET_MODULE_BODY(className,funcName)  _SCRIPT_SET_MODULE_4(className, __VA_ARGS__)

#define SCRIPT_SET_MODULE(className, ...) _SCRIPT_SET_MODULE(className, __VA_ARGS__)

// MODULE : SCRIPT_NEW_MODULE

// #define SCRIPT_NEW_MODULE_N(N)

// #define SCRIPT_NEW_MODULE_DO

// #define SCRIPT_NEW_MODULE(...) SCRIPT_NEW_MODULE_

// #define SCRIPT_NEW_MODULE_1(className)

// #define SCRIPT_NEW_MODULE_2(className, scriptPath)


#define SCRIPT_NEW_MODULE_1(className) sol::state& lua = getModule<Script>().get(); auto module=lua.create_table(#className);

#define SCRIPT_NEW_MODULE_2(className,scriptPath) SCRIPT_NEW_MODULE_1(className) lua.script_file(scriptPath);


// ARG_NUM

#define _SCRIPT_ARG_NUM(...) _SCRIPT_ARG_NUM_VALS(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _SCRIPT_ARG_NUM_VALS(N1, N2, N3, N4, N5, N6, N7, N8, N9, N10, N, ...) N


	void testInit() {
		// SCRIPT_NEW_MODULE(BattleField,pos,size,setPos,setSize,instance)
	}
	
	void init() {
		sol::state& lua = getModule<Script>().get();
		lua.script_file("../src/Szczur/Modules/BattleField/test.lua");
	}
	
	
	// void initScript() { sol::state& lua=getModule<Script>().get(); auto mod=lua.create_table(#NAME);
	// mod.set_function(#NAME_X , & 
	
	
	// SCRIPT_NEW_MODULE(BattleField,addPawn)
	// SCRIPT_INIT_CLASSES_IN_MODULE(Pawn)
	
	
	// void initScript() {
		// SCRIPT_NEW_MODULE(BattleField)
		// SCRIPT_SET_MODULE(BattleField, addPawn)
		// Script::initClasses<Pawn>();
	// }
	
	void initScript() {
		// sol::state& lua = getModule<Script>().get();
		// auto mod = lua.create_table("BattleField");
		SCRIPT_NEW_MODULE_2(BattleField, "../src/Szczur/Modules/BattleField/BattleField.lua")
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