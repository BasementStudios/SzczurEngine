#include "ProfessionBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat
{
    ProfessionBar::ProfessionBar(SkillArea& skillArea)
    :
    _skillArea(skillArea)
    {
        _icon = new ImageWidget;
        auto click = [&](Widget* owner){
            _onClick();
        };

        _icon->setCallback(Widget::CallbackType::onPress, click);
    }

    void ProfessionBar::setPosition(float x, float y)
    {
        _icon->setPosition(x, y);
    }

    void ProfessionBar::setTexture(sf::Texture* texture)
    {
        _icon->setTexture(texture);
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        _icon->setScale({dim/size.x, dim/size.y});
    }
    void ProfessionBar::setParent(Widget* parent)
    {
        parent->add(_icon);
    }
    void ProfessionBar::setProfession(const std::string& profession)
    {
        _profession = profession;
    }
    const std::string& ProfessionBar::getProfession() const
    {
        return _profession;
    }

    void ProfessionBar::_onClick()
    {
        _skillArea.setProfession(_profession);
    }
    
}