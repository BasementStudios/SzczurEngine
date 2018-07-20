local dialog = Dialog.load("Assets/Dialogs/A1/SareraRezydencja/dialog")



-- Options 1

local options = dialog:newOptions()
options:addRunner(0, 3)


options.add = {
	majorTarget = 0;
	minorTarget = 0;
	skip = true;
}


-- Options 2

options = dialog:newOptions()
options:addRunner(0, 0)


options.add = {
	majorTarget = 0;
	minorTarget = 1;
	finishing = true;
}

options.add = {
	majorTarget = 0;
	minorTarget = 2;
	finishing = true;
}


dialog:startWith(0, 3)
dialog:play()
