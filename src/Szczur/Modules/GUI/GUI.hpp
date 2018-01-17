#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Utility/Modules.hpp"
#include "Szczur/Modules/Canvas/Canvas.hpp"
#include "Szczur/Modules/Assets/Assets.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Json.hpp"

#include "Widget.hpp"
#include "GuiJson.hpp"
#include "GuiAssetsManager.hpp"

 
namespace rat {
    class GUI : public Module<Canvas, Assets, Input, Window> { 
    public:
        template<typename Tuple>
        GUI(Tuple&& tuple) :
        Module(tuple) {
            _assets.loadFromFile<sf::Texture>("data/button.png");
            _assets.loadFromFile<sf::Texture>("data/button-active.png");
            _assets.loadFromFile<sf::Texture>("data/button-clicked.png");
            _assets.loadFromFile<sf::Texture>("data/check.png");
            _assets.loadFromFile<sf::Texture>("data/check-on.png");
            _assets.loadFromFile<sf::Font>("data/consolab.ttf");
            _assets.loadFromFile<sf::Texture>("data/button.png");
            _assets.loadFromFile<Json>("data/json.json");
            _guiJson.init(_assets.get<Json>("data/json.json"), &_assets, &_root, getModule<Window>().getWindow().getSize());
        }

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