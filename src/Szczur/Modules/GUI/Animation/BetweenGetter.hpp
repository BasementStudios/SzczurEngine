#pragma once

#include <algorithm>
#include <cmath>

#include <string>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace rat
{
namespace gui
{
    template <typename T>
    struct BetweenGetter
    {
        T operator()(const T& from, const T& to, float t)
        {
            return (to - from) * t + from;
        }
    };

    template <>
    struct BetweenGetter<sf::Color>
    {
        sf::Color operator()(const sf::Color& from, const sf::Color& to, float t)
        {
            float r = (float(to.r) - float(from.r)) * t + float(from.r);
            float g = (float(to.g) - float(from.g)) * t + float(from.g);
            float b = (float(to.b) - float(from.b)) * t + float(from.b);

            return {
                static_cast<sf::Uint8>(std::clamp(r, 0.f, 255.f)),
                static_cast<sf::Uint8>(std::clamp(g, 0.f, 255.f)),
                static_cast<sf::Uint8>(std::clamp(b, 0.f, 255.f))
            };
        }
    };

    template <>
    struct BetweenGetter<sf::FloatRect>
    {
        sf::FloatRect operator()(const sf::FloatRect& from, const sf::FloatRect& to, float t)
        {
            return {{
                (to.left - from.left) * t + from.left,
                (to.top - from.top) * t + from.top
            }, {
                (to.width - from.width) * t + from.width,
                (to.height - from.height) * t + from.height
            }};
        }
    };

    template <>
    struct BetweenGetter<std::string>
    {
        std::string operator()(const std::string& from, const std::string& to, float t)
        {
            if(notStarted)
            {
                notStarted = false;
                prop = float(from.size()) / float(from.size() + to.size());
                inverProp = 1.f / prop;
                eInverProp = 1.f / (1.f - prop);
                maxSI = float(from.size()) - 1.f;
                maxEI = float(to.size()) - 1.f;
            }

            if(t < prop)
            {
                int index = int( maxSI * (1.f - t * inverProp) );
                index = std::clamp(index, 0, int(maxSI));
                return {from.begin(), from.begin() + index};
            }
            else
            {
                int index = int((t - prop) * eInverProp * maxEI);
                index = std::clamp(index, 0, int(maxEI));
                return {to.begin(), to.begin() + index};
            }
        }

    private:
        bool notStarted{true};
        float prop{1.f};
        float inverProp{1.f};
        float eInverProp{1.f};

        float maxSI;
        float maxEI;
    };
}
}