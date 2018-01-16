#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Json.hpp"

#include "Widget.hpp"
#include "GuiJson.hpp"
#include "GuiAssetsManager.hpp"

 
namespace rat {
    class GUI;

    template<typename T>
    class _GUI : public Module<T> { 
    public:
        _GUI();
        void input(const sf::Event& event);
        void update(float deltaTime);
        void render();
        void reload();
    private:
        Widget _root;
        GuiJson _guiJson;
        BasicGuiAssetsManager _assets;
    };
    REGISTER_GAME_MODULE(GUI, _GUI);
}

#include "GUI.tpp"