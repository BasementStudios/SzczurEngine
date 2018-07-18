#include "SkillSlot.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"

#include "Szczur/Modules/PrepScreen/Skill/Skill.hpp"
#include "Szczur/Modules/PrepScreen/Prepscreen.hpp"

namespace rat
{
    SkillSlot::SkillSlot(PrepScreen& prepScreen)
    :
    _prep(prepScreen)
    {
        GlyphTypes glyphes;
        GlyphesConverter converter;
        _glyphesTexs.clear();
        //_glyphesTexs.reserve(4);
        for(auto glyph : glyphes)
        {
            _glyphesTexs.emplace(converter.toEnum(glyph), nullptr);
        }

        const sf::Vector2f prSize = { 0.12f, 0.12f };

        _background = new ImageWidget;
        _background->makeChildrenPenetrable();
        _background->makeChildrenUnresizable();
        _background->setPropSize(prSize);

        _background->setCallback(Widget::CallbackType::onPress, [this](auto){
            this->_onClick();
        });
        _background->setCallback(Widget::CallbackType::onHoverIn, [this](auto){
            this->_onHoverIn();
        });
        _background->setCallback(Widget::CallbackType::onHoverOut, [this](auto){
            this->_onHoverOut();
        });
        

        _icon = new ImageWidget;
        _background->add(_icon);
        _icon->setPropSize(prSize);
        _icon->setPropPosition(0.5f, 0.5f);

        _ppCost = new TextWidget;
        _background->add(_ppCost);
        _ppCost->setCharacterPropSize(0.04f);
        _ppCost->setColor(sf::Color::White);
        _ppCost->setOutlineColor(sf::Color::Black);
        _ppCost->setOutlinePropThickness(0.04f);
        _ppCost->setPropOrigin(0.5f, 0.5f);
        //_ppCost->makeStaticPropPositing();
        _ppCost->setPropPosition(1.f, 1.f);
        _ppCost->hide();

        _glyphes.resize(3, nullptr);
        for(auto& glyphIcon : _glyphes)
        {
            glyphIcon = new ImageWidget;
            glyphIcon->setPropSize(0.03f, 0.03f);
            glyphIcon->setPropOrigin(0.75f, 0.75f);
            //glyphIcon->makeStaticPropPositing();
            glyphIcon->hide();
            _background->add(glyphIcon);
        }
    }
    void SkillSlot::setParent(Widget* parent)
    {
        parent->add(_background);
    }

    void SkillSlot::setSkill(const Skill* skill)
    {
        _skill = skill;
        _icon->setTexture(skill->getTexture());

        _canBeBought = _prep.canSkillBeBought(skill);

        auto& ppCost = _skill->getCostInfo();
        size_t cost = ppCost.getCost();
        _ppCost->setString(std::to_string(cost));
        _ppCost->show();

        size_t glyphesAmount = ppCost.getNumberOfRequirements();

        for(auto* glyphIcon : _glyphes) glyphIcon->hide();

        size_t i = 0;
        for(auto& [glyph, power] : ppCost)
        {
            auto* widget = _glyphes[i];
            widget->show();

            auto* texture = _glyphesTexs.find(glyph)->second;
            widget->setTexture(texture);

            sf::Vector2f pos = {1.f, 0.f};

            if(glyphesAmount == 2)
            {
                if(i == 0) pos.x = 0.8f;
                else pos.y = 0.2f;
            }
            else if(glyphesAmount == 3)
            {
                if(i == 0) pos.x = 0.65f;
                else if(i == 2) pos.y = 0.35f;
            }

            widget->setPropPosition(pos);

            ++i;
        }
    }

    void SkillSlot::loadAssetsFromGUI(GUI& gui)
    {
        size_t i = 0;
        GlyphesConverter converter;
        for(auto& [glyph, texture] : _glyphesTexs)
        {
            const std::string path = "Assets/PrepScreen/Glyphes/" + converter.toString(glyph) + ".png";
            texture = gui.getAsset<sf::Texture>(path);
        }   


        _ppCost->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoSerif-Regular.ttf"));
        _background->setTexture(gui.getTexture("Assets/PrepScreen/SkillBack.png"));
    }

    void SkillSlot::recalculateAvailability()
    {
        bool isBought = _skill->isBought();
        if(_isKnownAsBought != isBought)
        {
            if(isBought)
            {
                _background->fullyDeactivate();
            }
            else
            {
                _background->fullyActivate();
            }
            _isKnownAsBought = isBought;
        }

        _canBeBought = _prep.canSkillBeBought(_skill);
        if(_canBeBought)
        {
            _background->setColorInTime({255, 255, 255}, 0.1f);
        }
        else
        {
            _background->setColorInTime({125, 125, 125}, 0.1f);
        }
    }

    void SkillSlot::removeSkill()
    {
        _icon->removeTexture();
        _ppCost->hide();
        for(auto* gl : _glyphes) gl->show();
    }

    void SkillSlot::_onClick()
    {      
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(!_skill) return;
            if(_isKnownAsBought) return;

            if(_prep.canSkillBeBought(_skill))
            {
                _prep.buySkill(_skill);
            }
        }
    }

    void SkillSlot::_onHoverIn()
    {
        if(_canBeBought)
        {
            _prep.dimPPsNeededToBuySkill(_skill);
        }
    }
    void SkillSlot::_onHoverOut()
    {
        if(_canBeBought)
        {
            _prep.normPPsNeededToBuySkill(_skill);
        }
    }
}