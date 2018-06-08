local velocity = 30

THIS.speedX = 10.0
THIS.speedY = THIS.speedX * 0.4
THIS.flipX = false

function THIS:onUpdate(dt)

	local arm = self:armature()

	self.dirX = 0
	self.dirY = 0

	if (Input.isKept(Input.Key_Up)) then
		self.dirY = -1
	elseif (Input.isKept(Input.Key_Down)) then
		self.dirY = 1
	end

	if (Input.isKept(Input.Key_Left)) then
		self.dirX = -1
		self.flipX = true
	elseif (Input.isKept(Input.Key_Right)) then
		self.dirX = 1
		self.flipX = false
	end

	if (self.dirX ~= 0 or self.dirY ~= 0) then
		self.angle = Math.atan2(self.dirY, self.dirX)

		self:move(Math.cos(self.angle) * self.speedX * dt * 60, 0, Math.sin(self.angle) * self.speedY * dt * 60)
	end

	arm:setFlipX(self.flipX)

	if (self.dirX ~= 0) then
		arm:fadeIn("Cedmin_Run_051", 0.25)
	elseif (self.dirY ~= 0) then
		arm:fadeIn("Cedmin_Walk_029", 0.25)
	else
		arm:fadeIn("Cedmin_Idle_008", 0.25)
	end
end
