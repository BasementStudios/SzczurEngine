#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"


namespace rat 
{
    class QuestLog : public Module<Input, Window, Script, GUI> 
    {
    public:
        QuestLog();
        void init();

        void update(float dt);
        void render();
    };

}