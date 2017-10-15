
Visual Studio Code jako IDE do C++ z MinGW i SFML
=================================================
`#vscode #ide #c++ #mingw #sfml #gitbash #github`





GitBash
-------
Dla Mingw musisz pobrać basha np. GitBasha ze strony https://git-scm.com/downloads).

![img](https://i.imgur.com/bzotw9y.jpg "img")

Pobierasz, a podczas instalacji, wszystko zostawiasz jak jest *(Cały czas klikasz **next**)*.





Mingw-w64
---------

### Instalacja

Na początku pobieramy `mingw-w64-install.exe` ze strony https://sourceforge.net/projects/mingw-w64/ 

Teraz instalujemy mingw-w64. Podczas instalacji najlepiej jest wybrać takie ustawienie:

* dla 64-bit
```
	Version: 7.1.0
	Architecture: x86_64
	Threads: posix
	Exception: seh
	Build revision: 2
```

* dla 32-bit (nie testowane)
```
	Version: 7.1.0
	Architecture: i686
	Threads: posix
	Exception: dwarf
	Build revision: 2
```

Scieżkę instalacji polecam wybrać jak najprostrzą. np. `C:\mingw-w64`.

### Konfiguracja

W folderze `.../mingw/bin` kopiujemy plik `mingw32-make.exe` jako `make.exe`.

Do zmiennych środowiskowych dodajemy folder `.../mingw/bin`.





Biblioteka SFML
---------------

### Pobieranie

Z linku https://www.sfml-dev.org/download/sfml/2.4.2/ pobieramy odpowiednie wersje:

- dla 64-bit: `GCC 6.1.0 MinGW (SEH)`
- dla 32-bit: `GCC 6.1.0 MinGW (DW2)`

Dobrze jest mieć obie na dysku.

### Informacja

Pliki nagłówkowe są w folderze `include`. Możemy je dodać do podpowiedzi.





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

Od teraz możesz dowolnie używać Visual Studio Code by pisać swoje programy. W nagrodę będziesz pracował niewolniczo dla Pana Tomasza "Gimpera" Działowego, w produkcji jego gry, na której zarobi tylko on.





