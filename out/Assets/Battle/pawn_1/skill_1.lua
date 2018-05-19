-- Skill: Move
local skill = THIS:newSkill("Dash in right")

skill:setType("CURSOR")

function skill:onActive() 
	skill:renderCircle(30)
end

function skill:onUpdate(dt)
	local angle = self:getPawn():getAngleToPoiner()
	self:getPawn():moveInDirection(angle, dt*100)
end



-- -- Skill: Move
-- local skill = THIS:newSkill("Dash")

-- skill.type = TYPE_CURSOR

-- skill.clock = Clock:new()

-- function skill:init()
-- 	self.angle = Math.angle(self:getCursorPosition(), self:getPawn():getPosition())
-- 	self.distance = Math.distance(self:getCursorPosition(), self:getPawn():getPosition())
-- 	self.clock:restart()
-- end

-- function skill:update(dt)
-- 	self:getPawn():moveInDirection(self.angle, dt*100)
-- 	if self.clock:getElapsedTime()>1.5 then
-- 		skill:kill()
		
-- 	end
-- end
