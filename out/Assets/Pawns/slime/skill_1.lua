-- Skill: Move
local skill = THIS:newSkill("Dash and hit")
THIS:addUsable(skill, 0)

skill:setType("SPACE")

function skill:onProvide(provider)
	local dis = self:getPawn():getDistanceTo(Input.getMousePosition())
	local pos = Input.getMousePosition()
	if dis>100 then
		local angle = self:getPawn():getAngleTo(Input.getMousePosition())
		pos = self:getPawn():getPosition()
		pos.x = pos.x+Math.cos(angle)*100
		pos.y = pos.y+Math.sin(angle)*100
	end
	provider:drawCircle(pos.x, pos.y, 20)
	provider:drawCircle(self:getPawn():getPosition(), 100)
	self.point = pos
end

function skill:onInit()
	self.angle = self:getPawn():getAngleTo(self.point)

	-- local newPos = Math.move(pos, angle, dis)
	-- self.dis = Math.distance(self:getPawn():getPosition(), self.point)
	self.dis = self:getPawn():getDistanceTo(self.point)
	self.steps = self.dis/5.0
	self.disStep = self.dis/self.steps
	-- self.clock = Utility.Clock:new()
	self:getPawn():setFlip(not self:getPawn():getDirection(self.angle))
	-- self.distance = self:getPawn():getDistanceToPointer()
end

function skill:onUpdate(dt)

	if self.steps>0 then
		self:getPawn():move(self.angle, self.disStep)
		self.steps = self.steps - 1
	else
		self:kill()
	end
	-- self:getPawn():moveInDirection(self.angle, dt*1000)
	-- if self.clock:elapsed()>0.20 then
	-- 	self:getPawn():setFrame(0)
	-- 	self:kill()
	-- 	self:getPawn():useSkill("Dash and hit : hit")
	-- elseif self.clock:elapsed()>0.14 then
	-- 	self:getPawn():setFrame(3)
	-- else
	-- 	self:getPawn():setFrame(2)
	-- end
end

-- hit
local hit = THIS:newSkill("Dash and hit : hit")
THIS:addUsable(hit, 1)

function hit:onInit()
	self.clock = Utility.Clock.new()
end

function hit:onUpdate(dt)
	if self.clock:elapsed()>0.20 then
		self:getPawn():setFrame(0)
		self:kill()
	elseif self.clock:elapsed()>0.10 then
		self:getPawn():setFrame(5)
	else
		self:getPawn():setFrame(4)
	end
end
