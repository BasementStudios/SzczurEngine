#include <algorithm>

#include "ColoredPPBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "../PPColors.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    ColoredPPBar::ColoredPPBar(PPContainer& source, Widget* parent)
    :
    _source(source)
    {
        _background = new ImageWidget;
        parent->add(_background);
        recalculate();
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
            auto found = _pps.find(color);
            if(found == _pps.end())
            {
                ImageWidget* newPP = new ImageWidget;
                newPP->setTexture(_textures[color]);
                _background->add(newPP);
                _pps.emplace(color, newPP);
            }
            else 
            {
                found->second->activate();
            }
            found->second->setPosition(float(i) * size, 0.f);
            i++;
        } 
        recalcOrigin();
    }
    
    void ColoredPPBar::initTexturesViaGui(GUI& gui)
    {
        PPColors colors;
        for(const auto& color : colors)
        {
            const std::string path = "assets/PrepScreen" + color + "PP.png";
            gui.addAsset<sf::Texture>(path);
            _textures.emplace(color, gui.getAsset<sf::Texture>(path));
        }
    }
}