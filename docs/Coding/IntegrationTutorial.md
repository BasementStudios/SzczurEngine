
Visual Studio Code jako IDE do C++ z MinGW, Boost, Lua, JSON i SFML 
===================================================================
`#vscode #ide #c++ #mingw #sfml #gitbash #github #boost #lua #json`







Wstęp
-------------------------------------------------------------------------------

### Porządkowanie bibliotek

Warto uporządkować sobie biblioteki - mogą być ich różne wersje (dla różnych kompilatorów) i mogą być one ze sobą niekompatybilne, dlatego zaleca się:

1. Posiadanie folderu dla bibliotek C++, np. `E:/Libraries`.

2. W tym folderze posiadanie folderów głównych (wypadkowanych/zainstalowanych) dla każdej bibliotek.

3. Posiadanie folderu do (np. `include`, ale też może być `src` czy `boost` jak w bibliotece Boost) dla nagłówków.

4. Posiadanie folderów od kompilatorów np. `MinGW-7.2-x86_64` z podfolderami `lib` (dla plików bibliotek np. `.a`, `.so`, `.dll`) i `bin` (dla binarnych/wykonywalnych, np. `.exe`).

Takie rozłożneie daje mniej wiecej osobne lokalizacje nagłówków (niezależnych od kompilatora), plików bibliotek i wykonywalnych (zależnych).

Pliki nagłówkowe są w folderze `include`, można je później dodać do systemu podpowiedzi w edytorze.







GitBash
-------------------------------------------------------------------------------

Git Bash dla Windowsa dostarcza podstawowe środowisko Git wraz z podstawowym podsystemem Unix z Bash. 



### Instalacja

