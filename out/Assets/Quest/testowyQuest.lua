
local nemo = QuestLog.addQuest("Nemo")
nemo:setNameIndex(13)

local root = nemo:getRoot()
root:addDescription(23)

local sharkChoice = root:addStep("SharkChoice")
sharkChoice:addTitle(76)

local kill = sharkChoice:addStep("Kill")
kill:addTitle(34)
kill:addDescription(11)
local req = kill:getReqs()
req:addCounter("kek", 1);
req:advanceCounter("kek", 1)
kill.onActivate = function () print("Juhu dziala atakujemy", req:getFromCounter("kek")) end

--addSubnode

local findSth = kill:addStep("FindSth")
findSth:addTitle(26)
findSth:addDescription(90)
findSth.onBlock = function () print("Nie atakujemy, po co") end


local goAround = sharkChoice:addStep("GoAround")
goAround:addTitle(31)
goAround:addDescription(69)

QuestLog.startQuest("Nemo")
sharkChoice:nextStep("Kill")
kill:nextStep()
