
local inosEye = QuestLog.addQuest("InosEye")
inosEye:setNameIndex(0)

local root = inosEye:getRoot()
root:addDescription(0)
root:addTitle(0)

local hagen = root:addStep("Hagen")
hagen:addTitle(1)

local mine = hagen:addStep("Mine")
--mine.onActivate = print("Testowy Kek")
mine:addTitle(2)
mine:addDescription(1)

local note = mine:addStep("Note")
note:addTitle(3)

local getEye = note:addStep("GetEye")
getEye:addDescription(3)
getEye:addTitle(4)

local repair = getEye:addStep("Repair")
repair:addTitle(5)



QuestLog.startQuest("InosEye")
--hagen:nextStep()
--mine:nextStep()
--note:nextStep()
--getEye:nextStep()
--repair:nextStep()
