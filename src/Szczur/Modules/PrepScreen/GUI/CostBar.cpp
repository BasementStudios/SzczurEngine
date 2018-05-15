#include "CostBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "../GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp" 


namespace rat
{
    CostBar::CostBar()
    :
    BaseBar()
    {
        _costPP = new ImageWidget;
        _costPP->setPropOrigin(1.f, 0.f);
        _addWidget(_costPP);

        _costAmount = new TextWidget;
        _addWidget(_costAmount);
        GlyphTypes glyphs;
        GlyphesConverter converter;
        for(auto& glyph : glyphs)
        {
           ImageWidget* reqWidget = new ImageWidget;
           _addWidget(reqWidget);
           reqWidget->invisible();
           _glyphs.emplace(converter.toEnum(glyph), reqWidget);
        }
        _recalculate();
    }

    sf::Vector2f CostBar::getPPSize() const
    {
        return {_dim, _dim};
    }
    

    void CostBar::setSkill(Skill* skill)
    {
        _skill = skill;
        auto& ppCost = _skill->getCostInfo();
        size_t cost = ppCost.getCost();

        if(cost == 0)
        {
            _costAmount->invisible();
        }
        else
        {
            _costAmount->visible();
            _costAmount->setString(std::to_string(cost));
        }

        _recalculateGlyphs();
    }
    
    void CostBar::loadAssetsFromGUI(GUI& gui)
    {
        size_t i = 0;
        GlyphesConverter converter;
        for(auto& [glyph, widget] : _glyphs)
        {
            const std::string path = "Assets/Test/" + converter.toString(glyph) + "Glyph.png";
            sf::Texture* texture = gui.getAsset<sf::Texture>(path);
            widget->setTexture(texture);
            widget->setSize(_dim, _dim);
        }        
        _costAmount->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _setIconTexture(gui.getAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png"));
    }

    void CostBar::_setIconTexture(sf::Texture* texture)
    {
        _costPP->setTexture(texture);
        _costPP->setSize(_dim, _dim); 
    }

    void CostBar::_recalculateCostAmount()
    {
        auto textSize = static_cast<sf::Vector2f>(_costAmount->getSize());
        auto ppSize = getPPSize();
        auto ppPos = _costPP->getPosition();

        auto newTextPos = ((ppSize - textSize)/2.f) + ppPos;

        _costAmount->setPosition(newTextPos);
    }

    void CostBar::setWidth(float width)
    {
        _width = width;
        _recalculate();
    }
    
    
    void CostBar::_recalculateGlyphs()
    {
        auto& cost = _skill->getCostInfo();

        size_t j = 0;
        for(auto& [glyph, widget] : _glyphs)
        {
            widget->invisible();
            widget->setPosition(0.f, 0.f);
        }

        size_t i = 0;
        _numOfActivated = cost.getNumberOfRequirements();
        for(auto& [glyph, power] : cost)
        {
            auto found = _glyphs.find(glyph);
            auto* widget = found->second;
            widget->visible();
        }
        _recalculate();
    }

    void CostBar::_recalculate()
    {
        if(_numOfActivated == 0) return;
        float dim = _padding + _dim;
        float glyphedWidth = float(_numOfActivated) * dim;
        size_t i = 0;
        for(auto& [glyph, widget] : _glyphs)
        {
            if(widget->isVisible())
            {
                widget->setPosition(float(i) * dim, 0.f);
                i++;
            }
        }
        _costPP->setPosition(0.f, _width);
    }
    
    
    
}