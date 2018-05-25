#include "Widget.hpp"
//#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "Test.hpp"

#include <iostream>
#include <algorithm>

#include "TransformAnimBasics/ColorAnim.hpp"

#include "Szczur/Utility/Logger.hpp"

//#undef GUI_DEBUG

namespace rat {
    Widget::Widget() :
    _parent(nullptr),
    _isHovered(false),
    _isPressed(false),
    _isActivated(true),
    _isVisible(true),
    _aboutToRecalculate(false),
    _color(255, 255, 255),
    _size(0u,0u) {

    }

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
        for(auto it : _children)
            delete it;
        _children.clear();
    }

    void Widget::setParent(Widget* parent) {
        _parent = parent;
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

    Widget* Widget::add(Widget* object) {
        if(object) {
            _children.push_back(object);
            object->setParent(this);
            //_aboutToRecalculate = true;
            _aboutToRecalculate = true;
        }
        else {
            LOG_ERROR("Widget given to Widget::add is nullptr")
        }
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
        if(!_isActivated) return false;
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
        if(!_isActivated) return;        

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
        if(!_isActivated) return;

        auto thisSize = getSize();
        
        event.mouseMove.x += int((_origin.x - _padding.x) * _winProp.x);
        event.mouseMove.y += int((_origin.y - _padding.y) * _winProp.y);

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

        for(auto* child : _children)
        {
            auto childPos = child->getPosition();
            sf::Event tempEvent(event);
            tempEvent.mouseMove.x -= int(childPos.x * _winProp.x);
            tempEvent.mouseMove.y -= int(childPos.y * _winProp.y);

            child->_onMoved(tempEvent);
        }
    }

    void Widget::input(sf::Event event) {
        if(isActivated()) 
        {
            _input(event);
            _inputChildren(event);
        }
    }

	void Widget::_inputChildren(sf::Event event)
    {
        for(auto child : _children) 
        {
            if(event.type == sf::Event::MouseMoved)
            {
                auto childPosition = child->getPosition();
                sf::Event tempEvent(event);
                tempEvent.mouseMove.x -= int(childPosition.x * _winProp.x);
                tempEvent.mouseMove.y -= int(childPosition.y * _winProp.y);
                child->input(tempEvent);
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
        if(isVisible()) {
            states.transform *= getTransform();
            
            #ifdef GUI_DEBUG
            _drawDebug(target, states);
	        #endif
            _draw(target, states);
            _drawChildren(target, states);
        }
    }

    void Widget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(_padding);

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
            shape.setPosition(0.f, float(_size.y) - _padding.x);
            target.draw(shape, states);

            shape.setSize({_padding.x, float(_size.y) - 2.f *_padding.y});
            shape.setPosition(0.f, _padding.y);
            target.draw(shape, states);
            shape.move(float(_size.x) - _padding.x, 0.f);
            target.draw(shape, states);
        }
	#endif

	void Widget::invokeToCalculate()
    {
        for(auto* child : _children) child->invokeToCalculate();

        if(_aboutToRecalculate) calculateSize();
    }
    

    void Widget::calculateSize() 
    {
        _aboutToRecalculate = false;
        auto oldSize = _size;
        _size = {};

        if(_isMinSizeSet)
        {
            _size.x = std::max(_size.x, _minSize.x);
            _size.y = std::max(_size.y, _minSize.y);
        }

        auto chSize = _getChildrenSize();
        _size.x = std::max(_size.x, chSize.x + (unsigned int)(2.f * _padding.x));
        _size.y = std::max(_size.y, chSize.y + (unsigned int)(2.f * _padding.y));

        _calculateSize();
        auto ownSize = _getSize();

        if(ownSize.x > _size.x) _size.x = ownSize.x;
        if(ownSize.y > _size.y) _size.y = ownSize.y;
        
        _recalcOrigin();
        if(_parent && _size != oldSize) _parent->_aboutToRecalculate = true;
    }

	sf::Vector2u Widget::_getChildrenSize()
    {
        sf::Vector2u size;
        for(auto child : _children) {
            auto childSize = child->getSize();
            auto childPosition = static_cast<sf::Vector2i>(child->getPosition());
            auto childOrigin = child->getOrigin();
            if(childPosition.x + childSize.x - childOrigin.x > size.x)
                size.x = childPosition.x + childSize.x - childOrigin.x;
            if(childPosition.y + childSize.y - childOrigin.y > size.y)
                size.y = childPosition.y + childSize.y - childOrigin.y;
        }
        return size;
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

    void Widget::move(const sf::Vector2f& offset) {
        sf::Transformable::move(offset);
        if(_parent) _parent->_aboutToRecalculate = true;   
    }
    void Widget::move(float offsetX, float offsetY) {
        move({offsetX, offsetY});
    }
    void Widget::setPosition(const sf::Vector2f& offset) {
        sf::Transformable::setPosition(offset);
        if(_parent) _parent->_aboutToRecalculate = true;   
    }
    void Widget::setPosition(float x, float y) {
        setPosition({x, y});
    }

    void Widget::setOrigin(const sf::Vector2f& origin)
    {
        _isPropOriginSet = false;
        _origin = origin;
        _recalcOrigin();
    }
	void Widget::setOrigin(float x, float y)
    {
        setOrigin({x, y});
    }

	void Widget::setPropOrigin(const sf::Vector2f& prop)
    {
        _isPropOriginSet = true;
        _propOrigin = prop;
        _recalcOrigin();
    }
	void Widget::setPropOrigin(float x, float y)
    {
        setPropOrigin({x, y});
    }

    void Widget::_recalcOrigin()
    {
        if(_isPropOriginSet)
        {
            auto size = static_cast<sf::Vector2f>(getSize());
            _origin = {size.x * _propOrigin.x, size.y * _propOrigin.y};
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
    
    
}