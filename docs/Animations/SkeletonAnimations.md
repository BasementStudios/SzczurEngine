
Animacje szkieletowe
====================

### Foldery

Pliki - zestawy - animacji będą umieszczone w kilku folderach:

- `Actors\Main\<NazwaPostaci>\*` - dla specyficznie określonych postaci, charakterystycznych i ważnych.
- `Actors\Act 1\<NazwaPostaci>\*` - dla postaci określonych w ramach konkretnego rozdziału.
- `Actors\Common\<NazwaPostaci>\* - dla postaci powtarzających się, wspólnych ponad rozdziałami.



### Pliki

- `skeleton.json` - w którym jest przechowywana informacja o ruchu, format z DragonBones.
- `texture.json`  - atlas tekstur, czyli pozycje odpowiednich elementów, format z DragonBones.
- `texture.png`   - tekstura, właściwa grafika, format z DragonBones.
- `lists.json`    - lista z kolejnością, wg. niżej opisanego formatu.



#### `lists.json`

Opisuje on listę (lub kolejność) odtwarzanych podanimacji dla danych stanów obiektu.

```js
{
	"idle": {
		"list": [
			0,
			1,
			2                   // mogą się powtarzać, wtedy będą odtwarzane częściej
		],
		"random": true          // jeśli true - używa losowego z listy
	],							
	"move": {/* ... */}         // dla innych stanów też listy.
}
```

#### Niestandardowe tekstury

Kolejne możliwe do zmiany przechowywane w plikach z nazwą wg.:
1. `texture`
2. Jeśli niestandardowa: `_`
3. Nazwa stanu tekstur, np. `dirty`

Przykład:
* `texture_blooded.png`, `texture_blooded.json`



### Standardowe ustawienia

Wykrywanie wszystkich stanów aniamcji (ile jest dla każdej) oraz losowość.

Pierwszą wybraną teksturą jest zwykła `texture` lub jeśli nie ma - kolejna.

Pierwsza uruchomiona animacja jest niezdefiniowana - program/skrypt zawsze wybiera.



### Nazewnictwo stanów

Kierunkowe:
1. Bazowa nazwa stanu (np. `move`).
2. Nazwa kierunku, podstanu (np. `left`)
3. Indeks podstanu.

Bez kierunkowe:
1. Bazowa nazwa stanu (np. `idle`).
2. Indeks podstanu

#### Przykłady:

Kierunkowe:
* `move_left_0`
* `run_right_3`

Bez kierunkowe:
* `idle_0`,
* `sit_0`



### Nazewnictwo postaci

Niech nazwy się nie powtarzają, najlepiej gdyby były już gotowymi, _unikalnymi_ nazwami.

Dla postaci ważnych, charakterystycznych i szczególnych pełne, niepowtarzalne nazwy (ewentualnie z dodatkowymi pseudonimami (np. `JohnMaly` `JohnDuzy`) czy coś), np. `Cedmin`, `WilczyBog`, `Nyan`.

Dla postaci mniej ważnych, ale dośc specyficznych - dość pełne nazwy, być może z nazwą miejsca, z ewentualnymi dopiskami liczbowymi, np. `AnkharmSouthTowerGuard01`.

Dla postaci mało ważnych, powtarzalnych - proste nazwy i dopiski liczbowe, np. `Guard001` `Villager012` `Citizen123`.



### API - skrypty i kod

1. Odtwarzanie animacji po:
	
	* bazowej nazwie stanu (wtedy kierunek z informacji obiektu, a kolejność z listy).
	* konkretnej nazwie (pełna nazwa, wiadomo co uruchomić).
	
2. Zmiany tekstur:
	
	* po nazwie.


