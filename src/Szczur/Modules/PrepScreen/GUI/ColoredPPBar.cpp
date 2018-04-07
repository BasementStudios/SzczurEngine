#include <algorithm>

#include "ColoredPPBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "../PPColors.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ColoredPPBar::ColoredPPBar(PPContainer& source)
    :
    _source(source)
    {
        _background = new Widget;
    }

    void ColoredPPBar::setCenter(float x, float y)
    {
         _background->setPosition(x, y);
    }

    void ColoredPPBar::setCenter(const sf::Vector2f& center)
    {
        _background->setPosition(center);
    }
    void ColoredPPBar::recalcOrigin()
    {
        _background->calculateSize();
        auto size = static_cast<sf::Vector2f>(_background->getSize());
        _background->setOrigin(size.x * 0.5f, 0.f);
    }

    void ColoredPPBar::recalculate()
    {     
        LOG_INFO(this, " : PrepScreen : ColoredPPBar : recalculating...");

        for(auto& [color, widget] : _pps)
        {
            widget->setPosition(0.f, 0.f);
            widget->deactivate();
        }

        size_t i = 0;
        float size = 80.f;
        for(auto& [coloredPP, amount] : _source)
        {
            const std::string& color = coloredPP.type;

            LOG_INFO(this, " : PrepScreen : ColoredPPBar : " + color + " adding");
            ImageWidget* pp{nullptr};

            auto found = _pps.find(color);
            if(found == _pps.end())
            {
                pp = new ImageWidget;
                pp->setTexture(_textures[color]);
                _background->add(pp);
                _pps.emplace(color, pp);
            }
            else 
            {
                pp = found->second;
                pp->activate();
            }
            sf::Vector2f newPos = {float(i) * size, 0.f};
            pp->setPosition(newPos);
            i++;

            LOG_INFO(this, " : PrepScreen : ColoredPPBar : " + color + " added");
            LOG_INFO(this, " : at position x: " + std::to_string(newPos.x) + " y: " + std::to_string(newPos.y) + ".");
        } 
        recalcOrigin();
        LOG_INFO(this, " : PrepScreen : ColoredPPBar : recalculated");
        
    }
    
    void ColoredPPBar::initTexturesViaGui(GUI& gui)
    {
        PPColors colors;
        for(const auto& color : colors)
        {
            const std::string path = "assets/PrepScreen/" + color + "PP.png";
            gui.addAsset<sf::Texture>(path);
            _textures.emplace(color, gui.getAsset<sf::Texture>(path));
        }
    }

    void ColoredPPBar::setParent(Widget* parent)
    {
        parent->add(_background);
    }
    
}