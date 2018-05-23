#pragma once

#include "TransformAnimationBase.hpp"

#include <array>

#include <SFML/Graphics.hpp>

namespace rat
{
    using ColorArray_t = std::array<sf::Uint8, 3>;
    class ColorAnim : public TransformAnimationBase
    {
    public:
        ColorAnim();
        virtual void update(float dt) override;
        void setAnim(const sf::Color fromValue, const sf::Color toValue, float inTime);
        sf::Color getActualColor() const;

    private:
        ColorArray_t _start;
        ColorArray_t _end;

        ColorArray_t fromColorToArray(const sf::Color color) const;
        sf::Color fromArrayToColor(const ColorArray_t& color) const;
    };
}