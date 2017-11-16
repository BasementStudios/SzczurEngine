
Struktura plików projektu
=========================





Foldery
-------

### Główne podfoldery

```
demo/				; Pliki do testowego, przykładowe uruchamiania silnika.
	*/*
docs/				; Dokumentacje, specyfikacje, schematy, informacje.
	*/*.md
	*/*.png
src/				; Źródła, w tym nagłówki.
	*/*.h		
	*/*.cpp	
obj/				; * Skompilowane obiekty, elementy do linkownia w aplikacje.
	*/*.o
out/
	$(ARCH)/*[.exe]	; * Wynikowe pliki wykonywalne dla konkretnych platform.

* - wyłączone z Gita - wpisane tutaj przez wzgląd na `Makefile`.
```





Pliki katalogu głównego
-----------------------

```
README.md			; Informacje ogólna o repozytorium i projekcie.
LICENSE.md			; Licencja własna, i informacje o licencjach bibliotek.
Makefile			; Sprypt `Makefile` do łatwej kompilacji i testowania silnika.
settings.mk			; Ustawienia do skryptu `Makefile`.
.gitignore			; Lista plików/katalogów do zignorowania dla Gita.
```




