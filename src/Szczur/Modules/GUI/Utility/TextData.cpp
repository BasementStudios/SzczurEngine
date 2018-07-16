#include "TextData.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat{
namespace gui{
    TextData::TextData(const sf::Text& base)
    :
    outlineThickness(base.getOutlineThickness()),
    outlineColor(base.getOutlineColor()),
    font(base.getFont()),
    characterSize(base.getCharacterSize()),
    color(base.getFillColor())
    {
    }
    void TextData::applyTo(sf::Text& text)
    {
        text.setOutlineColor(outlineColor);
        text.setOutlineThickness(outlineThickness);
        if(font) text.setFont(*font);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);
    }
}
}