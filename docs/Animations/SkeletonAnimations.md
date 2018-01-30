
Animacje szkieletowe
====================



### Nazewnictwo

Wymogi co do nazw:

- Unikalne (bez powtórzeń),
- Bez znaków specjalnych (w tym spacji i znaków polskich),
- Naturalne po angielsku,
- W stylu `CamelCase`,
- Najlepiej wybierane już jako gotowe.


#### Elementy (kości, sloty i tekstury)

Kości

- Nazwy określające element obiektu, np. `LeftArm`, `Torso`...

Sloty

- Nazwa kości z ~tekstem~ numerem oznaczającym typ slotu (_musicie zgadywać co się pod numerkiem kryje chyba_), np. `LeftArm`, `LeftArm2`...
- ~~Nazwa kości, podkreślnik (`_`) i nazwa określająca przeznaczenie, zawartość slotu, np. `LeftArm_ChainMailArmor`, `LeftArm_PlateArmor`...~~

Tekstury

- Nazwa tekstury to nazwa slotu, odstęp w postaci znaku podkreślnika (`_`) i numer (_też trzeba zgadywać chyba `=,=`, bo dla niektórych cyferki są prostsze..._), np. `LeftArm_001`, `LeftArm2_003`...
- ~~Nazwa slotu, podkreślnik (`_`) i nazwa określająca zawartość tekstury, np. `LeftArm_ChainMailArmor_Normal`, `LeftArm_ChainMailArmor_Broken`, `LeftArm_ChainMailArmor_Blooded`...~~


#### Stany

1. Bazowa nazwa stanu (np. `move`).
2. Nazwa zwrotu (np. `left`)
3. Indeks podanimacji.

##### Przykłady:

Kierunkowe:
* `move_left_0`
* `run_right_3`

##### Wyjątki

`...? @todo pomyśleć i dopisać ;D`


#### Postacie 

Dla postaci ważnych, charakterystycznych i szczególnych pełne, niepowtarzalne nazwy (ewentualnie z dodatkowymi pseudonimami (np. `JohnMaly` `JohnDuzy`) czy coś), 

- np. `Cedmin`, `WilczyBog`, `Nyan`.

Dla postaci mniej ważnych, ale dośc specyficznych - dość pełne nazwy, być może z nazwą miejsca, z ewentualnymi dopiskami liczbowymi, bez wypełnienia zerami, 

- np. `AnkharmSouthTowerGuard1`.

Dla postaci mało ważnych, powtarzalnych - proste nazwy i dopiski liczbowe, bez wypełnienia zerami, 

- np. `Guard1`, `Villager12`, `Citizen123`.

Notka: Dla silnika i tak to wszystko jedno; edytor jednak używa tego przy kategoryzowaniu.


#### Obiekty

Dla obiektów zazwyczaj przyjmujemy nazwy proste z dopiskami liczb, bez wypełnienia zerami, 

- np. `Tree1`, `Grass12`, `Stone123`.

Notka: Dla silnika i tak to wszystko jedno; edytor jednak używa tego przy kategoryzowaniu.



### Foldery

Pliki - zestawy - animacji powinny być umieszczane w kilku folderach:

- `Actors\Main\...\<NazwaPostaci>\*` - dla specyficznie określonych postaci, charakterystycznych i ważnych.
- `Actors\<NazwaLokacji>\...\<NazwaPostaci>\*` - dla postaci określonych w ramach konkretnych lokacji.
- `Actors\Common\...\<NazwaPostaci>\*` - dla postaci powtarzających się, wspólnych ponad lokacjami.

Dla obiektów będą to:

- `Objects\<NazwaLokacji>\...\<NazwaObiektu>\*` - dla obiektów określonych w ramach konkretnych lokacji.
- `Objects\Common\...\<NazwaObiektu>\*` - dla obiektów wspólnych między lokacjami (najwięcej chyba).

Oznaczenie `...` ma w zamyśle wskazywać na możliwość utworzenia (dowolnej) dalszej struktury folderów, kategoryzacji, w celu ułatwienia pracy. Silnik powinien wykrywać dopiero właściwe pliki w końcowych katalogach struktur.

W tych folderach głównych dla obiektów powinny istnieć foldery dla kierunków (np. `90`, `120`, `150`) oraz kilka plików głównych opisanych niżej.




### Pliki

W folderach dla kierunków:

- `skeleton.json` - w którym jest przechowywana informacja o ruchu, format z DragonBones.
- `textures/`     - folder z tekstrurami.
- `> *.png`       - masa plików tekstur (w folderze `textures/`).



### Standardowe ustawienia

Wykrywanie wszystkich stanów aniamcji (ile jest dla każdej) oraz losowość.

Pierwszą wybraną teksturą jest zwykła `texture` lub jeśli nie ma - kolejna.

Pierwsza uruchomiona animacja jest niezdefiniowana - program/skrypt zawsze wybiera.



### API - skrypty i kod

1. Odtwarzanie animacji po:

* bazowej nazwie stanu (kierunek z informacji obiektu (prędkość? obrót?, a kolejność z listy).
* konkretnej nazwie/ustawieniem (pełna nazwa animacji, z wskazaniem kierunku..., wiadomo co uruchomić).

2. Zmiany tekstur/slota:

* po nazwach.


