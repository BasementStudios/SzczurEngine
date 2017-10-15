
Struktura plików gry
====================





Foldery
-------

### Główne podfoldery

```
Scripts/			; Inne skrypty.
Actions/			; Współdzielone, typowe zdarzenia, w tym akcje ciągłe.
Objects/			; Typy obiektów świata gry, np. dźwignia, drzewo - zestawy grafik, animacji.
Actors/				; Typy aktorów - bardziej stworzenia, np. postacie, potwory - zestawy -||-.
Maps/				; Mapy, w tym listy obiektów, wyzwalaczy i zdarzeń map, konfiguracja.
Sounds/				; Jednorazowe dźwięki, dość uniwersalne - odtwarzane z naciskiem na buforowanie.
Musics/				; Powtarzające się lub ciągłe muzyka, podkład - odtwarzane strumieniowo.
Dubbing/			; Dubbing dialogów - osobno, bo być tak powinno. ;)
Fonts/				; Niestandardowe czcionki, proste ikonki.
Interface/			; Elementy związane z UI, interfejsem, w tym konfiguracja.
Cutscenes/			; Scenki między lokacjami, zdarzeniami w grze itd.
Items/				; Przedmioty, grafiki, informacje, statystyki.
Shaders/			; Skrypty, maski, grafiki shaderów.
Other/				; Inne rzeczy, np. logo.
```



### Dalsze podfoldery

W folderach niemających zdefiniowanej dalszej struktury - np. `Sounds`, ale też `Maps/<FolderMapy>` - można używać kolejnych folderów, np. `Sounds/Magic`, czy też `Maps/<FolderMapy>/back`.





Pliki
-----

### Nazwenictwo plików

* W strukturze plików poniżej są zawarte także rozszerzenia - to tylko dla rozróżnienia typu danych (czy dźwiękowy, graficzny czy czystej informacji). 

* Dany plik/zasób może być przekierowaniem do innego zasobu - wtedy jako rozszerzenie powinnien użyć `lnk` oraz w treści powinien mieć jedynie nazwę/id/ścieżkę zasobu na jaki wskazuje.

* Standardowo obowiązkowe elementy są zaznaczone z wykrzyknikiem (`!`) z początku komentarza struktury. Elementy z pytajnikiem (`?`) - bardzo niepewne.

* Oprócz plików ze struktury mogą istnieć także inne pliki - np. przy grafikach powinien być plik projektu do utworzenia danego zasobu. Nie będą one używane przez silnik, ale warto uwspólnić takie dane, np. do późniejszej łatwej edycji.

* Inne informacje, np. przeznaczenie pliku, są w formie komentarza (`; `) na strukturze poniżej.

* Format plików konfiguracyjnych, list obiektów to JSON, struktura kolejnych została opisana także tutaj.

* Format plików skryptowych, wyzwalaczy, zdarzeń, akcji to LUA, zostały one opisane w `Scripts.md`.



### Struktura plików

Folder - z `/`, plik bez. Dla opisu informacji zawartych w plikach konfiguracyjnych użyto `-` jako wskazanie na pole lub podelement. Od `; ` - komentarz.

