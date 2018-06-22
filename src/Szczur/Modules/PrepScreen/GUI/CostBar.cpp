#include "CostBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/ListWidget.hpp"
#include "../GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp" 


namespace rat
{
    CostBar::CostBar()
    :
    BaseBar()
    {
        setPropSize(0.18f, 0.04f);

        _costPP = new ImageWidget;
        _costPP->setPropPosition(1.f, 0.f);
        _costPP->setPropSize(0.04f, 0.04f);
        _costPP->makeChildrenUnresizable();
        _addWidget(_costPP);

        _costAmount = new TextWidget;
        _costAmount->setPropPosition(0.5f, 0.5f);
        _costAmount->setCharacterSize(18);
        _costPP->add(_costAmount);

        GlyphTypes glyphs;
        GlyphesConverter converter;

        auto* glyphList = new ListWidget;
        _addWidget(glyphList);
        glyphList->makeHorizontal();
        glyphList->setBetweenPadding(3.f);

        for(auto& glyph : glyphs)
        {
           ImageWidget* reqWidget = new ImageWidget;
           reqWidget->setPropSize(0.04f, 0.04f);
           glyphList->add(reqWidget);
           reqWidget->invisible();
           _glyphs.emplace(converter.toEnum(glyph), reqWidget);
        }
    }  

    void CostBar::setSkill(const Skill* skill)
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

    void CostBar::removeSkill()
    {
        for(auto& [glyph, widget] : _glyphs) widget->invisible();
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
        }        
        _costAmount->setFont(gui.getAsset<sf::Font>("Assets/fonts/NotoMono.ttf"));
        _costPP->setTexture(gui.getAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png"));
    }
   
    void CostBar::_recalculateGlyphs()
    {
        auto& cost = _skill->getCostInfo();

        for(auto& [glyph, widget] : _glyphs) widget->invisible();
        
        for(auto& [glyph, power] : cost)
        {
            auto found = _glyphs.find(glyph);
            auto* widget = found->second;
            widget->visible();
        }
    }  
}