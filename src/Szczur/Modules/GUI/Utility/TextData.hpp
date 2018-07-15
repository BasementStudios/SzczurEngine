#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

namespace rat{
namespace gui{
    struct TextData
    {
        TextData() = default;
        TextData(const sf::Text& base);

        void applyTo(sf::Text& text);

        float outlineThickness{0.f};
        sf::Color outlineColor{0u, 0u, 0u, 0u};
        const sf::Font* font{nullptr};
        sf::Color color{255, 255, 255, 255};
        size_t characterSize{0u};
    };

}

}