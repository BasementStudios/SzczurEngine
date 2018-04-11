#include "SkillBar.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"

#include "GrayPPBar.hpp"

namespace rat
{
    SkillBar::SkillBar(GrayPPBar& sourceBar)
    :
    _sourceBar(sourceBar),
    _isBought(false)
    {
        _bar = new ImageWidget;
        _icon = new ImageWidget;
        _bar->add(_icon);
        _icon->setPosition(4.f, 4.f);
        //_nameText = new TextWidget;
        //_bar->add(_nameText);
        _sourceBar.getSource();


        auto click = [&](Widget* owner){
            _onClick();
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
    }

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
    }
    void SkillBar::setIconTexture(sf::Texture* icon)
    {
        _icon->setTexture(icon);
    }

    void SkillBar::activate()
    {
        _bar->activate();
        _icon->activate();
    }
    void SkillBar::deactivate()
    {
        _bar->deactivate();
        _icon->deactivate();
        std::cout << "Deactivating\n";
    }

    void SkillBar::_onClick()
    {
        
        auto& source = _sourceBar.getSource();
        if(!_isBought)
        {
            if(_skill->canBeBoughtFrom(source))
            {
                _skill->buyFrom(source);
                _sourceBar.recalculate();
                _isBought = true;
                _bar->setTexture(_textureLocked);
            }
            //_bar->setTexture(_textureLocked);
        }
        else
        {
            _skill->returnCostsTo(source);
            _sourceBar.recalculate();

            _isBought = false;
            _bar->setTexture(_textureBar);
        }
            
    }
    


}