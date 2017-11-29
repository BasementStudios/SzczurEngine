
Visual Studio Code jako IDE do C++ z MinGW, Boost, Lua, JSON i SFML 
===================================================================
`#vscode #ide #c++ #mingw #sfml #gitbash #github #boost #lua #json`



GitBash
-------

1. Do MinGW musimy pobrać `bash`a np. GitBasha ze strony https://git-scm.com/downloads). Poza dostarczeniem podstawowego `bash`a jest to już pewne środowisko (tekstowe) do pracy z GitHubem.

	![img](https://i.imgur.com/bzotw9y.jpg "img")

2. Pobieramy, a podczas instalacji, wszystko zostawiamy jak jest *(Cały czas klikamy **next**)*.





Mingw-w64
---------

### Instalacja

1. Na początku pobieramy MinGW w64 ze strony https://sourceforge.net/projects/mingw-w64/files/ 

    _SzczurEngine_ zakłada używanie _C++17_, więc raczej będzie to wersja 7.X, np. `7.2`. Najłatwiej wybrać architekturę `x86_64` (co umożliwi kompilacje na 32 jak i 64 bitowe maszyny), system wątków `posix` (bardziej popularny i funkcjonalny, nawet działa na Windowsie, [o ironio](https://stackoverflow.com/questions/17242516/mingw-w64-threads-posix-vs-win32)), a system wyjątków `seh` ([tutaj po informacje](https://stackoverflow.com/questions/15670169/what-is-difference-between-sjlj-vs-dwarf-vs-seh)).

2. Wypakowujemy do wybranego katalogu - Ścieżkę wybrać jak dość prostą, najlepiej bez spacji np. `C:\mingw-w64`.



### Konfiguracja

1. W folderze `.../mingw/bin` kopiujemy plik `mingw32-make.exe` jako `make.exe`. 

2. Do zmiennych środowiskowych dodajemy folder `.../mingw/bin`, np. `C:\mingw-w64\mingw\bin`.

3. W konsoli możemy sprawdzić pomyślność instalacji komendą `g++ --version`. 





Biblioteka SFML
---------------

### Pobieranie

1. Z linku https://nightlybuilds.ch/project/show/1/SFML/ pobieramy odpowiednie wersje, najbliższe kompilatorowi:

	- dla 64-bit: `MinGW Builds Posix SEH x64`
	- dla 32-bit: `MinGW Builds Posix Dwarf x32`

	![img](https://i.imgur.com/UNLaRon.jpg "img")

2. Wypakowujemy je. Dobrze jest mieć obie na dysku, w dość sensowym i wspólnym miejscu, np. `E:\Libraries`.



### Informacja

Pliki nagłówkowe są w folderze `include`. Możemy je później dodać do podpowiedzi. 





Boost
-----

### Pobieranie 

1. Pobieramy najnowszą wersję biblioteki ze strony http://www.boost.org/ .

	![img](https://i.imgur.com/EhLWGhW.jpg "img")

2. Wypakowujemy pobrane archiwum w odpowiednie miejsce. Dobrym miejscem jest np. `E:\Libraries`. 
3. Wypakowany folder nazywamy np. `boost_builder` - jest to zestaw, z którego skompilujemy bibliotekę.



### Kompilacja

1. Otwieramy terminal i wchodzimy do folderu `boost_builder`.
2. Tworzymy folder `stage`. 
3. Wchodzimy do `tools\build`.
4. Używamy komendy: 
	```
	bootstrap.bat gcc
	``` 
5. Następnie:
    ```
    b2.exe install toolset=gcc --prefix=..\..\stage
    ```
6. Cofamy się do folderu `boost_builder` i używamy: 
	```
	stage\bin\b2.exe toolset=gcc --build-type=complete stage
	```
7. Czekamy do końca kompilacji... czyli **dłuższą** chwilę ;) 



### Polecane

Chętni mogą sobie uporządkować libkę poprzez: 

1. Stworzenie folderu np. `boost-1.65.1` w sensownym miesjcu.
2. Stworzenie tam folderu `include`, do którego przenosimy folder `boost`,
3. Przeniesienie folderu `stage\lib` i zmienienie jego nazwy na np. `lib_MinGW-7.2.0-32/64`.

Pliki nagłówkowe są w folderze `boost` lub `include`, można je później dodać do systemu podpowiedzi w edytorze.





Skrypty Lua 
-----------

### Pobieranie

1. Wchodzimy na https://www.lua.org/download.html i pobieramy Lua (w projekcie Szczur używamy `lua-5.3.4`).

	![img](https://i.imgur.com/ekjAmfY.jpg "img")

2. Wypakowujemy pobrane archiwum w odpowiednie miejsce, np. `E:\Libraries`. Mamy więc tam folder `lua-5.3.4`.



### Kompilacja

1. Wchodzimy terminalem do tego folderu.
2. Używamy `make mingw` dla kompilacji z MinGW.



### Polecane

Chętni mogą sobie uporządkować libkę poprzez: stworzenie folderu np. `lua-5.3.4` w sensownym miesjcu, stworzenie tam folderu `include`, do którego przenosimy folder `boost_builder\boost`, przeniesienie folderu `boost_builder\stage\lib` i zmienienie jego nazwy na np.  `lib_MinGW-7.2.0-32/64`

Pliki nagłówkowe są w folderze `include`, można je później dodać do systemu podpowiedzi w edytorze.


#### Biblioteka `sol2`

Jest to biblioteka ułatwiająca działanie z silnikiem skryptowym Lua.

Z https://github.com/ThePhD/sol2/releases pobieramy najnowszą wersję bilbioteki (może być sam plik nagłówkowy `.hpp`), wrzucamy do odpowiedniego folderu (np. `E:\Libraries` i np. `Lua-sol2\include`). 

![img](https://i.imgur.com/OYmdfkd.jpg "img")





JSON
----

JSON jest popularnym, prostym i dość wygodnym formatem do przetrzymywania konfiguracji.

### Pobieranie

Możemy go pobrać z https://github.com/nlohmann/json/releases. Pobrany plik nagłówkowy warto wrzucić do odpowiedniego folderu (np. `E:\Libraries\JSON-2.1.1-header-only\include`).

![img](https://i.imgur.com/sHDso2Y.jpg "img")





GitHub
------

### Instalacja

GitHub jest już dostarczony przez zainstalowanego wcześniej GitBasha.



### Pobieranie repozytorium projektu

1. Wybieramy miejsce, w którym ma się znajdować nasz projekt. Naciskamy prawym klawiszem myszy na pustym obszarze i wybieramy opcję `Git Bash Here`. Można też otworzyć Git Basha np. z menu start i ręcznie przejść do odpowiedniego katalogu.
	
	![img](https://i.imgur.com/3ykWZTv.jpg "img")
	
2. W wyświetlonym oknie, wpisujemy
	
	```
	git clone https://github.com/SzczurProject/SzczurEngine.git
	```
	
3. Następnie zatwierdzamy enterem.
	
	![img](https://i.imgur.com/GYPC6Gc.jpg "img")
	
	_W konsoli Git Basha wklejamy zawartość schowka kombinacją klawiszy `Shift` + `Insert` lub prawym na pasek okna i Wklej._
	
Bardzo polecany jest także GitKraken - https://www.gitkraken.com/ - będącym przyjemnym i pomocnym graficznym interfejsem do Gita.

![img](https://i.imgur.com/kTDPuwy.png "img")



### Polecane

Kursy i poradniki dot. Gita: 

* _Poznaj GIT w 15 minut_ (PS: irl 7:43) https://www.youtube.com/watch?v=LEystZRF8fQ
* _How to Use GitHub with GitKraken_ https://www.youtube.com/watch?v=f0y_xCeM1Rk





Visual Studio Code
------------------

### Instalacja

1. Na początku, musimy pobrać Visual Studio Code ze strony https://code.visualstudio.com/download/.

	![img](https://i.imgur.com/LauCfo9.jpg "img")

2. Wybieramy odpowiednią wersję i przeprowadzasz instalację programu.



### Konfiguracja

1. Na początku musimy pobrać rozszerzenie C/C++ od Microsoftu

	![img](https://i.imgur.com/tA9s0pC.jpg "img")

2. Otwierasz jakiś plik (np. `Game.cpp`) i najeżdżając na include kursorem naciskasz żaróweczkę która się pojawi obok. Następnie wybierasz pozycję:
`Add to "includePath": ${workspaceRoot}/include`

	![img](https://i.imgur.com/AOZIYmz.jpg "img")

	(Jeżeli żarówka się nie pojawia, to naciśnij `Ctrl` + `Shift` + `P`, a następnie wpisz `Reload Window`)

	W ten sposób zapewniliśmy sobie sugestie.

3. Ostatnią rzeczą którą musisz zrobić, jest ustawienie Basha jako domyślnego shella
*(terminala, odpalanego kombinacją klawiszy `Ctrl` + `~`)* oraz przypisanie skrótu do kompilacji kodu.
Przechodzimy do ustawień `File/Preferences/Settings` i dodajemy po prawej taką linijkę:
`"terminal.integrated.shell.windows": "..\..\Git\bin\bash.exe(Ścieżka do basha którego pobraliśmy wcześniej)"`

	![img](https://i.imgur.com/l0bjStH.jpg "img")



### Ustawianie kompilacji pod konkretny przycisk

1. By ustawić kompilacje pod jakimś przyciskiem, musisz utworzyć `tasks.json`. By to zrobić, naciskasz `Ctrl` + `Shift` + `P` i wpisujesz:
`Tasks: Configure Task Runner`

	![img](https://i.imgur.com/uZIbzs8.jpg "img")

2. Następnie wybiarasz opcję: `Others`

	![img](https://i.imgur.com/62e1Kog.jpg "img")

3. W nowo utworzonym pliku, zmieniasz wartości na takie i zapisujemy:

	```json
	{
	    "version": "2.0.0",
	    "tasks": [
	        {
	            "taskName": "Build",
	            "type": "shell",
	            "command": "make run"
	        }
	    ]
	}
	```

	![img](https://i.imgur.com/Z4ttLcB.jpg "img")

4. Wchodzisz w ustawienia skrótów: `File/Preferences/Keyboard Shortcuts` i wybierasz wskazaną na zdjęciu kursorem opcję: `keybindings.json`

	![img](https://i.imgur.com/Ovh4xNi.jpg "img")

	Następnie w miejscu po prawej, dodajesz:
	
	```json
	{
	    "key": "f5",
	    "command": "workbench.action.tasks.runTask",
	    "args": "Build"
	}
	```
	
	![img](https://i.imgur.com/LfoarVb.jpg "img")
	
	Teraz po naciśnięciu F5 powinna wywołać się komenda `make run` , która poprzez `Makefile` skompiluje i uruchomi do testu naszą aplikację.





Koniec
------

### Nasz `Makefile`

Co do naszego `Makefile` - wymaga on (bardziej dla Windowsa) pliku `settings.mk` w którym będą nadpisane zmienne, przynajmniej do ścieżek folderów bibliotek (nagłówków, bilbiotek do linkera i wykonywanlnych wersji), na przykład:

```makefile
# SFML 32bit
INC_DIR_SFML_32 := e:/Libraries/SFML-2.4.2-MinGW-7.2-i686/include
LIB_DIR_SFML_32 := e:/Libraries/SFML-2.4.2-MinGW-7.2-i686/lib
BIN_DIR_SFML_32 := e:/Libraries/SFML-2.4.2-MinGW-7.2-x86_64/bin
# SFML 64bit
INC_DIR_SFML_64 := e:/Libraries/SFML-2.4.2-MinGW-7.2-x86_64/include
LIB_DIR_SFML_64 := e:/Libraries/SFML-2.4.2-MinGW-7.2-x86_64/lib
BIN_DIR_SFML_64 := e:/Libraries/SFML-2.4.2-MinGW-7.2-x86_64/bin

# Boost 32bit
INC_DIR_BOOST_32 := e:/Libraries/Boost-1.61.1-MinGW-7.2-i686/include
LIB_DIR_BOOST_32 := e:/Libraries/Boost-1.61.1-MinGW-7.2-i686/lib
BIN_DIR_BOOST_32 := e:/Libraries/Boost-1.61.1-MinGW-7.2-i686/bin
# Boost 64bit
INC_DIR_BOOST_64 := e:/Libraries/Boost-1.61.1-MinGW-7.2-x86_64/include
LIB_DIR_BOOST_64 := e:/Libraries/Boost-1.61.1-MinGW-7.2-x86_64/lib
BIN_DIR_BOOST_64 := e:/Libraries/Boost-1.61.1-MinGW-7.2-x86_64/bin

# Lua 32bit
INC_DIR_LUA_32 := e:/Libraries/Lua-5.3.4-MinGW-7.2-i686/include
LIB_DIR_LUA_32 := e:/Libraries/Lua-5.3.4-MinGW-7.2-i686/lib
BIN_DIR_LUA_32 := e:/Libraries/Lua-5.3.4-MinGW-7.2-i686/bin
# Lua 64bit
INC_DIR_LUA_64 := e:/Libraries/Lua-5.3.4-MinGW-7.2-x86_64/include
LIB_DIR_LUA_64 := e:/Libraries/Lua-5.3.4-MinGW-7.2-x86_64/lib
BIN_DIR_LUA_64 := e:/Libraries/Lua-5.3.4-MinGW-7.2-x86_64/bin

# Sol2
HEADER_INC_SOL2 := e:/Libraries/Lua-Sol2-2.18.7-header-only/include

# JSON
HEADER_INC_JSON := e:/Libraries/JSON-2.1.1-header-only/include
```



### Gratulacje Szczurze!

Od teraz możesz pisać w Visual Studio Code C++ z MinGW, Boost, Lua, JSON i SFML.