```
Objects/
	LeverBehindInARoom/
		; Zbiory animacji/grafik?
		; 	Każdy stan obiektu/postaci może mieć swój zbiór animacji (kierunki).
		;	Kolejne cyfry to rózne kierunki animacji: 
		;	0 - obiekt, postać patrzy w stronę kamery
		;	Pozostałe w zależności od maksymalne - kolejne części obrotu zgodnie
		;	z kierunkiem zegara. Np. gdy maks. jest 16 to 7 to patrzący w przeciwnym 
		; 	kierunku, a np. 15 (ostatni) jest prawie do kamery, ale lekko w prawo.
		; Notka: jeśli jest tylko 0 - można odrazu użyć np. `standing`, bez folderu.
		
		; Przykład dla animacji poklatkowej (Sprite)
		moving/					; Zbiór animacje/grafik dla ruchu.
			0.json				; ! Plik konfiguracyjny dla animacji,
				- frames		; ! Liczba klatek dla animacji
				- timestamps []	; Różnice w czasie dla kolejnych klatek, lub jedna wartość
								; 	jak wspólna różnica w czasie dla kolejnych klatek.
				- mode			; ? Tryb animowania: 
								;	0, 'static' lub brak - klatka odrazu jest nakładana, 
								; 	1, 'linear' - przenikanie, liniowe zmienianie przeźroczystości,
								;	2, 'ease-all' - przenikanie stępione, wolny start, koniec,
								;	3, 'ease-in' - przenikanie stępione z początku, wolny start,
								;	4, 'ease-out' - przenikanie stępione z końca, wolny koniec,
								;   lub tablica - wartości przeźroczystości dla przejścia Beziera.
				- lx			; ? Długości w odpowiednich osiach - wymusza odpowiednie rozmiary.
				- ly			; ? Przy wymuszeniu jest tworzony nowy plik i dopiero ładowany,
								; 	stary zostaje przemianowany z suffixem do nazwy: `.invaild`.
				- ox			; Punkt/pozycja pochodzenia - Wskzany punkt jest miejscem względem 
				- oy			;   animacji, od którego jest uznawany środek obiektu dla dla gry.
								;	Jesli nie określone oXYZ - używane są środkowe z wymiarów  elementu.
				- cx			; Szerokość kolizji. Jeśli tryb koła - promień koła od środka.
				- cy			; Wysokość kolizji. Jeśli zero lub nieokreślony - tryb koła.
			0.png				; ! Właściwe grafiki, klatki itd.
		
		; Przykład dla animacji szkieletowej (Armature)
		standing/
			0.json				; ! Konfiguracja animacji szkieletowej w formacie DrangonBones/Spine. @todo
								; 	W tym także muszą być `o-xyz` oraz `c-xyz`, jak w animacji poklatkowej.
			0.png				; ! Elementy grafik do animacji szkieletowej.
		
		; Ogólna, wspólna konfiguracja obiektu
		config.json
			- ox, oy, cx, cy	; Zminne od pochodzenia, kolizji, długości itd. moga być opisane wspólnie.

Actors/							; Każdy `Actor` to także `Object`, więc elementy dziedziczone.
	Guard03/
		portraits/				; Portrety/wizerunki - używane przy dialogach.
			%state%.png			; Jako %state% może być wstawiony stan postaci 
								; (np. wściekły `angry`, płaczący `crying` itd.).
			%state%.json		; Konfiguracja animacji (Sprite lub Armature) lub grafiki.
		config.json				; ! Informacje o postaci.
			- name				; ! Nazwa własna, wyświetlana.
			; informacje zależne od mechanik - @todo do ustalenia itd.
			- tags				; Lista tagów postaci, np. `good,old,mage`.
			- description		; Opis postaci w dzienniku.
			- health
			- mana
			- attack
			; i inne informacje...

Maps/
	South Castle Tower Floor 1/
		back.png				; ! Podstawowe tło mapy (ten i kolejne też mogą być animacją,
								;	wystarczy dodać konfiguracje w `json` tutaj jako przykłady bez animacji).
		center.png				; Warstwa środkowa, np. do prostego oświetlania i cieni.
		front.png				; Warstwa przykrywająca obiekty, np. dachy, zasłonięte elementy budynku.
		cover.png				; Warstwa ostatnia, ograniczająca widoczność np. mgła, przesłony, finalne cienie.
		objects.json			; Definiuje pozycje, typ obiektu/aktora na mapie. 
								;	Może zawierać inne informacje do nadpisania (np. HP).
			- objects [{}]
				- id			; Nazwa/ID, używane do połączenia danego obiektu z wyzwalaczami i zdarzeniami.
				- type			; ! Nazwa typu odpowiedniego obiektu, np. `Guard03`.
				- x				; ! Początkowa pozycja
				- y				; !	Notka: Kolejnośc na liście jest kolejnością rysowania przez silnik,
				- z				; !		jeśli odległość od kamery jest identyczna, a pozycja się pokrywa.
				- angle			; Początkowy obrót obiektu (i jego grafik/animacji).
				- state			; Początkowy stan animacji
				- frame			; Początkowa klatka, stan animacji.
				- action		; Przypisana akcja, zachowanie.
				; i inne niestandardowe informacje, np. dot. postaci (np. jakieś statystyki).
		scripts.lua				; Definiuje wyzwalacze i wydarzenia na mapie.
			@info Omówiony w innym pliku (`Scripts.md`)
		config.json				; ! Konfiguracja mapy
			- name				; ! Nazwa wyświetlana
			; i inne informacje, np.
			- music.list []		; Lista nazw standardowej muzyki w tle.
			- music.random		; Flaga, czy losowe utwory (standardowo tak).

Sounds/							; Jednorazowe dźwięki, dość uniwersalne - odtwarzane z naciskiem na buforowanie.
	Some Sound Name.mp3
	
Musics/							; Powtarzające się lub ciągłe muzyka, podkład - odtwarzane strumieniowo.
	Example Background.mp3
	
Dubbing/						; Dubbing dialogów - osobno, bo być tak powinno. ;) PS: bez "dubbingu" obiektów, np. drzew :P
	Rock "klapklapklap".mp3
	
Fonts/
	Consolas.ftf

Interface/
	IngameMenu/
		config.lua				; ! Plik opisujący rozmieszczenie i typy elementów interfejsu oraz wywołane przez nie skrypty.
		buttons/				; Grafiki, animacje itd. także damy tutaj, niekoniecznie w folderach.
			exit.f6.r2.png		; Mogą być animowane.
			exit_pressed.png	; Nazwy oczywiście przykładowe.

Items/
	Stick sword/
		graphic.png				; ! Podgląd w menu (może być animacją z konfiguracją `json` itd).
		config.json				; ! Konfiguracja przedmiotu
			- name				; ! Nazwa wyświetlana
			; informacje zależne od mechanik... @todo
			- tags				; Lista tagów przedmiotu, np. `warrior,sword,short,wooden`.
			- description		; Opis przedmiotu w menu. 
			- durability		; Liczba pozostałych użyć. 
			- health			; Odnawiane punkty życia.
			- mana				; Odnawiane punkty many.
			- attack			; Zadawane obrażenia.
			; i inne informacje...

Other/
	Logo: SzczurEngine.png
	Logo: Hajsownicy.f3.r1.png  ; Animacje także tutaj możliwe. 
	Logo: Gimper.png
	HUD.png
```




