local pawn = THIS

pawn:armature():setSpeed(1.0)
pawn:armature():fadeIn("Idle", 0.1, 0)
pawn:armature():play("Idle", 0) 

pawn:setDamageAnimation("Zabojczy_kas", 0.1, false, 2.0)

pawn.currentSkill = -1
pawn:setTime(0)
pawn.skillIsRunning = false

function pawn:setFlipForPositionRev(pos)
	local delta = self:getPosition() - pos
	pawn:setFlipForPosition(self:getPosition() + delta)
end

function pawn:nextSkill()
	-- self.currentSkill = self.currentSkill + 1
	-- if self.currentSkill >= 2 then
	-- 	self.currentSkill = 0
	-- end
	self.skillIsRunning = false

	self.currentSkill = Math.random(3)-1

	-- self.currentSkill = 2

	if self.currentSkill == 0 then
		self.skillA:onPrepare()
	elseif self.currentSkill == 1 then
		self.skillB:onPrepare()
	elseif self.currentSkill == 2 then
		self.skillC:onPrepare()
	end
end

function pawn:onUpdate()

	if self:isTimeFull() and self.skillIsRunning == false then
		self:setTime(0)
		if self.currentSkill == 0 then
			self.skillA:onUsed()
			self.skillIsRunning = true
		elseif self.currentSkill == 1 then
			self.skillB:onUsed()
			self.skillIsRunning = true
		elseif self.currentSkill == 2 then
			self.skillC:onUsed()
			self.skillIsRunning = true
		elseif self.currentSkill == -1 then
			self:nextSkill();
		end
	end

end

function pawn:onIndicator(indicator)

	if self.currentSkill == 0 then
		self.skillA:onIndicator(indicator)
	elseif self.currentSkill == 1 then
		self.skillB:onIndicator(indicator)
	elseif self.currentSkill == 2 then
		self.skillC:onIndicator(indicator)
	end

	-- indicator:renderCircleIndicator(1, self:getPosition(), 300, 0)

end

---------------- Skill A -------------------

pawn.skillA = {}
local skillA = pawn.skillA
skillA.pawn = pawn
skillA.isUsing = false

function skillA:onPrepare()

	local delta = Battle.getPlayer():getPosition() - self.pawn:getPosition()
	self.distance = GLM.length(delta)
	if self.distance > 1000.0 then self.distance = 1000.0 end
	self.pos = self.pawn:getPosition() + self.distance*GLM.normalize(delta)

	self.angle =  GLM.angle(self.pawn:getPosition(), self.pos)
end

function skillA:onIndicator(indicator)
	if self.isUsing == false then
		indicator:renderCircleIndicator(1, self.pos, 250, 1)		
		indicator:renderLineIndicator(1, self.pawn:getPosition(), self.distance, 200.0, self.angle, 0)
	else 
		indicator:renderCircleIndicator(1, self.pos, 150, 1)		
	end
end

function skillA:onUsed()
	
	self.isUsing = true

	local effect = Battle.newEffect()
	effect:setDuration(0.4)
	effect.vector = GLM.normalize(self.pos - self.pawn:getPosition()) * 30.0 * self.distance / 1000.0
	effect.pawn = self.pawn
	effect.skill = self

	function effect:onUpdate()
		if self.pawn then
			self.pawn:move(self.vector)
		end
	end

	function effect:onFinish()
		if self.pawn then
			self.skill.isUsing = false
			self.pawn:nextSkill()
		end
	end

	effect:activate()

	self.pawn:setFlipForPositionRev(self.pos)
	self.pawn:armature():playOnce("dash", 0.1, false, 3.0)
	-- self.pawn:armature():playOnce("dash", 0.2, false, 2.0)

end

---------------- Skill B -------------------

pawn.skillB = {}
local skillB = pawn.skillB
skillB.pawn = pawn
skillB.isUsing = false

function skillB:onPrepare()

end

function skillB:onIndicator(indicator)

	self.pos = self.pawn:getPosition()
	local delta = Battle.getPlayer():getPosition() - self.pos 
	self.pos = self.pos + 300.0 * GLM.normalize(delta)

	indicator:renderCircleIndicator(1, self.pos, 500, 0)

