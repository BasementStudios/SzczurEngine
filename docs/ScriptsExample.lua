
-- Odwołania do obiektów (używane do wyzwalaczy i zdarzeń)
objGateGuard = Map.findCharacter("GateGuard")

-- Zmienne zdarzeń (są one przechowywane przy zapisie gry)
--[[
	Zmienne
		Typy: BooleanInteger Floating String
		Nazw np. `varRequiredGold` można używać oczywiście bez przedrostka `var`. Przy `Switch.<typ>(scope, name, [defval])` mamy `scope` - określający zasięg tej zmiennej;
			- jeśli jest to obiekt w grze, np. postać - wartość ta jest przypisywana do jego istancji (więc może istnieć nawet kilka takich na mapie).
			- jeśli jest to mapa `(Map, "name")` - wartość ta jest przypisana na daną mapę (w każdej mapie może być inna), więć trzeba odwołać się do danej mapy (można także do innych niż obecna: `MapManager.findMap("InnaMapa")`).
			- jeśli jest to ciąg znaków - jest dostępna tak na całą grę poprzez ciąg znaków.
		Kolejnym argumentem jest `name` - po prostu nazwa, identyfikator zmiennej.
		Ostatnią nieobowązkową jest `defval` - wartość domyślna. Jeśli nie podane, są to wartości: zerowe, fałszywe czy też puste.
	
	Dynamiczne teksty
		Do dynamicznego tekstu, w dialogach czy dzienniku, można użyć `$(name)` lub zamiast tekstu użyć funkcji zwracającej tekst. Gdzieś niżej przykład.
	
	Uwaga:
		Przedrostek `var` jest użyteczny do zaznaczenia w rysie zdarzenia, że to zmienna, ale przy dynamicznym tekście i `$(name)` używamy nazwy np. `RequiredGold`, a nie aliasu `varRequiredGold`.
]]--
varFirstMeeting	  	= Switch.Boolean(objGateGuard, "FirstMeeting", true)
varRequiredGold   	= Switch.Integer(objGateGuard, "RequiredGold", 100)
varPayed			= Switch.Boolean(objGateGuard, "Payed", false)
varHasPermit      	= Switch.Boolean("GateGuard", "HasPermit")

-- Lista wyzwalaczy
triggers = {
	--[[
		TriggerOnPressed(key)
			Dodaje wyzwalacz sprawdzany przy naciśnięciu przycisku - jednokrotnie.
		TriggerOnReleaseed(key)
			Dodaje wyzwalacz sprawdzany przy puszczeniu przycisku - jednokrotnie.
		Notka:
			Jeśli chcemy sprawdzać ciągle przyciśnięcie, robimy to przez np. `Input.isPressed(Input.MoveUp)` (można też konkretne przyciski, ale zachowajmy choć trochę konfigurowalności).
	]]-
	-- tutaj brak przykładu, teoretycznie nie bardzo powinno się tak robić /ofc imo/, ale widzę zastosowanie np. w jakiś zagadkach, że gracz podchodzi pod coś i ma klikać np. Atak + Strzałki ruchu żeby np. przesunąć kamień...
	
	--[[
		TriggerOnAction(Target)
			Dodaje wyzwalacz sprawdzany przy akcji, np. `Action.Dialog` albo np. `Action.Attack`.
			Uwaga! Dostępny jest wtedy `Target`.
	]]--
	[TriggerOnAction()] = function(Target) 
		-- tu przy każdorazową próbą wywołania dialogu
		if Target == objGateGuard
			invokeEvent("GateGuard/Dialog")
		end
	end,
	
	--[[
		TriggerEvery(ms)
			Dodaje wyzwalacz sprawdzany/wywoływany co dany czas w milisekundach.
	]]--
	[TriggerEvery(1337)] = function() 
		-- co minimalnie 1337 milisekund
		local choose = math.random();
		if(choose -= 0.33 < 0)
			invokeEvent("GateGuard/GoAround")
		elseif(choose -= 0.33 < 0)
			invokeEvent("GateGuard/QuiteSinging")
		else
			-- Można też na przyklad nic xd
		end
	end,
	
	--[[
		Trigger
			Sprawdzany co aktualizacje stanu silnika (czyli jak najczęściej :P).
	]]--
	-- też brak, ale może się jakoś przydać; the same as TriggerEvery(0)
}

