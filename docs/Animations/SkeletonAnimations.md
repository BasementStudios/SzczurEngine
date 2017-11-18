
Animacje szkieletowe
====================



### Nazewnictwo

Niech nazwy się nie powtarzają, najlepiej gdyby były już gotowymi, **unikalnymi** nazwami, **bez znaków specjalnych** (w tym spacji i znaków polskich), z już bardziej estetyczną personalną prośbą: **małymi angielskimi** znakami.


#### Elementy (tekstur)

Normalne elementy niech będą nazwane naturalnie, np. `head` itp.

Elementy, które są w ramach zmiany tekstury, niech mają dopiski z przodu (najlepiej coś o tym mówiące): nazwa grupy tekstur i `_`, np. `blooded_`, `leather_`, `dirty_`.


#### Stany

Kierunkowe:
1. Bazowa nazwa stanu (np. `move`).
2. Nazwa kierunku, podstanu (np. `left`)
3. Indeks podanimacji.

Bez kierunkowe:
1. Bazowa nazwa stanu (np. `idle`).
2. Indeks podanimacji.

#### Przykłady:

Kierunkowe:
* `move_left_0`
* `run_right_3`

Bez kierunkowe:
* `idle_0`,
* `sit_0`


#### Postacie 

Dla postaci ważnych, charakterystycznych i szczególnych pełne, niepowtarzalne nazwy (ewentualnie z dodatkowymi pseudonimami (np. `JohnMaly` `JohnDuzy`) czy coś), 

- np. `Cedmin`, `WilczyBog`, `Nyan`.

Dla postaci mniej ważnych, ale dośc specyficznych - dość pełne nazwy, być może z nazwą miejsca, z ewentualnymi dopiskami liczbowymi, 

- np. `AnkharmSouthTowerGuard01`.

Dla postaci mało ważnych, powtarzalnych - proste nazwy i dopiski liczbowe, 

- np. `Guard001`, `Villager012`, `Citizen123`.

#### Obiekty

Dla obiektów zazwyczaj przyjmujemy nazwy proste z dopiskami liczb, 

- np. `Tree001`, `Grass012`, `Stone123`.



### Foldery

Pliki - zestawy - animacji powinny być umieszczane w kilku folderach:

- `Actors\Main\...\<NazwaPostaci>\*` - dla specyficznie określonych postaci, charakterystycznych i ważnych.
- `Actors\<NazwaLokacji>\...\<NazwaPostaci>\*` - dla postaci określonych w ramach konkretnych lokacji.
- `Actors\Common\...\<NazwaPostaci>\*` - dla postaci powtarzających się, wspólnych ponad lokacjami.

Dla obiektów będą to:

- `Objects\<NazwaLokacji>\...\<NazwaObiektu>\*` - dla obiektów określonych w ramach konkretnych lokacji.
- `Objects\Common\...\<NazwaObiektu>\*` - dla obiektów wspólnych między lokacjami (najwięcej chyba).

Oznaczenie `...` ma w zamyśle wskazywać na możliwość utworzenia (dowolnej) dalszej struktury folderów, kategoryzacji, w celu ułatwienia pracy. Silnik powinien wykrywać dopiero właściwe pliki w końcowych katalogach struktur.



### Pliki

- `skeleton.json` - w którym jest przechowywana informacja o ruchu, format z DragonBones.
- `texture.json`  - atlas tekstur, czyli pozycje odpowiednich elementów, format z DragonBones.
- `texture.png`   - tekstura, właściwa grafika, format z DragonBones.
- `lists.json`    - lista z kolejnością animacji, wg. niżej opisanego formatu.
- `groups.json`   - dane o wykorzystanych grupach elementów do szkieletów.


#### Kolejność animacji i podanimacji (`lists.json`)

Plik `lists.json` opisuje listę (lub kolejność) odtwarzanych podanimacji dla danych stanów obiektu.

