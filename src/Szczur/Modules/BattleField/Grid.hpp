#pragma once

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Script/Script.hpp"

namespace rat { namespace battle_field
{

class Grid 
{
private:

	sf::Vector2f pos {0,0};
	sf::Vector2f cellSize {30,30};
	sf::Vector2i size {1,1};
	sf::Color fillColor {40, 50, 60, 255};
	sf::Color selectColor {80, 90, 100, 255};
	sf::Color outlineColor {90, 100, 100, 255};
	
	std::vector<sf::Vector2f> cells;

	sf::Vector2i selectedCell {-1, -1};
	
public:
	
	
private:

	void _updateCells() {
		cells.clear();
		for(int j=0; j<size.y; ++j) 
		for(int i=0; i<size.x; ++i) {
			cells.emplace_back(
				pos.x + i*(cellSize.x-4),
				pos.y + j*(cellSize.y-4)
			);
		}
	}

public:

	const sf::Vector2f& getCellSize() {
		return cellSize;
	}
	
	void select(int x, int y) {
		selectedCell.x = x;
		selectedCell.y = y;
	}
	
	void unselect() {
		selectedCell.x = -1;
		selectedCell.y = -1;
	}
	
	bool selected() {
		return selectedCell.x != -1 && selectedCell.y != -1;
	}
	
	void render(sf::RenderTexture& canvas) {
		sf::RectangleShape shape;
		shape.setSize(cellSize);
		shape.setOutlineThickness(-4);
		shape.setFillColor(fillColor);
		shape.setOutlineColor(outlineColor);
		
		for(auto& pos : cells) {
			shape.setPosition(pos);
			canvas.draw(shape);
		}
		
		if(selected()) {			
			shape.setFillColor(selectColor);
			shape.setPosition(cells[selectedCell.x + selectedCell.y*size.x]);
			canvas.draw(shape);
		}
	}
	
	// Send position -> return index position
	sf::Vector2i inside(float x, float y) {
		for(unsigned int i=0; i<cells.size(); ++i) {
			sf::Vector2f pos = cells[i];
			if(pos.x < x && pos.y < y && pos.x + cellSize.x > x && pos.y + cellSize.y > y) return sf::Vector2i(i%size.x, i/size.x);
		}
		return sf::Vector2i(-1, -1);
	}
	
	// Send index position -> return position in center of cell
	sf::Vector2f getCenterCellPosition(int x, int y) {
		sf::Vector2f ret = cells[x + y*size.x];
		ret.x += cellSize.x/2.0;
		ret.y += cellSize.y/2.0;
		return ret;
	}
	
	// Send index position -> return position
	sf::Vector2f getCellPosition(int x, int y) {
		sf::Vector2f ret = cells[x + y*size.x];
		return ret;
	}
	
	void setPosition(float x, float y) {
		pos.x = x;
		pos.y = y;
		_updateCells();
	}
	
	void setSize(int x, int y) {
		size.x = x;
		size.y = y;
		_updateCells();
	}
	
	void setCellSize(float x, float y) {
		cellSize.x = x;
		cellSize.y = y;
		_updateCells();
	}
	
	void setStat(sol::table stat) {
		if(stat["pos"].valid()) setPosition(stat["pos"][1], stat["pos"][2]);
		if(stat["size"].valid()) setSize(stat["size"][1], stat["size"][2]);
		if(stat["cellSize"].valid()) setCellSize(stat["cellSize"][1], stat["cellSize"][2]);
	}
	
	static void initScript(Script& script) {
		auto object = script.newClass<Grid>("Grid", "BattleField");
		SCRIPT_SET_CLASS(Grid, pos, cellSize, size, setPosition, setSize, setCellSize)
		object.makeInstance();
		object.makeStatter();
		object.init();
	}
};

}}