-- Lista wydarzeń
events = {
	--[[
		Eventy dla "Gate Guard"
			Każde zdarzenie jest wywoływany przez wyzwalacz(e).
		
		Nazewnictwo:
			Nazwy są unikatowe na mapę (inaczej się pokryją), fajnie by było, gdyby były także unikatowe na mapę.
			Do nazw możemy użyć pseudościeżek - np. `GateGuard/Dialog` 
	]]--
	[Event("GateGuard/Dialog")] = function() 
		-- Standardowa odzywka
		if(varPayed)
			Dialog.run([
				"Guard03", 	"Strażnik bramy", 
					"Eee? Jeszcze czegoś chcesz?"
			])
			
			-- Wyjście z warstwy kodu zdarzenia
			return
		end
		
		-- Dodatkowy kawałek dialogu przy pierwszy spotkaniu
		if(varFirstMeeting)
			-- Ustawienie zmiennej przechowującej informacje, czy pierwsze spotkanie się już odbyłyło.
			varFirstMeeting = false
			--[[
				Dialog.run([texts], [options])
					Dialogi składają się z tekstu - wypowiadanych przez konkretne postacie - i opcji do wyboru przez gracza, do których jest przypisany dalszy "kierunek" działania.
					
					Dla tekstu dane przekazujemy w formacie list listy ciągów znaków; każde kolejne pola opisują kawałek dialogu do odegrania:
					np.
					- "Guard03" oznacza ID rodzaju postaci w plikach gry, po czym gra wie jak ona wygląda w grze (przy czym na mapie może być więcej niż jeden raz użyty dany typ).
					- "Strażnik bramy" oznacza podpis w oknie dialogu dla postaci. Może być użyteczny do np. tymczasowego "ukrycia" prawdziweej nazwy postaci np. napisem "Tajemnicza osoba".
					- "..." - właściwa kwestia do wyświetlenia. Na jej podstawie będzie w pewien sposób określany plik do odtworzenia dla dubbingu. &* ;)
					
					Można też dodać tutaj opcje do dialogu, w formacie listy: `"tekst", "EventDo/Wykonania"` czy dla krótszych może być wygodnie bez robienia jeszcze jednego podzdarzenia: `"tekst", function() ... end` (ps: przykład gdzieś niżej jest ;) )
			]]--
			Dialog.run([
				"Guard03", 	"Strażnik bramy", 
					"Przejście kosztuje cwaniaczku. 100 sztuk złota za przepustkę, albo wypad!",
				"Cedmin",	"Cedmin", 			
					"Widzę, że miasto się ceni.",
				"Guard03",	"Strażnik bramy", 	
					"Jeżeli zaraz stąd nie odejdziesz włóczęgo, zobaczysz również pokaz glenarmskiego fechtunku."
			])
			--[[
				Journal.newQuest(name, [map], [location])
					Dziennik ma zawierać także informacje o zadaniach. Dodajemy nowe zadanie i od teraz możemy w danym skrypcie odwoływać się do niego.
				
				Journal.appendText([name], texts)
					Postęp zadań powinien być zaznaczany w dzienniku kolejnymi wpisami - do dodania ich można użyć tej funkcji. Jeśli podany jest tylko jeden argument, uznawany jest za treść - teksty - o które ma być zaaktualizowany ostatnio wspomniane (np. przy tworzeniu) zadanie.
			]]--
			Journal.newQuest("Enter to Glenarm", "Glenarm")
			Journal.appendText([
				"Dotarłem do bram Glenarm, lecz napotkałem problem...", 
				"", 
				"Strażnik nie chce mnie przepuścić, rząda 100 sztuk złota"
			])
		end
		
		-- Opcja zapłaty, jeśli jest posiadana
		if(Player.hasItem("Gold", varRequiredGold)) 
			--[[
				Dialog.appendOption(text, function_or_event)
					Podobnie jak przy tekstach (appendText, czy to w dialogach czy w dzienniku), pojawienie się niektórych opcji powinno być warunkowe.
					Drugim argumentem może być funkcja do wykonania lub nazwa zdarzenia do wywołania.
				
				Dynamiczne teksty
					Można użyć dynamicznych tekstów poprzez wstawienie zamiast nich funkcji zwracających tekst lub przy użyciu funkcji. Niżej jest przykład.
			]]--
			Dialog.appendOption(function() 
				return "Zapłać " + varRequiredGold + " złota." 
			end, "GateGuard/Pay")
			-- oczywiście można też zrobić 3 warunki i 3 razy pisać opcje dla 100, 150 i 200, ale no, gdzie jest łatwiej XD /i chciałem zaobrazować opcje - więc to nie przymus ;)
		end
		
		-- Opcja wykorzystania przepustki, jeśli posiadana.
		if(varHasPermit) 
			Dialog.appendOption("Pokaż glejt.", "GateGuard/Permit")
		end
		
		-- Opcja groźby, jeśli już nie wypróbowana.
		if(not varRequiredGold == 200)
			Dialog.appendOption("Wystosuj groźbę.", "GateGuard/Threat")
		end
		
		-- Normalne wyjście z dialogu
		Dialog.appendOption("Odejdź", function()
			-- ta funkcja dzieje się po wyborze "Odejdź"
			--[[
				Zmienne lokalne
					Jest możliwość używania zmiennych loklanych. Nie są one zapisywane i powinny być używane w funkcjach (czyli np. w zdarzeniach, czy tez tu). Używamy wtedy `local name`, przypisujemy wartość i działamy dalej.
					
				Uwaga:
					Zasięg zmiennych loklanych jest tylko w odrębie danej funkcji.
			]]--
			local tekst = "Miło się rozmawiało, ale muszę już iść."
			if varRequiredGold >= 150 
				tekst = "Na razie odstąpie z zapłatą"
			end
			Dialog.run([
				"Cedmin",	"Cedmin",	tekst
			])
			-- oczywiście można także zrobić warunek i pisać 2x Dialog.run albo zrobić to na `Dialog.appendText` i dopiero `Dialog.run` - chciałem zaprezentować możliwość jako opcję, nie przymus.
		end)
		
		-- Dodane poprzez `Dialog.appendOption` i `Dialog.appendText` są umieszczane po tych określonych bezpośrednio przy `Dialog.run`.
		-- Uruchomienie dialogu
		Dialog.run([
			"Guard03", 	"Strażnik bramy",	
				"Jeszcze tutaj?! Nie ma pieniędzy, nie ma przejścia! Mam ci to przeliterować?"
		])
	end,
	
	[Event("GateGuard/Pay")] = function()
		if(varRequiredGold == 100)
			-- Dopiero poznanie "prawdziwej" ceny
			varRequiredGold = 150
			Dialog.run([
				"Cedmin",	"Cedmin",			
					"Masz tu złoto. A teraz dawaj papier.",
				"Guard03",	"Strażnik bramy",	
					"Papier mogę dać, ale pieczątka będzie kosztować dodatkowe 50 monet."
			])
			Journal.appendText([
				"", "Strażnik mnie wyrolował. Teraz rząda 150 sztuk złota" 
			])
			invokeEvent("GateGuard/Dialog");
		else
			-- Właściwa zapłata
			Player.removeItem("Gold", varRequiredGold)
			-- Różne odpowiedzi w zależności od przypadku
			if(varRequiredGold == 200)
				-- Przypadek po groźbie
				Dialog.run([
					"Cedmin", 	"Cedmin", 			"Oto 200 monet za papier, a teraz dawaj tą o przepustkę.",
					"Guard03",	"Strażnik bramy",	"Niech ci będzie, proszę bardzo, haha."
				])
			else --if(varRequiredGold == 150) 
				-- Gracz wie o pieczątkach
				Dialog.run([
					"Cedmin", 	"Cedmin", 			"Oto 100 monet za papier, i 50 za pieczątkę, a teraz proszę o przepustkę.",
					"Guard03",	"Strażnik bramy",	"No i dobrze. Oto obiecana przepustka."
				])
			end
			Journal.appendText([
				"", "Zapłaciłem za przepustkę strażnikowi. Wreszcie się udało przejść."
			]);
			Journal.finishQuest("Glenarm", "Enter to Glenarm")
			varPayed = true
		end
	end,
	
	[Event("GateGuard/Threat")] = function()
		-- Próba groźby
		-- potencjalne rozwinięcie eventu -> rezygnacja z groźby lub walka ;)
		Dialog.run([
			"Cedmin",	"Cedmin",	
				"Chyba sobie kpisz! Przepuść mnie natychmiast, albo zmienię twoje życie w koszmar!",
			"Guard03",	"Strażnik bramy",
				"Właśnie sobie przypomniałem, że od wczoraj przepustka bez dwóch pieczęci jest nieważna. Każda 50 monet.",
			"Guard03",	"Strażnik bramy",
				"Ponadto chroni mnie sam król, więc jeżeli coś mi się stanie, Ty i wszyscy Twoi bliscy zawiśniecie na stryczku."
		])
		Journal.appendText([
			"Strażnik staje się irytujący, chce teraz za przejście 200 sztuk złota."
		])
		varRequiredGold = 200
	end,
	
	[Event("GateGuard/Permit")] = function()
		-- Okazanie przepustki
		Dialog.run([
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
		])
	end
}