Każda lista charakryzuje się 3 właściwościami:
* wagą - waga zazwyczaj definuje dadatkową szansę w przypadku losowości - jest tak, gdy lista wyższa (w której znajduje się obecna, od której mamy obecną wagę) jest losowa. Jeśli tak nie jest, jest to liczba powtórzeń dla listy podanimacji. Każdy zwykły (nie-lista) element ma wagę 1.
* losowością - jeśli jest włączona, kolejna podanimacja z listy będzie wybierana losowo wśród elementów listy, z uwzględnieniem wag.
* listą elementów - jest to właściwa lista elementów. Elementem listy może być liczba oznaczająca podanimację do wykonania lub kolejna lista.

Przykładowa zawartość pliku `lists.json` wraz z komentarzem:
```js
{
    "idle": [                   // Dla każdej animacji jest przynajmniej jedna lista - główna.
        
        -1,                     // Pierwsza liczba w liście; Wartość absolutna definiuje wagę,
                                //  a minus przy liczbie ustawia listę jako losową. Potem są elementy.
        
        0,                      // Liczba oznaczająca podanimację do wykonania. Jako, że główna lista
                                //  jest losowa, ma ona szansę 1 do 9 (suma wag w liście głównej).
        
        [                       // Kolejna lista
                                
            1,                  // Tu mamy przypadek bez losowości i z wagą 1 dla listy wyższej.
            2,                  // Mamy tutaj 2 podanimacje, kolejno wywoływane: z numerami 2 i 1.
            1                   // Oczywiście mogą być dalsze "poziomy" list, ale to przykład.
        ],
        
        [
            5,                  // Mimo, że podlista ta ma jeden element, ma ona sens - ma ona wagę 5,
            1                   //  co zapobiega równoznacznemu pięciokrotnemu wpisaniu numeru podanimacji.
                                //  Jest to najczęściej używana podanimacja w przykładzie: 5 / 9 szans.
        ],
        
        [
            -1,                 // Podlista z losowością. Użycie tej podlisty z głównej ma 1 / 9 szans.
            3,                  // Łatwo można policzyć, że te podelementy mają więc 1 / 18 szans każdy.
            4
        ]
    ],
    
    "move": [/* ... */]         // Dla innych stanów też podobne listy.
}
```

Uwagi:
- W przykładzie `//` i wszystko w linni po to komentarz i nie powinno być to w pliku. 
- Struktura może być rozpisana nawet w jednej linii i niemal bez spacji, ale lepiej się edytuje z dobrymi wycięciami.
- W przypadku, gdy podanimacja nie istnieje jest omijana i jest używana następna.


#### Niestandardowe tekstury (`groups.json`)

Niektóre postacie/obiekty będa mieć inne tekstury lub/i współdzielić model. W `groups.json` będą opisane kolejne grupy z tekstur, które mogą być nadane postaci/obiektowi.

Normalna postać z standardową grupą (lub w przypadku wzkazania niewłaściwej, lub brakach w danej grupie) używa elementów z grupy standardowej, której elementy nie mają żadnego dopisku z przodu dot. nazwy grupy (patrz: nazwewnictwo elementów).

Pewnie często grupy będą tworzone wyłącznie z elementów z odpowiednimi przedrostkami dot. grupy, ale nie zawsze. W każdym razie, używane elementy są rozpisane w pliku `groups.json`:

```js
{
    "blooded": [
        "blooded_head",
        "blooded_arms",
        "blooded_beniz",
        "dirty_torso"
    ],
    "dirty": [
        "dirty_head",
        "dirty_arms",
        "dirty_torso",
        "dirty_boots"
    ]
}
```



### Standardowe ustawienia

Wykrywanie wszystkich stanów aniamcji (ile jest dla każdej) oraz losowość.

Pierwszą wybraną teksturą jest zwykła `texture` lub jeśli nie ma - kolejna.

Pierwsza uruchomiona animacja jest niezdefiniowana - program/skrypt zawsze wybiera.



### API - skrypty i kod

1. Odtwarzanie animacji po:

* bazowej nazwie stanu (wtedy kierunek z informacji obiektu, a kolejność z listy).
* konkretnej nazwie (pełna nazwa, wiadomo co uruchomić).

2. Zmiany tekstur:

* po nazwie.


