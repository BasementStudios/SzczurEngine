#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"

#include "Pawn.hpp"
#include "Board.hpp"

namespace rat
{
using namespace battle_field;

struct BattleField : public Module<Canvas, Script, Input>
{
	std::vector<std::unique_ptr<Pawn>> pawns;
	std::unique_ptr<Board> board;
	
	template <typename Tuple>
	BattleField(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_CONSTRUCTOR();
		initScript();
		init();
	}
	
	void setBoard(std::unique_ptr<Board> &newBoard) {
		board = std::move(newBoard);
	}
	
	Board* getBoard() {
		return board.get();
	}
	
	auto addPawn(Pawn* newPawn) {		
		pawns.emplace_back(newPawn);
		return pawns.back().get();
	}
	
	void reset() {
		pawns.clear();
	}
		

	void render() {
		if(board) board->render(getModule<Canvas>());
		for(auto& obj : pawns) {
			obj->render(getModule<Canvas>());
		}
	}

	void update() {
		auto& input = getModule<Input>();
		if(input.isPressed(Keyboard::Space)) {
			reset();
			init();
		}
	}

	void testInit() {
	
	}
	
	void init() {
		sol::state& lua = getModule<Script>().get();
		lua.script_file("../src/Szczur/Modules/BattleField/test.lua");
	}
	
	void initScript() {
		auto module = getModule<Script>().newModule("BattleField", "../src/Szczur/Modules/BattleField/BattleField.lua");
		SCRIPT_SET_MODULE(BattleField, addPawn, setBoard, getBoard, reset)
		getModule<Script>().initClasses<Pawn, Board, Grid>();
	}
};

}