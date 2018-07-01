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

        void setTextSize(sf::Vector2u size);

        void setFont(sf::Font* font);

        void setCharacterSize(size_t size);

    protected:
        virtual void _update(float deltaTime) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual sf::Vector2f _getSize() const override;
        virtual void _recalcPos() override;
        virtual void _setColor(const sf::Color& color) override;
    private:
        sf::Text _text;
        sf::Vector2u _size;

        bool _toWrap;

        sf::String& _wrapText(sf::String& temp);
        virtual void _callback(CallbackType type) override;
    };
}