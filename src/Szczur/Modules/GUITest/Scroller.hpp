#pragma once

#include "NinePatch.hpp"

namespace rat
{
    class Scroller : public sf::Drawable, public sf::Transformable
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);

        void setPathTexture(const sf::Texture* texture);
        void setScrollerTexture(sf::Texture* texture, int boundHeight);
        void setBoundTexture(const sf::Texture* texture);

        void setScrollerPropHeight(float propY);

        void setSize(const sf::Vector2i& size);
        void setSize(int width, int height);
        void setWidthProportion(float proportion);
        void setBoundShiftProportion(float proportion);

        sf::Vector2u getSize() const;

        void setProportion(float proportion);
        void moveProportion(float proportionOffset);
        void setScrollerPosition(const sf::Vector2f& position);
    private:

        sf::Sprite _path;
        NinePatch _scroller;

        sf::Sprite _upperBound;
        sf::Sprite _bottomBound;
        float _boundShiftProp{0.7f};   

        bool _areBoundsSet{false};     
        bool _isScrollerSet{false};
        bool _isPathSet{false};

        float _widthProp{1.f};
        float _scrollerHeightProp{1.f};
        int _scrollerLength{60};

        sf::Vector2i _size{0, 0};
        sf::Vector2f _position{0.f, 0.f};


        float _proportion{0.f};   

        void _recalcPathSize();
        void _recalcPathPos();

        void _recalcScrollerSize();
        void _recalcScrollerPos();
        void _recalcScrollerPosByProp();

        void _recalcBoundPos();
        void _recalcBoundSize();

        float _getRealPathLength() const;
        float _getRealBoundLength() const;

        void _recalcAll();

    };
}