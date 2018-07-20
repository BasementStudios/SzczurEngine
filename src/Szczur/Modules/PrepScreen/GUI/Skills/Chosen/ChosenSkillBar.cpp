#include "ChosenSkillBar.hpp"

#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ChosenSkillBar::ChosenSkillBar(PrepScreen& prepScreen)
    :
    _prepScreen(prepScreen)
    {

         const sf::Vector2f prSize = {0.1f, 0.1f};

        _border = new ImageWidget;
        _addWidget(_border);
        _border->setPropSize(prSize);
        _border->makeChildrenPenetrable();

        _icon = new ImageWidget;
        _addWidget(_icon);
        _icon->setPropSize(prSize);
        _icon->setPropPosition(0.5f, 0.5f);


        _border->setCallback(Widget::CallbackType::onHoverIn, [this](auto){
            if(hasSkill())
            {
                _icon->setColorInTime({200, 200, 200}, 0.2f);
            }
        });

        _border->setCallback(Widget::CallbackType::onHoverOut ,[this](auto){
            if(hasSkill())
            {
               _icon->setColorInTime({255, 255, 255}, 0.2f);
            }
        });

        _icon->setCallback(Widget::CallbackType::onRelease, [this](auto){
            _onClick();
        });
    }

    void ChosenSkillBar::initAssetsViaGUI(GUI& gui)
    {
        _border->setTexture(gui.getTexture("Assets/PrepScreen/SkillBack.png"));
    }

    void ChosenSkillBar::setSkill(const Skill* skill)
    {
        if(skill == nullptr)
        {
            _hasSkill = false;
            _icon->removeTexture();
        }
        else
        {
            _hasSkill = true;
            auto* texture = skill->getTexture();
            _icon->setTexture(texture);
            _icon->setColor({255, 255, 255});
        }
        _skill = skill;
    }
    void ChosenSkillBar::removeSkill()
    {
        setSkill(nullptr);
    }
    bool ChosenSkillBar::hasSkill() const
    {
        return _hasSkill;
    }
    bool ChosenSkillBar::isFree() const
    {
        return !_hasSkill;
    }
    
    void ChosenSkillBar::_onClick()
    {
        if(hasSkill()) {_prepScreen.returnSkill(_skill); std::cout << "Returned\n"; }
    }

    void ChosenSkillBar::swapSkillsWith(ChosenSkillBar& other)
    {
        const Skill* othersSkill = other._skill;

        other.setSkill(_skill);
        setSkill(othersSkill);
    }
}