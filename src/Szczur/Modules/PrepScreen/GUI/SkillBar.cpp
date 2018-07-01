#include "SkillBar.hpp"

#include "Szczur/Modules/PrepScreen/PrepScreen.hpp"

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

    SkillBar::SkillBar(PrepScreen& prepScreen)
    :
    _prepScreen(prepScreen),
    BaseBar( [this]
        {
            auto* base = new ListWidget; 
            //base->makeHorizontal();
            base->makeChildrenPenetrable();
            base->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
                _onClick();
            });
            base->setCallback(Widget::CallbackType::onHoverIn, [&](Widget* owner){
                _onHoverIn();
            });
            base->setCallback(Widget::CallbackType::onHoverOut, [&](Widget* owner){
                _onHoverOut();
            });
            return base;
        } () )
    {
        _iconWindow = new WindowWidget; 
        _icon = new ImageWidget;
        _infoBar = new WindowWidget;
        _name = new TextWidget;

        auto* list = new ListWidget;
        list->makeHorizontal();
        _addWidget(list);

        _iconWindow->setPropSize(0.09f, 0.09f);
        _iconWindow->setScale(0.3f, 0.3f);
        list->add(_iconWindow);       
        
        _icon->setPropSize(0.08f, 0.08f);
        _icon->setPropPosition(0.5f, 0.5f);
        _iconWindow->add(_icon);

        _infoBar->setPropSize(0.21f, 0.09f);
        _infoBar->setScale(0.3f, 0.3f);
        _infoBar->setPadding(5, 5);
        _infoBar->makeChildrenPenetrable();
        _infoBar->makeChildrenUnresizable();
        list->add(_infoBar);

        _name->setCharacterSize(12);
        _name->setPropPosition(0.f, 0.f);
        _infoBar->add(_name);

        _costBar.setParent(_infoBar);
        _costBar.setPropPosition(0.f, 1.f);

        _titleWindow = new WindowWidget;
        _titleWindow->setPropSize(0.3f, 0.f);
        _titleWindow->setPadding(0.f, 5.f);
        _titleWindow->setScale(0.3f, 0.3f);
        _addWidget(_titleWindow);


        _title = new TextWidget;
        _title->setCharacterSize(12);
        _title->setColor({255, 255, 255});
        _title->setPropPosition(0.5f, 0.5f);
        _title->setString("Hejjjj");

        _titleWindow->add(_title);

        _titleWindow->fullyDeactivate();
        
        _getBase()->fullyDeactivate();
    }

    void SkillBar::setSkill(const Skill* skill)
    {
        _skill = skill;
        _name->setString(skill->getName());
        _costBar.setSkill(skill);
        _icon->setTexture(skill->getTexture());

        _canBeBought = _prepScreen.canSkillBeBought(skill);

        if(skill->isBought())
        {
            _getBase()->fullyDeactivate();
        }
        else
        {
            _getBase()->fullyActivate();
        }  

        _titleWindow->fullyDeactivate();
    }
    void SkillBar::removeSkill()
    {
        _skill = nullptr;
        _getBase()->fullyDeactivate();
        _costBar.removeSkill();
    }

    void SkillBar::recalculateAvailability()
    {
        bool isBought = _skill->isBought();
        if(_isKnownAsBought != isBought)
        {
            if(isBought)
            {
                _getBase()->fullyDeactivate();
                _titleWindow->fullyDeactivate();
            }
            else
            {
                _getBase()->fullyActivate();
            }
            _isKnownAsBought = isBought;
        }

        _canBeBought = _prepScreen.canSkillBeBought(_skill);
        if(_canBeBought)
        {
            _getBase()->setColorInTime({255, 255, 255}, 0.1f);
        }
        else
        {
            _getBase()->setColorInTime({125, 125, 125}, 0.1f);
        }
    }
    

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
    }

    void SkillBar::_onClick()
    {      
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(!_skill) return;
            if(_isKnownAsBought) return;

            if(_prepScreen.canSkillBeBought(_skill))
            {
                _prepScreen.buySkill(_skill);
            }
        }
        else
        {
            if(_titleWindow->isFullyDeactivated())
            {
                _titleWindow->fullyActivate();
            }
            else
            {
                _titleWindow->fullyDeactivate();
            }
        }
    }

    void SkillBar::_buy()
    {
        //auto ppCost = _skill->getCostInfo().getCost();
        //_prepScreen.takePP(ppCost);
        //_prepScreen.buySkill(_skill);
    }

    void SkillBar::_onHoverIn()
    {
        if(_canBeBought)
        {
            _getBase()->setColorInTime({180, 180, 180}, 0.1f);
            _prepScreen.dimPPsNeededToBuySkill(_skill);
        }
    }
    void SkillBar::_onHoverOut()
    {
        if(_canBeBought)
        {
            _getBase()->setColorInTime({255, 255, 255}, 0.1f);
            _prepScreen.normPPsNeededToBuySkill(_skill);
        }
    }


    void SkillBar::loadAssetsFromGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/Test/Bar.png");
        _iconWindow->setTexture(barTex, 6);
        _infoBar->setTexture(barTex, 6);
        _titleWindow->setTexture(barTex, 6);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/anirm.ttf"));
        _title->setFont(gui.getAsset<sf::Font>("Assets/fonts/anirm.ttf"));
        _costBar.loadAssetsFromGUI(gui);
    }
    
}