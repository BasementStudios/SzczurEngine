#include "SkillBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"

#include "GrayPPArea.hpp"
#include "SkillArea.hpp"
#include "ChosenSkillArea.hpp"

#include "Szczur/Utility/Logger.hpp"


namespace rat
{
    
     sf::Vector2u SkillBar::_size = {240, 72};

    SkillBar::SkillBar()
    :
    BaseBar( [this]
        {
            auto* base = new ListWidget; 
            base->makeHorizontal();
            base->makeChildrenPenetrable();
            base->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
                _onClick();
            });
            base->setCallback(Widget::CallbackType::onHoverIn, [&](Widget* owner){
                _onHoverIn();
                owner->setColor({180, 180, 180}, 0.3f);
            });
            base->setCallback(Widget::CallbackType::onHoverOut, [&](Widget* owner){
                _onHoverOut();
                owner->setColor({255, 255, 255}, 0.3f);
            });
            return base;
        } () )
    {
        setSize(_size);

        _iconWindow = new WindowWidget; 
        _icon = new ImageWidget;
        _infoBar = new WindowWidget;
        _name = new TextWidget;

        _iconWindow->setPropSize(0.09f, 0.09f);
        _iconWindow->setScale(0.3f, 0.3f);
        _addWidget(_iconWindow);        
        
        _icon->setPropSize(0.08f, 0.08f);
        _icon->setPropPosition(0.5f, 0.5f);
        _iconWindow->add(_icon);

        _infoBar->setPropSize(0.21f, 0.09f);
        _infoBar->setScale(0.3f, 0.3f);
        _infoBar->setPadding(5, 5);
        _infoBar->makeChildrenPenetrable();
        _addWidget(_infoBar);

        _name->setCharacterSize(20);
        _name->setPropPosition(0.f, 0.f);
        _infoBar->add(_name);

        _costBar.setParent(_infoBar);
        _costBar.setPropPosition(0.f, 1.f);
    }

    void SkillBar::setSkill(const Skill* skill)
    {
        _skill = skill;
        _name->setString(skill->getName());
        _costBar.setSkill(skill);
        _icon->setTexture(skill->getTexture());

        if(skill->isBought())
        {
            _getBase()->invisible();
            _getBase()->deactivate();
        }
        else
        {
            _getBase()->visible();
            _getBase()->activate();
        }  
    }
    void SkillBar::removeSkill()
    {
        _skill = nullptr;
        _getBase()->invisible();
        _getBase()->deactivate();
        _costBar.removeSkill();
    }

    void SkillBar::recalculateAvailability()
    {
        bool isBought = _skill->isBought();
        if(_isKnownAsBought != isBought)
        {
            if(isBought)
            {
                _getBase()->invisible();
                _getBase()->deactivate();
            }
            else
            {
                _getBase()->invisible();
                _getBase()->deactivate();
            }
            _isKnownAsBought = isBought;
        }
    }
    

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
    }

    void SkillBar::_onClick()
    {      
        //_sourceArea.recalculate();

        /*
        if(isBought()) return;
        if(!canBeBought()) return;

        buy();;
        */  
    }

    void SkillBar::_buy()
    {
        //auto ppCost = _skill->getCostInfo().getCost();
        //_prepScreen.takePP(ppCost);
        //_prepScreen.buySkill(_skill);
    }
    bool SkillBar::_canBeBought() const
    {
        const auto& cost = _skill->getCostInfo();

        /*if(!_prepScreen.hasPPAmount(cost.getCost())) return false;
        for(auto& [id, power] : cost)
        {
            if(!_prepScreen.hasEnoughPowerfulGlyph(id, power)) return false;
        }*/
        return false;
        return true;
        
    }

    void SkillBar::_onHoverIn()
    {
    }
    void SkillBar::_onHoverOut()
    {
    }


    void SkillBar::loadAssetsFromGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/Test/Bar.png");
        _iconWindow->setTexture(barTex, 6);
        _infoBar->setTexture(barTex, 6);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _costBar.loadAssetsFromGUI(gui);
    }
    
}