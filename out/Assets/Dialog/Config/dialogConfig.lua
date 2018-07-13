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
	list:setPropPosition(0, 0.5)
end

function dialogGUI:clearButtons()
	self.list:clear()
end

function dialogGUI:getButton(r, g, b, a, iconId)
	local button = self.list:addImage()
	button:setTexture(GUI.getTexture("Assets/Dialog/config/button.png"))
	button:setColor(50, 50, 50, 0)
	button:setPropSize(0.1, 0.02)
	button:makeChildrenPenetrable()
	button:makeChildrenUncolorable()

	function button:onHoverIn() 

	end
	function button:onHoverOut() 
	end
	
	local text = button:addText()
	text:setFont(GUI.getFont("Assets/Dialog/config/BKANT.TTF"))
	text:setCharacterPropSize(0.03)
	text:setPropPosition(0.5, 0.5)
	text:setColor(r, g, b, a)

	function text:onHoverIn()
		self:setColor(
			((r+50 < 255) and r+50 or 255),
			((g+50 < 255) and g+50 or 255),
			((b+50 < 255) and b+50 or 255),
			255
		)
	end
	function text:onHoverOut() 
		self:setColor(r, g, b, a)
	end
	return text;
end