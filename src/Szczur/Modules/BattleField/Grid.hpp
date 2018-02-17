#pragma once

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

namespace rat { namespace battle_field
{

struct Grid {

	sf::Vector2f pos {0,0};
	sf::Vector2f cellSize {30,30};
	sf::Vector2i size {1,1};
	
	void render(Canvas& canvas) {
		sf::RectangleShape shape;
		shape.setSize(cellSize);
		shape.setOutlineThickness(-4);
		shape.setFillColor(sf::Color(40, 50, 60));
		shape.setOutlineColor(sf::Color(90, 100, 100));
		
		for(int i=0; i<size.x; ++i) 
		for(int j=0; j<size.y; ++j) {
			shape.setPosition(sf::Vector2f(
				pos.x + i*(cellSize.x-4),
				pos.y + j*(cellSize.y-4)
			));
			canvas.draw(Canvas::LayerID::Game, shape);
		}
	}
	
	sf::Vector2f getCellPosition(int x, int y) {
		return sf::Vector2f(
			pos.x + x*(cellSize.x-4) + 4,
			pos.y + y*(cellSize.y-4) + 4
		);
	}
	
	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
	}
	
	void setSize(int x, int y) {
		size.x = x;
		size.y = y;
	}
	
	void setCellSize(float x, float y) {
		cellSize.x = x;
		cellSize.y = y;
	}
	
	void setStat(sol::table stat) {
		if(stat["pos"].valid()) setPosition(stat["pos"][1], stat["pos"][2]);
		if(stat["size"].valid()) setSize(stat["size"][1], stat["size"][2]);
		if(stat["cellSize"].valid()) setCellSize(stat["cellSize"][1], stat["cellSize"][2]);
	}
	
	static void initScript(Script& script) {
		auto object = script.newClass<Grid>("Grid", "BattleField");
		SCRIPT_SET_CLASS(Grid, pos, cellSize, size, setPosition, setSize, setCellSize)
		object.set("instance", [](){return std::unique_ptr<Grid>(new Grid());});
		object.makeStatter();
		object.init();
	}
};

}}