local skill = THIS

--[[ 
	Types:
	POINT_TYPE
	ENEMY_SELECT_TYPE
	FRIEND_SELECT_TYPE
	ANY_SELECT_TYPE
]]-- 

skill.type = POINT_TYPE

function skill:signal(x, y) 
	-- move
	local effector = self.move(x, y)

	-- attack
	effector.onFinish = function()

	end

end