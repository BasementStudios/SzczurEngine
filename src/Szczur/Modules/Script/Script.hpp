#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"

namespace rat
{
class Script : public Module<Canvas>
{
private:
	
	sol::state _lua;

public:

	template <typename Tuple>
	Script(Tuple&& tuple);

	Script(const Script&) = delete;
	Script& operator = (const Script&) = delete;
	Script(Script&&) = delete;
	Script& operator = (Script&&) = delete;

	void init() {
		_lua.open_libraries(sol::lib::base, sol::lib::io, sol::lib::table);
		initSFML();
	}
	
	void initSFML() {
		sol::table sfml = _lua.create_table("SFML");
		sfml.new_simple_usertype<sf::Vector2f>("Vector2f",
			"x", &sf::Vector2f::x,
			"y", &sf::Vector2f::y
		);
	}
	
	void render() {
		
	}
	
	sol::state& get() {
		return _lua;
	}
	
};

template <typename Tuple>
Script::Script(Tuple&& tuple) :
	Module(tuple)
{
	LOG_CONSTRUCTOR();
	init();
}
}
