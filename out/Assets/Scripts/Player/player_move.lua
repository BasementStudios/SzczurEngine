function THIS:onUpdate(dt)
	local velocity = 9*dt*60
	if(Input.isKept(Input.Key_Up)) then
		self:move(0, 0, -velocity / 1.2)
	end
	if(Input.isKept(Input.Key_Left)) then
		self:move(-velocity, 0, 0)
	end
	if(Input.isKept(Input.Key_Down)) then
		self:move(0, 0, velocity / 1.2)
	end
	if(Input.isKept(Input.Key_Right)) then
		self:move(velocity, 0, 0)
	end

	local armature = self:armature()

	if (Input.isPressed(Input.Key_Left)) then
		armature:fadeIn("Run_051", 0.25)
		armature:setFlipX(true)
	elseif (Input.isPressed(Input.Key_Right)) then
		armature:fadeIn("Run_051", 0.25)
		armature:setFlipX(false)
	elseif (Input.isPressed(Input.Key_Up)) then
		armature:fadeIn("Walk_029", 0.25)
	elseif (Input.isPressed(Input.Key_Down)) then
		armature:fadeIn("Walk_029", 0.25)
	end

end
