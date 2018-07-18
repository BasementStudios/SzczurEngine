
SzczurEngine
============



SzczurEngine
------------

Omówiony niżej Projekt Szczur ma korzystać z własnego silnika: SzczurEngine pisanego w C++ (standardy C++14/17) z bibliotekami SFML (z własnym rozszerzeniem do 3D), Boost, Lua (z Sol2), OpenAL, FFmpeg, OpenGL (GLAD, GLM) oraz JSON. Silnik będzie budowany dla systemów Windows i Linux, 32 bitowej i 64 bitowej. Naszym zadaniem jest zaprojektowanie, zaprogramowanie i przetestowanie kolejnych elementów silnika oraz gry.



SzczurProjekt
-------------

Projekt Szczur jest inicjatywą podjętą przez Gimpera i społeczność Hajsowników (internetową widownię, skupisko fanów twórczości Gimpera). Polega ona na stworzeniu własnej gry z wykorzystaniem talentów, dostarczenie nowych doświadczeń oraz rozwijanie obecnych umiejętności wsród społeczności. A przede wszystkim - wspólna zabawa i praca :)

##### Społeczniościówki:

- Facebook Szczur Projektu: https://www.facebook.com/SzczurOfficial/
- Facebook Gimpera: https://www.facebook.com/GimperOfficial/
- Grupa Hajsowników na Facebook: https://www.facebook.com/groups/OdrodzeniHajsownicy/
- Twitch Gimpera: https://www.twitch.tv/gimper
- YouTube Gimpera: https://www.youtube.com/user/Gimperr

##### Trailer gry:

[![Szczur Projekt - Trailer](https://img.youtube.com/vi/UHkPLtlzz7E/0.jpg)](https://www.youtube.com/watch?v=UHkPLtlzz7E)

#### Trochę historii

##### Początki

Na początku projektu SZCZUR, w czasie pierwszych rekrutacji, pierwszych streamów i pierwszych przyjmowanych osób planem było wykorzystanie RPG Makera i jego silnika. Jest to bardzo prosty program/silnik oparty o skrypty w Ruby. Wtedy nie było nas aż tak wielu, i byliśmy jeszcze "skrypterami" i tworzyliśmy lub modyfikowaliśmy skrypty z internetu w celu ich implementacji w naszej grze. Już wtedy pracowaliśmy używając GitHuba.

Pracowaliśmy wtedy w bardzo ograniczonym środowisku; wiele możliwości było poza naszym zasięgiem. Nie było mowy o zaawansowanych mechanikach, czy dowolnie skonstruowanej perspektywie i interfejsie, jak to jest obecnie. Silnik RPGM zapewniał podstawy, ale jednocześnie był kulą u nogi. Już wtedy podzieliliśmy się pracą na moduły: za części całości odpowiadały inne osoby, tak np. MrRaiN był odpowiedzialny za dziennik.

##### Własny silnik

Po pewnym czasie narodziła się koncepcja stworzenia własnego silnika od podstaw. Rozważaliśmy także Unity, ale chcieliśmy zrobić coś własnego, by przy okazji zdobyć trochę doświadczenia, zrozumieć kryjące się za tym wszystkim zależności i mieć możliwość ewentualnej sprzedarzy samego silnika. Wybór padł na bibliotekę SFML przy użyciu języka C++.
Pierwszym rzutem była wersja będąca bardzo prostą kopią RPGM. Po pewnym czasie okazało się, że przy nowych naborach osób, już wtedy programistów aniżeli skrypterów, musieliśmy przebudować silnik - i wyszło na to, że zaczeliśmy go pisać od nowa. Międzyczasie zmieniła się wizja gry, perspektywa, więc już nie mogliśmy się wzorować RPGM. Międzyczasie zdefiniowaliśmy sprawy teoretyczne: styl kodu, licencje, ogólne założenia... Zaczeliśmy także korzystać z Trello.

##### Obecny stan

Silnik jak i projekt nadal rósł - w ludziach, w możliwościach, planach i co najważniejsze - w postępie. W trakcie prac i testów okazało się, że przejście w wyświetlanie obiektów na przestrzeni 3D będzie optymalnym rozwiązaniem. Praca nadal jest podzielona między moduły. Do silnika doszły już nowe względem RPGM technologie, np. animacje szkieletowe DragonBones. Stworzone są dość solidne podwaliny jądra silnika. Są już zrealizowane moduły dot. interfejsu, dźwięku, zasobów czy wyświetlania. Mamy już edytory map, dialogów, efektów dźwiękowych. Działają skrypty i podstawowe oświetlenie. Za rogiem jest już skończona walka, efekty specjalne oraz cienie. Powstają pierwsze etapy gry.



Dział programistów
------------------

* Krzysiek (Krzychor)
* Kuba (Delfin)
* Kamil (Sajko)
* Jan (Janix) Izydorczyk
* Kuba (Mapet)
* Patryk (Stritch)
* Kuba (MetRiko)
* Piotr (H4RRY)
* Tomasz (Knayder)
* Patryk (PsychoX) Ludwikowski

##### Dawny skład

* Mateusz (Zetiar)
* Patryk (Dubgron)
* Patryk (PerriX)
* Kamil (MrRain)



Informacje
----------

Dokumentacje i więcej informacji dostępne w folderze `docs`.

##### Kompilacja

Proces wdrażania się w projekt, a przy tym ręcznej kompilacji opisany jest w `docs/Coding/IntegrationTutorial.md`.
