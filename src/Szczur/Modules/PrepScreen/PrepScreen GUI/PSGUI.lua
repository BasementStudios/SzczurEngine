
local PSInterface = GUI.addInterface()

local ppBar = PSInterface:addWidget({
	position = {0, 0};
	size = {1280, 120};
})

local coloredPPBar = PPbar:addWidget({
	position = {0, 0};
	size = {1280, 80};
})

local colorlessPPBar = PPbar:addWidget({
	position = {0, 80};
	size = {1280, 40};
})

local professionBar = PSInterface:addWidget({
	position = {0, 120};
	size = {200, 480};
})

local meleIcon = professionBar:addImage({
	position = {40, 0};
	size = {120, 120};
	
})
local rangeIcon = professionBar:addImage({
	position = {40, 120};
	size = {120, 120};
})
local auraIcon = professionBar:addImage({
	position = {40, 240};
	size = {120, 120};
})
local innerFireIcon = professionBar:addImage({
	position = {40, 360};
	size = {120, 120};
})

local centerWindow = PSInterface:addWidget({
	position = {200, 120};	
	size = { 760, 480 };
})

local optionList = centerWindow:addScroll({
	position = {40, 40};
	size = {240, 400};
	scrollSpeed = 5;
})

local skills = {}

for i = 0, 6 do
	skills[i] = optionList:addWidget({
		position = {0, i * 100};
		size = { 240, 80 };
	})
end