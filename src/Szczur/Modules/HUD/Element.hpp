#pragma once

#include <SFML/Graphics.hpp>

namespace rat {
    class Element : public sf::Drawable, public sf::Transformable {
    public:
        Element(const std::string& file, float radius, sf::Color color);

        void setTargetRadius(float value);
        void setTargetPosition(sf::Vector2f value);

        void moveTargetPosition(sf::Vector2f offset);

        void update(float deltaTime);

        Element* kill();

        bool isDead() const;
    private:
        sf::CircleShape _background;
        sf::Sprite _icon;

        sf::Texture _texture;

        bool _followPosition;
        bool _followRadius;
        bool _ready;
        bool _isDead;
        bool _aboutToDie;
        float _targetRadius;
        sf::Vector2f _targetPosition;

        void _followTargetRadius(float deltaTime);
        void _followTargetPosition(float deltaTime);

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}