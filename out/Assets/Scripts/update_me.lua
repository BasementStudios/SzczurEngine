-- print(THIS:getScriptable():onUpdate())

-- print("X")

-- function THIS:getScriptable():onUpdate(dt)
	-- print("+3")
	-- end

print("yyy")

local this = THIS
local scriptable = this:getScriptable()
local inputController = this:getInputController()
local interactable = this:getInteractable()

local velocity = 20
local a = 0

function scriptable:onUpdate(dt)
	a = dt
end

function interactable:onInteraction()
	print(a)
end

--if(inputController) then
	function inputController:onUpdate(input, dt)
		if(input:isKept(Input.Key_W)) then
			this:move(0, 0, -velocity)
		end
		if(input:isKept(Input.Key_A)) then
			this:move(-velocity, 0, 0)
		end
		if(input:isKept(Input.Key_S)) then
			this:move(0, 0, velocity)
		end
		if(input:isKept(Input.Key_D)) then
			this:move(velocity, 0, 0)
		end
	end
--end

-- THIS:getScriptable().onUpdate = function(self, dt)
	-- print("+c")
	-- end

-- THIS:getScriptable():onUpdate = function(self, dt)
	-- print("+2")
	-- end