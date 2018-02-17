#pragma once

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
	
	void addPawn(int x, int y) {
		pawns.emplace_back(new battle_field::Pawn());
		auto obj = pawns.back().get();
		auto vec = grid.getCellPosition(x, y);
		obj->setPosition(vec.x + 4, vec.y + 4);
		obj->setSize(grid.cellSize.x - 8*2, grid.cellSize.y - 8*2);
	}
	
	void render(Canvas& canvas) {
		grid.render(canvas);
		for(auto& obj : pawns) {
			obj->render(canvas);
		}
	}
	
	static auto instance() {
		return std::unique_ptr<Board>(new Board());
	}
	
	static void initScript(Script& script) {	
		auto object = script.newClass<Board>("Board", "BattleField", "../src/Szczur/Modules/BattleField/Board.lua");
		
		SCRIPT_SET_CLASS(Board, grid, instance, addPawn)
		
		object.init();
	}
};

}}