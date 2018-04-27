#pragma once

#include <SFML/Graphics.hpp>

#include "Patch.hpp"

namespace rat
{
    class Scroller : public sf::Drawable
    {
    public:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setPosition(float x, float y);
        void setPosition(const sf::Vector2f& pos);

        void setSize(int x, int y);
        void setSize(const sf::Vector2i& size);

        void setScale(float x, float y);
        void setScale(const sf::Vector2f& scale);

        void setBarTexture(sf::Texture* texture);
        void setScrollerTexture(sf::Texture* texture);

        void setProportion(float proportion);

        float getProportion() const;
    private:
        Patch _bar{Patch::Direction::Vertical};

        sf::Sprite _scroller;

        sf::Vector2i _size;
        float _scrollerLength{0.f};
        float _scrollerPathLength{0.f};

        sf::Vector2f _scrollerRelPos{0.f, 0.f};
        sf::Vector2f _scrollerRelPath{0.f, 0.f};


        bool _isLocked{false};
        void _setScrollerPosition(float x, float y);
        void _setScrollerPosition(const sf::Vector2f& pos);

        void _recalcSize();
        void _recalcPos();
        void _recalcOrigin();

        void _recalcRelPath();
        void _recalcRelPos();
        void _recalcRelPos(float prop);
    };
}