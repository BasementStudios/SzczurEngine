#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"

#include "Widget.hpp"
#include "TextWidget.hpp"

namespace rat {
    class GUI : public ModuleBase<Canvas> { using ModuleBase::ModuleBase;
    public:

        void init();
        void input(sf::Event event);
        void update(float deltaTime);
        void render();
    private:

        Widget _root;
    };
}