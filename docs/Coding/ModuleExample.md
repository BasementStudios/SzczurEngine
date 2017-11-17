
Przykład modułu
---------------



### Plik główny modułu

Każdy moduł powinien mieć swój główny plik z główną klasą modułu, np. `YourModule.hpp`, w odpowiednim dla modułu katalogu, czyli `src/Szczur/Modules/` i folder o nazwie jak moduł: `YourModule`.

```cpp
#include "Szczur/Utility/Modules.hpp"

namespace rat {
    class YourModule : public ModuleBase<Dependencies...> 
	{
		using ModuleBase::ModuleBase;
		
    private:
        
        // ...
        
    public:
        
        // Nie tworzyć własnego konstruktora!
        
        void init(); // jeśli chcemy zasymulować konstruktor
        
        void input(const sf::Event& event); // jeżeli chcemy obsłużyć eventy
        
        void update(float deltaTime); // logika modułu
        
        void render(); // render modułu
    };
}
```



### Lista modułów

Aby moduł został załadowany, powinien zostać dodany do listy modułów w `Application.hpp`.

```cpp
#include "Szczur/Modules/YourModule/YourModule.hpp"

namespace rat {
    class Application {
    private:

        ModulesHolder<
            // dopisać do listy
            YourModule
        > _modules;
    
        // ...
   };
}
```



### Uwagi

Kolega `Stritch` powinien udzielić więcej informacji, jeśli wystąpią jakies większe trudności.


