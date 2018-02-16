--[[
	Class script: Button
	
	[src]	
	:setPosition(x,y)
	:setSize(x,y)	
	.instance()	
	size
]]
	
local Button = ScriptTest.Button

function Button:setStat(stat)
	if stat.pos then self:setPosition(stat.pos[1], stat.pos[2]) end
	if stat.size then self:setSize(stat.size[1], stat.size[2]) end
	end
	
function Button:more(delta)
	self.size.y = self.size.y + delta
	end