
Modułowość
==========





Założenia
---------

### Strukturalna zależność

* **Jądro silnika jest zawsze nadrzędnym "modułem"** (_właściwie nazwanie tego modułem to tylko dla zaobrazowania istoty_), pisane ostrożnie oraz jest niby niezależne.

* **Moduł nadrzędny - dostarcza podrzędnym** jakieś rzeczy, funkcje, dane, interfejsy, miejsca pod komponenty.

* **Moduł porzędny względem któregoś - zależeny od danego**, wymaga istnienia i działania takowego, bo korzysta z dostarczonych rzeczy. 
	
Np. podrzędny jakiś `ItemsWindow` od jakiegoś `IngameMenu`, bo np. używa funkcji `IngameMenu.pushButton()`, żeby dodać np. wyświetlenie swojego okna po kliknięciu przycisku w tym menu.
	
* Każdy moduł powinien być możliwy do łatwego usunięcia/dodania, przy czym nie powinien destabilizować całego silnika (np. zmuszając do wielokrotnych edycji kodu), a jedynie niepozwalać się załadować innym, które z niego wyłączają.

Współny plik ładujący, swego rodzaju Initializer, gdze tylko - jest dobrym rozwiązaniam.



### @TODO 




Struktura
---------

```
				Core
				  |
```

