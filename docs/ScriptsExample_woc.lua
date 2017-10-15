
-- Odwołania do obiektów (używane do wyzwalaczy i zdarzeń)
objGateGuard = Map.findCharacter("GateGuard")

-- Odwołania do typów obiektów
typeGuard03 = ObjectTypes.

-- Zmienne zdarzeń (są one przechowywane przy zapisie gry)
varFirstMeeting	  	= Switch.Boolean(objGateGuard, "FirstMeeting", true)
varRequiredGold   	= Switch.Integer(objGateGuard, "RequiredGold", 100)
varPayed			= Switch.Boolean(objGateGuard, "Payed", false)
varHasPermit      	= Switch.Boolean("GateGuard", "HasPermit")

-- Lista wyzwalaczy
triggers = {
	[TriggerOnAction()] = function(Target) 
		if (Target == objGateGuard) then
			invokeEvent("GateGuard/Dialog")
		end
	end,
	[TriggerEvery(1337)] = function() 
		local choose = math.random();
		if (choose -= 0.33 < 0) then
			invokeEvent("GateGuard/GoAround")
		elseif (choose -= 0.33 < 0) then
			invokeEvent("GateGuard/QuiteSinging")
		end
	end
}

-- Lista wydarzeń
events = {
	[Event("GateGuard/Dialog")] = function() 
		-- Standardowa odzywka
		if (varPayed) then
			Dialog.run({
				"Guard03", 	"Strażnik bramy", 
					"Eee? Jeszcze czegoś chcesz?"
			})
			return
		end
		
		-- Dodatkowy kawałek dialogu przy pierwszy spotkaniu
		if (varFirstMeeting) then
			varFirstMeeting = false
			Dialog.run({
				"Guard03", 	"Strażnik bramy", 
					"Przejście kosztuje cwaniaczku. 100 sztuk złota za przepustkę, albo wypad!",
				"Cedmin",	"Cedmin", 			
					"Widzę, że miasto się ceni.",
				"Guard03",	"Strażnik bramy", 	
					"Jeżeli zaraz stąd nie odejdziesz włóczęgo, zobaczysz również pokaz glenarmskiego fechtunku."
			})
			Journal.newQuest("Enter to Glenarm", "Glenarm")
			Journal.appendText({
				"Dotarłem do bram Glenarm, lecz napotkałem problem... Strażnik nie chce mnie przepuścić, rząda 100 sztuk złota. "
			})
		end
		
		-- Opcja zapłaty, jeśli jest posiadana
		if (Player.hasItem("Gold", varRequiredGold)) then
			Dialog.appendOption(function() return "Zapłać " + varRequiredGold + " złota." end, "GateGuard/Pay")
		end
		
		-- Opcja wykorzystania przepustki, jeśli posiadana.
		if (varHasPermit) then
			Dialog.appendOption("Pokaż glejt.", "GateGuard/Permit")
		end
		
		-- Opcja groźby, jeśli już nie wypróbowana.
		if (not varRequiredGold == 200) then
			Dialog.appendOption("Wystosuj groźbę.", "GateGuard/Threat")
		end
		
		-- Normalne wyjście z dialogu
		Dialog.appendOption("Odejdź", function()
			local tekst = "Miło się rozmawiało, ale muszę już iść."
			if (varRequiredGold >= 150) then
				tekst = "Na razie odstąpie z zapłatą"
			end
			Dialog.run({
				"Cedmin",	"Cedmin",	tekst
			})
		end)
		
		-- Uruchomienie dialogu
		Dialog.run({
			"Guard03", 	"Strażnik bramy",	
				"Jeszcze tutaj?! Nie ma pieniędzy, nie ma przejścia! Mam ci to przeliterować?"
		})
	end,
	
	[Event("GateGuard/Pay")] = function()
		if (varRequiredGold == 100) then
			-- Dopiero poznanie "prawdziwej" ceny
			varRequiredGold = 150
			Dialog.run({
				"Cedmin",	"Cedmin",			
					"Masz tu złoto. A teraz dawaj papier.",
				"Guard03",	"Strażnik bramy",	
					"Papier mogę dać, ale pieczątka będzie kosztować dodatkowe 50 monet."
			})
			Journal.appendText({
				"Strażnik mnie wyrolował. Teraz rząda 150 sztuk złota. " 
			})
			invokeEvent("GateGuard/Dialog");
		else
			-- Właściwa zapłata
			Player.removeItem("Gold", varRequiredGold)
			if (varRequiredGold == 200) then
				Dialog.run({
					"Cedmin", 	"Cedmin", 			"Oto 200 monet za papier, a teraz dawaj tą o przepustkę.",
					"Guard03",	"Strażnik bramy",	"Niech ci będzie, proszę bardzo, haha."
				})
			else 
				Dialog.run({
					"Cedmin", 	"Cedmin", 			"Oto 100 monet za papier, i 50 za pieczątkę, a teraz proszę o przepustkę.",
					"Guard03",	"Strażnik bramy",	"No i dobrze. Oto obiecana przepustka."
				})
			end
			Journal.appendText({
				"Zapłaciłem za przepustkę strażnikowi. Wreszcie się udało przejść. "
			});
			Journal.finishQuest("Glenarm", "Enter to Glenarm")
			varPayed = true
		end
	end,
	
	[Event("GateGuard/Threat")] = function()
		-- Próba groźby
		Dialog.run({
			"Cedmin",	"Cedmin",	
				"Chyba sobie kpisz! Przepuść mnie natychmiast, albo zmienię twoje życie w koszmar!",
			"Guard03",	"Strażnik bramy",
				"Właśnie sobie przypomniałem, że od wczoraj przepustka bez dwóch pieczęci jest nieważna. Każda 50 monet.",
			"Guard03",	"Strażnik bramy",
				"Ponadto chroni mnie sam król, więc jeżeli coś mi się stanie, Ty i wszyscy Twoi bliscy zawiśniecie na stryczku."
		})
		Journal.appendText({
			"Strażnik staje się irytujący, chce teraz za przejście 200 sztuk złota. "
		})
		varRequiredGold = 200
	end,
	
	[Event("GateGuard/Permit")] = function()
		-- Okazanie przepustki
		Dialog.run({
			"Guard03",	"Strażnik bramy",	
				"Gdzie leziesz, jeszcze jeden krok, a...",
			"Cedmin",	"Cedmin",			
				"Wstrzymaj wodze 'rycerzu' jeżeli chcesz jutro tutaj jeszcze stać. [pokazuje dokument]",
			"Guard03",	"Strażnik bramy",	
				"SKĄD TO MASZ?! Toż to przecież przepustka z podpisem samego...",
			"Cedmin",	"Cedmin",			
				"Tak, tak. Widzę, że umiesz czytać. A teraz przesuń się bo nie mogę przejść.",
			"Guard03",	"Strażnik bramy",	
				"Tak jest sir!"
		})
	end
}
