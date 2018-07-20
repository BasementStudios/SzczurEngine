local pawn = THIS

pawn:armature():setSpeed(1.0)
pawn:armature():fadeIn("idle_fight", 0.1, 0)
pawn:armature():play("idle_fight", 0) 

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

	self.currentSkill = Math.random(2)-1

	-- self.currentSkill = 0

	-- self.currentSkill = 2

	if self.currentSkill == 0 then
		self.skillA:onPrepare()
	elseif self.currentSkill == 1 then
		self.skillB:onPrepare()
	end
end

function pawn:onUpdate()

	if self:isTimeFull() and self.skillIsRunning == false then
		self:setTime(0)
		if self.currentSkill == 0 then
			self.skillA:onUsed()
			-- self.skillIsRunning = true
		elseif self.currentSkill == 1 then
			self.skillB:onUsed()
			-- self.skillIsRunning = true
		elseif self.currentSkill == -1 then
			self:nextSkill()
		end
	end

end

function pawn:onIndicator(indicator)

	if self.currentSkill == 0 then
		self.skillA:onIndicator(indicator)
	elseif self.currentSkill == 1 then
		self.skillB:onIndicator(indicator)
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

			local trigger = Battle.newTrigger()
			trigger:setRadius(200.0)
			trigger:setPosition(self.pawn:getPosition())
			trigger.effect = self
			trigger.pawn = self.pawn
			function trigger:onOverlap(target)
				if target == Battle.getPlayer() then

					target:addHealth(-10)
					local effect = Battle.newEffect()
					effect:setDuration(0.15)
					effect.vector = 30.0*GLM.normalize(target:getPosition() - self.pawn:getPosition())
					effect.target = target
					function effect:onUpdate()
						self.target:move(self.vector)
					end

					effect:activate()

					self.effect:kill()
				end
			end

			trigger:runAndKill()

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
	self.pawn:armature():playOnce("idle_fight_wykrok", 0.1, false, 3.0)
	-- self.pawn:armature():playOnce("dash", 0.2, false, 2.0)

end

---------------- Skill B -------------------

pawn.skillB = {}
local skillB = pawn.skillB
skillB.pawn = pawn
skillB.isUsing = false

function skillB:onPrepare()

	local delta = Battle.getPlayer():getPosition() - self.pawn:getPosition()
	self.pos = self.pawn:getPosition() + 330.0*GLM.normalize(delta)
	self.angle =  GLM.angle(self.pawn:getPosition(), self.pos)
end

function skillB:onIndicator(indicator)
	indicator:renderCircleIndicator(1, self.pos, 200, 0)
end

function skillB:onUsed()
	
	local trigger = Battle.newTrigger()
	trigger:setRadius(330.0)
	trigger:setPosition(self.pos)
	trigger.vector = 20.0*GLM.normalize(self.pos - self.pawn:getPosition())
	function trigger:onOverlap(target)
		if target == Battle.getPlayer() then
			target:addHealth(-10)

			local effect = Battle.newEffect()
			effect:setDuration(0.2)
			effect.vector = self.vector
			effect.target = target
			function effect:onUpdate()
				self.target:move(self.vector)
			end

			effect:activate()

		end
	end
	trigger:runAndKill()

	self.pawn:nextSkill()

	self.pawn:setFlipForPositionRev(self.pos)
	self.pawn:armature():playOnce("idle_fight_zamach", 0.1, false, 3.0)

end