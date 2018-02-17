--[[
	Simple script test
]]

local board = BattleField.Board.instance()

BattleField.setBoard(board)

board.grid:setPosition(100,100)
board.grid:setCellSize(70,60)
board.grid:setSize(5,7)

board:addPawn(4, 3)
-- board:addPawn(2, 1)
board:addPawn(3, 2)

-- ============================

-- local board = BattleField.Board.instance()

-- BattleField.setBoard(board)

-- board.grid.stat = {
	-- pos = {300,100};
	-- cellSize = {70,50};
	-- size = {6,5};
-- }

-- board:addPawn(0,1)
-- -- board:addPawn(2,2)
-- board:addPawn(5,4)
-- board:addPawn(5,2)

-- ============================

-- board.pos = {300, 150}

-- board:setStat({
	-- pos = {300, 150};
	-- cellSize = {40, 50};
	-- size = {10, 5};
-- })

-- board:setPosition(300,100)
-- board:setCellSize(30, 30)
-- board:setSize(5,5)


-- board.lol = {
	-- x = 4;
	-- y = 2;
-- }
-- print(board.lol.x)

-- for i = 0,14 do	
	-- local pawn = BattleField.newPawn()
	-- pawn:setPosition(250+i*42, 100)
	-- pawn:setSize(40, 60)
	-- if i%2 == 0 then pawn:more(40) end
	-- end
	
-- for i = 0,14 do
	-- local pawn = BattleField.newPawn()
	-- local stat = {
		-- pos = {250+i*42, 250};
		-- size = {40, 60};
	-- }
	-- pawn:setStat(stat)
	-- if i%2 == 0 then pawn:more(40) end
	-- end
	
-- for i = 0,14 do
	-- local pawn = BattleField.newPawn({
		-- pos = {250+i*42, 400};
		-- size = {40, 60};
	-- })
	-- if i%2 == 0 then pawn:more(40) end
	-- end


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