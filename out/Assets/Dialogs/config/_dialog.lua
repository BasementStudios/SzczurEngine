GUI.addFont("Assets/Dialogs/config/BKANT.TTF")
GUI.addTexture("Assets/Dialogs/config/background.png")

local interface = GUI.addInterface()

local character = interface:addWidget({
    position={350,50}
})

local background = interface:addImage({
    texture=GUI.getTexture("Assets/Dialogs/config/background.png");
    position={1440/2-800/2, 500}
})


local container = background:addWidget({
    position = {220,110+240};
})

local scrollArea = background:addArea({
    position={190,110};
    size = {500,80};
    scrollSpeed = 10;
})

local area = background:addTextArea({
    position={100,100};
    font=GUI.getFont("Assets/Dialogs/config/BKANT.TTF");
    fontSize=20;
    size={40,3};
})

local name = background:addText({
    font=GUI.getFont("Assets/Dialogs/config/BKANT.TTF");
    fontSize=20;
    position = {260,60};
})

Dialog.GUI.set = {
    area = area;
    name = name;
    container = container;
    character = character;
    interface = interface;
    creator = function(i, button)
            button.font = GUI.getFont("Assets/Dialogs/config/BKANT.TTF");
            button.fontSize = 20;
            button.position = {-100, i*30 - 250};
            button.color = {116, 111, 131};
            button.text = "asdasdasdasd"
            button.onHoverIn = function(self)
                    self.color = {196, 194, 208};
                end;
            button.onHoverOut = function(self)
                    self.color = {116, 111, 131};
                end;
        end;
}