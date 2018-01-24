#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class DialogStruct {
    public:
        DialogStruct();
        ~DialogStruct();

        std::string* add(std::string* text);

        std::string* next();

        void restart();
    private:
        std::vector<std::string*> _texts; 
        std::vector<std::string*>::iterator _current;
    };
}