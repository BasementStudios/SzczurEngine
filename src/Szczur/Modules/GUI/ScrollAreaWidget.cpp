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
    _offset(0.f) 
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

    sf::Vector2u ScrollAreaWidget::_getSize() const {
        return getMinimalSize();
    }

    void ScrollAreaWidget::_draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(_scroller, states);
    }

    void ScrollAreaWidget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        _renderTexture.clear(sf::Color::Transparent);
        auto childrenStates = sf::RenderStates::Default;
        childrenStates.transform *= _childrenTransform;

        for(auto child : _children) _renderTexture.draw(*child, childrenStates);

        _renderTexture.display();
        target.draw(sf::Sprite(_renderTexture.getTexture()), states);
    }

    void ScrollAreaWidget::_update(float deltaTime) {
    }

    void ScrollAreaWidget::_input(const sf::Event& event) 
    {
        float maxOffset = -(_childrenHeight - float(_getSize().y));
        float oldProp = _scroller.getProportion();
        if(_isHovered && event.type == sf::Event::MouseWheelScrolled) 
        {
           float propOffset = -static_cast<float>(event.mouseWheelScroll.delta) * 0.07f;
           _scroller.moveProportion(propOffset);
        }
        
        sf::Event tempEvent(event);
        if(event.type == sf::Event::MouseMoved)
        {
            tempEvent.mouseMove.x -= (_scroller.getPosition().x) * _winProp.x;
            tempEvent.mouseMove.y -= (_scroller.getPosition().y) * _winProp.y;
        }
        _scroller.input(tempEvent);
        

        if(oldProp != _scroller.getProportion())
        {
            _childrenTransform.translate(0.f, -_offset);
            _offset = maxOffset * _scroller.getProportion();
            _childrenTransform.translate(0.f, _offset);

        }
        
    }
    /*
    void ScrollAreaWidget::_inputChildren(sf::Event event)
    {
        if(_isHovered)
        {
            for(auto it : _children) 
            {
                if(event.type == sf::Event::MouseMoved)
                {
                    auto itPosition = it->getPosition();
                    sf::Event tempEvent(event);
                    tempEvent.mouseMove.x += int((-itPosition.x) * _winProp.x);
                    tempEvent.mouseMove.y += int((-itPosition.y - _offset) * _winProp.y);
                    it->input(tempEvent);
                }
                else it->input(event);
            }
        }
    }*/
    sf::Vector2f ScrollAreaWidget::_getChildrenShift() const
    {
        return { 0.f, _offset};
    }

    sf::Vector2u ScrollAreaWidget::_getChildrenSize()
    {
        return {};
    }
    

    void ScrollAreaWidget::_calculateSize()
    {
        auto size = getSize();

        size.x = std::max(size.x, _minScrollSize.x);
        size.y = std::max(size.y, _minScrollSize.y);

        [[maybe_unused]] float barWidth = float(_minScrollSize.x);
        float barX = float(size.x - _minScrollSize.x);

        _scroller.setPosition(barX, 0.f);
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