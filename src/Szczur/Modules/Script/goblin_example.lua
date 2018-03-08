--[[
Pawn object script

Enemy name: Goblin gunner
Size: 1x1
Max hp: 20

Skills:
1. Hit - make 4 dmg in range 1 | cooldown: 4s
2. Heal - give 5 hp for nearest ally in range 3 | cooldown: 20s
3. Shoot - make 6 fire dmg in range 4, attack through walls | cooldown: 12s
4. Speed up! - gain 50% less turn time for 4 turns | cooldown: 30s

Passives:
1. Grouping - gain +1 dmg for every goblin on battle field
2. Just no fire! - gain +50% more dmg from fire attack

Behaviors:
1. At start: change role [Warrior], 20% use [Speed up!], 80% use [Shoot]
2. When player hp 50% or less: use [Shoot]
3. When pawn hp 20% or less: change role [Coward]
4. When can: use [Shoot], use [Hit] 
5. When chance: 40% use [Heal], 60% use [Speed up!] 

]]--


State.create = function()
	local stats = {
		name = "Goblin gunner";
		maxHp = 20;
		hp = maxHp;
		tags = {"goblin"};
	}
	This:setStats(stats)
	end
	
State.skills = function()

-- SKILLS

	-- Hit 
	local skill = {
		name = "Hit";
		rangeType = "cross";
		range = 1;
		targetType = "enemy";
		cooldown = 4.0;
		damage = 4;
	}
	This:addSkill(skill)
	
	-- Heal
	skill = {
		name = "Heal";
		rangeType = "rect";
		range = 3;
		throughWalls = true;
		targetType = "friend";
		autoSelect = "nearest_target";
		cooldown = 20.0;
		heal = 5;
	}
	This:addSkill(skill)

	-- Shoot
	skill = {
		name = "Shoot";
		rangeType = "rect";
		range = 4;
		throughWalls = true;
		targetType = "enemy";
		cooldown = 12.0;
		damage = 6;
		damageTags = {"fire"};
	}
	This:addSkill(skill)
	
	-- statMod = {stat = "turn_time", mod = "*", value = 0.5}
	-- statMod = {"turn_time", "*", 0.5}
	-- modifiers = {turnTime = {"*", 0.5}}
	
	-- Speed up!
	skill = {
		name = "Speed up";
		targetType = "self";
		cooldown = 30.0;
		effect = {
			turnTime = 4;
			statMod = {
				turnTime = {"*", 0.5};
			};
		};
	}
	This:addSkill(skill)
	
-- PASSIVES

	-- Grouping
	passive = {
		name = "Grouping";
		effect = {
			
		}
	}
	This:addPassive(passive)
	
	skill = This:newSkill("Grouping")
	skill:passive(true) -- false[Def]
	effect = skill:makeEffect()
	effect:onBeginTurn(function(target)
		local number = BattleField.numberOfTag("goblin")
		target:statMod("damage", "+", number)
		end
	)
	
	-- Just no fire!
	skill = This:newSkill("Just no fire!")
	skill:passive(true)
	effect = skill:makeEffect()
	effect:onGainDamage(function(target, damage)
		if damage.hasTag("fire") then damage.value *= 1.5 end
		end
	)
	end
	
State.behaviors = function()
	behavior = This:newBehavior(1)
	behavior:atStart(function(target)
		
		end
	