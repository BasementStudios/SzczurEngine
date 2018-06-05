local o = World.getScene():addEntity("path", "Test")

function o:printMeSomeTimes(tabName)

	for key, value in pairs(self[tabName]) do
		print(key..": ("..type(value)..") "..tostring(value))
		if(type(value) == "function") then
			value()
		elseif(type(value) == "userdata") then
			print("Entity name: "..value:getName())
		end
	end

end

o.myTab = {
	myInt = 10;
	myReal = 3.14159;	
	myString = "Hello world!";
	myPointer = World.getScene():getEntity("Player");
	myFunction = function() print("I'm function!") end;
}

o.myTab2 = {
	myInt = 10*2;
	myReal = 3.14159*2.0;
	myString = "Bye bye!";
	myPointer = World.getScene():getEntity("Camera");
	myFunction = function() print("I'm function too!") end;
}

print("===================================\n")

print("-------- myTab --------")
o:printMeSomeTimes("myTab")

print("\n-------- myTab2 --------")
o:printMeSomeTimes("myTab2")