1. Do MinGW musimy pobrać `bash`a np. GitBasha ze strony https://git-scm.com/downloads). 

	![img](https://i.imgur.com/bzotw9y.jpg "img")

2. Pobieramy i instalujemy (np. wszystko zostawiamy jak jest i cały czas klikamy _next_).







MinGW-w64
-------------------------------------------------------------------------------

MinGW jest podstawowym zestawem do kompilacji GNU na system Windows, `w64` jest wersją z kompilacją na 64 bitową architekturę.



### Instalacja

1. Na początku pobieramy MinGW w64 ze strony https://sourceforge.net/projects/mingw-w64/files/ 

    _SzczurEngine_ zakłada używanie _C++17_, więc raczej będzie to wersja _7.X_, np. `7.2`. Najłatwiej wybrać architekturę `x86_64` (co umożliwi kompilacje na 32 jak i 64 bitowe maszyny), system wątków `posix` (bardziej popularny i funkcjonalny, nawet działa na Windowsie, [o ironio](https://stackoverflow.com/questions/17242516/mingw-w64-threads-posix-vs-win32)), a system wyjątków `seh` ([tutaj po informacje](https://stackoverflow.com/questions/15670169/what-is-difference-between-sjlj-vs-dwarf-vs-seh)).

	...więc wersja `MinGW-W64 GCC-7.2` - `x86_64-posix-seh` jest okej ;)

2. Wypakowujemy do wybranego katalogu - Ścieżkę wybrać jak dość prostą, najlepiej bez spacji np. `C:/mingw64`.



### Konfiguracja

1. W folderze głównym z folderu `bin` kopiujemy plik `mingw32-make.exe` jako `make.exe` (w tym samym folderze).

2. Do zmiennych środowiskowych dodajemy folder folder `bin` z folderu głównego, np. `C:/mingw64/bin`.

3. W konsoli możemy sprawdzić pomyślność instalacji komendą `g++ --version`. 







Biblioteka SFML
-------------------------------------------------------------------------------

Simple and Fast Multimedia Library – wieloplatformowa biblioteka programistyczna ułatwiająca tworzenie gier oraz programów multimedialnych.



### Instalacja

1. Z linku https://nightlybuilds.ch/project/show/1/SFML/ pobieramy odpowiednie wersje, najbliższe kompilatorowi, na przykład:

	- dla 64-bit: `MinGW Builds Posix SEH x64`
	- dla 32-bit: `MinGW Builds Posix Dwarf x32`

	![img](https://i.imgur.com/UNLaRon.jpg "img")

2. Wypakowujemy je. Dobrze jest mieć obie na dysku, w dość sensowym i wspólnym miejscu, np. `E:/Libraries`. 







Boost
-------------------------------------------------------------------------------

Boost – kolekcja bibliotek programistycznych poszerzających możliwości języka C++.



### Pobieranie 

1. Pobieramy najnowszą wersję biblioteki ze strony http://www.boost.org/ .

	![img](https://i.imgur.com/EhLWGhW.jpg "img")

2. Wypakowujemy pobrane archiwum w odpowiednie miejsce z odpowiednią nazwą. Dobrym miejscem jest np. `boost-1.65.1` w folderze `E:/Libraries`.



### Kompilacja

1. Otwieramy terminal i wchodzimy do wypakowanego folderu.

2. Wchodzimy do `tools/build`.
	```
	cd tools/build
	```
3. Używamy komendy: 
	```
	./bootstrap.bat gcc
	``` 
4. Następnie:
    ```
    ./b2.exe install --prefix=../../stage
    ```
5. Cofamy się do folderu głównego (wypakowanego):
	```
	cd ../../
	```
6. Używamy: 
	```
	./stage/bin/b2.exe toolset=gcc --build-type=complete stage
	```
7. Czekamy do końca kompilacji... czyli **dłuższą** chwilę ;) 







Skrypty Lua 
-------------------------------------------------------------------------------

Lua − język skryptowy pierwotnie zaprojektowany dla rozszerzenia funkcjonalności różnych aplikacji.



### Pobieranie

1. Wchodzimy na https://www.lua.org/download.html i pobieramy Lua (w projekcie Szczur używamy `lua-5.3.4`).

	![img](https://i.imgur.com/ekjAmfY.jpg "img")

2. Wypakowujemy pobrane archiwum w odpowiednie miejsce, np. `E:/Libraries`. Mamy więc folder `Lua-5.3.4`.



### Kompilacja

1. Wchodzimy terminalem do tego folderu.

2. Używamy `make mingw` dla kompilacji z MinGW.

Tutaj, jako że Lua kompiluje się do... `src` - pozwole sobie przypomnieć o wstępie, i zaproponować odpowiednie przeniesienie skompilowanych plików.



### Biblioteka `sol2`

Jest to biblioteka ułatwiająca działanie z silnikiem skryptowym Lua.

1. Z https://github.com/ThePhD/sol2/releases pobieramy wersję bilbioteki (może być sam plik nagłówkowy `.hpp`).

2. Pobrany plik nagłówkowy wrzucamy do odpowiedniego folderu (np. `E:/Libraries/Lua-sol2-2.18.7/include`). 

![img](https://i.imgur.com/OYmdfkd.jpg "img")







JSON
-------------------------------------------------------------------------------

JSON jest popularnym, prostym i dość wygodnym formatem do przetrzymywania konfiguracji.



### Pobieranie

1. Z https://github.com/nlohmann/json/releases pobieramy wersję biblioteki (może być sam plik nagłówkowy `.hpp`).

2. Pobrany plik nagłówkowy wrzucamy do odpowiedniego folderu (np. `E:/Libraries/JSON-2.1.1/include`).

![img](https://i.imgur.com/sHDso2Y.jpg "img")







GitHub
-------------------------------------------------------------------------------

GitHub – hostingowy serwis internetowy przeznaczony dla projektów programistycznych wykorzystujących system kontroli wersji Git.



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

* _Poznaj GIT w 15 minut_ (PS: irl 7:43) - https://www.youtube.com/watch?v=LEystZRF8fQ
* _How to Use GitHub with GitKraken_ - https://www.youtube.com/watch?v=f0y_xCeM1Rk







Visual Studio Code
-------------------------------------------------------------------------------

Visual Studio Code jest dość wyważonym: lekkim, a jednocześnie bardzo funkcjonalnym edytorem.



### Instalacja

1. Na początku, musimy pobrać Visual Studio Code ze strony https://code.visualstudio.com/download/.

	![img](https://i.imgur.com/LauCfo9.jpg "img")

2. Wybieramy odpowiednią wersję i przeprowadzasz instalację programu.



### Konfiguracja

1. Na początku musimy pobrać rozszerzenie C/C++ od Microsoftu.

	![img](https://i.imgur.com/tA9s0pC.jpg "img")

2. Otwieramy jakiś plik (np. `Game.cpp`) i najeżdżając na `#include` kursorem naciskamy żaróweczkę, która się pojawi obok. 

3. Następnie wybieramy pozycję: `Add to "includePath": ${workspaceRoot}/include`

	![img](https://i.imgur.com/AOZIYmz.jpg "img")

	(Jeżeli żarówka się nie pojawia, to naciśnij `Ctrl` + `Shift` + `P`, a następnie wpisz `Reload Window`)

4. Jeśli nie ma tej opcji lub mamy błąd, zawsze mamy możliość edycji ścieżek do nagłówków poprzez opcje `Edit "includePath" setting`.

	Dodajemy tam ścieżki do folderów z nagłówkami każdej z bibliotek.

	W ten sposób zapewniliśmy sobie sugestie.

3. Ostatnią rzeczą, którą musimy zrobić, jest ustawienie Basha jako domyślnego `shell`a *(terminala, odpalanego kombinacją klawiszy `Ctrl` + `~`)*.

	Przechodzimy do ustawień `File`/`Preferences`/`Settings` i dodajemy po prawej taką linijkę: `"terminal.integrated.shell.windows": "..\..\Git\bin\bash.exe"` (ścieżka do basha, którego pobraliśmy wcześniej; tutaj relatywna do instalacji VS Code).

	![img](https://i.imgur.com/l0bjStH.jpg "img")



### Ustawianie skrótu dla kompilacji 

By ustawić kompilacje pod jakimś przyciskiem, musimy dodać do pliku `tasks.json` informacje o . 

1. Naciskamy `Ctrl` + `Shift` + `P` i wpisujemy: `Tasks: Configure Task Runner`

	![img](https://i.imgur.com/uZIbzs8.jpg "img")

2. Następnie wybieramy opcję: `Others`

	![img](https://i.imgur.com/62e1Kog.jpg "img")

3. W nowo utworzonym pliku, zmieniamy wartości na takie i zapisujemy:

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

4. Wchodzimy w ustawienia skrótów: `File/Preferences/Keyboard Shortcuts`

5. Wybieramy wskazaną na zdjęciu kursorem opcję: `keybindings.json`

	![img](https://i.imgur.com/Ovh4xNi.jpg "img")

6. Następnie w miejscu po prawej, dodajemy:
	
	```json
	{
		"key": "f5",
		"command": "workbench.action.tasks.runTask",
		"args": "Build"
	}
	```
	
	![img](https://i.imgur.com/LfoarVb.jpg "img")

Teraz po naciśnięciu F5 powinna wywołać się komenda `make run`, która poprzez `Makefile` skompiluje i uruchomi do testu naszą aplikację.

Analogicznie można robić inne zadania uruchamiające inne aplikacje itd. dodają odpowiednio nowe elementy na listach w plikach `tasks.json` i `keybindings.json`.







Koniec
-------------------------------------------------------------------------------

### Nasz `Makefile`

Co do naszego `Makefile` - wymaga on pliku `settings.mk` w którym będą nadpisane zmienne, przynajmniej do ścieżek folderów bibliotek (nagłówków, bilbiotek do linkera i wykonywanlnych wersji), na przykład:

```makefile
# SFML 32bit
INC_DIR_SFML_32 := e:/Libraries/SFML-2.4.2/include
LIB_DIR_SFML_32 := e:/Libraries/SFML-2.4.2/MinGW-7.2-x86_64/lib
BIN_DIR_SFML_32 := e:/Libraries/SFML-2.4.2/MinGW-7.2-x86_64/bin
# SFML 64bit
INC_DIR_SFML_64 := e:/Libraries/SFML-2.4.2/include
LIB_DIR_SFML_64 := e:/Libraries/SFML-2.4.2/MinGW-7.2-x86_64/lib
BIN_DIR_SFML_64 := e:/Libraries/SFML-2.4.2/MinGW-7.2-x86_64/bin

# Boost 32bit
INC_DIR_BOOST_32 := e:/Libraries/Boost-1.65.1/boost
LIB_DIR_BOOST_32 := e:/Libraries/Boost-1.65.1/MinGW-7.2-x86_64/lib
BIN_DIR_BOOST_32 := e:/Libraries/Boost-1.65.1/MinGW-7.2-x86_64/bin
# Boost 64bit
INC_DIR_BOOST_64 := e:/Libraries/Boost-1.65.1/boost
LIB_DIR_BOOST_64 := e:/Libraries/Boost-1.65.1/MinGW-7.2-x86_64/lib
BIN_DIR_BOOST_64 := e:/Libraries/Boost-1.65.1/MinGW-7.2-x86_64/bin

# Lua 32bit
INC_DIR_LUA_32 := e:/Libraries/Lua-5.3.4/src
LIB_DIR_LUA_32 := e:/Libraries/Lua-5.3.4/MinGW-7.2-x86_64/lib
BIN_DIR_LUA_32 := e:/Libraries/Lua-5.3.4/MinGW-7.2-x86_64/bin
# Lua 64bit
INC_DIR_LUA_64 := e:/Libraries/Lua-5.3.4/src
LIB_DIR_LUA_64 := e:/Libraries/Lua-5.3.4/MinGW-7.2-x86_64/lib
BIN_DIR_LUA_64 := e:/Libraries/Lua-5.3.4/MinGW-7.2-x86_64/bin

# Lua sol2
HEADER_INC_SOL2 := e:/Libraries/Lua-sol2-2.18.7/include
# JSON
HEADER_INC_JSON := e:/Libraries/JSON-2.1.1/include

# DragonBones \w SFML wrapper
SRC_DIRS += e:/Libraries/DragonBones/DragonBones/src e:/Libraries/DragonBones/SFML/src e:/Libraries/DragonBones/3rdParty
INC_DIRS += e:/Libraries/DragonBones/DragonBones/src e:/Libraries/DragonBones/SFML/src e:/Libraries/DragonBones/3rdParty
```

Uwagi:

- Na Linuxie nadal obowiązują katalogi ogólne z nagłówkami (np. `/usr/local/include`), ale można podać tu ścieżki, jeśli biblioteka ma być użyta z innej lokacji.

- Można uzyć `MXE = yes` oraz `MXE_DIR = /sciezka/to/mxe` dla kompilacji z środowiska MXE.

- `Makefile` może mieć kolorki (_XD_), zezwalane za pomocą `COLORS = yes`.

- Boost po kompilacji ma w swoim `lib` nazwy z tagiemm kompilatora, np. `mgw72`, i wersji Boosta, np. `1_65_1`. 

	Jeśli chcemy takich używać - można, ale należy ustawić sobie w `settings.mk` odpowiednie zmienne:

	```makefile
	CXXFLAGS_STATIC_BOOST  :=
	 LDFLAGS_STATIC_BOOST  := -lboost_container-mgw72-mt-1_65_1
	CXXFLAGS_DYNAMIC_BOOST :=
	 LDFLAGS_DYNAMIC_BOOST := -lboost_container-mgw72-mt-s-1_65_1
	```

	Taka polityka `Makefile` została przyjęta, bo skompilowana wersja biblioteki na danej wersji kompilatora powinna być dobrze zakatalogowana - wyodrębiona od innych wersji (np. `E:/Libraries/Boost-1.65.1/MinGW-7.2-x86_64`).

	Oczywiście z flagami zaznaczającymi użycie każdej z bibliotek Boosta z nazwą z `mgw??` (tutaj przykładowe, ale są inne raczej). 

	Aby temu zapobiec, można zmienić nazwy na te bez tagów (np. `mgw72-` i `-1_65_1`), np. komendami: 
	
	```bash
	for FILE in ./*; do mv "$FILE" "${FILE/mgw??-/}"; done
	for FILE in ./*; do mv "$FILE" "${FILE/-?_*./.}"; done
	```

	(uruchomioną w terminalu `bash`a w folderze `lib`).



### Gratulacje Szczurze!

Od teraz możesz pisać w Visual Studio Code C++ z MinGW, Boost, Lua, JSON i SFML.






