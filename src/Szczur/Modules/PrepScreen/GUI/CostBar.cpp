#include "CostBar.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "../PPColors.hpp"

#include "Szczur/Utility/Logger.hpp" 


namespace rat
{
    CostBar::CostBar()
    {
        _base = new Widget;

        _costPP = new ImageWidget;
        _base->add(_costPP);

        _costAmount = new TextWidget;
        _base->add(_costAmount);
        setGrayPPPosition(0.f, 150.f);
        PPColors colors;
        for(auto& color : colors)
        {
           ImageWidget* reqWidget = new ImageWidget;
           _base->add(reqWidget);
           reqWidget->invisible();
           _coloredPPs.emplace(color, reqWidget);
        }
    }

    sf::Vector2f CostBar::getPPSize() const
    {
        return {_dim, _dim};
    }
    

    void CostBar::setSkill(Skill* skill)
    {
        _skill = skill;
        auto& ppCost = _skill->getCostInfo();
        size_t amountOfColPPs = ppCost.getNumberOfRequirements();
        size_t cost = ppCost.getCost();

        if(cost == 0 || cost == 1)
        {
            _costAmount->invisible();
        }
        else
        {
            _costAmount->visible();
            _costAmount->setString(std::to_string(cost));
        }

        _coloredPPs.clear();

        _recalculateCostAmount();
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

            auto size = static_cast<sf::Vector2f>(texture->getSize());
            widget->setScale({_dim/size.x, _dim/size.y});
        }        
        _costAmount->setFont(gui.getAsset<sf::Font>("assets/fonts/NotoMono.ttf"));
        _setIconTexture(gui.getAsset<sf::Texture>("assets/PrepScreen/PP.png"));
    }

    void CostBar::setParent(Widget* parent)
    {
        parent->add(_base);
    }

    void CostBar::_setIconTexture(sf::Texture* texture)
    {
        _costPP->setTexture(texture);
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        _costPP->setScale({_dim/size.x, _dim/size.y}); 
    }

    void CostBar::setPosition(float x, float y)
    {
        _base->setPosition(x, y);
    }

    void CostBar::_recalculateCostAmount()
    {
        auto textSize = static_cast<sf::Vector2f>(_costAmount->getSize());
        auto ppSize = getPPSize();
        auto ppPos = _costPP->getPosition();

        auto newTextPos = ((ppSize - textSize)/2.f) + ppPos;

        _costAmount->setPosition(newTextPos);
    }
    
    void CostBar::_recalculateColored()
    {
        auto& cost = _skill->getCostInfo();

        size_t j = 0;
        for(auto& [color, widget] : _coloredPPs)
        {
            widget->visible();
            widget->setPosition(float(j++)*(_dim + _padding), 0.f);
        }

        size_t i = 0;
        for(auto& [color, power] : cost)
        {
            std::cout << _skill->getName() << ": color: " << color <<"\n";
            auto found = _coloredPPs.find(color);
            auto* widget = found->second;
            if(widget->isVisible()) std::cout << "Is visible\n";
            widget->visible();
            widget->setPosition(float(i++)*(_dim + _padding), 0.f);
        }
    }
    
    
}