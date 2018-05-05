#include "BaseBar.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat
{
    BaseBar::BaseBar()
    {
        _base = new Widget;
    }
    void BaseBar::setPosition(const sf::Vector2f& position)
    {
        setPosition(position.x, position.y);
    }
    void BaseBar::move(float offsetX, float offsetY)
    {
        move({offsetX, offsetY});
    }
    void BaseBar::move(const sf::Vector2f& offset)
    {
        setPosition(_base->getPosition() + offset);
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
    void BaseBar::deactivate()
    {
        _base->deactivate();
    }
    void BaseBar::activate()
    {
        _base->activate();
    }
    /*
    void BaseBar::invisible()
    {
        _base->invisible();
    }
    void BaseBar::visible()
    {
        _base->visible();
    }
    */
        
}