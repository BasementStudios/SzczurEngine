#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"
#include "Szczur/Modules/GUITest/Scroller.hpp"

namespace rat {
    class Script;
    class ScrollAreaWidget : public Widget {
    public:
        ScrollAreaWidget();

        static void initScript(Script& script);
/*
        void setSize(size_t x, size_t y);
        void setSize(sf::Vector2u size);*/
        void setScrollerTexture(sf::Texture* texture);
        void setPathTexture(sf::Texture* texture);
        void setBoundsTexture(sf::Texture* texture);        

        virtual void calculateSize() override;

        void setScrollSpeed(float speed);
        float getScrollSpeed() const;
    protected:
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _update(float deltaTime) override;
		virtual void _input(const sf::Event& event) override;
        virtual sf::Vector2u _getSize() const override;
		virtual void _calculateSize() override;
    private:
        mutable sf::RenderTexture _renderTexture;
        float _offset;
        float _scrollSpeed;
        Scroller _scroller;
        sf::Vector2u _minScrollSize{30, 50};

        virtual void _callback(CallbackType type) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}