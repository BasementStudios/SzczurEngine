#include "BaseBar.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat
{
    BaseBar::BaseBar()
    {
        _base = new Widget;
    }
    void BaseBar::setPosition(float x, float y)
    {
        _base->setPosition(x, y);
    }
    void BaseBar::setParent(Widget* parent)
    {
        parent->add(_base);
    }
    void BaseBar::_addWidget(Widget* addon)
    {
        _base->add(addon);
    }
    void BaseBar::_setWidgetSize(ImageWidget* widget, float x, float y)
    {
        auto* texture = widget->getTexture();
        if(!texture) return;
        auto size = static_cast<sf::Vector2f>(texture->getSize());
        widget->setScale({x/size.x, y/size.y});
    }
    
        
}