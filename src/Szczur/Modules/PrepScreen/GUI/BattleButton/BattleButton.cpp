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

        const sf::Vector2f prSize = {0.463f, 0.1f};
    
        _background->setMainPatchPropSize({0.3f, 0.3f});
        _background->setPropSize(prSize);
        _background->makeChildrenPenetrable();

        _background->add(_text);
        _text->setCharacterPropSize(0.05f);
        _text->setOutlinePropThickness(0.05f);
        _text->setColor({220, 200, 200});
        _text->setOutlineColor(sf::Color::Black);
        _text->setPropPosition(0.5f, 0.5f);
        _text->setString("Do walki!");
    }
    void BattleButton::setParent(Widget* parent)
    {
        parent->add(_background);
    }
    void BattleButton::setCallback(const std::function<void(Widget*)>& call)
    {
        _background->setCallback(Widget::CallbackType::onPress, call);
    }
    void BattleButton::loadAssetsFromGUI(GUI& gui)
    {
        _text->setFont(gui.getFont("Assets/fonts/NotoSerif-Regular.ttf"));
        _background->setTexture(gui.getTexture("Assets/PrepScreen/SkillBack.png"), 30);
    }
}