--[[
	Simple script test
]]

local board = BattleField.Board.instance()
BattleField.setBoard(board)

local player = BattleField.PawnPlayer.instance()
board:setPawn(player, 1, 1)

local pawn = BattleField.Pawn.instance()
board:setPawn(pawn, 2, 3)

pawn = BattleField.Pawn.instance()
board:setPawn(pawn, 4, 2)

player:select(true)

print("xyz")