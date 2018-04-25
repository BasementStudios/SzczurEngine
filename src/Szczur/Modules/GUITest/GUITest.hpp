#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

namespace rat
{
    class GUITest : public Module<Input, Window, Script> 
    {
    public:
        GUITest();
        void init();
        void update(float deltaTime = (1.f/60.f));
        void render();
    private:
        sf::RenderTexture _canvas;    
    };

}