GUI.addFont("dialogs/config/consolab.ttf")
--GUI.addFont("data/Cinzel-Black.otf")
GUI.addFont("dialogs/config/Cinzel-Regular.otf")
GUI.addTexture("dialogs/config/dial_back.png")
GUI.addTexture("dialogs/config/dial_front.png")

local interface = GUI.addInterface()

local character = interface:addWidget({
    position={460,50}
})

local background = interface:addImage({
    texture=GUI.getTexture("dialogs/config/dial_back.png");
    position={1440/2-800/2, 200}
})


local container = background:addWidget({
    position = {220,110+240};
})

local scrollArea = background:addArea({
    position={190,110+240};
    size = {500,80};
    scrollSpeed = 10;
})

local area = scrollArea:addTextArea({
    position = {0,0};
    font=GUI.getFont("dialogs/config/Cinzel-Regular.otf");
    fontSize=20;
    size={40,3};
})

local front = background:addImage({
    texture=GUI.getTexture("dialogs/config/dial_front.png");
    position={0,0}
})

local name = background:addText({
    position = {260,60+230};
    font=GUI.getFont("dialogs/config/Cinzel-Regular.otf");
    fontSize=20
})

Dialog.GUI.set = {
    area = area;
    name = name;
    container = container;
    character = character;
    interface = interface;
    creator = function(i, button)
            button.font = GUI.getFont("dialogs/config/Cinzel-Regular.otf");
            button.fontSize = 20;
            button.position = {0, i*20};
            button.color = {250,130,230};
            button.text = "asdasdasdasd"
            button.onHoverIn = function(self)
                    self.color = {250,200,230};
                end;
            button.onHoverOut = function(self)
                    self.color = {250,130,230};
                end;
        end;
}