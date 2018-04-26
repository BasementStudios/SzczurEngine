#pragma once

#include <SFML/Graphics.hpp>

namespace rat
{
    class Patch
    {
    public:
        enum class Direction{ None, Vertical, Horizontal, All };

        

        void draw(sf::RenderTarget& target) const;        

        void setSize(int x, int y);
        void setSize(sf::Vector2i size);
        void setPosition(float x, float y);
        void setTexture(sf::Texture* texture);
        void setTextureRect(const sf::IntRect& rect);
        void setScale(float x, float y);
        void setDirection(Direction direction);


    private:
        Direction _direction{Direction::Horizontal};

        mutable sf::Sprite _sprite;
        sf::Texture* _texture{nullptr}; 
        sf::Vector2i _size{0, 0};
        sf::Vector2f _position{0.f, 0.f};
        sf::Vector2u _textureRect;
        int _elementAmount{1};
        sf::Vector2i _elementDim{0, 0};
        sf::Vector2f _scale{1.f, 1.f};

        bool _isSizeSet{false};
        bool _isTextureRectSet{false};

        void _recalcTexturesAreas();
        void _recalcSpriteSize();
        void _recalcSpritePos() const;
        void _recalcRecurrence();

    };
}