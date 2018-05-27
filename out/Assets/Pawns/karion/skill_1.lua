-- Skill: Move
local skill = THIS:newSkill("Dash and hit")
THIS:addUsable(skill, 0)

skill:setType("SPACE")

function skill:onProvide(provider) 
	-- self:renderCircle(canvas, 20)
	provider:drawCircle(Input.getMousePosition(), 20)
end

function skill:onInit()
	-- self.angle = self:getPawn():getAngleToPointer()
	self.angle = Math.angle(self:getPawn():getPosition(), Input.getMousePosition())
	self.clock = Utility.Clock.new()

	self:getPawn():setFlip(self:getPawn():getDirection(self.angle))
end

function skill:onUpdate(dt)
	self:getPawn():move(self.angle, dt*700)
	if self.clock:elapsed()>0.20 or self:getPawn():isCollision() then
		self:getPawn():setFrame(0)
		self:kill()
		self:getPawn():useSkill("Dash and hit : hit")
	elseif self.clock:elapsed()>0.10 then
		self:getPawn():setFrame(3)
	else
		self:getPawn():setFrame(2)
	end
end

-- hit
local hit = THIS:newSkill("Dash and hit : hit")
-- THIS:addUsable(hit, 1)

function hit:onInit()
	self.clock = Utility.Clock.new()
	local trigger = self:makeTrigger()
	trigger:setDuration(0.2)
	trigger:nearestOnly()
	trigger:setRadius(50)
	trigger:setPosition(self:getPawn():getPosition())
	function trigger:onInit()
		self:getTarget():addHp(-15)
		self.angle = self:getTarget():getAngleTo(self:getPosition())
	end
	function trigger:onUpdate(dt) 
		self:getTarget():move(self.angle-3.14159, dt*300);
		self:getCaster():move(self.angle, dt*300);
	end
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
