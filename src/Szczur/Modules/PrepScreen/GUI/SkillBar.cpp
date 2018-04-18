#include "SkillBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"

#include "GrayPPBar.hpp"
#include "SkillArea.hpp"

#include "Szczur/Utility/Logger.hpp"


namespace rat
{
     sf::Vector2f SkillBar::_size = {240.f, 72.f};

    SkillBar::SkillBar(SkillArea& parentArea)
    :
    _parentArea(parentArea),
    _sourceBar(_parentArea.getSourceBar()),
    _isBought(false)
    {
        _bar = new ImageWidget;
        _icon = new ImageWidget;
        _bar->add(_icon);
        _icon->setPosition(4.f, 4.f);


        _infoBar = new Widget;
        _bar->add(_infoBar);
        _infoBar->setPosition(64.f + 4.f + 4.f, 4.f + 4.f);

        _nameText = new TextWidget;
        _infoBar->add(_nameText);
        _nameText->setPosition(0.f, 0.f);
        _nameText->setCharacterSize(14);

        _costBar.setParent(_infoBar);
        _costBar.setPosition(0.f, _size.y - 16.f - _costBar.getPPSize().y);
        _costBar.setGrayPPPosition(_size.x - 16.f - _costBar.getPPSize().x - 64.f, 0.f);
        //_costBar.setGrayPPPosition(10.f, 0.f);

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
    void SkillBar::move(float x, float y)
    {
        _bar->move(x, y);
    }
    

    void SkillBar::setBarTexture(sf::Texture* mainTexture, sf::Texture* lockTexture)
    {
        _textureBar = mainTexture;
        _textureLocked = lockTexture;
        _bar->setTexture(_textureBar);
        auto size = static_cast<sf::Vector2f>(mainTexture->getSize());
        _bar->setScale({_size.x/size.x, _size.y/size.y});
    }

    void SkillBar::setSkill(Skill* skill)
    {
        std::cout << "SkillBar: ";
        std::cout << skill->getName() << " initing...\n";
        _skill = skill;
        _nameText->setString(skill->getName());
        _costBar.setSkill(skill);
        _icon->setTexture(skill->getTexture());
        _isBought = _skill->isBought();
    }

    const std::string& SkillBar::getIconPath() const
    {
        assert(_skill);
        return _skill->getTexturePath();
    }
    void SkillBar::setFont(sf::Font* font)
    {
        _nameText->setFont(font);
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
        if(!_isBought)
        {
            if(_skill->canBeBoughtFrom(source))
            {
                _skill->buyFrom(source);
                _sourceBar.recalculate();
                _isBought = true;
                _bar->setTexture(_textureLocked);
                //_parentArea.recalculate();
            }
        }
        else
        {
            _skill->returnCostsTo(source);
            _sourceBar.recalculate();
            _isBought = false;
            _bar->setTexture(_textureBar);
        }
    }

    void SkillBar::loadAssetsFromGUI(GUI& gui)
    {
        _costBar.loadAssetsFromGUI(gui);
    }
    
    bool SkillBar::operator<(const SkillBar& rhs)
    {
        if(_isBought == rhs._isBought)
        {
            return _skill->getName() < rhs._skill->getName();
        }
        return !_isBought;
    }
}