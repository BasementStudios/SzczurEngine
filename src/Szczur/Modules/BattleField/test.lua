--[[
	Simple script test
]]


for i = 0,14 do	
	local pawn = BattleField.newPawn()
	pawn:setPosition(250+i*42, 100)
	pawn:setSize(40, 60)
	if i%2 == 0 then pawn:more(40) end
	end
	
for i = 0,14 do
	local pawn = BattleField.newPawn()
	local stat = {
		pos = {250+i*42, 250};
		size = {40, 60};
	}
	pawn:setStat(stat)
	if i%2 == 0 then pawn:more(40) end
	end
	
for i = 0,14 do
	local pawn = BattleField.newPawn({
		pos = {250+i*42, 400};
		size = {40, 60};
	})
	if i%2 == 0 then pawn:more(40) end
	end


-- State.init = function()
	-- local stat = {
		-- name = "pawn_1"
		-- label = "Click me!";
		-- pos = {x = 10, y = 20};
		-- size = {x = 100, y = 200};		
	-- }
	-- local pawn = ScriptTest.newpawn()
	-- pawn:setStat(stat)
	-- end