--[[ 
	Script:	Battle gui
	
	Key words:
	stat
	init
	instance
	
--]]

local Board = BattleField.Board

function Board:_instance()
	self.grid.stat = {
		pos = {200, 100};
		cellSize = {150, 90};
		size = {6, 5};
	}
	end