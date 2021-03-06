#pragma once 


#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class Script;

    namespace gui { class AnimData; }

    class TextWidget : public Widget {
    public:
        TextWidget();

        TextWidget(const sf::Text& text, sf::Font* font);

        static void initScript(Script& script);

        size_t getLength();

        void addLetter(char letter);

        std::string getString() const;
        void setString(const std::string& str);
        void setStringInTime(const std::string& str, float time);
        void setStringInTime(const std::string& str, const gui::AnimData& data);

        void setFont(sf::Font* font);

        const sf::Font* getFont() const;

        void setCharacterSize(unsigned int size);
        void setCharacterPropSize(float prop);
        float getCharacterPropSize() const;

        unsigned int getCharacterSize() const;

        void setOutlineThickness(float thickness);
        void setOutlinePropThickness(float prop);
        void setOutlineColor(const sf::Color& color);

        void removeLast();
    protected:
        virtual sf::Vector2f _getSize() const override;
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual void _recalcPos() override;
        virtual void _setColor(const sf::Color& color) override;
        virtual void _recalcElementsPropSize() override;
    private:
        virtual void _callback(CallbackType type) override;

        float _chPropSize;
        bool _hasChPropSize = false;
        void _calcChPropSize();

        float _outlinePropThickness;
        bool _hasOutlinePropThickness{false};
        void _calcOutlinePropThickness();


        sf::Text _text;
    };
}