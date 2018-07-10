GUI.addFont("Assets/Dialog/Config/BKANT.TTF")
GUI.addTexture("Assets/Dialog/Config/background.png")

local interface = GUI.addInterface()


local background = interface:addImage()
background:setTexture(GUI.getTexture("Assets/Dialog/Config/background.png"))
background:setPosition(1440/2-800/2, 400)


local container = background:addWidget()
container:setPosition(220,110+240)

local scrollArea = background:addArea()
scrollArea:setPosition(190,110)
scrollArea:setSize(500,80)
scrollArea:setScrollSpeed(10)

local area = background:addTextArea()
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
            button:setColor(116, 111, 131)
            button:setString("asdasdasdasd") 
            button.onHoverIn = function(self)
                    self:setColor(196, 194, 208)
                end;
            button.onHoverOut = function(self)
                    self:setColor(116, 111, 131)
                end;
        end;
}