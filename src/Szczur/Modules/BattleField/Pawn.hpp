#pragma once

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat { namespace battle_field
{

struct Pawn
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
		return std::unique_ptr<Pawn>(new Pawn());
	}



// auto object = script.newClass<Pawn>();
// SCRIPT_SET_CLASS(Pawn, size, pos, setPosition, setSize, instance)
// SCRIPT_INIT_CLASS(Pawn, "../src/Szczur/Modules/BattleField/Pawn.lua")
	
	static void initScript(Script& script) {	
		auto object = script.newClass<Pawn>("Pawn", "BattleField", "../src/Szczur/Modules/BattleField/Pawn.lua");
		SCRIPT_SET_CLASS(Pawn, size, pos, setPosition, setSize, instance)
		object.initClass();
		
		// auto& lua = script.get();
		// sol::table module = lua["BattleField"];
		
		// auto object = module.create_simple_usertype<Pawn>();
		
		
		// // Variables
		// object.set("size", &Pawn::size);
		// object.set("pos", &Pawn::pos);
		// // Methods
		// object.set("setPosition", &Pawn::setPosition);
		// object.set("setSize", &Pawn::setSize);
		// // Functions
		// object.set("instance", &Pawn::instance);
		// object.set("is", [](sol::object obj) {return obj.is<Pawn*>() || obj.is<std::unique_ptr<Pawn>>();});
		
		
		// script.initClass(object, "../src/Szczur/Modules/BattleField/Pawn.lua");
		
		// module.set_usertype("Pawn", object);
		// lua.script_file("../src/Szczur/Modules/BattleField/Pawn.lua");
	}
};

}}