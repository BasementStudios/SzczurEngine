#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "NinePatch.hpp"
#include "Patch.hpp"

namespace rat
{
    class GUITest : public Module<Input, Window, GUI> 
    {
    public:
        GUITest();
        void init();
        void update(float deltaTime = (1.f/60.f));
        void render();
    private:
        sf::RenderTexture _canvas;   

        float _scale{1.f}; 

        NinePatch testPatch; 
    };

}