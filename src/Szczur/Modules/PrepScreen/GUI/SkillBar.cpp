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
    BaseBar()
    {
        // auto* base = new ListWidget; 
        // //base->makeHorizontal();
        // base->makeChildrenPenetrable();

        _iconWindow = new WindowWidget; 
        _icon = new ImageWidget;
        _infoBar = new WindowWidget;
        _name = new TextWidget;


        //const sf::Vector2f prSize(0.312037037037037f, 0.1148148148148f);
        const sf::Vector2f prSize(0.4f, 0.1148148148148f);

        _infoBar->setPropSize(prSize);
        _infoBar->setMainPatchPropSize({prSize.y, prSize.y});
        _infoBar->makeChildrenPenetrable();
        _addWidget(_infoBar);
        _infoBar->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
            _onClick();
        });
        _infoBar->setCallback(Widget::CallbackType::onHoverIn, [&](Widget* owner){
            _onHoverIn();
        });
        _infoBar->setCallback(Widget::CallbackType::onHoverOut, [&](Widget* owner){
            _onHoverOut();
        });

        _filter = new WindowWidget;
        _filter->setPropSize(prSize);
        _filter->setMainPatchPropSize(prSize);
        _filter->makeChildrenUncolorable();
        _filter->hide();
        //_iconWindow->add(_filter);
        _infoBar->add(_filter);

        auto* mainList = new ListWidget;
        _filter->add(mainList);

        _firstSlot = new WindowWidget;
        mainList->add(_firstSlot);
        _firstSlot->makeChildrenUnresizable();
        _firstSlot->setPropSize(prSize);
        _firstSlot->setMainPatchPropSize({prSize.y, prSize.y});

        auto* list = new ListWidget;
        list->makeHorizontal();
        _firstSlot->add(list);


        _iconWindow->setPropSize(prSize.y, prSize.y);
        _iconWindow->setMainPatchPropSize({prSize.y, prSize.y});
        list->add(_iconWindow);       
        
        _icon->setPropSize({prSize.y - 0.005f, prSize.y - 0.005f}); //0888979591836735f
        _icon->setPropPosition(0.5f, 0.5f);
        _iconWindow->add(_icon);

        auto* infos = new Widget;
        infos->setPropSize(prSize.x - prSize.y, prSize.y);
        infos->setPropPadding(0.011020408163265f, 0.011020408163265f);
        infos->makeChildrenUnresizable();

        list->add(infos);

        _name->setCharacterPropSize(0.02f);
        infos->add(_name);

        _costBar.setParent(infos);
        _costBar.setPropPosition(0.f, 1.f);

        //_titleWindow = new WindowWidget;
        //_titleWindow->setPropSize(0.3f, 0.f);
        //_titleWindow->setPadding(0.f, 5.f);
        //_titleWindow->setScale(0.3f, 0.3f);
        // _addWidget(_titleWindow);


        _titlePar = new Widget;
        mainList->add(_titlePar);
        _titlePar->setPropSize(prSize.x, 0.f);
        _titlePar->setPropPadding(0.f, 0.01f);


        _title = new TextWidget;
        _title->setCharacterPropSize(0.02f);
        _title->setColor({255, 255, 255});
        _title->setPropPosition(0.5f, 0.5f);
        _title->setString("Hejjjj");

        _titlePar->add(_title);

        _titlePar->fullyDeactivate();
        
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

        _titlePar->fullyDeactivate();
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
                _titlePar->fullyDeactivate();
            }
            else
            {
                _getBase()->fullyActivate();
            }
            _isKnownAsBought = isBought;
        }

        _canBeBought = _prepScreen.canSkillBeBought(_skill);
        _filter->hide();
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
            if(_titlePar->isFullyDeactivated())
            {
                _titlePar->fullyActivate();
            }
            else
            {
                _titlePar->fullyDeactivate();
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
            //_getBase()->setColorInTime({180, 180, 180}, 0.1f);
            //_filter->show();
            _prepScreen.dimPPsNeededToBuySkill(_skill);
        }
    }
    void SkillBar::_onHoverOut()
    {
        if(_canBeBought)
        {
            //_getBase()->setColorInTime({255, 255, 255}, 0.1f);
            _filter->hide();
            _prepScreen.normPPsNeededToBuySkill(_skill);
        }
    }


    void SkillBar::loadAssetsFromGUI(GUI& gui)
    {
        auto* barTex = gui.getAsset<sf::Texture>("Assets/PrepScreen/SkillBack.png");
        _iconWindow->setTexture(barTex, 30);
        _infoBar->setTexture(barTex, 30);
        _firstSlot->setTexture(barTex, 30);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf"));
        _title->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf"));
        _costBar.loadAssetsFromGUI(gui);
        _filter->setTexture(gui.getTexture("Assets/PrepScreen/SkillBarFilter.png"), 280, 100);
    }
    
}