end

function skillB:onUsed()

	local effect = Battle.newEffect()
	effect:setDuration(1.2)
	effect.pos = self.pos
	effect.state = 0
	effect.pawn = self.pawn

	local trigger = Battle.newTrigger()
	effect.trigger = trigger
	trigger:setPosition(self.pos)
	trigger:setRadius(500.0)
	function trigger:onOverlap(target)
		if target == Battle.getPlayer() then
			target:addHealthHit(-5)
		end
	end

	function effect:onUpdate()
		if self.pawn then
			if self.state == 0 then
				self.state = 1
				self.trigger:run()
				self.pawn:setFlipForPositionRev(self.trigger:getPosition())
			elseif self:getCurrentDuration() > 0.2 and self.state == 1 then
				self.state = 2
				self.trigger:run()
				self.pawn:setFlipForPositionRev(self.trigger:getPosition())
			elseif self:getCurrentDuration() > 0.4 and self.state == 2 then
				self.state = 3
				self.trigger:run()
				self.pawn:setFlipForPositionRev(self.trigger:getPosition())
			elseif self:getCurrentDuration() > 0.6 and self.state == 3 then
				self.state = 4
				self.trigger:run()
				self.pawn:setFlipForPositionRev(self.trigger:getPosition())
			end
		end
	end

	function effect:onFinish()
		if self.pawn then
			self.trigger:kill()
			self.pawn:nextSkill()
		end
	end

	effect:activate()

	self.pawn:armature():playOnce("RozpruwanieAll", 0.1, false, 3.0)
end

---------------- Skill C -------------------

pawn.skillC = {}
local skillC = pawn.skillC
skillC.pawn = pawn
skillC.isUsing = false

function skillC:onPrepare()
	self.radius = 200
	self.pos = Battle.getPlayer():getPosition()
	self.lockPos = false
end

function skillC:onIndicator(indicator)
	if self.isUsing == false then
		self.pos = Battle.getPlayer():getPosition()
	end
	indicator:renderCircleIndicator(1, self.pos, self.radius, 0)
end

function skillC:onUsed()

	self.isUsing = true

	local effect = Battle.newEffect()
	effect:setDuration(2.8)
	effect.state = 0
	effect.pawn = self.pawn
	effect.skill = self

	function effect:onUpdate()
		if self.pawn then
			if self.state == 0 then
				self.skill.radius = 200 + self:getCurrentDuration()*150
				self.skill.pos = Battle.getPlayer():getPosition()
				if self:getCurrentDuration()>1.8 then
					self.state = 1
					local delta = self.skill.pos - self.pawn:getPosition()
					self.vector = GLM.length(delta)*GLM.normalize(delta)/20

					self.pawn:setFlipForPositionRev(self.pawn:getPosition())
					self.pawn:armature():playOnce("Skok_Na_Krowe_All", 0.1, false, 1.5)
				end
			elseif self.state == 1 and self:getCurrentDuration()>2.5 then
				self.skill.maxRadius = self.skill.radius
				self.state = 2
			elseif self.state == 2 then
				self.pawn:move(self.vector)
			end
		end
	end

	function effect:onFinish()
		if self.pawn then
			self.skill.isUsing = false
			self.pawn:nextSkill()

			local trigger = Battle.newTrigger()
			trigger:setRadius(self.skill.maxRadius)
			trigger:setPosition(self.pawn:getPosition())
			trigger.pawn = self.pawn
			function trigger:onOverlap(target)
				if target == Battle.getPlayer() then
					local effect = Battle.newEffect()
					effect.target = target
					effect:setDuration(5.0)
					local delta = target:getPosition() - self.pawn:getPosition()
					effect.vector = 20.0*GLM.normalize(delta)
					target:addHealth(-20)

					function effect:onUpdate()
						self.target:setStatus(0)
						self.target:addHealth(-0.05)
						if self:getCurrentDuration() < 0.4 then
							self.target:move(self.vector)
						end
					end
					effect:activate()
				end
			end

			trigger:runAndKill()

			self.pawn:armature():playOnce("Rozpruwanie_B1", 0.1, false, 3.0)
		end
	end

	effect:activate()

end
