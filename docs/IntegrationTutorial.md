
Visual Studio Code jako IDE do C++ z MinGW, Boost i SFML 
========================================================
`#vscode #ide #c++ #mingw #sfml #gitbash #github #boost'





GitBash
-------
Dla Mingw musisz pobrać basha np. GitBasha ze strony https://git-scm.com/downloads).

![img](https://i.imgur.com/bzotw9y.jpg "img")

Pobierasz, a podczas instalacji, wszystko zostawiasz jak jest *(Cały czas klikasz **next**)*.





Mingw-w64
---------

### Instalacja

Na początku pobieramy MinGW w64 (`SzczurEngine` zakłada C++17, więc raczej 7.x) ze strony https://sourceforge.net/projects/mingw-w64/files/ - może być to link `i686-posix-dwarf` dla `x32` i `x86_64-posix-seh` dla dodatkowo `x64`.

Scieżkę instalacji polecam wybrać jak najprostrzą. np. `C:\mingw-w64`.

### Konfiguracja

W folderze `.../mingw/bin` kopiujemy plik `mingw32-make.exe` jako `make.exe`.

Do zmiennych środowiskowych dodajemy folder `.../mingw/bin`, np. `C:\mingw-w64\mingw\bin`.

W konsoli możemy sprawdzić pomyślność instalacji komendą `g++ --version`.





Biblioteka SFML
---------------

### Pobieranie

Z linku https://nightlybuilds.ch/project/show/1/SFML/ pobieramy odpowiednie wersje, najbliższe kompilatorowi:

- dla 64-bit: `MinGW Builds Posix SEH x64`
- dla 32-bit: `MinGW Builds Posix Dwarf x32`

Dobrze jest mieć obie na dysku, w dość sensowym i wspólnym miejscu, np. `E:\Libraries`.

### Informacja

Pliki nagłówkowe są w folderze `include`. Możemy je dodać do podpowiedzi.





Boost
-----

Pobieramy najnowszą wersję biblioteki ze strony http://www.boost.org/ i wypakowujemy w odpowienie miejsce. Dobrym miejscem jest np. także `E:\Libraries`. Wypakowne archiwum nazywamy np. `boost_builder` - bo jest to zestaw, z którego skompiluje się bilbioteka.

Otwieramy terminal i wchodzimy do folderu `boost_builder` i tworzymy folder `stage`. Wchodzimy do `tools\build` i używay komendy `bootstrap.bat gcc` a następnie `b2.exe install toolset=gcc --prefix=..\..\stage`. Cofamy się do folderu `boost_builder` i używamy `stage\bin\b2.exe toolset=gcc --build-type=complete stage`. Czekamy... dłuższą chwilę ;)

Chętni mogą sobie uporządkować libkę poprzez: stworzenie folderu np. `boost-1.65.1` w sensownym miesjcu, stworzenie tam folderu `include`, do którego przenosimy folder `boost_builder\boost`, przeniesienie folderu `boost_builder\stage\lib` i zmienienie jego nazwy na np.  `lib_MinGW-7.2.0-32/64`





GitHub
------

### Pobieranie repozytorium projektu

Wybieramy miejsce, w którym ma się znajdować nasz projekt. Naciskamy prawym klawiszem myszy na pustym obszarze i wybieramy opcję `Git Bash Here`. Można też otworzyć Git Basha np. z menu start i ręcznie przejść do odpowiedniego katalogu.

![img](https://i.imgur.com/3ykWZTv.jpg "img")

W wyświetlonym oknie, wpisujemy

```
git clone https://github.com/mcrain345/SzczurEngine.git
````

Następnie zatwierdzamy enterem.

![img](https://i.imgur.com/GYPC6Gc.jpg "img")

_(W Git Bashu, wklejamy zawartość schowka kombinacją klawiszy `Shift` + `Insert`)_

Bardzo polecany jest także GitKraken - https://www.gitkraken.com/ - będącym przyjemnym i pomocnym graficznym interfejsem do Gita.





Visual Studio Code
------------------

### Instalacja

Na początku, musimy pobrać Visual Studio Code ze strony https://code.visualstudio.com/download/.

![img](https://i.imgur.com/LauCfo9.jpg "img")

Wybieramy odpowiednią wersję i przeprowadzasz instalację programu.



### Konfiguracja

Na początku musimy pobrać rozszerzenie C/C++ od Microsoftu

![img](https://i.imgur.com/tA9s0pC.jpg"img")

Otwierasz jakiś plik (np. Game.cpp) i najeżdżając na include kursorem naciskasz żaróweczkę która się pojawi obok. Następnie wybierasz pozycję:
`Add to "includePath": ${workspaceRoot}/include`

![img](https://i.imgur.com/AOZIYmz.jpg"img")

(Jeżeli żarówka się nie pojawia, to naciśnij `Ctrl` + `Shift` + `P`, a następnie wpisz `Reload Window`)

W ten sposób zapewniliśmy sobie sugestie.

Ostatnią rzeczą którą musisz zrobić, jest ustawienie Basha jako domyślnego shella
*(terminala, odpalanego kombinacją klawiszy `Ctrl` + `~`)* oraz przypisanie skrótu do kompilacji kodu.
Przechodzimy do ustawień `File/Preferences/Settings` i dodajemy po prawej taką linijkę:
`"terminal.integrated.shell.windows": "..\..\Git\bin\bash.exe(Ścieżka do basha którego pobraliśmy wcześniej)"`

![img](https://i.imgur.com/l0bjStH.jpg"img")



### Ustawianie kompilacji pod konkretny przycisk

By ustawić kompilacje pod jakimś przyciskiem, musisz utworzyć `tasks.json`. By to zrobić, naciskasz `Ctrl` + `Shift` + `P` i wpisujesz:
`Tasks: Configure Task Runner`

![img](https://i.imgur.com/uZIbzs8.jpg"img")

Następnie wybiarasz opcję: `Others`

![img](https://i.imgur.com/62e1Kog.jpg"img")

W nowo utworzonym pliku, zmieniasz wartości na takie i zapisujemy:

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

![img](https://i.imgur.com/Z4ttLcB.jpg"img")

Wchodzisz w ustawienia skrótów: `File/Preferences/Keyboard Shortcuts` i wybierasz wskazaną na zdjęciu kursorem opcję: `keybindings.json`

![img](https://i.imgur.com/Ovh4xNi.jpg"img")

Następnie w miejscu po prawej, dodajesz:

```json
{
    "key": "f5",
    "command": "workbench.action.tasks.runTask",
    "args": "Build"
}
```

![img](https://i.imgur.com/LfoarVb.jpg"img")





Koniec
------

### Gratulacje Szczurze!

Od teraz możesz pisać w Visual Studio Code C++ z MinGW, Boost i SFML.




