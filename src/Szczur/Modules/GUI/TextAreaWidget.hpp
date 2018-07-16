#pragma once

#include <SFML/Graphics.hpp>

#include "Widget.hpp"

namespace rat {
    class Script;
    class TextAreaWidget : public Widget 
    {
    public:
        enum class Align
        {
            Left,
            Center,
            Right
        };
        TextAreaWidget();
        TextAreaWidget(sf::Vector2u size, sf::Font* font);

        static void initScript(Script& script);

        void setString(const std::string& text);
        const std::string& getString() const;

        void setFont(sf::Font* font);
        const sf::Font* getFont() const;

        void setCharacterSize(size_t size);
        size_t getCharacterSize() const;

        void setCharacterPropSize(float prop);
        float getCharacterPropSize() const;

        void setAlign(Align align);
        Align getAlign() const;

        void setOutlineThickness(float thickness);
        void setOutlinePropThickness(float prop);
        void setOutlineColor(const sf::Color& color);

    protected:
        virtual void _draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        virtual sf::Vector2f _getSize() const override;
        virtual void _calculateSize() override;
        virtual void _recalcPos() override;
        virtual void _setColor(const sf::Color& color) override;
        virtual void _recalcElementsPropSize() override;
    private:
        sf::Vector2u _size;

        //sf::Text _text;

        std::vector<sf::Text> _texts;

        std::string _str;
        Align _align = Align::Left;

        float _chPropSize;
        bool _hasChPropSize = false;

        float _outlinePropThickness;
        bool _hasOutlinePropThickness{false};
        void _calcOutlinePropThickness();

        float _width = 0.f;

        void _calcChPropSize();

        void _wrap();
        void _convertToMultiLines();
        void _calcTextPos();
        float _getAlignFactor() const;

        void _updateFont();
        bool _isFontChanged = false;
        void _updateChSize();
        bool _isChChanged = false;

        sf::String& _wrapText(sf::String& temp);
        virtual void _callback(CallbackType type) override;
    };
}