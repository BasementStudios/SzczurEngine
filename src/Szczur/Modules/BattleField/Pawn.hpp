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

	static void initScript(Script& script) {	
		auto& lua = script.get();
		sol::table battleField = lua["BattleField"];
		
		auto pawn = battleField.create_simple_usertype<Pawn>();
		
		// Variables
		pawn.set("size", &Pawn::size);
		pawn.set("pos", &Pawn::pos);
		// Methods
		pawn.set("setPosition", &Pawn::setPosition);
		pawn.set("setSize", &Pawn::setSize);
		// Functions
		pawn.set("instance", &Pawn::instance);
		pawn.set("is", [](sol::object obj) {return obj.is<Pawn*>() || obj.is<std::unique_ptr<Pawn>>();});
		
		battleField.set_usertype("Pawn", pawn);
		lua.script_file("../src/Szczur/Modules/BattleField/Pawn.lua");
	}
};

}}