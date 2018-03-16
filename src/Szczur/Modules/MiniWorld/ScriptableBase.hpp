#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

struct ScriptableBase {
	
	std::string name = "no_name";
	std::string scriptPath = "";
	
	sol::table ref;
	Script *script;
	
	sol::function initFunction;	
	sol::function updateFunction;
	
	sol::table pack;
	
	// Set `script` and `pack`, return pack
	sol::table& prepare(Script& script);
	
	
	void setName(const std::string& name) {
		this->name = name;
	}
	
	template <typename Name, typename Value>
	void set(Name&& _name, Value&& _value) {
		pack.set(_name, _value);
	}
	
	sol::table getPack() {
		return pack;
	}
	
	// Run set file with script
	template <typename T> 
	void loadScript(T* owner, const std::string& filepath) {
		auto& lua = script->get();
		lua.set("THIS", owner);
		try {
			lua.script_file(filepath);
			ref = lua["THIS"];
			scriptPath = filepath;
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot load "<<filepath<<'\n'<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	template <typename T> 
	void runFileScript(T* owner, const std::string& filepath) {
		auto& lua = script->get();
		lua.set("THIS", owner);
		try {
			lua.script_file(filepath);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot load "<<filepath<<'\n'<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	template <typename T> 
	void runScript(T* owner, const std::string& code) {
		auto& lua = script->get();
		lua.set("THIS", owner);
		try {
			lua.script(code);
		}
		catch(sol::error e) {
			std::cout<<"[ERROR] Cannot script\n"<<std::flush;
			std::cout<<e.what()<<'\n'<<std::flush;
		}
	}
	
	// void loadScript(auto target, const std::string& filepath);
	
	static initScript(Script& script) {
		auto object = script.newClass<ScriptableBase>("ScriptableBase", "ScriptEditor");
		object.set("pack", &ScriptableBase::pack);
		object.init();
	}
};

}