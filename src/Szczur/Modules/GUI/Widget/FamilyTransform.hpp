#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace rat
{
namespace gui
{
    class FamilyTransform
    {
    public:
        void setPosition(const sf::Vector2f& position);
        void setPosition(float x, float y);
        const sf::Vector2f& getPosition() const;

        void setSize(const sf::Vector2f& size);
        void setSize(float width, float height);
        const sf::Vector2f& getSize() const;

        void setOrigin(const sf::Vector2f& origin);
        void setOrigin(float x, float y);
        const sf::Vector2f& getOrigin() const;

        const sf::Vector2f& getGlobalPosition() const;
        const sf::Vector2f& getDrawPosition() const;


        void applyParentPosition(const sf::Vector2f& globalPosition, const sf::Vector2f& drawPos);

        bool isPointIn(const sf::Vector2f& point) const;
    private:
        sf::Vector2f _pos;
        sf::Vector2f _origin;

        sf::Vector2f _globalPos;
        sf::Vector2f _drawPos;
        sf::Vector2f _size;
    };
}
}