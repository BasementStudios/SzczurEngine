#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "NinePatch.hpp"
#include "Patch.hpp"
#include "Scroller.hpp"

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
        float _prop{1.f}; 
        float _shift{0.7f};
        sf::Vector2f _size{30, 300};

        NinePatch testPatch; 
        Scroller scroller;
    };

}