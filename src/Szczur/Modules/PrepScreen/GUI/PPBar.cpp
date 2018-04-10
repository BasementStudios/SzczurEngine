#include <algorithm>

#include "PPBar.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "../PPColors.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    PPBar::PPBar(PPContainer& source)
    :
    _source(source)
    {
        _background = new Widget;
    }

    void PPBar::setCenter(float x, float y)
    {
         _background->setPosition(x, y);
    }

    void PPBar::setCenter(const sf::Vector2f& center)
    {
        _background->setPosition(center);
    }
    
    void PPBar::recalcOrigin()
    {
        _background->calculateSize();
        auto size = static_cast<sf::Vector2f>(_background->getSize());
        _background->setOrigin(size.x * 0.5f, 0.f);
    }

    void PPBar::setParent(Widget* parent)
    {
        parent->add(_background);
    }
    
}