#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class Script;
    class TextAreaWidget : public Widget {
    public:
        TextAreaWidget();
        TextAreaWidget(sf::Vector2u size, sf::Font* font);

        static void initScript(Script& script);

        void setString(const std::string& text);

        void setSize(sf::Vector2u size);

        void setFont(sf::Font* font);

        void setCharacterSize(size_t size);

        void setColor(const sf::Color& color);

    protected:
        virtual void _update(float deltaTime) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual sf::Vector2u _getSize() const override;
    private:
        bool _toWrap;

        sf::Text _text;
        sf::Vector2u _size;

        void _wrapText();
        virtual void _callback(CallbackType type) override;
    };
}