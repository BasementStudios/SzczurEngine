#include "Widget.hpp"
//#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "Test.hpp"
#include <iostream>

#include "Szczur/Utility/Logger.hpp"

namespace rat {
    Widget::Widget() :
    _parent(nullptr),
    _isHovered(false),
    _isPressed(false),
    _isActivated(true),
    _isVisible(true),
    _aboutToRecalculate(false),
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
            calculateSize();
        }
        else {
            LOG_ERROR("Widget given to Widget::add is nullptr")
        }
        return object;
    }

    void Widget::input(sf::Event event) {
        if(isActivated()) {
            _input(event);
            switch(event.type) {
                case sf::Event::MouseMoved: {
                    auto thisSize = getSize();
                    
                    event.mouseMove.x += int(_origin.x * _winProp.x);
                    event.mouseMove.y += int(_origin.y * _winProp.y);
                    if(
                        event.mouseMove.x >= 0 &&
                        event.mouseMove.x <= thisSize.x * _winProp.x &&
                        event.mouseMove.y >= 0 &&
                        event.mouseMove.y <= thisSize.y * _winProp.y
                    ) {
                        if(!_isHovered) {
                            _callback(CallbackType::onHoverIn);
                            _isHovered = true;
                        }
                    }
                    else {
                        if(_isHovered) {
                            _callback(CallbackType::onHoverOut);
                            _isHovered = false;
                        }
                    }
                    break;
                }

                case sf::Event::MouseButtonPressed: {
                    if(_isHovered) {
                        _callback(CallbackType::onPress);
                        _isPressed = true;
                    }
                    break;
                }

                case sf::Event::MouseButtonReleased: {
                    if(_isPressed) {
                        _isPressed = false;
                        _callback(CallbackType::onRelease);         
                    }
                    break;
                }

                default: break;
            }
            for(auto it : _children) {
                if(event.type == sf::Event::MouseMoved) {
                    auto itPosition = it->getPosition();
                    sf::Event tempEvent(event);
                    tempEvent.mouseMove.x -= int(itPosition.x * _winProp.x);
                    tempEvent.mouseMove.y -= int(itPosition.y * _winProp.y);
                    it->input(tempEvent);
                }
                else
                    it->input(event);
            }
        }
    }

    void Widget::update(float deltaTime) {
        if(isActivated()) {
            _update(deltaTime);

            if(_isHovered) 
                _callback(CallbackType::onHover);

            if(_isPressed)
                _callback(CallbackType::onHold);

            for(auto& it : _children)
                it->update(deltaTime);
        }

        if(_aboutToRecalculate)
            calculateSize();
        
    }

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(isVisible()) {
            states.transform *= getTransform();

            //  Uncomment to get into debug mode :D
            sf::RectangleShape shape;
            shape.setSize(static_cast<sf::Vector2f>(getSize()));
            shape.setFillColor(sf::Color(0,0,255,70));
            shape.setFillColor(sf::Color::Transparent);
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(1.f);
            target.draw(shape, states);
            
            _draw(target, states);
            for(auto it : _children)
                target.draw(*it, states);
        }
    }

    void Widget::calculateSize() {
        _aboutToRecalculate = false;
        _calculateSize();
        _size = {0u,0u};
        auto ownSize = _getSize();

        if(ownSize.x > _size.x) _size.x = ownSize.x;
        if(ownSize.y > _size.y) _size.y = ownSize.y;

        if(_isMinSizeSet)
        {
            _size.x = std::max(_size.x, _minSize.x);
            _size.y = std::max(_size.y, _minSize.y);
        }

        for(auto child : _children) {
            auto childSize = child->getSize();
            auto childPosition = static_cast<sf::Vector2i>(child->getPosition());
            auto childOrigin = child->getOrigin();
            if(childPosition.x + childSize.x - childOrigin.x > _size.x)
                _size.x = childPosition.x + childSize.x - childOrigin.x;
            if(childPosition.y + childSize.y - childOrigin.y > _size.y)
                _size.y = childPosition.y + childSize.y - childOrigin.y;
        }
        _recalcOrigin();
    }

    sf::Vector2u Widget::_getSize() const {
        return {0u, 0u};
    }

    sf::Vector2u Widget::getSize() const {
        return _size;
    }

	sf::Vector2u Widget::getMinimalSize() const
    {
        if(!_isMinSizeSet) return {};
        return _minSize;
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
        _aboutToRecalculate = true;
    }
    void Widget::move(float offsetX, float offsetY) {
        sf::Transformable::move(offsetX, offsetY);
        _aboutToRecalculate = true;
    }
    void Widget::setPosition(const sf::Vector2f& offset) {
        sf::Transformable::setPosition(offset);
        _aboutToRecalculate = true;
    }
    void Widget::setPosition(float x, float y) {
        sf::Transformable::setPosition(x, y);
        _aboutToRecalculate = true;
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
        if(_parent) _parent->calculateSize();
    }
        

    void Widget::setSize(sf::Vector2u size)
    {
        _isMinSizeSet = true;
        _minSize = size;
        calculateSize();
    }
	void Widget::setSize(size_t width, size_t height)
    {
        setSize({(unsigned int)width, (unsigned int)height});
    }

    sf::Vector2f Widget::_winProp{1.f, 1.f};
	void Widget::setWinProp(sf::Vector2f prop)
    {
        _winProp = prop;
    }
    
    
}