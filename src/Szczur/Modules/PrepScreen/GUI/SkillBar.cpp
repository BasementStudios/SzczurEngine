#include "SkillBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"

#include "GrayPPBar.hpp"

namespace rat
{
    SkillBar::SkillBar(GrayPPBar& source)
    :
    _sourceBar(source)
    {
        _bar = new ImageWidget;
        _icon = new ImageWidget;
        _bar->add(_icon);
        _icon->setPosition(2, 2);
        _nameText = new TextWidget;
        _bar->add(_nameText);

        auto click = [&](Widget* owner){
            PPContainer& source = _sourceBar.getSource();
            if(!_isBought)
            {
                if(_skill->canBeBoughtFrom(source))
                {
                    _skill->buyFrom(source);
                    _sourceBar.recalculate();

                    _isBought = true;
                    _bar->setTexture(_textureLocked);
                }
                _bar->setTexture(_textureLocked);
            }
            else
            {
                _skill->returnCostsTo(source);
                _sourceBar.recalculate();

                _isBought = false;
                _bar->setTexture(_textureBar);
            }
        };

        _bar->setCallback(Widget::CallbackType::onPress, click);
    }

    void SkillBar::setParent(Widget* parent)
    {
        parent->add(_bar);
    }

    void SkillBar::setPosition(float x, float y)
    {
        _bar->setPosition(x, y);
    }
    void SkillBar::setPosition(const sf::Vector2f& pos)
    {
        _bar->setPosition(pos);
    }

    void SkillBar::setBarTexture(sf::Texture* mainTexture, sf::Texture* lockTexture)
    {
        _textureBar = mainTexture;
        _textureLocked = lockTexture;
        _bar->setTexture(_textureBar);
    }

    void SkillBar::setSkill(Skill* skill)
    {
        _skill = skill;
        _icon->setTexture(skill->getIcon());
    }


}