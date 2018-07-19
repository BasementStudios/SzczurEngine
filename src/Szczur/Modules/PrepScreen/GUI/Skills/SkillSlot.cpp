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

        _base = new Widget;
        _base->setPropSize(prSize);
        _base->makeChildrenPenetrable();

        _glyphes.resize(3, nullptr);
        for(auto& glyphIcon : _glyphes)
        {
            glyphIcon = new ImageWidget;
            glyphIcon->setPropSize(0.027f, 0.027f);
            glyphIcon->setPropOrigin(0.5f, 0.5f);
            glyphIcon->makeStaticPropPositing();
            glyphIcon->hide();
            _base->add(glyphIcon);
        }

        _background = new ImageWidget;
        _base->add(_background);
        _background->makeChildrenUnresizable();
        _background->setPropSize(prSize);

        _base->setCallback(Widget::CallbackType::onPress, [this](auto){
            this->_onClick();
        });
        _base->setCallback(Widget::CallbackType::onHoverIn, [this](auto){
            this->_onHoverIn();
        });
        _base->setCallback(Widget::CallbackType::onHoverOut, [this](auto){
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

    }
    void SkillSlot::setParent(Widget* parent)
    {
        parent->add(_base);
    }

    void SkillSlot::setSkill(const Skill* skill)
    {
        _skill = skill;
        _icon->setTexture(skill->getTexture());
        _icon->setColor({255, 255, 255, 255});

        _canBeBought = _prep.canSkillBeBought(skill);

        auto& ppCost = _skill->getCostInfo();
        size_t cost = ppCost.getCost();
        _ppCost->setString(std::to_string(cost));
        _ppCost->show();

        size_t glyphesAmount = ppCost.getNumberOfRequirements();

        for(auto glyphIcon : _glyphes) glyphIcon->hide();

        size_t i = 0;
        for(auto& [glyph, power] : ppCost)
        {
            auto* widget = _glyphes[i];
            widget->show();

            auto* texture = _glyphesTexs.find(glyph)->second;
            widget->setTexture(texture);

            sf::Vector2f pos = {0.f, 0.5f};
            const float a = 0.2f;

            if(glyphesAmount == 2)
            {
                if(i == 0) pos.y = a;
                else pos.y = 1.f - a;
            }
            else if(glyphesAmount == 3)
            {
                if(i == 0) pos.y = a;
                else if(i == 2) pos.y = 1.f - a;
            }

            widget->setPropPosition(pos);

            ++i;
        }
        _glyphesAmount = i;
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
        _canBeBought = _prep.canSkillBeBought(_skill);

        if(isBought)
        {
            _onBought();
        }
        else
        {
            _onSale();
            if(_canBeBought)
            {
                _base->setColorInTime({255, 255, 255}, 0.1f);
            }
            else
            {
                _base->setColorInTime({90, 90, 90}, 0.1f);
            }
        }
        _isKnownAsBought = isBought;


        
    }

    void SkillSlot::removeSkill()
    {
        _icon->removeTexture();
        _ppCost->hide();
        for(auto* gl : _glyphes) gl->hide();
        _base->setColor(sf::Color::White);
        //_skill = nullptr;
    }

    void SkillSlot::_onSale()
    {
        _icon->setColor({255, 255, 255, 255});
        _ppCost->show();
        for(int i = 0; i < _glyphesAmount; ++i) _glyphes[i]->show();
    }
    void SkillSlot::_onBought()
    {
        _icon->setColor({64, 64, 64, 200});
        //_ppCost->hide();
        //for(auto* gl : _glyphes) gl->hide();
        _base->setColor(sf::Color::White);
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