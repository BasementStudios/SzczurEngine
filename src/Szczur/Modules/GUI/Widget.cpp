#include "Widget.hpp"
//#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "Test.hpp"

#include <iostream>
#include <algorithm>

#include "TransformAnimBasics/ColorAnim.hpp"
#include "InterfaceWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

//#undef GUI_DEBUG

namespace rat 
{
    Widget::Widget() :
    _parent(nullptr),
    _isHovered(false),
    _isPressed(false),
    _isActivated(true),
    _isVisible(true),
    _aboutToRecalculate(false),
    _color(255, 255, 255),
    _size(0u,0u) 
    {}

    Widget::~Widget() {
        for(auto it : _children)
            delete it;
    }

    void Widget::initScript(Script& script) {
        auto object = script.newClass<Widget>("Widget", "GUI");
        basicScript(object);
        object.init();
    }

    void Widget::clear() {
        for(auto it : _children) delete it;
        _children.clear();
        _clear();
    }

    Widget* Widget::operator[](size_t index)
    {
        if(_children.size() <= index)
        {
            LOG_ERROR("Widget::[] can't return child at index ", index);
            return nullptr;
        }
        return _children[index];
    }
	const Widget* Widget::operator[](size_t index) const
    {
        if(_children.size() <= index)
        {
            LOG_ERROR("Widget::[] can't return child at index ", index);
            return nullptr;
        }
        return _children[index];
    }

	size_t Widget::getChildrenAmount() const
    {
        return _children.size();
    }
    

    void Widget::setParent(Widget* parent) 
    {
        _parent = parent;
        if(parent->_interface) setInterface(parent->_interface);
        else setInterface(nullptr);
    }
    void Widget::setInterface(const InterfaceWidget* interface)
    {
        _interface = interface;
        if(_propSizeMustBeenCalculatedViaInterface && _interface)
        {
            _updatePropSize();
            _propSizeMustBeenCalculatedViaInterface = false;
        }

        for(auto* child : _children)
        {
            child->setInterface(interface);
        }
    }

    Widget* Widget::setCallback(CallbackType key, Function_t value) {
        _callbacks.insert_or_assign(key, value);
        return this;
    }

    Widget* Widget::setLuaCallback(CallbackType key, SolFunction_t value) {
        _luaCallbacks.insert_or_assign(key, value);
        return this;
    }

    void Widget::_callback(CallbackType type) {
        if(auto it = _luaCallbacks.find(type); it != _luaCallbacks.end())
            std::invoke(it->second, this);
        if(auto it = _callbacks.find(type); it != _callbacks.end())
            std::invoke(it->second, this);
    }

    Widget* Widget::add(Widget* object) 
    {
        if(object) 
        {
            _children.push_back(object);
            object->setParent(this);
            _addWidget(object);
            _aboutToRecalculate = true;
        }
        else LOG_ERROR("Widget given to Widget::add is nullptr");

        return object;
    }

    void Widget::invokeInput(const sf::Event& event)
    {
        switch(event.type)
        {
            case sf::Event::MouseMoved: _onMoved(event); break;
            case sf::Event::MouseButtonPressed: _onPressed(); break;
            case sf::Event::MouseButtonReleased: _onRealesed(); break;
        }
    }

