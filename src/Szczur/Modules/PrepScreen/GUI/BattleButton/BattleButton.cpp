#include "BattleButton.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"

namespace rat
{
    BattleButton::BattleButton()
    {
        _background = new WindowWidget;
        _text = new TextWidget;

        const sf::Vector2f prSize = {0.2f, 0.05f};
    
        _background->setMainPatchPropSize(prSize);
        _background->setPropSize(prSize);

        _background->add(_text);
        _text->setCharacterPropSize(0.07f);
        _text->setOutlinePropThickness(0.05f);
        _text->setOutlineColor(sf::Color::Black);
        _text->setPropPosition(0.5f, 0.5f);
    }
    void BattleButton::setParent(Widget* parent)
    {
        parent->add(_background);
    }
    void BattleButton::setCallback(std::function<void(Widget*)>& call)
    {

    }
    void BattleButton::loadAssetsFromGUI(GUI& gui)
    {

    }
}