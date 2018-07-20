local pawn = THIS

function pawn:onUpdate()
	self:addHealthHit(0.05)
end

-- Init
pawn:armature():setSpeed(1.0)
pawn:armature():fadeIn("Cedmin_Fight_Idle", 0.1, 0)

-- Skill: "Dash"
local skillA = pawn:newSkill("Dash and hit")
skillA:setCostPP(2)
pawn.skillA = skillA
skillA.isUsing = false

function skillA:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self.targetPosition = Battle.getCursorPosition()
		self:use(true)
	end
end

function skillA:onSpellIndicator(indicator)
	if self.isUsing then
		indicator:renderCircleIndicator(0, self.circlePos, self:getPawn():getRadius()+10.0)
	else
		local angle = self:getPawn():getAngleTo(Battle.getCursorPosition())
		self.circlePos = Battle.getMovedPosition(self:getPawn():getPosition(), angle, 900.0)

		local radius = self:getPawn():getRadius()+30.0
		indicator:renderCircleIndicator(0, self.circlePos, radius, 1)
		indicator:renderLineIndicator(0, self:getPawn():getPosition(), 1000.0-radius, 200.0, angle, 0)
	end
end

function skillA:onUsed()
	-- print("USED!")

	local effect = Battle.newEffect()
	self.effect = effect

	effect.skill = self
	effect.direction = self:getPawn():getNormalDirection(self.targetPosition)
	effect.owner = self:getPawn()
	effect:setDuration(0.4)

	function effect:onUpdate()
		-- print("X")
		if self.owner then
			self.owner:move(self.direction, 35.0)

			local trigger = Battle.newTrigger()
			trigger:setRadius(self.owner:getRadius()+10.0)
			trigger:setPosition(self.owner:getPosition())
			trigger.owner = self.owner
			trigger.parent = self

			function trigger:onOverlap(target)
				if target ~= self.owner then
					local hitEffect = Battle.newEffect()
					hitEffect:setDuration(0.3)
					hitEffect.target = target
					hitEffect.vector = GLM.normalize(target:getPosition()-self:getPosition())*50.0
					function hitEffect:onUpdate()
						if self.target then
							self.target:move(self.vector)
						end
					end
					hitEffect:activate()
					target:addHealthHit(-50)
					-- self.owner:addHealth(-30)

					self:kill()
					self.parent:kill()
					print("Hit!")
				end
			end

			trigger:runAndKill()

		end
	end

	function effect:onFinish()
		if self.owner then
			-- self.owner:armature():play("Cedmin_Fight_Idle", 1)
			-- self.owner:armature():setSpeed(1.0)
			self.skill.isUsing = false
			-- self:getPawn():armature():playOnce("Cedmin_Attack_Full_003", 1, 4.0, false)
			self.owner:armature():setSpeed(1.0)
			self.owner:armature():fadeIn("Cedmin_Fight_Idle", 0.2, 0)
			self.owner:armature():playOnce("Cedmin_Attack_Hit_023", 0.2, false, 4.0)
		end
	end

	effect:activate()
	self.isUsing = true
	
	self:getPawn():setFlipForPosition(self.targetPosition)
	-- self:getPawn():armature():setSpeed(5.5)
	-- self:getPawn():armature():playOnce("Cedmin_Dodge_3", 1, 7.0, false)
	-- self:getPawn():armature():playOnce("Cedmin_Run_051", 0.1, false, 1.4)
	self:getPawn():armature():setSpeed(2.5)
	self:getPawn():armature():fadeIn("Cedmin_Krok_Cienia_Start", 0.001, 1)
	-- self:getPawn():armature():fadeIn("Cedmin_Run_051", 0.2, 0)
	-- self:getPawn():armature():play("Cedmin_Fight_Idle", 1)
end

-- function pawn:onCollision(target)
-- 	if target and self.skillA.effect and self.skillA.effect:isActive() then
-- 		print("Pawn!")
-- 		self.skillA.effect:kill();

-- 		local effect = Battle.newEffect()
-- 		effect:setDuration(0.3)
-- 		effect.target = target
-- 		effect.direction = self:getNormalDirection(target:getPosition())
-- 		function effect:onUpdate()
-- 			if self.target then
-- 				self.target:move(self.direction, 20.0)
-- 			end
-- 		end
-- 		effect:activate()
-- 		target:addHealth(-30)

-- 	end
-- 	if not target then 
-- 		print("Wall!")
-- 	end
-- end

-- Skill: "Hit"
local skillB = pawn:newSkill("Stun hit")
skillB:setCostPP(1)

function skillB:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self.pos = self.circlePos
		self:use(false)
	end
end

function skillB:onSpellIndicator(indicator)
	local angle = self:getPawn():getAngleTo(Battle.getCursorPosition())
	self.circlePos = Battle.getMovedPosition(self:getPawn():getPosition(), angle, 250.0)
	indicator:renderCircleIndicator(0, self.circlePos, 250.0)
