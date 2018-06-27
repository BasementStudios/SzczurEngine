#include "ScrollAreaWidget.hpp"
#include <iostream>

#include "Test.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Logger.hpp"

namespace rat {
    ScrollAreaWidget::ScrollAreaWidget() :
    Widget(),
    _scrollSpeed(10.f),
    _offset(0) 
    {
        _scroller.setWidthProportion(2.1f);
        setSize(10 + _minScrollSize.x, _minScrollSize.y);
        _aboutToRecalculate = true;
    }

    void ScrollAreaWidget::initScript(Script& script) {
        auto object = script.newClass<ScrollAreaWidget>("ScrollAreaWidget", "GUI");

        basicScript(object);
        object.setProperty(
            "size",
            [](ScrollAreaWidget& owner){owner.getSize();},
            [](ScrollAreaWidget& owner, sol::table tab){owner.setSize(tab[1], tab[2]);}
        );
        object.setProperty(
            "scrollSpeed",
            [](ScrollAreaWidget& owner){owner.getScrollSpeed();},
            [](ScrollAreaWidget& owner, float speed){owner.setScrollSpeed(speed);}
        );
        object.init();
    }
    void ScrollAreaWidget::setScrollerTexture(sf::Texture* texture, int boundsHeight)
    {
        _scroller.setScrollerTexture(texture, boundsHeight);
    }
    void ScrollAreaWidget::setPathTexture(sf::Texture* texture)
    {
        _scroller.setPathTexture(texture);
    }
    void ScrollAreaWidget::setBoundsTexture(sf::Texture* texture)
    {
        _scroller.setBoundTexture(texture);
    }

    void ScrollAreaWidget::setScrollSpeed(float speed) {
        _scrollSpeed = speed;
    }

    float ScrollAreaWidget::getScrollSpeed() const {
        return _scrollSpeed;
    }

    sf::Vector2i ScrollAreaWidget::_getSize() const {
        return getMinimalSize();
    }

    void ScrollAreaWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_scroller, states);
    }

    void ScrollAreaWidget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        _renderTexture.clear(sf::Color::Transparent);
        

        for(auto child : _children) _renderTexture.draw(*child);

        _renderTexture.display();
        _displaySprite.setTexture(_renderTexture.getTexture());
        target.draw(_displaySprite, states);
    }

    void ScrollAreaWidget::_update(float deltaTime) {
    }

    void ScrollAreaWidget::_recalcPos()
    {
        auto size = getSize();
        size.x = std::max(size.x, _minScrollSize.x);

        float barX = float(size.x - _minScrollSize.x);

        auto basePos = static_cast<sf::Vector2f>(gui::FamilyTransform::getDrawPosition());

        _scroller.setPosition(basePos + sf::Vector2f{float(barX), 0});
        _displaySprite.setPosition(basePos + sf::Vector2f(getPadding()));
    }

    void ScrollAreaWidget::_recalcChildrenPos()
    {
        for(auto* child : _children)
        {
            child->gui::FamilyTransform::applyParentPosition(gui::FamilyTransform::getAbsolutePosition(), {0, -_offset});
        }
    }

    void ScrollAreaWidget::_input(const sf::Event& event) 
    {
        float maxOffset = -(_childrenHeight - _getSize().y);

        float oldProp = _scroller.getProportion();

        if(_isHovered && event.type == sf::Event::MouseWheelScrolled) 
        {
           float propOffset = float(-event.mouseWheelScroll.delta) * 0.01f * _scrollSpeed;
           _scroller.moveProportion(propOffset);
        }
        
        _scroller.input(event);
        

        if(oldProp != _scroller.getProportion())
        {
            _offset = int(maxOffset * _scroller.getProportion());
            _isPosChanged = true;
        }
    }


    sf::Vector2i ScrollAreaWidget::_getChildrenSize()
    {
        return {};
    }
    

    void ScrollAreaWidget::_calculateSize()
    {
        auto size = getSize();

        size.x = std::max(size.x, _minScrollSize.x);
        size.y = std::max(size.y, _minScrollSize.y);

        float barWidth = float(_minScrollSize.x);
        _scroller.setSize(_minScrollSize.x, size.y);

        sf::Vector2u rTexSize = { size.x - _minScrollSize.x - (unsigned int)(getPadding().x * 2.f), size.y - (unsigned int)(getPadding().y * 2.f) };

        auto* window = detail::globalPtr<Window>; 
        window->pushGLStates(); 
        _renderTexture.create(rTexSize.x, rTexSize.y); 
        window->popGLStates();

        _childrenHeight = float(std::max(Widget::_getChildrenSize().y, size.y));
        _childrenHeightProp = _childrenHeight/float(size.y);
        _scroller.setScrollerHeightProp(_childrenHeightProp);
    }

    void ScrollAreaWidget::_callback(CallbackType type) 
    {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

}