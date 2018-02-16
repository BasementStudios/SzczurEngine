--[[
	Base script for ScriptTest module
]]

function Script:setStat(target, stat)
	for k, v = in pairs(prop)
		table.insert(target, k, v)
		end
	end

--[[
	Simple gui with buttons
]]


-- a)

State.init = function()
	local button = ScriptTest:newButton()
	button.name = "button_1"
	button.label = "Click me!";
	button.pos = {x = 10, y = 20};
	button.size = {x = 100, y = 200};		
	end
	
-- b)
	
State.init = function()
	local button = ScriptTest:newButton()
	local stat = {
		name = "button_1";
		label = "Click me!";
		pos = {x = 10, y = 20};
		size = {x = 100, y = 200};
	}
	button:setStat(stat)
	end
	
-- c)
	
State.init = function()
	local button = ScriptTest:newButton({
		name = "button_1";
		label = "Click me!";
		pos = {x = 10, y = 20};
		size = {x = 100, y = 200};
	})		
	end