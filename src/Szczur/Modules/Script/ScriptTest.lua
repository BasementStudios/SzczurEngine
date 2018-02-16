--[[
	Module script: ScriptTest 
	
	[src]
	.addButton(Button)
]]

--[[
	Add new button to gui

	a) .newButton()
	b) .newButton(Button)
	c) .newButton(Stat)
]]

ScriptTest.newButton = function(...)
	local Button = ScriptTest.Button

	-- a)
	if ... == nil then return ScriptTest.addButton(Button.instance()) end
	
	arg = {...}	
	-- b)
	if Button.is(arg[1]) then
		return ScriptTest.addButton(arg[1])
	-- c)
	else
		local button = Button.instance()
		button:setStat(arg[1])
		return ScriptTest.addButton(button)
		end
	end