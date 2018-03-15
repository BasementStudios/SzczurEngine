#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "Szczur/Modules/ScriptEditor/ScriptableBase.hpp"

namespace rat {

struct TestObject {
	
	sf::Vector2f size{100,100};
	sf::Vector2f pos{200,200};
	int color{0};
	
	void render(sf::RenderTexture &canvas) {
		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setOutlineThickness(-3);
		shape.setOutlineColor({0,0,0,255});
		if(color == 0) {
			shape.setFillColor({200,70,70,255});
		}
		else if(color == 1) {
			shape.setFillColor({70,70,200,255});
		}
		canvas.draw(shape);
	}
	
	void init() {
		if(initFunc.valid()) initFunc(this);
	}	
	
	void update() {
		if(updateFunc.valid()) updateFunc(this);
	}
	
	/////////////////////////////////////////// SCRIPT ///////////////////////////////////////////
	
	ScriptableBase base;
	
	sol::function initFunc;
	sol::function updateFunc;
	
	// Set file with script for this object and run this script
	void loadScript(const std::string& filepath);
	
	// Return `base`
	ScriptableBase& getBase();
	
	// For Lua pack
	void setScriptModule(Script& script);
	
	// For C++ class
	static void initScript(Script& script);
};

}