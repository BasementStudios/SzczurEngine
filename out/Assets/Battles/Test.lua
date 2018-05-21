local battle = BattleScene.makeBattle()

-- Battle
battle:setSize(900, 500)

-- Player
local player = battle:addPawn("Assets/Battle/karion")
BattleScene:changePawn(player)
player:setPosition(100,100)

-- Slime_1
local enemy = battle:addPawn("Assets/Battle/slime")
enemy:setPosition(200,200)

-- Slime_2
enemy = battle:addPawn("Assets/Battle/slime")
enemy:setPosition(300,200)