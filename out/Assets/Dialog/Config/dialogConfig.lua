local dialogGUI = Dialog.GUI

local iconSize = 80

function dialogGUI:onInit()
    GUI.addFont("Assets/Dialog/config/FogSans.otf")
    --GUI.addTexture("Assets/Dialogs/config/background.png")
    GUI.addTexture("Assets/Dialog/config/button.png")
    GUI.addTexture("Assets/Dialog/config/icons.png")

	GUI.addTexture("Assets/Dialog/config/map1.png")
 
	local inter = GUI.addInterface()
	local img = inter:addImage()
	img:setTexture(GUI.getTexture("Assets/Dialog/config/map1.png"))
	img:setPropSize(2, 1)

 
    self.interface = GUI.addInterface()
    self.dialogText = self.interface:addTextArea()
    self.list = self.interface:addList()
end
 
function dialogGUI:reset()
    --.interface:setPropPadding(0.1, 0.03)
    local dT = self.dialogText
	dT:setPadding(0, 35)
    dT:setPropPosition(0.5, 1)
    dT:setFont(GUI.getFont("Assets/Dialog/config/FogSans.otf"))
    dT:setCharacterPropSize(0.035)
    dT:setToCenterAlign()
    dT:setPropSize(1, 0.0)
    dT:setOutlinePropThickness(0.1)
    dT:setOutlineColor(5, 5, 15, 255)
    dT:setColor(79, 64, 131, 255)
   
    local list = self.list
    list:setPropBetweenPad(0.025)
    list:setPropPosition(0.1, 0.60)
    list:makeStaticPropPositing()
    list:setPropOrigin(0.0, 1.0)
end
 
function dialogGUI:clearButtons()
    self.list:clear()
end
 
function dialogGUI:getButton(caption, r, g, b, a, iconId)
    local button = self.list:addImage()
    button:setTexture(GUI.getTexture("Assets/Dialog/config/button.png"))
    button:setColor(0, 0, 0, 128)
	button:hide()

    button:setPropSize(0.4, 0.05)
    button:setPropPadding(0.17, 0.0)

    button:makeChildrenUnresizable()
    button:makeChildrenPenetrable()
    button:makeChildrenUncolorable()
    button:setFullSizeFilling()

	local list = button:addList ()
	list:setPropPosition (0, 0.5)
	list:makeHorizontal()
	list:setPropBetweenPad(0.015)
	local icon = list:addImage()
	icon:setPropSize (0.04, 0.04)
	icon:setPropPosition(0,0.5)
	if iconId >= 0 then
		icon:setTexture(GUI.getTexture("Assets/Dialog/config/icons.png"))
		icon:setTextureRect({iconId*iconSize, 0, iconSize, iconSize})
	end


	local text = list:addText()

    text:setFont(GUI.getFont("Assets/Dialog/config/FogSans.otf"))
    text:setCharacterPropSize(0.035)
    --text:setPropPosition(0.0, 0.5)
    text:setColor(r, g, b, a)
    text:setString(caption)
    text:setOutlinePropThickness(0.1)
    text:setOutlineColor(5, 5, 15, 255)
   
    function button:onHoverOut()
        self:hide()
        text:setColor(r, g, b, a)
    end
   
    function button:onHoverIn()
        self:show()
        text:setColor(
            ((r-50 > 0) and r-50 or 0),
            ((g-50 > 0) and g-50 or 0),
            ((b-50 > 0) and b-50 or 0),
            255
        )
    end
    return text;
end
