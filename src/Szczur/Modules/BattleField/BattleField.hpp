#pragma once

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include <sol.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"

#include "Pawn.hpp"
#include "Board.hpp"

namespace rat
{

struct BattleField : public Module<Canvas, Script, Input, Window>
{
	std::unique_ptr<battle_field::Board> board;
	
	template <typename Tuple>
	BattleField(Tuple&& tuple) :
		Module(tuple)
	{
		LOG_CONSTRUCTOR();
		initScript();
		reset();
	}
	
	void setBoard(std::unique_ptr<battle_field::Board> &newBoard) {
		board = std::move(newBoard);
	}
	
	battle_field::Board* getBoard() {
		return board.get();
	}
	
	void render() {
		if(board) board->render(getModule<Canvas>());
	}

	void update() {
		auto& input = getModule<Input>();
		if(input.isPressed(Keyboard::Space)) reset();
		if(board) board->update(getModule<Window>().getWindow());
	}
	
	void reset() {
		getModule<Script>().scriptFile("../src/Szczur/Modules/BattleField/Scripts/test.lua");
	}
	
	void initScript() {
		using namespace battle_field;
		Script& script = getModule<Script>();
		auto module = script.newModule("BattleField");
		SCRIPT_SET_MODULE(BattleField, getBoard, setBoard)
		script.initClasses<
			Pawn, 
			Board, 
			Grid, 
			PawnPlayer
		>();
	}
};

}