    bool Widget::_onPressed()
    {
        if(!_isActivated || _isFullyDeactivated) return false;
        if(!_isHovered) return false;
        
        bool isAnyPressed = false;
        for(auto i = _children.rbegin(); i < _children.rend(); ++i)
        {
            auto* child = *i;
            isAnyPressed |= child->_onPressed();
            if(isAnyPressed) break;
        }
        if(isAnyPressed && !_areChildrenPenetrable) return true;
        _isPressed = true;
        _callback(CallbackType::onPress);
        return true;
    }
    void Widget::_onRealesed()
    {
        if(!_isActivated || _isFullyDeactivated) return;        

        for(auto* child : _children)
        {
            child->_onRealesed();
        }
        
        if(!_isPressed) return;
        _isPressed = false;
        _callback(CallbackType::onRelease);    
    }
	void Widget::_onMoved(sf::Event event)
    {
        if(!_isActivated || _isFullyDeactivated) return;

        auto thisSize = getSize();
        
        event.mouseMove.x += int(_origin.x - _padding.x);
        event.mouseMove.y += int(_origin.y - _padding.y);

        bool isMouseOverlap = event.mouseMove.x >= 0 &&
            event.mouseMove.x <= thisSize.x * _winProp.x &&
            event.mouseMove.y >= 0 &&
            event.mouseMove.y <= thisSize.y * _winProp.y;

        if(isMouseOverlap)
        {
            if(!_isHovered) 
            {
                _callback(CallbackType::onHoverIn);
                _isHovered = true;
            }
        }
        else 
        {
            if(_isHovered) 
            {
                _callback(CallbackType::onHoverOut);
                _isHovered = false;
            }
        }

        _onMovedChildren(event);
    }

    void Widget::_onMovedChildren(sf::Event event)
    {
        {
            auto childrenShift = _getChildrenShift();
            event.mouseMove.x -= childrenShift.x;
            event.mouseMove.y -= childrenShift.y;
        }

        size_t i = 0;
        for(auto childIt = _children.begin(), childEnd = _children.end(); childIt < childEnd; ++childIt, ++i)
        {
            auto* child = *childIt;

            auto childPos = child->getPosition();
            sf::Event tempEvent(event);
            tempEvent.mouseMove.x -= int(childPos.x * _winProp.x);
            tempEvent.mouseMove.y -= int(childPos.y * _winProp.y);

            child->_onMoved(tempEvent);
 
            auto childShift = _getChildShiftByIndex(i);
            event.mouseMove.x -= int(childShift.x * _winProp.x);
            event.mouseMove.y -= int(childShift.y * _winProp.y);
        }
        /*
        for(auto* child : _children)
        {
            auto childPos = child->getPosition();
            sf::Event tempEvent(event);
            tempEvent.mouseMove.x -= int(childPos.x * _winProp.x);
            tempEvent.mouseMove.y -= int(childPos.y * _winProp.y);

            child->_onMoved(tempEvent);
        }*/
    }

    void Widget::input(sf::Event event) {
        if(isActivated()  && !_isFullyDeactivated) 
        {
            _input(event);
            if(event.type == sf::Event::MouseMoved)
            {
                event.mouseMove.x -= int(_padding.x * _winProp.x);
                event.mouseMove.y -= int(_padding.y * _winProp.y);
            }
            _inputChildren(event);
        }
    }

	void Widget::_inputChildren(sf::Event event)
    {
        {
            auto childrenShift = _getChildrenShift();
            event.mouseMove.x -= childrenShift.x;
            event.mouseMove.y -= childrenShift.y;
        }
        size_t i = 0;
        for(auto child : _children) 
        {
            if(event.type == sf::Event::MouseMoved)
            {
                auto childPosition = child->getPosition();
                sf::Event tempEvent(event);
                tempEvent.mouseMove.x -= int(childPosition.x * _winProp.x);
                tempEvent.mouseMove.y -= int(childPosition.y * _winProp.y);
                child->input(tempEvent);

                auto childShift = _getChildShiftByIndex(i++);
                event.mouseMove.x -= int(childShift.x * _winProp.x);
                event.mouseMove.y -= int(childShift.y * _winProp.y);
            }
            else child->input(event);
        }
    }
    

