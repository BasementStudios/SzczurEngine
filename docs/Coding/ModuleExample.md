
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
public:

    template <typename Tuple>
    YourModule(Tuple&& tuple, /* your ctor args */);

};

template <typename Tuple>
YourModule::YourModule(Tuple&& tuple, /* your ctor args */) :
    Module(tuple), /* your ctor initializer list */
{

}

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
_modules.getModule<ModuleName>() // w klasie Apllication
```
```cpp
getModule<ModuleName>() // w środku innego modułu
```



### Uwagi

Kolega `Patryk (Stritch)` powinien udzielić więcej informacji, jeśli wystąpią jakieś większe trudności.
