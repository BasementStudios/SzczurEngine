--[[
	Simple gui with buttons
]]


for i = 0,14 do	
	local button = ScriptTest.newButton()
	button:setPosition(250+i*42, 100)
	button:setSize(40, 60)
	if i%2 == 0 then button:more(40) end
	end
	
for i = 0,14 do
	local button = ScriptTest.newButton()
	local stat = {
		pos = {250+i*42, 250};
		size = {40, 60};
	}
	button:setStat(stat)
	if i%2 == 0 then button:more(40) end
	end
	
for i = 0,14 do
	local button = ScriptTest.newButton({
		pos = {250+i*42, 400};
		size = {40, 60};
	})
	if i%2 == 0 then button:more(40) end
	end


-- State.init = function()
	-- local stat = {
		-- name = "button_1"
		-- label = "Click me!";
		-- pos = {x = 10, y = 20};
		-- size = {x = 100, y = 200};		
	-- }
	-- local button = ScriptTest.newButton()
	-- button:setStat(stat)
	-- end