#include "SkillBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "GrayPPBar.hpp"
#include "SkillArea.hpp"
#include "ChosenSkillArea.hpp"

#include "Szczur/Utility/Logger.hpp"


namespace rat
{
    
     sf::Vector2u SkillBar::_size = {240, 72};

    SkillBar::SkillBar(SkillArea& parentArea)
    :
    _parentArea(parentArea),
    _chosenArea(_parentArea.getChosenSkillArea()),
    _sourceBar(_parentArea.getSourceBar())
    {
        _iconWindow = new WindowWidget; 
        _icon = new ImageWidget;
        _infoBar = new WindowWidget;
        _name = new TextWidget;

        
    }

    void SkillBar::setSkill(Skill* skill)
    {
        _skill = skill;
        _nameText->setString(skill->getName());
        _costBar.setSkill(skill);
        _icon->setTexture(skill->getTexture());
    }

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
    }

    void SkillBar::activate()
    {
        _bar->activate();
        _bar->visible();
        _icon->activate();
        _icon->visible();
        if(!_bar->getTexture()) std::cout << "Texture is bad\n";
        _isActivate = true;
    }
    void SkillBar::deactivate()
    {
        _bar->deactivate();
        _bar->invisible();
        _icon->deactivate();
        _icon->invisible();
        _isActivate = false;
    }

    bool SkillBar::isActivate() const
    {
        return _isActivate;
    }

    void SkillBar::_onClick()
    {      
        auto& source = _sourceBar.getSource();
        if(!isBought())
        {
            if(_skill->canBeBoughtFrom(source) && _chosenArea.hasFreeSpace())
            {
                _skill->buyFrom(source);

                _chosenArea.addSkill(_skill);

                _parentArea.recalculate();
                _sourceBar.recalculate();
            }
        }
    }

    void SkillBar::loadAssetsFromGUI(GUI& gui)
    {
        gui.addAsset<sf::Texture>("assets/Test/SkillBar.png");
        _bar->setTexture(gui.getAsset<sf::Texture>("assets/Test/SkillBar.png"));
        _nameText->setFont(gui.getAsset<sf::Font>("assets/fonts/NotoMono.ttf"));
        _costBar.loadAssetsFromGUI(gui);
    }
    
}