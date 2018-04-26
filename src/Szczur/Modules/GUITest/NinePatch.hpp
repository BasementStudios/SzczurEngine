#pragma once 

#include <vector>

#include <SFML/Graphics.hpp>

namespace rat
{
    class NinePatch
    {
    public:
        NinePatch();

        void draw(sf::RenderTarget& target);

        void setTexture(sf::Texture* texture);
        void setTexture(sf::Texture* texture, int padding);
        void setTexture(sf::Texture* texture, int paddingWidth, int paddingHeight);
        void setTexture(sf::Texture* texture, const sf::IntRect& rect);

        void setSize(int x, int y);
        void setPosition(float x, float y);

        void setInnerSize(const sf::IntRect& rect);
        void setInnerSize(int x, int y, int width, int height);

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

        sf::Vector2f _position{0.f, 0.f};

        sf::Sprite _topBar;
        sf::Sprite _bottomBar;
        sf::Sprite _leftBar;
        sf::Sprite _rightBar;

        sf::Sprite _topLeftCorner;
        sf::Sprite _topRightCorner;
        sf::Sprite _bottomLeftCorner;
        sf::Sprite _bottomRightCorner;

        sf::Sprite _inner;

        std::vector<sf::Sprite*> _sprites;

        void _recalcTexturesAreas();
        void _recalcSpritesSize();
        void _recalcSpritesPos();
    };
}