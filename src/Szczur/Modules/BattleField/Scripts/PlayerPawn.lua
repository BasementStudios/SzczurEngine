--[[ 
	Script:	Battle gui
	
	Key words:
	stat
	init
	instance
	
--]]

local Player = BattleField.PlayerPawn

function Player:_instance()
	self.grid.stat = {
		pos = {200, 100};
		cellSize = {150, 90};
		size = {6, 5};
	}
	end