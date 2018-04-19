#include "ChosenSkillBar.hpp"

#include "ChosenSkillArea.hpp"
#include "SkillArea.hpp"
#include "GrayPPBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillBar::ChosenSkillBar(ChosenSkillArea& parentArea)
    :
    _parentArea(parentArea),
    _skillArea(parentArea.getSkillArea()),
    _sourceBar(_skillArea.getSourceBar())
    {
        _base = new ImageWidget;
        _icon = new ImageWidget;
        _base->add(_icon);
        _icon->setPosition(0.f, 0.f);

        auto onClick = [&](Widget* owner){
            _onClick();
        };

        _base->setCallback(Widget::CallbackType::onPress, onClick);
    }

    void ChosenSkillBar::initAssetsViaGUI(GUI& gui)
    {
        sf::Texture* texture = gui.getAsset<sf::Texture>("assets/PrepScreen/MenuBarTest.png");
        _base->setTexture(texture);
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        _base->setScale({120.f/size.x, 120.f/size.y});
    }

    void ChosenSkillBar::setSkill(Skill* skill)
    {
        if(skill == nullptr)
        {
            _hasSkill = false;
            _icon->invisible();
        }
        else
        {
            _hasSkill = true;
            _icon->visible();
            auto* texture = skill->getTexture();
            _setIconTexture(texture);
        }
        _skill = skill;
    }
    bool ChosenSkillBar::hasSkill() const
    {
        return _hasSkill;
    }
    bool ChosenSkillBar::isFree() const
    {
        return !_hasSkill;
    }
    
    
    void ChosenSkillBar::setPosition(float x, float y)
    {
        _base->setPosition(x, y);
    }
    void ChosenSkillBar::setParent(Widget* parent)
    {
        parent->add(_base);
    }
    void ChosenSkillBar::_setIconTexture(sf::Texture* texture)
    {
        _icon->setTexture(texture);
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        _icon->setScale({_size.x/size.x, _size.y/size.y});  
    }
    
    void ChosenSkillBar::_onClick()
    {
        if(_hasSkill)
        {
            _skill->returnCostsTo(_sourceBar.getSource());
            _skill = nullptr;
            _hasSkill = false;
            _icon->invisible();
            _sourceBar.recalculate();
            _skillArea.recalculate();
            //_parentArea.recalculate();
        }
    }
    void ChosenSkillBar::setSize(float x, float y)
    {
        _size = {x, y};
        auto baseSize = static_cast<sf::Vector2f>(_base->getTexture()->getSize());
        _base->setScale({x/baseSize.x, y/baseSize.y});
        if(_hasSkill)
        {
            auto iconSize = static_cast<sf::Vector2f>(_icon->getTexture()->getSize());
            _icon->setScale({x/iconSize.x, y/iconSize.y});
        } 
    }
    void ChosenSkillBar::swapSkillsWith(ChosenSkillBar& other)
    {
        Skill* othersSkill = other._skill;

        std::cout << "Skill: " << (_skill ? _skill->getName() : "nullptr")
        << " swaped into Skill: " << (othersSkill ? othersSkill->getName() : "nullptr") << "\n";


        other.setSkill(_skill);
        setSkill(othersSkill);
    }
}