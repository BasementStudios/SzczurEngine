#pragma once

#include <SFML/System/Vector2.hpp>

namespace rat
{
    class GUIInterface
    {
    public:
        void updateSizeByWindowSize(const sf::Vector2u& winSize);
        const sf::Vector2f& getSize() const;
        void setWidthToHeightProportion(float prop);
    private:
        sf::Vector2f _size;
        bool _hasProportion{false};
        float _widthToHeightProp{16.f/9.f};

    };
}

