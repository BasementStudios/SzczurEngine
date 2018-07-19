#pragma once

#include "Szczur/Utility/Modules/Module.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"

#include "NinePatch.hpp"
#include "Patch.hpp"
#include "Scroller.hpp"

namespace rat
{
    class WindowWidget; class ImageWidget; class Widget; class ScrollAreaWidget; class ListWidget; class TextWidget; class TextAreaWidget;
    class GUITest : public Module<Input, Window, GUI> 
    {
    public:
        GUITest();
        void init();
        void update(float deltaTime = (1.f/60.f));
        void render();
        void input(const sf::Event& event);
        ~GUITest() { /*delete widget;*/ }
    private:
        sf::RenderTexture _canvas;   

        float _scale{1.f}; 
        float _prop{0.f}; 
        float _shift{0.7f};
        sf::Vector2f _size{30, 30.f};

        bool randomBool{false};

        ImageWidget* image{nullptr};
        InterfaceWidget* _widget{nullptr};
        ImageWidget* _imageWidget{nullptr};
        ListWidget* list{nullptr};
        TextAreaWidget* fps{nullptr};
        ScrollAreaWidget* scroll{nullptr};
    };

}