#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Szczur/Modules/GUI/Widget/FamilyTransform.hpp"

#include "NinePatch.hpp"

namespace rat
{
    class Scroller : public sf::Drawable, protected gui::FamilyTransform
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& position);

        void setPathTexture(const sf::Texture* texture);
        void setScrollerTexture(sf::Texture* texture, int boundHeight);
        void setBoundTexture(const sf::Texture* texture);

        void setScrollerPropHeight(float propY);

        void setSize(const sf::Vector2f& size);
        void setSize(float width, float height);
        void setWidthProportion(float proportion);
        void setScrollerHeightProp(float prop);
        void setBoundShiftProportion(float proportion);

        sf::Vector2f getSize() const;
        sf::Vector2f getScrollerSize() const;
        sf::Vector2f getScrollerPosition() const;

        void setProportion(float proportion);
        void moveProportion(float proportionOffset);
        float getProportion() const;

        sf::FloatRect getBound() const;

        void setScrollerPosition(const sf::Vector2f& position);

        void applyFamilyTransform(const sf::Vector2f& globalPos, const sf::Vector2f& drawPos);

        void input(const sf::Event& event);
    private:
        bool _isClicked{false};
        bool _hasBeenClicked{false};
        bool _isHovered{false};
        sf::Vector2f _clickedShift;

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
        float _scrollerLength{60.f};

        sf::Vector2f _size;
        sf::Vector2f _position;


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

        sf::Vector2f _getScrollerGlobalPos() const;

    };
}