end

function skillB:onUsed() 
	self:getPawn():setTime(self:getPawn():getMaxTime()*0.50)

	local trigger = Battle.newTrigger()
	trigger:setPosition(self.pos)
	trigger:setRadius(250)
	trigger.owner = self:getPawn()

	function trigger:onOverlap(target)
		if target ~= self.owner then

			local effect = Battle.newEffect()
			effect:setDuration(0.06)
			effect.target = target
			effect.direction = Battle.getNormalDirection(self:getPosition(), target:getPosition())
			function effect:onUpdate()
				if self.target then
					self.target:move(self.direction, 40.0)
				end
			end

			effect:activate()

			target:addHealthHit(-20)
		end
	end

	trigger:runAndKill()

	self:getPawn():setFlipForPosition(self.pos)
	self:getPawn():armature():fadeIn("Cedmin_Fight_Idle", 0.1, 0)
	self:getPawn():armature():playOnce("Cedmin_Uderzenie_Rekojescia", 0.1, false, 2.0)

end

-- Skill: "Run"
local skillC = pawn:newSkill("Move")
skillC:setCostPP(0)
skillC.isUsing = false

function skillC:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self.pos = self.indPos
		self.dis = self.indDistance
		self:use(false)
	end
end

function skillC:onSpellIndicator(indicator)

	if not self.isUsing then
		self.indAngle = self:getPawn():getAngleTo(Battle.getCursorPosition())
		self.indDistance = self:getPawn():getDistanceTo(Battle.getCursorPosition())
		if self.indDistance > 1000 then
			self.indDistance = 1000
		end
	else 
		self.indDistance = self:getPawn():getDistanceTo(self.pos)
	end

	indicator:renderLineIndicator2(0, self:getPawn():getPosition(), self.indDistance, 200.0, self.indAngle)
	self.indPos = Battle.getMovedPosition(self:getPawn():getPosition(), self.indAngle, self.indDistance)

end

function skillC:onUsed()
	self:getPawn():setTime(self:getPawn():getMaxTime()/2.0)
	self.isUsing = true

	local effect = Battle.newEffect()
	local pawn = self:getPawn()

	effect.direction = pawn:getNormalDirection(self.pos)
	effect.skill = self
	effect.owner = pawn
	effect:setDuration(0.6)

	function effect:onUpdate()
		if self.owner then
			effect.owner:move(self.direction, 25.0*self.skill.dis/1000.0)
		end
	end

	function effect:onFinish()
		self.skill.isUsing = false;
	end

	effect:activate()
	
	pawn:setFlipForPosition(self.pos)
	pawn:armature():setSpeed(1.0)
	pawn:armature():playOnce("Cedmin_Run_051", 0.1, false, 1.5)
end

-- Skill: "Bomb"
local skillD = pawn:newSkill("Magic bomb")
skillD:setCostPP(3)
skillD.isUsing = false

function skillD:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self:use(true)
	end
end

function skillD:onSpellIndicator(indicator)
	if self.isUsing == false then
		self.indMaxDis = 2000.0

		local delta = Battle.getCursorPosition()-self:getPawn():getPosition()
		self.indDis = GLM.length(delta)

		if self.indDis > self.indMaxDis then
			self.indDis = self.indMaxDis
		elseif self.indDis < 200.0 then
			self.indDis = 200.0
		end

		self.indPos = self:getPawn():getPosition()+GLM.normalize(delta)*self.indDis

		self.indRadius = 200.0 + 500.0*GLM.sqr(self.indDis/self.indMaxDis)
	end

	indicator:renderCircleIndicator(0, self.indPos, self.indRadius)
end

