local pawn = THIS


-- Init
pawn:armature():setSpeed(1.0)
pawn:armature():fadeIn("Cedmin_Fight_Idle_011", 0.1, 0)

-- Skill: "Dash"
local skillA = pawn:newSkill("Dash")
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
		indicator:renderCircleIndicator(self.circlePos, 400.0)
	else
		local angle = self:getPawn():getAngleTo(Battle.getCursorPosition())
		self.circlePos = Battle.getMovedPosition(self:getPawn():getPosition(), angle, 900.0)

		indicator:renderCircleIndicator(self.circlePos, 400.0, 1)
		indicator:renderLineIndicator2(self:getPawn():getPosition(), 1000.0, 200.0, angle, 0)
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
		end
	end

	function effect:onFinish()
		if self.owner then
			-- self.owner:armature():play("Cedmin_Fight_Idle_011", 1)
			-- self.owner:armature():setSpeed(1.0)
			self.skill.isUsing = false
			-- self:getPawn():armature():playOnce("Cedmin_Attack_Full_003", 1, 4.0, false)
			self.owner:armature():fadeIn("Cedmin_Fight_Idle_011", 0.2, 0)
			self.owner:armature():playOnce("Cedmin_Attack_Hit_023", 0.2, false, 4.0)
		end
	end

	effect:activate()
	self.isUsing = true
	
	self:getPawn():setFlipForPosition(self.targetPosition)
	-- self:getPawn():armature():setSpeed(5.5)
	-- self:getPawn():armature():playOnce("Cedmin_Dodge_3", 1, 7.0, false)
	-- self:getPawn():armature():playOnce("Cedmin_Run_051", 0.1, false, 1.4)
	self:getPawn():armature():setSpeed(1.0)
	self:getPawn():armature():fadeIn("Cedmin_Run_051", 0.2, 0)
	-- self:getPawn():armature():play("Cedmin_Fight_Idle_011", 1)
end

function pawn:onCollision(target)
	if target and self.skillA.effect and self.skillA.effect:isActive() then
		print("Pawn!")
		self.skillA.effect:kill();

		local effect = Battle.newEffect()
		effect:setDuration(0.3)
		effect.target = target
		effect.direction = self:getNormalDirection(target:getPosition())
		function effect:onUpdate()
			if self.target then
				self.target:move(self.direction, 20.0)
			end
		end
		effect:activate()
		target:addHealth(-30)

	end
	if not target then 
		print("Wall!")
	end
end

-- Skill: "Hit"
local skillB = pawn:newSkill("Hit")

function skillB:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self.pos = self.circlePos
		self:use(true)
	end
end

function skillB:onSpellIndicator(indicator)
	local angle = self:getPawn():getAngleTo(Battle.getCursorPosition())
	self.circlePos = Battle.getMovedPosition(self:getPawn():getPosition(), angle, 250.0)
	indicator:renderCircleIndicator(self.circlePos, 250.0)
end

function skillB:onUsed() 

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

			target:addHealth(-20)
		end
	end

	trigger:runAndKill()

	self:getPawn():setFlipForPosition(self.pos)
	self:getPawn():armature():fadeIn("Cedmin_Fight_Idle_011", 0.1, 0)
	self:getPawn():armature():playOnce("Cedmin_Attack_Hit_023", 0.1, false, 3.0)

end

-- Skill: "Run"
local skillC = pawn:newSkill("Run")
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

	indicator:renderLineIndicator2(self:getPawn():getPosition(), self.indDistance, 200.0, self.indAngle)
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
	effect:setDuration(0.4)

	function effect:onUpdate()
		if self.owner then
			effect.owner:move(self.direction, 35.0*self.skill.dis/1000.0)
		end
	end

	function effect:onFinish()
		self.skill.isUsing = false;
	end

	effect:activate()
	
	pawn:setFlipForPosition(self.pos)
	pawn:armature():setSpeed(1.0)
	pawn:armature():playOnce("Cedmin_Run_051", 0.1, false, 2.0)
end

-- Skill: "Bomb"
local skillD = pawn:newSkill("Bomb")
skillD.isUsing = false

function skillD:onSelectedUpdate()
	if Battle.isSkillButtonPressed() then
		self:use(false)
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

	indicator:renderCircleIndicator(self.indPos, self.indRadius)
end

function skillD:onUsed()
	self.isUsing = true
	-- self:getPawn():setTime(self:getPawn():getMaxTime()*0.50)
	self:getPawn():setTime(self:getPawn():getMaxTime())

	local effect = Battle.newEffect(self)
	effect:setDuration(0.5)

	effect.pos = self.indPos
	effect.dis = self.indDis
	effect.radius = self.indRadius
	effect.maxDis = self.indMaxDis
	effect.dir = self:getPawn():getNormalDirection(effect.pos)

	effect.bomb = Battle.newSprite("fireBomb")
	effect.bomb:setOrigin(GLM.Vec2(382.0/2.0, 276.0/2.0))
	effect.bomb:setPosition(self:getPawn():getPosition())

	function effect:onUpdate()
		local currentDis = Battle.getDistance(self:getSkill():getPawn():getPosition(), self.bomb:getPosition())

		local startAngle = 0.7
		self.bomb:setHeight(Math.sin(startAngle+(3.14159-startAngle)*currentDis/self.dis)*1100)

		effect.bomb:setScale(0.6 + 0.3*currentDis/self.dis)

		self.bomb:move(self.dir*60.0*self.dis/self.maxDis)

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
				explosionEffect:setDuration(self:getRadius()*0.0002)			
				explosionEffect.target = target
				explosionEffect.vector = 50.0*GLM.normalize(target:getPosition()-self:getPosition())

				function explosionEffect:onUpdate()
					if self.target then
						self.target:move(self.vector)
					end
				end

				explosionEffect:activate()

				target:addHealth(-20)
			end			
		end

		-- trigger:run()
		trigger:runAndKill()

		-- create explosion
		local explosion = Battle.newAnimationSprite("smallExplosion");
		explosion:removeOnLastFrame(true)
		-- explosion:setFrame(0)
		explosion:setFrames(GLM.Vec2(400,400), 5, 20)
		explosion:setOrigin(GLM.Vec2(200,200))
		explosion:setTimestamp(0.02)
		explosion:setScale(self.radius*0.008);
		explosion:setPosition(self.pos)

	end

	effect:activate()


	self:getPawn():setFlipForPosition(effect.pos)
	self:getPawn():armature():playOnce("Cedmin_Attack_Hit_023", 0.1, false, 3.0)
end