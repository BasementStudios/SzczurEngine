-- print(THIS:getScriptable():onUpdate())

-- print("X")

-- function THIS:getScriptable():onUpdate(dt)
	-- print("+3")
	-- end

print("yyy")

local scriptable = THIS:getScriptable()

function scriptable:onUpdate(dt) 
	print("+e")
	end

-- THIS:getScriptable().onUpdate = function(self, dt)
	-- print("+c")
	-- end

-- THIS:getScriptable():onUpdate = function(self, dt)
	-- print("+2")
	-- end