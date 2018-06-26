#include "FamilyTransform.hpp"

namespace rat
{
namespace GUI
{
    void FamilyTransform::setPosition(const sf::Vector2i& position)
    {
        _globalPos -= _absPos;
        _drawPos -= _absPos;

        _absPos = position;

        _globalPos += _absPos;
        _drawPos += _absPos;
    }
    void FamilyTransform::setPosition(int x, int y)
    {
        setPosition({x, y});
    }
    const sf::Vector2i FamilyTransform::getPosition() const
    {
        return _absPos;
    }

    void FamilyTransform::setSize(const sf::Vector2i& size)
    {
        _size = size;
    }
    void FamilyTransform::setSize(int width, int height)
    {
        setSize({width, height});
    }
    const sf::Vector2i& FamilyTransform::getSize() const
    {
        return _size;
    }

    void FamilyTransform::setOrigin(const sf::Vector2i& origin)
    {
        _globalPos += _origin;
        _drawPos += _origin;

        _origin = origin;

        _globalPos -= _origin;
        _drawPos -= _origin;
    }
    void FamilyTransform::setOrigin(int x, int y)
    {
        setOrigin({x, y});
    }
    const sf::Vector2i& FamilyTransform::getOrigin() const
    {
        return _origin;
    }

    void FamilyTransform::applyParentPosition(const sf::Vector2i& globalPosition, const sf::Vector2i& drawPos)
    {
        _globalPos = globalPosition;
        _globalPos += _absPos;
        _globalPos -= _origin;

        _drawPos = drawPos;
        _drawPos += _absPos;
        _drawPos -= _origin;

    }

    bool FamilyTransform::isPointIn(const sf::Vector2i& point) const
    {
        return  point.x > _globalPos.x &&
                point.x < _globalPos.x + _size.x &&
                point.y > _globalPos.y &&
                point.y < _globalPos.y + _size.y; 
    }

    const sf::Vector2i& FamilyTransform::getAbsolutePosition() const
    {
        return _absPos;
    }
    const sf::Vector2i& FamilyTransform::getDrawPosition() const
    {
        return _drawPos;
    }
}
}