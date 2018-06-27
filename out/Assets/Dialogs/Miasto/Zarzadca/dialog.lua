local dialog = Dialog.load("Assets/Dialogs/Miasto/Zarzadca/dialog")


dialog:bindCharacter("Karion", "Assets/Dialogs/Miasto/Zarzadca/Karion.png")
dialog:bindCharacter("Karion ", "Assets/Dialogs/Miasto/Zarzadca/KarionzSarrera.png")
dialog:bindCharacter("Sarera", "Assets/Dialogs/Miasto/Zarzadca/Sarrera.png")
dialog:bindCharacter("Zarządca", "Assets/Dialogs/Miasto/Zarzadca/Zarzadca.png")
dialog:bindCharacter("Zarządca ", "Assets/Dialogs/Miasto/Zarzadca/Zarzadcazzbojami.png")

-- Options 1

local options = dialog:newOptions()
options:addRunner(0, 0)


options.add = {
	majorTarget = 1;
	minorTarget = 0;
}

options.add = {
	majorTarget = 2;
	minorTarget = 0;
}


-- Options 2

options = dialog:newOptions()
options:addRunner(2, 0)
options:addRunner(1, 0)


options.add = {
	majorTarget = 3;
	minorTarget = 0;
	skip = true;
}


-- Options 3

options = dialog:newOptions()
options:addRunner(3, 0)


options.add = {
	majorTarget = 4;
	minorTarget = 0;
	finishing = true;
}

options.add = {
	majorTarget = 5;
	minorTarget = 0;
	finishing = true;
}


dialog:startWith(0, 0)
dialog:play()
