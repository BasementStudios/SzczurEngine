-- Skill: Move
local skill = THIS:newSkill("Dash and hit")

skill:setType("SPACE")

function skill:onProvide(canvas) 
	self:renderCircle(canvas, 20)
end

function skill:onInit()
	self.angle = self:getPawn():getAngleToPointer()
	self.clock = Utility.Clock:new()
	self:getPawn():setFlip(self:getPawn():getDirection(self.angle))
end

function skill:onUpdate(dt)
	self:getPawn():moveInDirection(self.angle, dt*700)
	if(self.clock:elapsed()>0.20) then
		self:getPawn():setFrame(0)
		self:kill()
		local skill_1 = self:getPawn():useSkill("Dash and hit : hit")
	elseif(self.clock:elapsed()>0.10) then
		self:getPawn():setFrame(3)
	else
		self:getPawn():setFrame(2)
	end
end

-- hit
local hit = THIS:newSkill("Dash and hit : hit")

function hit:onInit()
	self.clock = Utility.Clock.new()
end

function hit:onUpdate(dt)
	-- print("value:", self.base)
	if self.clock:elapsed()>0.20 then
		self:getPawn():setFrame(0)
		self:kill()
	elseif self.clock:elapsed()>0.10 then
		self:getPawn():setFrame(5)
	else
		self:getPawn():setFrame(4)
	end
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
