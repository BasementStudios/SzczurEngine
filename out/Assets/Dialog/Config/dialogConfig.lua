local dialogGUI = Dialog.GUI
 
function dialogGUI:onInit()
    GUI.addFont("Assets/Dialog/config/BKANT.TTF")
    --GUI.addTexture("Assets/Dialogs/config/background.png")
    GUI.addTexture("Assets/Dialog/config/button.png")
    GUI.addTexture("Assets/Dialog/config/icons.png")
 
 
    self.interface = GUI.addInterface()
    self.dialogText = self.interface:addTextArea()
    self.listHolder = self.interface:addWidget()
    self.list = self.listHolder:addList()
end
 
function dialogGUI:reset()
    --.interface:setPropPadding(0.1, 0.03)
    local dT = self.dialogText
    dT:setPropPosition(0.5, 1)
    dT:setFont(GUI.getFont("Assets/Dialog/config/BKANT.TTF"))
    dT:setCharacterPropSize(0.02)
    dT:setToCenterAlign()
   
    local lH = self.listHolder
    lH:setPropPosition(0, 0.5)
    lH:setPropSize(0, 0.3)
   
    local list = self.list
    list:setPropBetweenPad(0.02)
    list:setPropPosition(0.3, 0.5)
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
    button:setPropSize(0.7, 0.03)
    button:makeChildrenUnresizable()
    button:makeChildrenPenetrable()
    button:makeChildrenUncolorable()
    button:setPropOrigin(0.1, 0.0)
   
   
    local text = button:addText()
    text:setFont(GUI.getFont("Assets/Dialog/config/BKANT.TTF"))
    text:setCharacterPropSize(0.03)
    text:setPropPosition(0.0, 0.5)
    text:setColor(r, g, b, a)
    text:setString(caption)
   
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
