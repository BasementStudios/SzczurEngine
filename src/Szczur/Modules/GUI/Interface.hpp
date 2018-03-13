#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Json.hpp"

#include "Widget.hpp"
#include "GuiAssetsManager.hpp"
#include "GuiJson.hpp"

namespace rat {
    class Interface : public sf::Drawable {
    public:
        Interface(BasicGuiAssetsManager* assets, sf::Vector2u windowSize);
        Interface(BasicGuiAssetsManager* assets, sf::Vector2u windowSize, const std::string& jsonFile);

        Widget* add(Widget* widget);

        void input(const sf::Event& event);
        void update(float deltaTime);

        void reload(sf::Vector2u newSize);
    private:
        Widget _root;
        BasicGuiAssetsManager* _assets;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}