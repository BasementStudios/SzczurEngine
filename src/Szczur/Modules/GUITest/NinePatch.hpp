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

        void setTexture(sf::Texture* texture);
        void setTexture(sf::Texture* texture, int padding);
        void setTexture(sf::Texture* texture, int paddingWidth, int paddingHeight);
        void setTexture(sf::Texture* texture, const sf::IntRect& rect);

        void setSize(int x, int y);
        /*void setPosition(float x, float y);*/

        void setInnerSize(const sf::IntRect& rect);
        void setInnerSize(int x, int y, int width, int height);

        void setScale(const sf::Vector2f& scale);
        void setScale(float x, float y);

    private:
        sf::Vector2i _size{0, 0};
        bool _isSizeSet{false};

        sf::Texture* _texture{nullptr};
        sf::IntRect _innerRect{0, 0, 0, 0};
        bool _isInnerRectSet{false};

        int _innerWidthBarTimes{0};
        float _innerWidthBar{0.f};

        int _innerHeightTimes{0};
        float _innerHeight{0.f};

        /*sf::Vector2f _position{0.f, 0.f};*/

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