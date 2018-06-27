local o = World.getScene():addEntity("path", "Test")

o:addSpriteComponent()

o.tex1 = World.getTextureDataHolder():getData("Assets/Objects/miasto/Background.png", true)
o.tex2 = World.getTextureDataHolder():getData("Assets/Objects/miasto/Building3.png", true)

o:sprite():setTextureData(o.tex1)

local o2 = World.getScene():cloneEntity(o)
o2:setName("Test2")

o2:sprite():setTextureData(o2.tex2)

local size = o:sprite():getTextureSize()
o:setOrigin(size.x*0.5, size.y*0.5, 0.0)
