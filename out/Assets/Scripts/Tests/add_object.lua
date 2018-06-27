local scene = World.getScene("MyScene")

local e = scene:addEntity("path", "hello")

local c = e:addSpriteComponent()

-- e:sprite():setTexture("Assets/Objects/miasto/Background.png")

-- print(c)

c:setTexture("Assets/Objects/miasto/Background.png")

e = World.getScene():getEntity("Coder")
e:addSpriteComponent()
e:move(0,500,0)
e:sprite():setTexture("Assets/Objects/miasto/Building3.png")
scene:removeEntity(World.getScene():getEntity("Camera"))