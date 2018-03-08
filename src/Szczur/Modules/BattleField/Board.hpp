#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"

#include "Pawn.hpp"
#include "Grid.hpp"

namespace rat { namespace battle_field
{

struct Board
{
	std::vector<std::unique_ptr<Pawn>> pawns;

	Grid grid;
	Script& script;
	
	// void addPawn(int x, int y) {
		// pawns.emplace_back(new battle_field::Pawn());
		// auto obj = pawns.back().get();
		// auto vec = grid.getCellPosition(x, y);
		// obj->setPosition(vec.x + 4, vec.y + 4);
		// obj->setSize(grid.cellSize.x - 8*2, grid.cellSize.y - 8*2);
	// }
	
	Board() :
		script(Script::ref()) { 
	}
	
	void render(Canvas& canvas) {
		grid.render(canvas);
		for(auto& obj : pawns) {
			obj->render(canvas);
		}
	}
	
	template <typename T>
	void setPawn(std::unique_ptr<T>& pawn, int x, int y) {
		pawns.emplace_back(std::move(pawn));
		auto obj = pawns.back().get();
		auto vec = grid.getCellPosition(x, y);
		obj->setPosition(vec.x + 8, vec.y + 8);
		obj->setSize(grid.getCellSize().x - 8*2, grid.getCellSize().y - 8*2);
	}
	
	void update(const sf::RenderWindow& window) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	
		sf::Vector2i indexPos = grid.inside(mousePos.x, mousePos.y);
		grid.select(indexPos.x, indexPos.y);
	}
	
	// static auto instance() {
		// auto board = new Board();
		// sol::function init = Script::ref().get()["BattleField"]["Board"]["_instance"];
		// if(init.valid()) init(board);
		// return std::unique_ptr<Board>(board);
	// }
	
	void setStat(sol::table stat) {
		grid.setStat(stat);
	}
	
	static void initScript(Script& script) {
		auto object = script.newClass<Board>("Board", "BattleField", "../src/Szczur/Modules/BattleField/Scripts/Board.lua");
		
		SCRIPT_SET_CLASS(Board, grid)
		
		object.setOverload("setPawn", 
			&Board::setPawn<Pawn>, 
			&Board::setPawn<PawnPlayer>
		);
		
		object.makeInstance();
		object.makeStatter();

		object.init();
	}
};

}}