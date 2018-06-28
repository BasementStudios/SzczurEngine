#include "FamilyTransform.hpp"

namespace rat
{
namespace gui
{
    void FamilyTransform::setPosition(const sf::Vector2f& position)
    {
        _globalPos -= _pos;
        _drawPos -= _pos;

        _pos = position;

        _globalPos += _pos;
        _drawPos += _pos;
    }
    void FamilyTransform::setPosition(float x, float y)
    {
        setPosition({x, y});
    }
    const sf::Vector2f& FamilyTransform::getPosition() const
    {
        return _pos;
    }

    void FamilyTransform::setSize(const sf::Vector2f& size)
    {
        _size = size;
    }
    void FamilyTransform::setSize(float width, float height)
    {
        setSize({width, height});
    }
    const sf::Vector2f& FamilyTransform::getSize() const
    {
        return _size;
    }

    void FamilyTransform::setOrigin(const sf::Vector2f& origin)
    {
        _globalPos += _origin;
        _drawPos += _origin;

        _origin = origin;

        _globalPos -= _origin;
        _drawPos -= _origin;
    }
    void FamilyTransform::setOrigin(float x, float y)
    {
        setOrigin({x, y});
    }
    const sf::Vector2f& FamilyTransform::getOrigin() const
    {
        return _origin;
    }

    void FamilyTransform::applyParentPosition(const sf::Vector2f& globalPosition, const sf::Vector2f& drawPos)
    {
        _globalPos = globalPosition + _pos - _origin;
        _drawPos = drawPos + _pos - _origin;
    }

    bool FamilyTransform::isPointIn(const sf::Vector2f& point) const
    {
        return  point.x > _globalPos.x &&
                point.x < _globalPos.x + _size.x &&
                point.y > _globalPos.y &&
                point.y < _globalPos.y + _size.y; 
    }

    const sf::Vector2f& FamilyTransform::getGlobalPosition() const
    {
        return _globalPos;
    }
    const sf::Vector2f& FamilyTransform::getDrawPosition() const
    {
        return _drawPos;
    }
}
}