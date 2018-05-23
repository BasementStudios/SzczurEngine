#pragma once 

#include <vector>

#include "Patch.hpp"

#include <SFML/Graphics.hpp>

namespace rat
{
    class NinePatch : public sf::Drawable, public sf::Transformable
    {
    public:
        NinePatch();

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setTexture(const sf::Texture* texture);
        void setTexture(const sf::Texture* texture, int padding);
        void setTexture(const sf::Texture* texture, int paddingWidth, int paddingHeight);
        void setTexture(const sf::Texture* texture, const sf::IntRect& rect);

        void setSize(const sf::Vector2i& size);
        void setSize(int x, int y);
        sf::Vector2i getSize() const;

        void setInnerSize(const sf::IntRect& rect);
        void setInnerSize(int x, int y, int width, int height);
        sf::IntRect getInnerTextureRect() const;

        void setScale(const sf::Vector2f& scale);
        void setScale(float x, float y);
        //sf::Vector2f getScale() const;

        sf::Vector2i getInnerPathSize() const;
        sf::Vector2i getInnerSize() const;

        void setColor(const sf::Color& color);

        sf::Vector2i getCornersCombinedSize() const;

    private:
        sf::Vector2i _size{0, 0};
        bool _isSizeSet{false};

        const sf::Texture* _texture{nullptr};
        sf::IntRect _innerRect{0, 0, 0, 0};
        bool _isInnerRectSet{false};

        int _innerWidthBarTimes{0};
        float _innerWidthBar{0.f};

        int _innerHeightTimes{0};
        float _innerHeight{0.f};

        Patch _topBar;
        Patch _bottomBar;
        Patch _leftBar;
        Patch _rightBar;

        Patch _topLeftCorner;
        Patch _topRightCorner;
        Patch _bottomLeftCorner;
        Patch _bottomRightCorner;

        Patch _inner;

        void _recalcTexturesAreas();
        void _recalcSpritesSize();
        void _recalcSpritesPos();
    };
}