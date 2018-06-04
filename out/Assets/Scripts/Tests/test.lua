local o = World.getScene():addEntity("path", "Test")

o:addScriptableComponent()
o:addSpriteComponent()

o:sprite():setTexture("Assets/Objects/miasto/Background.png")

function o:onUpdate()
	self:move(1,1,0)
end