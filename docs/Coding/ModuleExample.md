
Przykład modułu
---------------



### Plik główny modułu

Każdy moduł powinien mieć swój główny plik z główną klasą modułu, np. `YourModule.hpp`, w odpowiednim dla modułu katalogu, czyli `src/Szczur/Modules/` i folder o nazwie jak moduł: `YourModule`.

```cpp
#include "Szczur/Utility/Module.hpp"

namespace rat
{

class YourModule : public Module<Dependencies...>
{
	// code
};

}
```



### Lista modułów

Aby moduł został załadowany, powinien zostać dodany do listy modułów w `Application.hpp`.

```cpp
#include "Szczur/Modules/YourModule/YourModule.hpp"

namespace rat
{

class Application
{
private:

	ModulesHolder</* other modules */, YourModule> _modules;

};

}
```



### Użycie

```cpp
initModule<ModuleName>(/* your ctor args */) // tworzenie modułu w klasie Application
```
```cpp
getModule<ModuleName>() // getter w klasie Application
```
```cpp
getModule<ModuleName>() // getter w środku innego modułu
```
```cpp
detail::globalPtr<ModuleName> // tylko w celach TESTÓW!
```



### Uwagi

Kolega `Patryk (Stritch)` powinien udzielić więcej informacji, jeśli wystąpią jakieś większe trudności.
