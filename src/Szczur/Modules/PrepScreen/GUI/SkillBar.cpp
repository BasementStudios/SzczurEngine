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
        setSize(_size);

        _iconWindow = new WindowWidget; 
        _icon = new ImageWidget;
        _infoBar = new WindowWidget;
        _name = new TextWidget;

        _iconWindow->setSize(_size.y, _size.y);
        _iconWindow->setScale(0.3f, 0.3f);
        _addWidget(_iconWindow);
        
        
        _icon->setSize(_size.y - 2 * round(_iconWindow->getPadding().x), _size.y - 2 * round(_iconWindow->getPadding().x));
        _iconWindow->add(_icon);

        _infoBar->setSize(_size.x - _size.y, _size.y);
        _infoBar->setPosition(_size.y, 0);
        _infoBar->setScale(0.3f, 0.3f);
        _infoBar->setPadding(10, 10);
        _infoBar->setCallback(Widget::CallbackType::onPress, [&](Widget* owner){
            _onClick();
        });
        _addWidget(_infoBar);

        _name->setCharacterSize(20);
        _infoBar->add(_name);
    }

    void SkillBar::setSkill(Skill* skill)
    {
        _skill = skill;
        _name->setString(skill->getName());
        _costBar.setSkill(skill);
        _icon->setTexture(skill->getTexture());
    }

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
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
        auto* barTex = gui.getAsset<sf::Texture>("Assets/Test/Bar.png");
        _iconWindow->setTexture(barTex, 6);
        _infoBar->setTexture(barTex, 6);
        _name->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _costBar.loadAssetsFromGUI(gui);
    }
    
}