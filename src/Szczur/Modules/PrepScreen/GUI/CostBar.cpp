#include "CostBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "../PPColors.hpp"

#include "Szczur/Utility/Logger.hpp" 


namespace rat
{
    CostBar::CostBar()
    :
    BaseBar()
    {
        _costPP = new ImageWidget;
        _addWidget(_costPP);

        _costAmount = new TextWidget;
        _addWidget(_costAmount);
        PPColors colors;
        for(auto& color : colors)
        {
           ImageWidget* reqWidget = new ImageWidget;
           _addWidget(reqWidget);
           reqWidget->invisible();
           _coloredPPs.emplace(color, reqWidget);
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

        _recalculateColored();
    }

    void CostBar::setGrayPPPosition(float x, float y)
    {
        _costPP->setPosition(x, y);
        _recalculateCostAmount();
    }
    
    
    void CostBar::loadAssetsFromGUI(GUI& gui)
    {
        size_t i = 0;
        for(auto& [color, widget] : _coloredPPs)
        {
            const std::string path = "assets/PrepScreen/" + color + "PP.png";
            sf::Texture* texture = gui.getAsset<sf::Texture>(path);
            widget->setTexture(texture);

            _setWidgetSize(widget, _dim, _dim);
        }        
        _costAmount->setFont(gui.getAsset<sf::Font>("assets/fonts/NotoMono.ttf"));
        _setIconTexture(gui.getAsset<sf::Texture>("assets/PrepScreen/PP.png"));
    }

    void CostBar::_setIconTexture(sf::Texture* texture)
    {
        _costPP->setTexture(texture);
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        _costPP->setScale({_dim/size.x, _dim/size.y}); 
    }

    void CostBar::_recalculateCostAmount()
    {
        auto textSize = static_cast<sf::Vector2f>(_costAmount->getSize());
        auto ppSize = getPPSize();
        auto ppPos = _costPP->getPosition();

        auto newTextPos = ((ppSize - textSize)/2.f) + ppPos;

        _costAmount->setPosition(newTextPos);
        std::cout << "PP pos: x: " << ppPos.x << " y: " << ppPos.y << "\n";
    }

    void CostBar::setWidth(float width)
    {
        _width = width;
        _recalculate();
    }
    
    
    void CostBar::_recalculateColored()
    {
        auto& cost = _skill->getCostInfo();

        size_t j = 0;
        for(auto& [color, widget] : _coloredPPs)
        {
            widget->invisible();
            widget->setPosition(0.f, 0.f);
        }

        size_t i = 0;
        _numOfActivated = cost.getNumberOfRequirements();
        for(auto& [color, power] : cost)
        {
            auto found = _coloredPPs.find(color);
            auto* widget = found->second;
            widget->visible();
        }
        _recalculate();
    }

    void CostBar::_recalculate()
    {
        std::cout << "Recalculating...\n";
        if(_numOfActivated == 0) return;
        float dim = _padding + _dim;
        float coloredWidth = float(_numOfActivated) * dim;
        /*
        if(coloredWidth < _width - _dim)
        {
            coloredWidth = _width - _dim;
            dim = coloredWidth/float(_numOfActivated);            
        }*/
        size_t i = 0;
        for(auto& [color, widget] : _coloredPPs)
        {
            if(widget->isVisible())
            {
                widget->setPosition(float(i) * dim, 0.f);
                i++;
            }
        }
        setGrayPPPosition(_width - dim, 0.f);
    }
    
    
    
}