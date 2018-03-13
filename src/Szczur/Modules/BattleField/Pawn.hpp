#pragma once

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Script/Script.hpp"

namespace rat { namespace battle_field
{

struct Pawn
{
	sf::Vector2f pos {0,0};
	sf::Vector2f size {0,0};
	sf::Color fillColor {20, 50, 60};
	sf::Color outlineColor {20, 100, 100};

	void setFillColor(int r, int g, int b) {
		fillColor = sf::Color(r,g,b,255);
	}
	
	void setOutlineColor(int r, int g, int b) {
		outlineColor = sf::Color(r,g,b,255);
	}
	
	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	
	void setSize(float x, float y) {
		size.x = x;
		size.y = y;
	}
	
	void setSize(const sf::Vector2f& vec) {
		size.x = vec.x;
		size.y = vec.y;
	}
	
	virtual void render(sf::RenderTexture& canvas) {
		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setFillColor(fillColor);
		shape.setOutlineColor(outlineColor);
		shape.setOutlineThickness(-4);
		canvas.draw(shape);
	}
	
	static void initScript(Script& script) {
		
		auto object = script.newClass<Pawn>("Pawn", "BattleField");
		
		SCRIPT_SET_CLASS(Pawn, pos, setFillColor, setOutlineColor)
		
		object.makeInstance();
		
		object.init();
	}
};

}}