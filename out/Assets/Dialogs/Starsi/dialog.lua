local dialog = Dialog.load("Assets/Dialogs/Starsi/dialog")



-- Options 1

local options = dialog:newOptions()
options:addRunner(0, 0)


options.add = {
	majorTarget = 1;
	minorTarget = 0;
	iconId = 2;
}

options.add = {
	majorTarget = 3;
	minorTarget = 0;

	--b:a(304): Karion pozostaje w miejscu.
	action = function()
		
		end;
	--e:a(304)

}


-- Options 2

options = dialog:newOptions()
options:addRunner(1, 0)
options:addRunner(1, 1)
options:addRunner(1, 2)
options:addRunner(1, 3)
options:addRunner(2, 0)
options:addRunner(1, 5)


options.add = {
	majorTarget = 1;
	minorTarget = 1;
	iconId = 2;
}

options.add = {
	majorTarget = 1;
	minorTarget = 2;
	iconId = 3;
}

options.add = {
	majorTarget = 1;
	minorTarget = 3;
	iconId = 4;
}

options.add = {
	majorTarget = 1;
	minorTarget = 4;
	iconId = 5;
}

options.add = {
	majorTarget = 1;
	minorTarget = 5;
	iconId = 9;

	--b:c(506): Jeśli we wcześniejszej rozmowie z Kultystą Karion dowie się o istnieniu świętej księgi może o nią zapytać Starszych.
	condition = function()
		
		end;
	--e:c(506)


	--b:a(506): Karion dostaje ksiege od Starszego
	action = function()
		
		end;
	--e:a(506)

}


-- Options 3

options = dialog:newOptions()
options:addRunner(1, 4)


options.add = {
	majorTarget = 2;
	minorTarget = 1;
}

options.add = {
	majorTarget = 2;
	minorTarget = 0;
}


-- Options 4

options = dialog:newOptions()
options:addRunner(2, 1)


options.add = {
	majorTarget = 2;
	minorTarget = 2;
}

options.add = {
	majorTarget = 2;
	minorTarget = 5;

	--b:a(1304): po dialogu Karion wychodzi ze świątyni.
	action = function()
		
		end;
	--e:a(1304)

	finishing = true;
}


-- Options 5

options = dialog:newOptions()
options:addRunner(2, 2)


options.add = {
	majorTarget = 2;
	minorTarget = 3;

	--b:a(1602): Po wyjsciu Karion zaczyna obficie krwawić wszelkimi otworami i umiera.
	action = function()
		
		end;
	--e:a(1602)

	finishing = true;
}

options.add = {
	majorTarget = 2;
	minorTarget = 4;

	--b:a(1603): po dialogu Karion wychodzi ze świątyni.
	action = function()
		
		end;
	--e:a(1603)

	finishing = true;
}


-- Options 6

options = dialog:newOptions()
options:addRunner(3, 0)


options.add = {
	majorTarget = 3;
	minorTarget = 2;

	--b:a(2202): Karion opuszcza świątynię z ogromnym bólem głowy
	action = function()
		
		end;
	--e:a(2202)

	finishing = true;
}

options.add = {
	majorTarget = 3;
	minorTarget = 1;

	--b:a(2203): Po wyjsciu Karion zaczyna obficie krwawić wszelkimi otworami i umiera.
	action = function()
		
		end;
	--e:a(2203)

	finishing = true;
}


dialog:startWith(0, 0)
dialog:play()
