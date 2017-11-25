#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"

#include "Widget.hpp"
#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "InputWidget.hpp"
#include "CheckWidget.hpp"

 
namespace rat {
    class GUI : public ModuleBase<Canvas> { using ModuleBase::ModuleBase;
    public:

        void init();
        void input(const sf::Event& event);
        void update(float deltaTime);
        void render();
    private:

        Widget _root;
    };
}