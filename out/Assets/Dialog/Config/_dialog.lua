GUI.addFont("Assets/Dialog/Config/BKANT.TTF")
GUI.addTexture("Assets/Dialog/Config/background.png")

local interface = GUI.addInterface()
local background = interface:addImage()
background:setTexture(GUI.getTexture("Assets/Dialog/Config/background.png"))
local x = 1440.0/2.0-800.0/2.0
local y = 400.0
background:setPosition(x, y)


local container = background:addWidget()
container:setPosition(220,110+240)

local scrollArea = background:addScrollArea()
scrollArea:setPosition(190,110)
scrollArea:setSize(500.0,80.0)
--scrollArea:setScrollSpeed(10)

local area = background:addTextArea()
area:setPosition(area:getPosition())
area:setPosition(100,100)
area:setFont(GUI.getFont("Assets/Dialog/Config/BKANT.TTF"))
area:setCharacterSize(20)
area:setToCenterAlign()


Dialog.GUI.set = {
    area = area;
    container = container;
    interface = interface;
    creator = function(i, button)
            button:setFont(GUI.getFont("Assets/Dialog/Config/BKANT.TTF"));
            button:setCharacterSize(20)
            button:setPosition(-100, i*30 - 250)
            button:setColor(116, 111, 131, 255)
            button:setString("asdasdasdasd") 
            button.onHoverIn = function(self)
                    self:setColor(196, 194, 208, 255)
                end;
            button.onHoverOut = function(self)
                    self:setColor(116, 111, 131, 255)
                end;
        end;
}