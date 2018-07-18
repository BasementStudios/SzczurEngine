#pragma once

#include <SFML/System/Vector2.hpp>

namespace rat
{
    struct ProportionalDimes
    {
        bool hasSize{false};
        sf::Vector2f size;

        bool hasPosition{false};
        sf::Vector2f position;

        bool hasOrigin{false};
        sf::Vector2f origin;
    };
}