function skillD:onUsed()
	self.isUsing = true
	-- self:getPawn():setTime(self:getPawn():getMaxTime()*0.50)
	-- self:getPawn():setTime(self:getPawn():getMaxTime())

	local effect = Battle.newEffect(self)
	effect:setDuration(1.2)

	effect.pos = self.indPos
	effect.dis = self.indDis
	effect.radius = self.indRadius
	effect.maxDis = self.indMaxDis
	effect.dir = self:getPawn():getNormalDirection(effect.pos)
	effect.state = 0


	function effect:onUpdate()
		if self:getCurrentDuration() > 0.33 and self.state == 0 then
			self.state = 1
			self:getSkill():getPawn():armature():playOnce("Cedmin_Rzut_Bomba_Rzut", 0.1 , false, 1.5)
		elseif self:getCurrentDuration() > 0.65 and self.state == 1 then
			self.state = 2
			self.bomb = Battle.newSprite("fireBomb")
			self.bomb:setOrigin(GLM.Vec2(382.0/2.0, 276.0/2.0))
			self.bomb:setPosition(self:getSkill():getPawn():getPosition())
		elseif self.state == 2 then 
			local currentDis = Battle.getDistance(self:getSkill():getPawn():getPosition(), self.bomb:getPosition())

			local startAngle = 0.8
			self.bomb:setHeight(Math.sin(startAngle+(3.14159-startAngle)*currentDis/self.dis)*1100)

			self.bomb:rotate(GLM.Vec3(0,0,0.2))

			effect.bomb:setScale(0.6 + 0.3*currentDis/self.dis)

			self.bomb:move(self.dir*60.0*self.dis/self.maxDis)
		end
	end

	function effect:onFinish()
		self.bomb:kill()
		self:getSkill().isUsing = false

		-- Create trigger
		local trigger = Battle.newTrigger()
		trigger:setPosition(self.pos)
		trigger:setRadius(self.radius)
		trigger.owner = self:getSkill():getPawn()

		local explosionEffect = Battle.newEffect()
		function trigger:onOverlap(target)
			if target ~= self.owner then

				local explosionEffect = Battle.newEffect()
				-- explosionEffect:setDuration(self:getRadius()*0.0002)			
				explosionEffect:setDuration(self:getRadius()*0.00025)			
				explosionEffect.target = target
				explosionEffect.vector = 50.0*GLM.normalize(target:getPosition()-self:getPosition())

				function explosionEffect:onUpdate()
					if self.target then
						self.target:setStatus(0)
						self.target:setStatus(1)
						-- self.target:setStatus(0)
						self.target:move(self.vector)
					end
				end

				explosionEffect:activate()

				target:addHealthHit(-20)
			end			
		end

		-- trigger:run()
		trigger:runAndKill()

		-- create explosion
		local explosion = Battle.newAnimationSprite("smallExplosion");
		explosion:removeOnLastFrame(true)
		-- explosion:setFrame(0)
		explosion:setFrames(GLM.Vec2(400,400), 5, 25)
		explosion:setOrigin(GLM.Vec2(200,200))
		explosion:setTimestamp(0.02)
		explosion:setScale(self.radius*0.008);
		explosion:setPosition(self.pos)

	end

	effect:activate()


	self:getPawn():setFlipForPosition(effect.pos)
	self:getPawn():armature():playOnce("Cedmin_Rzut_Bomba_Start", 0.1, false, 3.0)
end

-- Skill: "Explosion"
local skillE = pawn:newSkill("Explosion")
skillE:setCostPP(4)
skillE.isUsing = false

function skillE:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self:use(false)
	end
end

function skillE:onSpellIndicator(indicator)

	if self.isUsing == false then
		self.rad1 = 300.0
		self.rad2 = 700.0
		self.rad3 = 800.0
		self.pos = self:getPawn():getPosition()
	end

	indicator:renderCircleIndicator(0, self.pos, self.rad1, 1)
	indicator:renderCircleIndicator(0, self.pos, self.rad2, 2)
	indicator:renderCircleIndicator(0, self.pos, self.rad3, 3)

end

function skillE:createExplosion(pos, radius)
	local explosion = Battle.newAnimationSprite("bigExplosion");
	explosion:removeOnLastFrame(true)
	explosion:stopAtLastFrame(true)
	explosion:setFrames(GLM.Vec2(400,400), 5, 25)
	explosion:setOrigin(GLM.Vec2(200,200))
	explosion:setTimestamp(0.015)
	explosion:setPosition(pos)
	explosion:setScale(radius*0.007)
end

function skillE:onUsed()

	local effect = Battle.newEffect(self)
	effect:setDuration(1.6)
	effect.pos = self.pos
	effect.rad1 = self.rad1
	effect.rad2 = self.rad2
	effect.rad3 = self.rad3
	effect.state = 0
	effect.pawn = self:getPawn()

	function effect:onUpdate()
		if self:getCurrentDuration() > 0.6 and self.state == 0 then
			self.state = 1
			self:getSkill():createExplosion(self.pos + GLM.Vec2(0, 10), self.rad1)
		elseif self:getCurrentDuration() > 0.8 and self.state == 1 then
			self.state = 2
			self:getSkill():createExplosion(self.pos + GLM.Vec2(0, 10), self.rad2)
		elseif self:getCurrentDuration() > 1.0 and self.state == 2 then
			self.state = 3
			self:getSkill():createExplosion(self.pos + GLM.Vec2(0, 10), self.rad3)
		end
	end

	function effect:onFinish()

		if self.pawn then
			self.pawn:armature():setSpeed(1.0)
			self.pawn:armature():fadeIn("Cedmin_Fight_Idle", 0.2, 0)
			self.pawn:armature():playOnce("Cedmin_Erupcja_Gniewu_Koniec", 0.2, false, 4.0)
		
			Battle.getCurrentScene():addBrokenPP(1)
			Battle.getCurrentScene():addMaxPP(-1)
		end

		effect:getSkill().isUsing = false
	end

	-- create explosion

	effect:activate()

	self:getPawn():armature():setSpeed(1.75)
	self:getPawn():armature():fadeIn("Cedmin_Erupcja_Gniewu_Start", 0.1, 1)

end