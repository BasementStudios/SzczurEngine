--[[
	Class script: Pawn
	Module: 	  BattleField
	
	[src]	
	:setPosition(x,y)
	:setSize(x,y)	
	.instance()	
	size
]]
	
local Pawn = BattleField.Pawn

function Pawn:setStat(stat)
	if stat.pos then self:setPosition(stat.pos[1], stat.pos[2]) end
	if stat.size then self:setSize(stat.size[1], stat.size[2]) end
	end
	
function Pawn:more(delta)
	self.size.y = self.size.y + delta
	end