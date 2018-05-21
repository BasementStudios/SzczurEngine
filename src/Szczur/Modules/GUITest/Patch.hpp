#pragma once

#include <SFML/Graphics.hpp>

namespace rat
{
    class Patch : public sf::Drawable, public sf::Transformable
    {
    public:
        enum class Direction{ None, Vertical, Horizontal, All };

        Patch() = default;
        Patch(Direction direction);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;       

        void setSize(int x, int y);
        void setSize(sf::Vector2i size);
        void setWidth(int width);
        void setHeight(int height);
        void setTexture(const sf::Texture* texture);
        void setTextureRect(const sf::IntRect& rect);
        void setScale(float x, float y);
        void setScale(const sf::Vector2f& scale);
        void setDirection(Direction direction);

        const sf::Vector2i& getSize() const;
        sf::Vector2i getElementSize() const;


    private:
        Direction _direction{Direction::Horizontal};

        mutable sf::Sprite _sprite;
        const sf::Texture* _texture{nullptr}; 
        sf::Vector2i _size{0, 0};
        sf::Vector2u _textureRect;
        sf::Vector2i _elementAmount{1, 1};
        sf::Vector2f _elementDim{0.f, 0.f};
        sf::Vector2f _scale{1.f, 1.f};

        bool _isSizeSet{false};
        bool _isTextureRectSet{false};

        void _recalcTexturesAreas();
        void _recalcSpriteSize();
        void _recalcSpritePos() const;
        void _recalcRecurrence();

    };
}