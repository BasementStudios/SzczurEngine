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
	
	virtual void render(Canvas& canvas) {
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
		auto object = script.newClass<Pawn>("Pawn", "BattleField", "../src/Szczur/Modules/BattleField/Pawn.lua");
		SCRIPT_SET_CLASS(Pawn, size, pos, setPosition, setSize, instance)
		object.init();
	}
};

// struct PawnPlayer : public Pawn {
	
	// bool selected = false;

	// void select(bool flag) {
		// selected = flag;
	// }
	
	// void render(Canvas& canvas) {
		// sf::RectangleShape shape;
		// shape.setSize(size);
		// shape.setPosition(pos);
		// shape.setFillColor(sf::Color(60+_selected*60, 50+_selected*60, 20));
		// shape.setOutlineColor(sf::Color(100, 20, 100));
		// shape.setOutlineThickness(-4);
		// canvas.draw(Canvas::LayerID::Game, shape);
	// }
	
	// static void initScript(Script& script) {	
		// auto object = script.newClass<PlayerPawn>("PlayerPawn", "BattleField");
		// object.set("instance", [](){return std::unique_ptr<PlayerPawn>(new PlayerPawn());});
		// SCRIPT_SET_CLASS(PlayerPawn, select)
		// object.set
		// object.init();
	// }
// };

}}