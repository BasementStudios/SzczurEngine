#include "ChosenSkillBar.hpp"

#include "ChosenSkillArea.hpp"
#include "SkillArea.hpp"
#include "GrayPPArea.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillBar::ChosenSkillBar(ChosenSkillArea& parentArea)
    :
    _parentArea(parentArea),
    _skillArea(parentArea.getSkillArea()),
    _sourceBar(_skillArea.getSourceArea())
    {
        _icon = new ImageWidget;
        _addWidget(_icon);
        _icon->setPosition(0.f, 0.f);

        auto onClick = [&](Widget* owner){
            _onClick();
        };

        _icon->setCallback(Widget::CallbackType::onPress, onClick);
    }

    void ChosenSkillBar::initAssetsViaGUI(GUI& gui)
    {
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
            //_sourceBar.recalculate();
            _skillArea.recalculate();
            //_parentArea.recalculate();
        }
    }
    void ChosenSkillBar::setSize(const sf::Vector2f& size)
    {
        setSize(size.x, size.y);
    }
    
    void ChosenSkillBar::setSize(float x, float y)
    {
        _icon->setSize((unsigned int)x, (unsigned int)y);
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