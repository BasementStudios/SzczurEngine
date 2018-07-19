#include "Script.hpp"

#include <cmath>
#include <iostream>

#include <sol.hpp>

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/ScriptClass.hpp"

namespace rat {
	

	void Script::init() {
		_lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::table, sol::lib::math, sol::lib::string);
		_lua["Math"] = _lua["math"];
		_lua["math"] = sol::nil;
		_lua["String"] = _lua["string"];
		_lua["string"] = sol::nil;
		initSFML();
		initGLM();
		initMainFunctions();
	}
	void Script::initMainFunctions() {
		auto script = _lua.create_table("Script");
		script.set_function("runScript", &Script::scriptFile, this);
	}
	void Script::scriptFile(const std::string& filePath) {
		_lua.script_file(filePath);
	}
	void Script::script(const std::string& code) {
		_lua.script(code);
	}

	sol::state& Script::get() {
		return _lua;
	}
	sol::table Script::newModule(const std::string& moduleName, const std::string &scriptPath) {
		_lua.create_table(moduleName);
		if (scriptPath != "") _lua.script_file(scriptPath);
		return _lua[moduleName];
	}

	Script::Script(){
		LOG_INFO(this, " : Module Script constructed");
		init();
		if (!_this) _this = this;
	}

	Script::~Script(){
		LOG_INFO(this, " : Module Script destructed");
	}

	Script& Script::ref() {
		return *_this;
	}
}
