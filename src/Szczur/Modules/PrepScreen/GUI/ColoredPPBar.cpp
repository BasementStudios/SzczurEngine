#include <algorithm>

#include "ColoredPPBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "../PPColors.hpp"

namespace rat
{
    ColoredPPBar::ColoredPPBar(PPContainer& source)
    :
    PPBar(source)
    {
    }

    void ColoredPPBar::recalculate()
    {     
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
            ImageWidget* pp{nullptr};

            auto found = _pps.find(color);
            if(found == _pps.end())
            {
                pp = new ImageWidget;
                pp->setTexture(_textures[color]);
                //pp->setScale({1.f, 1.f});
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
        } 
        recalcOrigin();
        
    }
    
    void ColoredPPBar::initTexturesViaGui(GUI& gui)
    {
        PPColors colors;
        for(const auto& color : colors)
        {
            const std::string path = "Assets/PrepScreen/" + color + "PP.png";
            gui.addAsset<sf::Texture>(path);
            _textures.emplace(color, gui.getAsset<sf::Texture>(path));
        }
    }  
}