#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat { namespace battle_field
{

struct PawnPlayer : public Pawn 
{
	bool selected = false;

	void select(bool flag) {
		selected = flag;
	}
	
	void render(Canvas& canvas) {
		sf::Color temp = fillColor;
		
		if(selected) {
			temp.r *= 1.5;
			if(temp.r>255) temp.r = 255;
			temp.g *= 1.5;
			if(temp.g>255) temp.g = 255;
			temp.b *= 1.5;
			if(temp.b>255) temp.b = 255;
		}

		sf::RectangleShape shape;
		shape.setSize(size);
		shape.setPosition(pos);
		shape.setFillColor(temp);
		shape.setOutlineColor(outlineColor);
		shape.setOutlineThickness(-4);
		canvas.draw(Canvas::LayerID::Game, shape);
	}
	
	static void initScript(Script& script) {	
		auto object = script.newClass<PawnPlayer>("PawnPlayer", "BattleField");
		SCRIPT_SET_CLASS(Pawn, setFillColor, setOutlineColor)
		SCRIPT_SET_CLASS(PawnPlayer, select)
		object.setProperty("selected", [](PawnPlayer &obj){return obj.selected;}, [](PawnPlayer &obj, bool value){obj.select(value);});
		
		object.makeInstance();
		object.init();
	}
};

}}