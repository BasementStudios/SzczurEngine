#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Json.hpp"

#include "Widget.hpp"
#include "GuiJson.hpp"
#include "GuiAssetsManager.hpp"

 
namespace rat {
    class GUI : public Module<Canvas>, Module<>::Inputable, Module<>::Renderable, Module<>::Updatable { 
        using Module::Module;
    public:
        void init();
        void input(const sf::Event& event);
        void update(float deltaTime);
        void render();
        void reload();
    private:
        Widget _root;
        GuiJson _guiJson;
        BasicGuiAssetsManager _assets;
    };
}