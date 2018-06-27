#include "BaseBar.hpp"

#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

namespace rat
{
    BaseBar::BaseBar()
    {
        _base = new Widget;
    }
    BaseBar::BaseBar(Widget* base)
    {
        _base = base;
    }
    void BaseBar::setPosition(const sf::Vector2i& position)
    {
        setPosition(position.x, position.y);
    }
    void BaseBar::move(int offsetX, int offsetY)
    {
        move({offsetX, offsetY});
    }
    void BaseBar::move(const sf::Vector2i& offset)
    {
        setPosition(_base->getPosition() + offset);
    }
    void BaseBar::setPosition(int x, int y)
    {
        _base->setPosition(x, y);
    }
    sf::Vector2i BaseBar::getPosition() const
    {
        return _base->getPosition();
    }

    void BaseBar::setPropSize(const sf::Vector2f& propSize)
    {
        _base->setPropSize(propSize);
    }
    void BaseBar::setPropSize(float propWidth, float propHeight)
    {
        _base->setPropSize(propWidth, propHeight);
    }
    void BaseBar::setPropPosition(const sf::Vector2f& propPos)
    {
        _base->setPropPosition(propPos);
    }
    void BaseBar::setPropPosition(float propX, float propY)
    {
        _base->setPropPosition(propX, propY);
    }
    
    void BaseBar::setParent(Widget* parent)
    {
        parent->add(_base);
    }
    void BaseBar::setSize(const sf::Vector2i& size)
    {
        _base->setSize(size);
    }
    void BaseBar::setSize(int width, int height)
    {
        setSize({width, height});
    }
    sf::Vector2i BaseBar::getSize() const
    {
        auto width = std::max(_base->getSize().x, _base->getMinimalSize().x);
        auto height = std::max(_base->getSize().y, _base->getMinimalSize().y);
        return {width, height};
    }
    
    void BaseBar::_addWidget(Widget* addon)
    {
        _base->add(addon);
    }
    void BaseBar::_addBar(BaseBar& addon)
    {
        addon.setParent(_base);
    }
    
    void BaseBar::deactivate()
    {
        _base->deactivate();
        _base->invisible();
    }
    void BaseBar::activate()
    {
        _base->activate();
        _base->visible();
    }
    bool BaseBar::isActivate() const
    {
        return _base->isActivated();
    }
    void BaseBar::setPropOrigin(const sf::Vector2f& propOrigin)
    {
        _base->setPropOrigin(propOrigin);
    }
    void BaseBar::setPropOrigin(float x, float y)
    {
        setPropOrigin({x, y});
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