#pragma once 


#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class Script;

    class TextWidget : public Widget {
    public:
        TextWidget();

        TextWidget(const sf::Text& text, sf::Font* font);

        static void initScript(Script& script);

        void setColor(const sf::Color& newColor);

        size_t getLength();

        void addLetter(char letter);

        std::string getString() const;
        void setString(const std::string& str);

        void setFont(sf::Font* font);

        const sf::Font* getFont() const;

        void setCharacterSize(unsigned int size);

        unsigned int getCharacterSize() const;

        void removeLast();
    protected:
        virtual sf::Vector2i _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _recalcPos() override;
    private:
        virtual void _callback(CallbackType type) override;

        sf::Text _text;
    };
}