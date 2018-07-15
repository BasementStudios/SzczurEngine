local dialogGUI = Dialog.GUI
 
function dialogGUI:onInit()
    GUI.addFont("Assets/Dialog/config/BKANT.TTF")
    --GUI.addTexture("Assets/Dialogs/config/background.png")
    GUI.addTexture("Assets/Dialog/config/button.png")
    GUI.addTexture("Assets/Dialog/config/icons.png")
 
 
    self.interface = GUI.addInterface()
    self.dialogText = self.interface:addTextArea()
    self.list = self.interface:addList()
end
 
function dialogGUI:reset()
    --.interface:setPropPadding(0.1, 0.03)
    local dT = self.dialogText
    dT:setPropPosition(0.5, 1)
    dT:setFont(GUI.getFont("Assets/Dialog/config/BKANT.TTF"))
    dT:setCharacterPropSize(0.03)
    dT:setToCenterAlign()
    dT:setPropSize(1, 0.0)
    dT:setOutlinePropThickness(0.03)
    dT:setOutlineColor(255, 0, 255, 255)
    dT:setColor(200, 0, 200, 255)
   
    local list = self.list
    list:setPropBetweenPad(0.025)
    list:setPropPosition(0.1, 0.75)
    list:makeStaticPropPositing()
    list:setPropOrigin(0.0, 1.0)
end
 
function dialogGUI:clearButtons()
    self.list:clear()
end
 
function dialogGUI:getButton(caption, r, g, b, a, iconId)
    local button = self.list:addImage()
    button:setTexture(GUI.getTexture("Assets/Dialog/config/button.png"))
    button:setColor(50, 50, 50, 0)

    button:setPropSize(0.4, 0.05)
    button:setPropPadding(0.17, 0.0)

    button:makeChildrenUnresizable()
    button:makeChildrenPenetrable()
    button:makeChildrenUncolorable()
    button:setFullSizeFilling()
   
   
    local text = button:addText()
    text:setFont(GUI.getFont("Assets/Dialog/config/BKANT.TTF"))
    text:setCharacterPropSize(0.035)
    text:setPropPosition(0.0, 0.5)
    text:setColor(r, g, b, a)
    text:setString(caption)
    text:setOutlinePropThickness(0.03)
    text:setOutlineColor(
        ((r+50 < 255) and r+50 or 255),
        ((g+50 < 255) and g+50 or 255),
        ((b+50 < 255) and b+50 or 255),
        255
    )
   
    function button:onHoverOut()
        self:setColor(255, 255, 255, 0)
        text:setColor(r, g, b, a)
    end
   
    function button:onHoverIn()
        self:setColor(255, 255, 255, 255)
        text:setColor(
            ((r+50 < 255) and r+50 or 255),
            ((g+50 < 255) and g+50 or 255),
            ((b+50 < 255) and b+50 or 255),
            255
        )
    end
    return text;
end
