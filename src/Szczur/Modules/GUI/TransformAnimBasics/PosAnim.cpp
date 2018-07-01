#include "PosAnim.hpp"

namespace rat
{

    PositionAnim::PositionAnim()
    {
        _setType(Types::Pos);
    }
    void PositionAnim::update(float dt)
    {
        _advanceTime(dt);
    }
    void PositionAnim::setAnim(const sf::Vector2f& fromValue, const sf::Vector2f& toValue, float inTime)
    {
        _start = fromValue;
        _end = toValue;
        _setHoldTime(inTime);
    }
    sf::Vector2f PositionAnim::getActualPos() const
    {
        return _start + ((_end - _start) * _getTimeProp());
    }
}
