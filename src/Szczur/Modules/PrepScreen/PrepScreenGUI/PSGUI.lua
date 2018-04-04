
PSInterface = GUI.addInterface()

local ppBar = PSInterface:addImage({
	position = {0, 0};
	GUI.addTexture("assets/PrepScreen/ppBar.png");
	texture = GUI.getTexture("assets/PrepScreen/ppBar.png");
})

local coloredPPBar = ppBar:addWidget({
	position = {0, 0};
})

colorlessPPBar = ppBar:addWidget({
	position = {0, 80};
})



professionBar = PSInterface:addImage({
	position = {0, 120};
	GUI.addTexture("assets/PrepScreen/professionBar.png");	
	texture = GUI.getTexture("assets/PrepScreen/professionBar.png");
	--size = {200, 480};
})

meleIcon = professionBar:addImage({
	position = {40, 0};
	--size = {120, 120};
	GUI.addTexture("assets/PrepScreen/meleIcon.png");
	texture = GUI.getTexture("assets/PrepScreen/meleIcon.png");
})
-- --[[
local rangeIcon = professionBar:addImage({
	position = {40, 120};
	--size = {120, 120};
	GUI.addTexture("assets/PrepScreen/rangeIcon.png");	
	texture = GUI.getTexture("assets/PrepScreen/rangeIcon.png");	
})

local auraIcon = professionBar:addImage({
	position = {40, 240};
	--size = {120, 120};
	GUI.addTexture("assets/PrepScreen/auraIcon.png");
	texture = GUI.getTexture("assets/PrepScreen/auraIcon.png");
})

local innerFireIcon = professionBar:addImage({
	position = {40, 360};
	--size = {120, 120};
	GUI.addTexture("assets/PrepScreen/inerFireIcon.png");
	texture = GUI.getTexture("assets/PrepScreen/inerFireIcon.png");
})


local centerWindow = PSInterface:addImage({
	position = {200, 120};
	GUI.addTexture("assets/PrepScreen/centerWindow.png");
	texture = GUI.getTexture("assets/PrepScreen/centerWindow.png");	
	--size = { 760, 480 };
})


local optionList = centerWindow:addArea({
	position = {40, 40};
	size = {240, 400};
	scrollSpeed = 15;
})

local skills = {}

GUI.addTexture("assets/PrepScreen/skillBar.png");

for i = 0, 5 do
	skills[i] = optionList:addImage({
		position = {0, i * 70};
		--size = { 240, 80 };
		texture = GUI.getTexture("assets/PrepScreen/skillBar.png");	
	})
end
-- --]]
