#pragma once

#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class Widget;

    class PrepScreen2 : public Module <Window, Input, Script, GUI> 
    {
    public:
        PrepScreen2();
        void init();

    private:
        Widget* _base{nullptr};
    };
}