    void Widget::update(float deltaTime) {
        if(isActivated()) {
            _update(deltaTime);
            _updateAnimations(deltaTime);

            if(_isHovered) 
                _callback(CallbackType::onHover);

            if(_isPressed)
                _callback(CallbackType::onHold);

            for(auto& it : _children)
                it->update(deltaTime);
        }
    }

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(isVisible() && !isFullyDeactivated()) {
            states.transform *= getTransform();
            
            #ifdef GUI_DEBUG
            _drawDebug(target, states);
	        #endif
            _draw(target, states);
            states.transform.translate(_padding);
            _drawChildren(target, states);
        }
    }

    void Widget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto it : _children) target.draw(*it, states);
    }

    #ifdef GUI_DEBUG
		void Widget::_drawDebug(sf::RenderTarget& target, sf::RenderStates states) const
        {
            sf::RectangleShape shape;
            shape.setOutlineColor({255, 255, 255, 125});
            shape.setOutlineThickness(1.f);
            shape.setFillColor(sf::Color::Transparent);
            shape.setSize(static_cast<sf::Vector2f>(getSize()));
            target.draw(shape, states);            

            shape.setOutlineColor(sf::Color::Transparent);
            /*
            shape.setSize(static_cast<sf::Vector2f>(getSize()) - _padding * 2.f);
            shape.setFillColor(sf::Color(0, 0, 255, 10));
            //shape.setFillColor(sf::Color::Transparent);
            shape.setPosition(getPadding());
            target.draw(shape, states);
            */

            shape.setFillColor({255, 0, 0, 100});
            shape.setSize({float(_size.x), _padding.y});
            shape.setPosition(0.f, 0.f);            
            target.draw(shape, states);            
            shape.setPosition(0.f, float(_size.y) - _padding.y);
            target.draw(shape, states);

            shape.setSize({_padding.x, float(_size.y) - 2.f *_padding.y});
            shape.setPosition(0.f, _padding.y);
            target.draw(shape, states);
            shape.move(float(_size.x) - _padding.x, 0.f);
            target.draw(shape, states);
        }
	#endif

    void Widget::makeChildrenUnresizable()
    {
        _areChildrenResizing = false;
        _aboutToRecalculate = true;
    }

	void Widget::invokeToCalculate()
    {
        for(auto* child : _children) child->invokeToCalculate();

        if(_aboutToRecalculate) calculateSize();
    }
    

    void Widget::calculateSize() 
    {
        auto oldSize = _size;
        _size = {};

        if(_isMinSizeSet)
        {
            _size.x = std::max(_size.x, _minSize.x);
            _size.y = std::max(_size.y, _minSize.y);
        }

        auto chSize = static_cast<sf::Vector2u>(_padding * 2.f);
        if(_areChildrenResizing)
        {
            chSize += _getChildrenSize();
        }
        _size.x = std::max(_size.x, chSize.x + (unsigned int)(2.f * _padding.x));
        _size.y = std::max(_size.y, chSize.y + (unsigned int)(2.f * _padding.y));

        _calculateSize();
        auto ownSize = _getSize();

        if(ownSize.x > _size.x) _size.x = ownSize.x;
        if(ownSize.y > _size.y) _size.y = ownSize.y;
        
        _recalcOrigin();

        if(_size != oldSize)
        {
            if(_parent) _parent->_aboutToRecalculate = true;
            _childrenPropSizesMustBeenRecalculated = true;
        }

        _aboutToRecalculate = false;
    }

	sf::Vector2u Widget::_getChildrenSize()
    {
        sf::Vector2u size;
        for(auto child : _children) 
        {
            auto childBound = child->_getBound();

            size.x = std::max(size.x, childBound.x);
            size.y = std::max(size.y, childBound.y);
        }
        return size;
    }

    sf::Vector2u Widget::_getBound() const
    {
        if(_isFullyDeactivated) return {};

        auto size = static_cast<sf::Vector2f>(getSize());
        auto position = static_cast<sf::Vector2f>(getPosition());
        auto origin = getOrigin();

        if(_props.hasPosition)
        {
            position = {0.f, 0.f};
            origin = {0.f, 0.f};
        }

        auto width = (unsigned int)(position.x + size.x - origin.x);
        auto height = (unsigned int)(position.y + size.y - origin.y);

        return {width, height};
    }
    

    sf::Vector2u Widget::_getSize() const {
        return {0u, 0u};
    }

    sf::Vector2u Widget::getSize() const {
        return _size;
    }

    void Widget::setPadding(const sf::Vector2f& padding)
    {
        _padding = padding;
        _aboutToRecalculate = true;
    }
	void Widget::setPadding(float width, float height)
    {
        setPadding({width, height});
    }
    sf::Vector2f Widget::getPadding() const
    {
        return _padding;
    }

    sf::Vector2i Widget::getInnerSize() const
    {
        auto size = getSize();
        auto minSize = getMinimalSize();
        size.x = std::max(size.x, minSize.x);
        size.y = std::max(size.y, minSize.y);
        return static_cast<sf::Vector2i>(size) - (static_cast<sf::Vector2i>(getPadding()) * 2);
    }

	sf::Vector2u Widget::getMinimalSize() const
    {
        if(!_isMinSizeSet) return {};
        return _minSize;
    }

	void Widget::setColor(const sf::Color& color)
    {
        _color = color;
        _setColor(color);
        for(auto* child : _children)
        {
            child->setColor(color);
        }
    }
	void Widget::setColor(const sf::Color& color, float inTime)
    {
        auto animCol = std::make_unique<ColorAnim>();
        animCol->setAnim(getColor(), color, inTime);
        _addAnimation(std::move(animCol));
    }
    
    
    void Widget::resetColor()
    {
        setColor({255, 255, 255});
    }
    sf::Color Widget::getColor() const
    {
        return _color;
    }


    void Widget::activate() {
        _isActivated = true;
    }

    void Widget::deactivate() {
        _isActivated = false;
    }

    bool Widget::isActivated() const {
        return _isActivated;
    }

    void Widget::visible() {
        _isVisible = true;
    }

    void Widget::invisible() {
        _isVisible = false;
    }

    bool Widget::isVisible() const {
        return _isVisible;
    }

    void Widget::fullyDeactivate()
    {
        if(_isFullyDeactivated) return;
        _isFullyDeactivated = true;
        if(_parent) _parent->_aboutToRecalculate = true;
    }
    void Widget::fullyActivate()
    {
        if(!_isFullyDeactivated) return;
        _isFullyDeactivated = false;
        if(_parent) _parent->_aboutToRecalculate = true;
    }
    bool Widget::isFullyDeactivated() const
    {
        return _isFullyDeactivated;
    }

    void Widget::move(const sf::Vector2f& offset) 
    {
        sf::Transformable::move(offset);
        if(_parent) _parent->_aboutToRecalculate = true;   
    }
    void Widget::move(float offsetX, float offsetY) 
    {
        move({offsetX, offsetY});
    }
    void Widget::setPosition(const sf::Vector2f& offset) 
    {
        sf::Transformable::setPosition(int(offset.x), int(offset.y));
        //sf::Transformable::setPosition(round(offset.x), round(offset.y));
        if(_parent) _parent->_aboutToRecalculate = true;   
    }
    void Widget::setPosition(float x, float y) 
    {
        setPosition({x, y});
    }

    void Widget::setPropPosition(const sf::Vector2f& propPos)
    {
        _props.hasPosition = true;
        _props.position = propPos;
        _props.position.x = std::max(0.f, std::min(1.f, _props.position.x));
        _props.position.y = std::max(0.f, std::min(1.f, _props.position.y));

        _propPosMustBeenRecalculated = true;
    }
	void Widget::setPropPosition(float propX, float propY)
    {
        setPropPosition({propX, propY});
    }

    void  Widget::setPropSize(const sf::Vector2f& propSize)
    {
        _props.hasSize = true;
        _props.size = propSize;
        _props.size.x = std::max(0.f, std::min(1.f, _props.size.x));
        _props.size.y = std::max(0.f, std::min(1.f, _props.size.y));

        if(_interface)
        {
            _updatePropSize();
        }
        else
        {
            _propSizeMustBeenCalculatedViaInterface = true;
        }
    }
    void Widget::setPropSize(float widthProp, float heightProp)
    {
        setPropSize({widthProp, heightProp});
    }

    void Widget::setOrigin(const sf::Vector2f& origin)
    {
        _props.hasOrigin = false;
        _origin = origin;
        _recalcOrigin();
    }
	void Widget::setOrigin(float x, float y)
    {
        setOrigin({x, y});
    }

	void Widget::setPropOrigin(const sf::Vector2f& prop)
    {
        _props.hasOrigin = true;
        _props.origin = prop;
        _recalcOrigin();
    }
	void Widget::setPropOrigin(float x, float y)
    {
        setPropOrigin({x, y});
    }

    void Widget::_recalcOrigin()
    {
        if(_props.hasOrigin)
        {
            auto size = static_cast<sf::Vector2f>(getSize());
            _origin = {size.x * _props.origin.x, size.y * _props.origin.y};
        }
        sf::Transformable::setOrigin(_origin);
        if(_parent) _parent->_aboutToRecalculate = true;
    }
        

    void Widget::setSize(sf::Vector2u size)
    {
        _isMinSizeSet = true;
        _minSize = size;
        _aboutToRecalculate = true;
    }
	void Widget::setSize(size_t width, size_t height)
    {
        setSize({(unsigned int)width, (unsigned int)height});
    }
	void Widget::lockSize()
    {
        setSize(getSize());
    }

	void Widget::_addAnimation(Animation_t animation)
    {
        auto type = animation->getType();
        if(_currentAnimations & type)
        {
            auto found = std::find_if(_animations.begin(), _animations.end(), [&type](const Animation_t& anim){
                return anim->getType() == type;
            });
            *found = std::move(animation);
        }
        else
        {
            _animations.emplace_back(std::move(animation));
            _currentAnimations |= type;
        }
    }
	void Widget::_updateAnimations(float dt)
    {
        for(auto& anim : _animations)
        {
            anim->update(dt);
        }
        for(auto& anim : _animations)
        {
            auto type = anim->getType();
            auto* animPtr = anim.get();
            
            switch (type)
            {
                case TransformAnimationBase::Types::Color:
                {
                    auto* animCol = static_cast<ColorAnim*>(animPtr);
                    setColor(animCol->getActualColor());
                } break;
            
                default:
                    break;
            }
        }
        for(auto& anim : _animations)
        {
            if(anim->isAnimationOver())
            {
                auto type = anim->getType();
                _currentAnimations &= (~type);
            }
        }        
        auto newEnd = std::remove_if(_animations.begin(), _animations.end(), [](Animation_t& anim){

            return anim->isAnimationOver();
        });
        _animations.erase(newEnd, _animations.end());
    }
    
	void Widget::makeChildrenPenetrable()
    {
        _areChildrenPenetrable = true;
    }
    

    sf::Vector2f Widget::_winProp{1.f, 1.f};

	void Widget::setWinProp(sf::Vector2f prop)
    {
        _winProp = prop;
    }

    void Widget::invokeToUpdatePropPosition()
    {
        if(_childrenPropSizesMustBeenRecalculated)
        {
            for(auto* child : _children) child->_updatePropPosition();
            _childrenPropSizesMustBeenRecalculated = false;
        }
        
        for(auto* child : _children) child->invokeToUpdatePropPosition();

        if(_propPosMustBeenRecalculated) _updatePropPosition();
    }

    void Widget::forceToUpdatePropSize()
    {
        _updatePropSize();

        for(auto* child : _children) child->forceToUpdatePropSize();
    }

    void Widget::_updatePropSize()
    {
        if(!(_props.hasSize && _interface)) return;

        auto updatedSize = static_cast<sf::Vector2u>(_interface->getSizeByPropSize(_props.size));

        setSize(updatedSize);
    }
	void Widget::_updatePropPosition()
    {
        if(!_props.hasPosition) return;
        if(!_parent) return;

        auto size = getSize();
        auto origin = getOrigin();
        auto parentSize = _parent->getSize() - static_cast<sf::Vector2u>(_parent->getPadding() * 2.f);

        auto posRange = parentSize - size;

        const float x = float(posRange.x) * _props.position.x + origin.x;
        const float y = float(posRange.y) * _props.position.y + origin.y;

        sf::Transformable::setPosition(x, y);

        _propPosMustBeenRecalculated = false;
    }
    
    
}