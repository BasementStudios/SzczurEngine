#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace rat
{
namespace GUI
{
    class FamilyTransform
    {
    public:
        void setPosition(const sf::Vector2i& position);
        void setPosition(int x, int y);
        const sf::Vector2i getPosition() const;

        void setSize(const sf::Vector2i& size);
        void setSize(int width, int height);
        const sf::Vector2i& getSize() const;

        void setOrigin(const sf::Vector2i& origin);
        void setOrigin(int x, int y);
        const sf::Vector2i& getOrigin() const;

        const sf::Vector2i& getAbsolutePosition() const;
        const sf::Vector2i& getDrawPosition() const;


        void applyParentPosition(const sf::Vector2i& globalPosition, const sf::Vector2i& drawPos);

        bool isPointIn(const sf::Vector2i& point) const;
    private:
        sf::Vector2i _absPos;
        sf::Vector2i _origin;

        sf::Vector2i _globalPos;
        sf::Vector2i _drawPos;
        sf::Vector2i _size;
    };
}
}