print("ASD")

local dialog = Dialog.load("dialogs/dialog_1/dialog")

-- Options 1

dialog:bindCharacter("Mroczny", "dialogs/dialog_1/cedmin.png")
dialog:bindCharacter("Locha", "dialogs/dialog_1/locha.png")

local options = dialog:newOptions(1)

options.add = {
    target = 2;
}

options.add = {
    target = 3;
    finishing = true;
}

-- Options 2

options = dialog:newOptions(2)

options.add = {
    target = 1;    
}

options.add = {
    target = 3;    
}

-- Options 3

options = dialog:newOptions(3)

options.add = {
    target = 1;
}

options.add = {
    target = 2;
}

dialog:startWith(1);
dialog:play();


