#include "ScrollAreaWidget.hpp"
#include <iostream>
#include <cassert>

#include "Test.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Utility/Logger.hpp"

#include "InterfaceWidget.hpp"

#include "Animation/Anim.hpp"

namespace rat {
    ScrollAreaWidget::ScrollAreaWidget() :
    Widget(),
    _scrollSpeed(10.f),
    _offset(0) 
    {
        _scroller.setWidthProportion(2.1f);
        setSize(10 + _minScrollSize.x, _minScrollSize.y);
        _aboutToRecalculate = true;
        resetScrollerPosition();
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
    void ScrollAreaWidget::setScrollerTexture(sf::Texture* texture, float boundsHeight)
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

    void ScrollAreaWidget::resetScrollerPosition()
    {
        setScrollerProp(0.f);
    }

    void ScrollAreaWidget::setScrollerProp(float prop)
    {
        _scrollerProp = prop;
        if(_aboutToRecalculate) return;
        _recalcScroller();
    }

    void ScrollAreaWidget::setScrollWidth(float width)
    {
        if(_isMinSizeSet) width = std::min(width, _minSize.x);
        _minScrollSize.x = width;
        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void ScrollAreaWidget::setScrollPropWidth(float propWidth)
    {
        _hasScrollerPropWidth = true;
        _scrollerPropWidth = propWidth;
        if(_interface) _recalcScrollerPropWidth();
        else _elementsPropSizeMustBeenCalculated = true;
    }

    void ScrollAreaWidget::_recalcScrollerPropWidth()
    {
        assert(_hasScrollerPropWidth);
        assert(_interface);

        float width = _interface->getSizeByPropSize({_scrollerPropWidth, 0.f}).x;
        setScrollWidth(width);
    }

    void ScrollAreaWidget::_recalcElementsPropSize()
    {
        if(_hasScrollerPropWidth) _recalcScrollerPropWidth();
    }
    void ScrollAreaWidget::makeScrollAutoHiding()
    {
        _isAutoHiding = true;
        _aboutToRecalculate = true;
    }

    void ScrollAreaWidget::_recalcScroller()
    {
        float maxOffset = -(_childrenHeight - _getSize().y);
        _scroller.setProportion(_scrollerProp);
        _offset = float(maxOffset * _scrollerProp);
        _isPosChanged = true;
        //_aboutToRecalculate = true;
    }

    void ScrollAreaWidget::resetScrollerPositionInTime(const gui::AnimData& data)
    {
        using ScrollAnim_t = gui::Anim<ScrollAreaWidget, gui::AnimType::Scroll, float>;
        auto setter = static_cast<void (ScrollAreaWidget::*)(float)>(&ScrollAreaWidget::setScrollerProp);
        auto scrollAnim = std::make_unique<ScrollAnim_t>(this, setter);

        scrollAnim->setAnim(_scroller.getProportion(), 0.f, data);
        _addAnimation(std::move(scrollAnim));
    }
    

    void ScrollAreaWidget::resetScrollerPositionInTime(float time)
    {
        resetScrollerPositionInTime(gui::AnimData{time});
    }

    sf::Vector2f ScrollAreaWidget::_getSize() const {
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

        _scroller.applyFamilyTransform(gui::FamilyTransform::getGlobalPosition(), gui::FamilyTransform::getDrawPosition());

        _scroller.setPosition(float(barX), 0);
        _displaySprite.setPosition(basePos + sf::Vector2f(getPadding()));
    }

    void ScrollAreaWidget::_recalcChildrenPos()
    {
        for(auto* child : _children)
        {
            child->applyFamilyTrans(gui::FamilyTransform::getGlobalPosition() + getPadding() + sf::Vector2f{0, _offset}, {0, _offset});
        }
    }

    void ScrollAreaWidget::_input(const sf::Event& event) 
    {
        float oldProp = _scroller.getProportion();

        if(_isHovered && event.type == sf::Event::MouseWheelScrolled) 
        {
           float propOffset = float(-event.mouseWheelScroll.delta) * 0.01f * _scrollSpeed;
           _scroller.moveProportion(propOffset);
        }
        
        _scroller.input(event);
        //_scroller.inputMouseMoved()
        float newProp = _scroller.getProportion();
        
        if(oldProp != newProp)
        {
            setScrollerProp(newProp);
        }
    }


    sf::Vector2f ScrollAreaWidget::_getChildrenSize()
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


        auto rTexHeight = (unsigned int)(size.y - (getPadding().y * 2.f));
        float rTexWidth = size.x - (getPadding().x * 2.f);

        _childrenHeight = float(std::max(Widget::_getChildrenSize().y, size.y));
        _childrenHeightProp = _childrenHeight/float(size.y);
        _scroller.setScrollerHeightProp(_childrenHeightProp);

        if(_childrenHeightProp <= 1.f && _isAutoHiding)
        {
            _scroller.invisible();
        }
        else
        {
            _scroller.visible();
            rTexWidth -= _minScrollSize.x;
            if(rTexWidth < 1.f) rTexWidth = 1.f;
        }


        sf::Vector2u rTexSize = {(unsigned int)(rTexWidth) , rTexHeight};
        if(rTexSize != _renderTexture.getSize())
        {
            auto* window = detail::globalPtr<Window>; 
            window->pushGLStates(); 
            _renderTexture.create(rTexSize.x, rTexSize.y); 
            window->popGLStates();
        }
        _recalcScroller();
    }

    void ScrollAreaWidget::_callback(CallbackType type) 
    {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

    sf::Vector2f ScrollAreaWidget::_getInnerSize() const
    {
        return {float(_renderTexture.getSize().x), 0.f};
    }

}