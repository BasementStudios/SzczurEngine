#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "Szczur/Modules/GUI/TransformAnimBasics/ColorAnim.hpp"

#include "NinePatch.hpp"
#include "Patch.hpp"
#include "Scroller.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

namespace rat
{
    class GUITest : public Module<Input, Window, GUI> 
    {
        using Tab_t = TransformAnimationBase;
    public:
        GUITest();
        void init();
        void update(float deltaTime = (1.f/60.f));
        void render();
        ~GUITest() { /*delete widget;*/ }
    private:
        sf::RenderTexture _canvas;   

        float _scale{1.f}; 
        float _prop{0.f}; 
        float _shift{0.7f};
        sf::Vector2f _size{30, 30.f};

        NinePatch testPatch; 
        Scroller scroller;
        Widget* widget{nullptr};
        Widget* widget3{nullptr};
        ImageWidget* iWidget{nullptr};
        ScrollAreaWidget* scroll{new ScrollAreaWidget};
        WindowWidget* win{new WindowWidget};

        ImageWidget* blue{nullptr};
        ImageWidget* red{nullptr};

        Tab_t*_animColor{new ColorAnim};
        sf::RectangleShape shape;

        bool hasBeenChanged{false};
    };

}