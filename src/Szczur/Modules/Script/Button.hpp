#pragma once

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat
{

struct Button
{
	sf::Vector2f pos {0,0};
	sf::Vector2f size {0,0};
	
	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	
	void setSize(float x, float y) {
		size.x = x;
		size.y = y;
	}
	
	void render(Canvas& canvas) {
		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color(20, 50, 60));
		shape.setOutlineColor(sf::Color(20, 100, 100));
		shape.setOutlineThickness(-4);
		canvas.draw(Canvas::LayerID::Game, shape);
	}
	
	static auto instance() {
		return std::unique_ptr<Button>(new Button());
	}
	
	static void initScript(Script& script) {
		auto& lua = script.get();
		sol::table scriptTest = lua["ScriptTest"];
		
		auto button = scriptTest.create_simple_usertype<Button>();
		// Methods
		button.set("setPosition", &Button::setPosition);
		button.set("setSize", &Button::setSize);		
		// Functions
		button.set("instance", &Button::instance);
		button.set("is", [](sol::object obj) {return obj.is<Button*>() || obj.is<std::unique_ptr<Button>>();});
		// Variables
		button.set("size", &Button::size);
		button.set("pos", &Button::pos);
		
		scriptTest.set_usertype("Button", button);		
		lua.script_file("../src/Szczur/Modules/Script/Button.lua");
	}
};

}