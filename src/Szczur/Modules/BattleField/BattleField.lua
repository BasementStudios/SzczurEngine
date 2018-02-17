--[[
	Module script: BattleField 
	
	[src]
	.addPawn(Pawn)
]]

--[[
	Add new pawn

	a) .newPawn()
	b) .newPawn(Pawn)
	c) .newPawn(Stat)
]]

BattleField.newPawn = function(...)
	local Pawn = BattleField.Pawn

	-- a)
	if ... == nil then return BattleField.addPawn(Pawn.instance()) end
	
	arg = {...}	
	-- b)
	if Pawn.is(arg[1]) then
		return BattleField.addPawn(arg[1])
	-- c)
	else
		local pawn = Pawn.instance()
		pawn:setStat(arg[1])
		return BattleField.addPawn(pawn)
		end
	end