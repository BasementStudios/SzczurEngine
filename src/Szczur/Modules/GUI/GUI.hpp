#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"

namespace rat {
    class GUI : public ModuleBase<Canvas> { using ModuleBase::ModuleBase;
    public:
        void init();
        void update(float deltaTime);
        void render();
    private:
        sf::RectangleShape _shape;

    };
}