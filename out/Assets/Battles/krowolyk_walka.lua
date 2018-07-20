-- local entity = THIS

-- function entity:onInteractive()

local scene = Battle.createScene(GLM.Vec3(0,0,0), GLM.Vec2(3000, 1500), 1.0)
scene:activate()
-- scene->addControlPawn("karion", 150, 150)
scene:addPlayer("karion", GLM.Vec2(-150, 150))
-- scene:addPawn("krowolyk", GLM.Vec2(2000, 750)):armature():setFlipX(true)
-- scene:addPawn("krowolyk", GLM.Vec2(1000, 500)):armature():setFlipX(true)
-- scene:addPawn("krowolyk", GLM.Vec2(1500, 700)):armature():setFlipX(true)
-- scene:addPawn("krowolyk", GLM.Vec2(2000, 750))
scene:addPawn("krowolyk", GLM.Vec2(2000, 750))

function scene:onFinish(isWin)
	if isWin then 
		print("Win (:")
	else 
		print("Lose :(")
	end
end

Battle.setSkills("Move", "Stun hit", "Dash and hit", "Magic bomb", "Explosion")

-- end