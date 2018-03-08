I. Moduły

1. Nowy moduł.

```cpp
class NewModule : public Module<...> 
{
// ...
public:

// ...
	template <typename Tuple>
	NewModule(Tuple&& tuple) : Module(tuple) {
		LOG_CONSTRUCTOR();
		initScript(); // <<<< inicjacja modułu do skryptu
		init();
	}
	
// ...	
	// Jeżeli do modułu NewModule należą klasy ClassA, ClassB i ClassC
	// to metoda initScript będzie wyglądać następująco
	void initScript() {
		using namespace new_module; // <<<< o ile takowy namespace istnieje
		Script& script = getModule<Script>();
		auto module = script.newModule("BattleField"); // <<<< Ta zmienna MUSI nazywać się "module"
		
		/* Obszar dodawania funkcjonalności */
		// np. (więcej o tym makro niżej)
		SCRIPT_SET_MODULE(NewModule, getPos, setPos, getSize, setSize)
		
		// Inicjacja klas modułu (zawierających metodę statyczną "initScript")
		script.initClasses<ClassA,  ClassB, ClassC>();
	}
};
```

2. Dostępne opcje dodawania funkcjonalności.

a) Makro SCRIPT_SET_MODULE

```cpp
SCRIPT_SET_MODULE(NewModule, a, b, c, ...)
SCRIPT_SET_MODULE(NewModule, d, e)

// gdzie:
// a, b, c, d, e, ... - argumenty w postaci metod i TYLKO metod (max 16 na makro)

```
Tak naprawdę makro `SCRIPT_SET_MODULE(NewModule, a, b, c)`
wywołuje takie instrukcje:
```cpp
module.set_function("a", &NewModule::a, this);
module.set_function("b", &NewModule::b, this);
module.set_function("c", &NewModule::c, this);
```

b) set_function (dla lambd)

```cpp
module.set_function("functionName", [&](int a, int b){return a+b;});
```

3. Przeciążanie nazw metod o różnych i takich samych nazwach.

Przykłady opracowane dla metod:
```cpp
void M::setValue(int value);
void M::setValue(const string &value);
void M::setValueFromInt(int value));
void M::setValueFromString(const string &value);
```

Możliwe kombinacje:
```cpp
// A
module.set_function("setValue", sol::overload(&M::setValueFromInt, &M::setValueFromString), this);
// B
module.set_function("setValue", sol::overload(sol::resolve<void(int)>(&M::setValue), &M::setValueFromString), this);
// C
module.set_function("setValue", sol::overload(sol::resolve<void(int)>(&M::setValue), sol::resolve<void(const string&)>(&M::setValue)), this);
```

II. Klasy

1. Nowa klasa.

```cpp
class NewClass
{
//...
public:
//...
	static void initScript(Script& script) {
		// 1. Zmienna poniżej MUSI nazywać się "object"
		// 2. Trzeci argument jest nieobowiązkowy (jest to ścieżka do pliku, który ma się wykonać podczas inicjalizacji klasy)
		auto object = script.newClass<NewClass>("NewModule", "NewClass", "../file.lua");
		
		/* Obszar dodawania funkcjonalności */
		SCRIPT_SET_CLASS(MyClass, field_1, setField_1, getField_1, setField_2, getField_2, getField_3) // Podobnie jak wcześniej
		
		// Makery (nieobowiązkowe)
		object.makeInstance();
		object.makeStatter();

		// Inicjacja nowej klasy do skryptu
		object.init();
	}
};
```

2. Dostępne opcje dodawania funkcjonalności.

a) Marko `SCRIPT_SET_CLASS`
```cpp
SCRIPT_SET_CLASS(MyClass, a, setA, getA, setB)
SCRIPT_SET_CLASS(MyClass, getB, getC, ...)
// gdzie:
// a, setA, getA, setB, getB, getC, ... - składowe i metody MyClass (max 16 na makro)
```
Tak naprawdę makro `SCRIPT_SET_CLASS(NewModule, a, b, c)`
wywołuje takie instrukcje:
```cpp
module.set("a", &NewClass::a);
module.set("b", &NewClass::b);
module.set("c", &NewClass::c);
```

