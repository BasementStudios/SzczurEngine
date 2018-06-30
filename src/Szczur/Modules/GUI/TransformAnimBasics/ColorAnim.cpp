#include "ColorAnim.hpp"

namespace rat
{
    ColorAnim::ColorAnim()
    {
        _setType(Types::Color);
    }
    void ColorAnim::update(float dt)
    {
        _advanceTime(dt);
    }
    void ColorAnim::setAnim(const sf::Color fromValue, const sf::Color toValue, float inTime)
    {
        _start = fromColorToArray(fromValue);
        _end = fromColorToArray(toValue);
        _setHoldTime(inTime);
    }
    sf::Color ColorAnim::getActualColor() const
    {
        std::array<float, 3u> diff;
        for(size_t i = 0; i < 3; i++)
        {
            diff[i] = float(_end[i]) - float(_start[i]);
        }
        ColorArray_t c;
        for(size_t i = 0; i < 3; i++)
        {
            auto addon = sf::Uint8(_getTimeProp() * float(diff[i]));
            c[i] = sf::Uint8(float(_start[i]) + addon);
        }
        return fromArrayToColor(c);
    }

    std::array<sf::Uint8, 3> ColorAnim::fromColorToArray(const sf::Color color) const
    {
        std::array<sf::Uint8, 3> ar;
        ar[0] = color.r; 
        ar[1] = color.g;
        ar[2] = color.b;
        return std::move(ar);
    }
    sf::Color ColorAnim::fromArrayToColor(const ColorArray_t& color) const
    {
        sf::Color c;
        c.r = color[0];
        c.g = color[1];
        c.b = color[2];
        return c;
    }
    
}