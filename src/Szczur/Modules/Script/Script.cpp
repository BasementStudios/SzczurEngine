#include <iostream>
#include "Script.hpp"
#include <sol.hpp>
#include <SFML/Graphics.hpp>
#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Script/ScriptClass.hpp"

namespace rat {
	

	void Script::init() {
		_lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::table, sol::lib::math);
		_lua["Math"] = _lua["math"];
		_lua["math"] = sol::nil;
		initSFML();
		initMainFunctions();
	}
	void Script::initMainFunctions() {
		auto script = _lua.create_table("Script");
		script.set_function("scriptFile", &Script::scriptFile, this);
	}
	void Script::initSFML() {
		sol::table sfml = _lua.create_table("SFML");
		sfml.new_simple_usertype<sf::Vector2f>("Vector2f",
			"x", &sf::Vector2f::x,
			"y", &sf::Vector2f::y
			);
		sfml.new_simple_usertype<sf::Vector2i>("Vector2i",
			"x", &sf::Vector2i::x,
			"y", &sf::Vector2i::y
			);
		sfml.new_simple_usertype<sf::Color>("Color",
			"r", &sf::Color::r,
			"g", &sf::Color::g,
			"b", &sf::Color::b,
			"a", &sf::Color::a
			);
		sfml.new_simple_usertype<sf::FloatRect>("FloatRect",
			"left", &sf::FloatRect::left,
			"top", &sf::FloatRect::top,
			"width", &sf::FloatRect::width,
			"height", &sf::FloatRect::height
			);
		sfml.new_simple_usertype<sf::FloatRect>("IntRect",
			"left", &sf::IntRect::left,
			"top", &sf::IntRect::top,
			"width", &sf::IntRect::width,
			"height", &sf::IntRect::height
			);
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
