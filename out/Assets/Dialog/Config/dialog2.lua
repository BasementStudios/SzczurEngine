local entity = THIS

function entity:onInit() 
	GUI.addFont("Assets/Dialogs/config/BKANT.TTF")
	--GUI.addTexture("Assets/Dialogs/config/background.png")
	GUI.addTexture("Assets/Dialogs/config/button.png")

    self.interface = GUI.addInterface()
	self.dialogText = interface.addTextArea()
	self.listHolder = interface.addWidget()
	self.list = listHolder:addList()
end

function entity:reset()
    --.interface:setPropPadding(0.1, 0.03)
	local dT = self.dialogText
	dT::setPropPosition(0.5, 1)
	dT:setFont(GUI.getFont("Assets/Dialogs/config/BKANT.TTF"))
	dT:setCharacterPropSize(0.06)
	dT:setToCenterAlign()
	
	local lH = self.listHolder
	lH:setPropPosition(0, 0.5)
	lH:setPropSize(0, 0.3)
	
	local list = self.list
	list:setPropBetweenPadding(0.02)
	list:setPropPosition(0, 0.5)
end

function entity:clearButtons()
	self.list:clear()
end

function entity:getButton()
	local button = self.list:addImage()
	button:setTexture(GUI.getTexture("Assets/Dialogs/config/button.png"))
	button:setColor(50, 50, 50, 0)
	button:setPropSize(0.1, 0.02)
	button:makeChildrenPenetrable()
	button:makeChildrenUncolorable()
	
	function button:onHoverIn(self) 
		self:setColor(50, 50, 50, 255)
	end
	function button:onHoverOut(self) 
		self:setColor(50, 50, 50, 0)
	end
	
	local text = button:addText()
	text:setFont(GUI.getFont("Assets/Dialogs/config/BKANT.TTF"))
	text:setCharacterPropSize(0.06)
	text:setPropPosition(0.5, 0.5)
	text:setColor(204, 0, 204)
	return button;
end
	
function entity:onUpdate() 
    if makeReset == true then
        makeReset = false
        self:reset()
    end
end