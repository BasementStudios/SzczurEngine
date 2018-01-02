#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class Element : public sf::Drawable, public sf::Transformable {
    public:
        Element(const std::string& file, float radius, sf::Color color);

        void update(float deltaTime);
    private:
        sf::CircleShape _background;
        sf::Sprite _icon;

        sf::Texture _texture;

        bool _isReady;
        float _radius;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}