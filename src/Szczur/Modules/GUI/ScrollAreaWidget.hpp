#pragma once

#include <SFML/Graphics.hpp>

#include "Szczur/Modules/Script/Script.hpp"

#include "Widget.hpp"

namespace rat {
    class ScrollAreaWidget : public Widget {
    public:
        ScrollAreaWidget();

        static void initScript(Script& script);

        void setSize(size_t x, size_t y);
        void setSize(sf::Vector2u size);

        sf::Vector2u getSize() const;

        virtual void calculateSize() override;

        void setScrollSpeed(float speed);
        float getScrollSpeed() const;
    protected:
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _update(float deltaTime) override;
		virtual void _input(const sf::Event& event) override;
        virtual sf::Vector2u _getSize() const override;
    private:
        mutable sf::RenderTexture _renderTexture;
        float _offset;
        float _scrollSpeed;

        virtual void _callback(CallbackType type) override;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}