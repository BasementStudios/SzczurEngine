#pragma once

#include <SFML/Graphics.hpp>

#include <algorithm>

#include "TextWidget.hpp"

namespace rat {
    class TextAreaWidget : public Widget {
    public:
        TextAreaWidget();
        TextAreaWidget(sf::Vector2u size, sf::Font* font);

        void setString(const std::string& text);

        void setSize(sf::Vector2u size);

        void setFont(sf::Font* font);

        void setCharacterSize(size_t size);

        void setColor(const sf::Color& color);

    protected:
        virtual void _update(float deltaTime) override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _input(const sf::Event& event) override;
        virtual sf::Vector2u _getSize() const override;
    private:
        bool _toCreate;
        bool _toUpdate;
        bool _toWrap;

        sf::Text _text;
        sf::RenderTexture _area;
        sf::Sprite _sprite;
        sf::Vector2u _size;
        sf::View _view;

        void _redrawArea();
        void _wrapText();
    };
}