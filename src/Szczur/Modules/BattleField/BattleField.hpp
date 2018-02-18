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
	
	void render() {
		if(board) board->render(getModule<Canvas>());
	}

	void update() {
		auto& input = getModule<Input>();
		if(input.isPressed(Keyboard::Space)) {
			// reset();
			init();
		}
	}

	void testInit() {
	
	}
	
	void init() {
		getModule<Script>().scriptFile("../src/Szczur/Modules/BattleField/test.lua");
		// sol::state& lua = getModule<Script>().get();
		// lua.script_file("../src/Szczur/Modules/BattleField/test.lua");
	}
	
	void initScript() {
		auto module = getModule<Script>().newModule("BattleField");
		SCRIPT_SET_MODULE(BattleField, setBoard, getBoard)
		getModule<Script>().initClasses<
			Pawn, 
			Board, 
			Grid, 
			PawnPlayer
		>();
	}
};

}