b) Metoda `set`
Przykłady:
```cpp
// A
object.set("size", size); // size musi być publiczne
// B
object.set("getSize", &NewClass::getSize); // nie ma znaczenia czy metoda coś zwraca
// C
object.set("createObject", &NewClass::createObject); // dla statycznych też działa
// D
object.set("hello", [](){std::cout<<"Hello world!";}, ); // lambdy nie straszne
```

c) Metoda 'setProperty'
Przykłady:
```cpp
// A
object.setProperty("size", &NewClass::getSize, &NewClass::setSize) // args: name, getter, setter
// przykład użycia w skrypcie: 
// obj.size = 5; print(obj.size); // UWAGA! Mimo, że wywołujemy metody nie używamy ':' tylko '.'
// B
object.setProperty("flag", [](NewClass &obj){return obj.flag;}, [](NewClass &obj, bool value){obj.setFlag(value);});
// przykład użycia w skrypcie: 
// obj.flag = true; print(obj.flag);
```

d) Metoda 'setOverload'
Przykłady:
```cpp
// A
object.setOverload("setType", &NewClass::setTypeFormInt, &NewClass::setTypeFormString, &NewClass::setTypeFormEnemy, ...)
// ... - możliwość podania nieskończonej liczby argumentów

// B
object.setOverload("set", &NewClass::set<int>, &NewClass::set<const string&>);
// Funkcje szablonowe też są możliwe do oskryptowania, ale należy zainicjować każdą z osobna
```

3. Przeciążanie nazw metod o różnych i takich samych nazwach.

Przykłady opracowane dla metod:
```cpp
void M::setValue(int value);
void M::setValue(const string &value);
void M::setValueFromInt(int value));
void M::setValueFromString(const string &value);
```

Możliwe kombinacje:
```cpp
// A
object.set("setValue", sol::overload(&M::setValueFromInt, &M::setValueFromString), this);
// B
object.set("setValue", sol::overload(sol::resolve<void(int)>(&M::setValue), &M::setValueFromString), this);
// C
object.set("setValue", sol::overload(sol::resolve<void(int)>(&M::setValue), sol::resolve<void(const string&)>(&M::setValue)), this);
```
Rozwiązań wykorzystujących `sol::resolve` można używać również w innych metodach np. setProperty, setOverload.
UWAGA! W `setProperty` NIE WOLNO używać `sol::overload`, property nie jest przeciążalne.

4. Maker: Automatyczny `instance`

Po dodaniu linijki `object.makeInstance();`,
W skrypcie będzie możliwe stworzenie instancji danej klasy w taki sposób:
```lua
-- UWAGA! Wymagany konstruktor bezargumentowy.
local obj = Module.Class.instance()
```

Dodatkowo jeżeli (już bezpośrednio w skrypcie) stworzymy metodę `_instance` wewnątrz naszej klasy 
to wywoła się ona zawsze wraz z wywołaniem metody `instance`.

5. Maker: Automatyczny `stat`
Po dodaniu linijki `object.makeStatter();`,
jeżeli w klasie istnieje specjalna metoda statyczna `setStat`,
możliwe będzie ustawienie wielu statusów jednocześnie (takie jakby "multi propreties").

Przykład:
```cpp
// Metoda statyczna
void NewClass::setStat(sol::table stat) { // Tablice w lua przesyłamy przez "kopię" (w rzeczywistości to jest referencja)
	if(stat["position"].valid()) setPosition(stat["position"][1], stat["position"][2]);
	if(stat["size"].valid()) { size.x = stat["size"][1]; size.y = stat["size"][2]; }
	if(stat["hp"].valid()) setHp(stat["hp"]);
}
```

Przykład użycia:
```lua
local obj = Module.Class.instance()
obj.stat = {
	hp = 20; -- możliwa dowolna kolejność
	position = {40, 70}
	-- nie trzeba używać wszystkich możliwych statusów
}
```