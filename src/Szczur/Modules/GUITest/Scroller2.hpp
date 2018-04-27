#pragma once

#include <SFML/Graphics.hpp>

namespace rat
{
    class Scroller2 : public sf::Drawable
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);
        void setPathTexture(const sf::Texture* texture);
        void setScrollerTexture(const sf::Texture* texture);

        void setSize(const sf::Vector2i& size);
        void setWidthProportion(float proportion);

        void setProportion(float proportion);
    private:

        sf::Sprite _path;
        sf::Sprite _scroller;

        float _widthProp{1.f};
        int _scrollerLength{60};

        sf::Vector2i _size{0, 0};
        sf::Vector2f _position{0.f, 0.f};

        bool _isScrollerTextureSet{false};
        bool _ispathTextureSet{false};

        float _pathLength{130.f};
        float _scrollerPos{0.f};

        float _proportion{0.f};
        

        void setSpriteSize(sf::Sprite& sprite, const sf::Vector2i& size)
        {
            auto texSize = static_cast<sf::Vector2f>(sprite.getTexture()->getSize());
            sf::Vector2f scale = {float(size.x)/texSize.x, float(size.y)/texSize.y};
        }   

        void _recalcScroller();

        void _recalcPathSize();
        void _recalcPathPos();
        void _recalcScrollerSize();
        void _recalcScrollerPos();
        void _recalcScrollerPosByProp();
        void _recalcSize();

        float _getRealPathLength() const;

    };
}