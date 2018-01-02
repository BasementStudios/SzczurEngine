#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"

namespace rat {
    class HUD : Module<Canvas>, Module<>::Inputable, Module<>::Renderable, Module<>::Updatable {
        using Module::Module;
    public:
        void init();
        void input(const sf::Event& event);
        void update(float deltaTime);
        void render();
    private:

    };
}