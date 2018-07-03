#include "Widget.hpp"
//#include "ImageWidget.hpp"
#include "TextWidget.hpp"
#include "TextAreaWidget.hpp"
#include "Test.hpp"

#include <iostream>
#include <algorithm>

#include "Animation/Anim.hpp"

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
    _color(255, 255, 255)
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
            case sf::Event::MouseMoved: _onMoved(sf::Vector2f{float(event.mouseMove.x), float(event.mouseMove.y)}); break;
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
	void Widget::_onMoved(const sf::Vector2f& mousePos)
    {
        if(!_isActivated || _isFullyDeactivated) return;

        if(gui::FamilyTransform::isPointIn(mousePos))
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

        for(auto* child : _children) child->_onMoved(mousePos);
    }

    void Widget::input(const sf::Event& event) {
        if(isActivated()  && !_isFullyDeactivated) 
        {
            _input(event);
            for(auto child : _children) child->input(event);
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

            for(auto& child : _children) child->update(deltaTime);
        }
    }

    void Widget::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if(isVisible() && !isFullyDeactivated()) {

            #ifdef GUI_DEBUG
            _drawDebug(target, states);
	        #endif
            _draw(target, states);
            _drawChildren(target, states);
        }
    }

    void Widget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto child : _children) target.draw(*child, states);
    }

    #ifdef GUI_DEBUG
		void Widget::_drawDebug(sf::RenderTarget& target, sf::RenderStates states) const
        {
            auto pos = static_cast<sf::Vector2f>(gui::FamilyTransform::getDrawPosition());

            sf::RectangleShape shape;
            shape.setPosition(pos);
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
            shape.setSize({float(_size.x), float(_padding.y)});
            shape.setPosition(pos);            
            target.draw(shape, states);            
            shape.setPosition(pos.x, pos.y + float(_size.y) - _padding.y);
            target.draw(shape, states);

            shape.setSize({float(_padding.x), float(_size.y) - 2.f *_padding.y});
            shape.setPosition(pos.x, pos.y + _padding.y);
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

        auto chSize = static_cast<sf::Vector2f>(_padding * 2.f);
        if(_areChildrenResizing)
        {
            chSize += _getChildrenSize();
        }
        _size.x = std::max(_size.x, chSize.x + 2 * _padding.x);
        _size.y = std::max(_size.y, chSize.y + 2 * _padding.y);

        _calculateSize();
        auto ownSize = _getSize();

        if(ownSize.x > _size.x) _size.x = ownSize.x;
        if(ownSize.y > _size.y) _size.y = ownSize.y;
        
        _recalcOrigin();

        if(_size != oldSize)
        {
            if(_parent)
            {
                _parent->_aboutToRecalculate = true;
                //_parent->_isPosChanged = true;
            }
            _childrenPropSizesMustBeenRecalculated = true;
        }

        gui::FamilyTransform::setSize(_size);

        _aboutToRecalculate = false;
    }

	sf::Vector2f Widget::_getChildrenSize()
    {
        sf::Vector2f size;
        for(auto child : _children) 
        {
            auto childBound = child->_getBound();

            size.x = std::max(size.x, childBound.x);
            size.y = std::max(size.y, childBound.y);
        }
        return size;
    }

    sf::Vector2f Widget::_getBound() const
    {
        if(_isFullyDeactivated) return {};

        auto size = getSize();
        auto position = getPosition();
        auto origin = getOrigin();

        if(_props.hasPosition)
        {
            position = {0, 0};
            origin = {0, 0};
        }

        auto width = (position.x + size.x - origin.x);
        auto height = (position.y + size.y - origin.y);

        return {width, height};
    }
    

    sf::Vector2f Widget::_getSize() const {
        return {0u, 0u};
    }

    sf::Vector2f Widget::getSize() const {
        return _size;
    }

    void Widget::setPadding(const sf::Vector2f& padding)
    {
        _padding = padding;
        if(_parent) _parent->_aboutToRecalculate = true;
        _isPosChanged = true;
        _propPosMustBeenRecalculated = true;

    }
	void Widget::setPadding(float width, float height)
    {
        setPadding({width, height});
    }
    sf::Vector2f Widget::getPadding() const
    {
        return _padding;
    }

    sf::Vector2f Widget::getInnerSize() const
    {
        auto size = getSize();
        auto minSize = getMinimalSize();
        size.x = std::max(size.x, minSize.x);
        size.y = std::max(size.y, minSize.y);
        return size - (getPadding() * 2.f);
    }

	sf::Vector2f Widget::getMinimalSize() const
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
    void Widget::setColorInTime(const sf::Color& color, const gui::AnimData& data)
    {
		using ColorAnim_t = gui::Anim<Widget, gui::AnimType::Color, sf::Color>;        
        auto animCol = std::make_unique<ColorAnim_t>(this, &Widget::setColor);
        animCol->setAnim(getColor(), color, data);
        _addAnimation(std::move(animCol));
    }
    
	void Widget::setColorInTime(const sf::Color& color, float inTime)
    {
        setColorInTime(color, gui::AnimData{inTime});
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
        if(_parent) _parent->_isPosChanged = true;
    }
    void Widget::fullyActivate()
    {
        if(!_isFullyDeactivated) return;
        _isFullyDeactivated = false;
        if(_parent) _parent->_aboutToRecalculate = true;
        if(_parent) _parent->_isPosChanged = true;
    }
    bool Widget::isFullyDeactivated() const
    {
        return _isFullyDeactivated;
    }

    void Widget::move(const sf::Vector2f& offset) 
    {
        setPosition(getPosition() + offset);  
    }
    void Widget::move(float offsetX, float offsetY) 
    {
        move({offsetX, offsetY});
    }
    void Widget::setPosition(const sf::Vector2f& offset) 
    {
        _props.hasPosition = false;
        gui::FamilyTransform::setPosition(offset);
        if(_parent) _parent->_aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void Widget::setPosition(float x, float y) 
    {
        setPosition({x, y});
    }


    void Widget::setPositionInTime(const sf::Vector2f& offset, const gui::AnimData& data)
    {
        using PosAnim_t = gui::Anim<Widget, gui::AnimType::Pos, sf::Vector2f>;
        auto setter = static_cast<void (Widget::*)(const sf::Vector2f&)>(&Widget::setPosition);

        auto posAnim = std::make_unique<PosAnim_t>(this, setter);
        posAnim->setAnim(getPosition(), offset, data);
        _addAnimation(std::move(posAnim));
    }
    

	void Widget::setPositionInTime(const sf::Vector2f& offset, float inTime)
    {
        setPositionInTime(offset, gui::AnimData(inTime));
    }
    
    sf::Vector2f Widget::getPosByGlobalPos(const sf::Vector2f& globalPos) const
    {
        return gui::FamilyTransform::getPosByGlobal(globalPos);
    }
    void Widget::setGlobalPosition(const sf::Vector2f& globalPos)
    {
        gui::FamilyTransform::setGlobalPos(globalPos);
        _isPosChanged = true;
        if(_parent) _parent->_aboutToRecalculate = true;
    }
    void Widget::setGlobalPosition(float globalX, float globalY)
    {
        setGlobalPosition({globalX, globalY});
    }

    const sf::Vector2f& Widget::getPosition() const
    {
        return gui::FamilyTransform::getPosition();
    }
    const sf::Vector2f& Widget::getGlobalPosition() const
    {
        return gui::FamilyTransform::getGlobalPosition();
    }

    void Widget::setPropPosition(const sf::Vector2f& propPos)
    {
        _props.hasPosition = true;
        _props.position = propPos;

        _propPosMustBeenRecalculated = true;
        _isPosChanged = true;
    }
	void Widget::setPropPosition(float propX, float propY)
    {
        setPropPosition({propX, propY});
    }

	void Widget::setPropPosition(const sf::Vector2f& propPos, const gui::AnimData& data)
    {
        using PosAnim_t = gui::Anim<Widget, gui::AnimType::Pos, sf::Vector2f>;
        
        auto setter = static_cast<void (Widget::*)(const sf::Vector2f&)>(&Widget::setPropPosition);

        auto posAnim = std::make_unique<PosAnim_t>(this, setter);
        posAnim->setAnim(_props.position, propPos, data);
        _addAnimation(std::move(posAnim));
    }
    
	void Widget::setPropPosition(const sf::Vector2f& propPos, float inTime)
    {
        setPropPosition(propPos, gui::AnimData(inTime));
    }
    

    void  Widget::setPropSize(const sf::Vector2f& propSize)
    {
        _props.hasSize = true;
        _props.size = propSize;
        _props.size.x = std::max(0.f, _props.size.x);
        _props.size.y = std::max(0.f, _props.size.y);

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
        gui::FamilyTransform::setOrigin(origin);
        _recalcOrigin();
        _isPosChanged = true;
        if(_parent) _parent->_aboutToRecalculate = true;
    }
	void Widget::setOrigin(float x, float y)
    {
        setOrigin({x, y});
    }

    sf::Vector2f Widget::getOrigin() const
    {
        return gui::FamilyTransform::getOrigin();
    }

	void Widget::setPropOrigin(const sf::Vector2f& prop)
    {
        _props.hasOrigin = true;
        _props.origin = prop;
        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
	void Widget::setPropOrigin(float x, float y)
    {
        setPropOrigin({x, y});
    }

    void Widget::_recalcOrigin()
    {
        if(_props.hasOrigin)
        {
            auto size = getSize();
            gui::FamilyTransform::setOrigin(float(float(size.x) * _props.origin.x), float(float(size.y) * _props.origin.y));
        }
    }
        

    void Widget::setSize(sf::Vector2f size)
    {
        _isMinSizeSet = true;
        _minSize = size;
        _aboutToRecalculate = true;
    }
	void Widget::setSize(float width, float height)
    {
        setSize({width, height});
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

    void Widget::_abortAnimation(gui::AnimType type)
    {
        if(!(type & _currentAnimations)) return;
        _currentAnimations &= (~type);
        auto newEnd = std::remove_if(_animations.begin(), _animations.end(), [type](Animation_t& anim){
            return anim->getType() == type;
        });
        _animations.erase(newEnd, _animations.end());
    }

	void Widget::_updateAnimations(float dt)
    {
        for(auto& anim : _animations)
        {
            anim->update(dt);
        }

        for(auto& anim : _animations)
        {
            if(!anim->isAlive())
            {
                auto type = anim->getType();
                _currentAnimations &= (~type);
            }
        }        
        auto newEnd = std::remove_if(_animations.begin(), _animations.end(), [](Animation_t& anim){

            return !anim->isAlive();
        });
        _animations.erase(newEnd, _animations.end());
    }
    
	void Widget::makeChildrenPenetrable()
    {
        _areChildrenPenetrable = true;
    }

    void Widget::invokeToCalcPropPosition()
    {
        if(_childrenPropSizesMustBeenRecalculated)
        {
            for(auto* child : _children) child->_updatePropPosition();
            _childrenPropSizesMustBeenRecalculated = false;
        }
        
        for(auto* child : _children) child->invokeToCalcPropPosition();

        if(_propPosMustBeenRecalculated) _updatePropPosition();
    }

	void Widget::_updatePropPosition()
    {
        if(!_props.hasPosition) return;
        if(!_parent) return;

        auto size = getSize();
        auto origin = getOrigin();
        auto parentSize = _parent ->_getInnerSize();

        auto posRange = parentSize - size;

        const float x = posRange.x * _props.position.x + origin.x;
        const float y = posRange.y * _props.position.y + origin.y;

        gui::FamilyTransform::setPosition(x, y);

        _propPosMustBeenRecalculated = false;
        _isPosChanged = true;
    }

    sf::Vector2f Widget::_getInnerSize() const
    {
        return gui::FamilyTransform::getSize() - (_padding * 2.f);
    }

    void Widget::forceToUpdatePropSize()
    {
        _updatePropSize();

        for(auto* child : _children) child->forceToUpdatePropSize();
    }

    void Widget::_updatePropSize()
    {
        if(_interface)
        {
            _recalcElementsPropSize();

            if(_props.hasSize)
            {
                auto updatedSize = _interface->getSizeByPropSize(_props.size);
                setSize(updatedSize);
            }
        }
    }

    void Widget::invokeToCalcPosition()
    {
        if(_isPosChanged)
        {
            _recalcPos();
            _recalcChildrenPos();
            _isPosChanged = false;
        }
        for(auto* child : _children) child->invokeToCalcPosition();
    }

    void Widget::_recalcChildrenPos()
    {
        for(auto* child : _children)
        {
            child->applyFamilyTrans(getGlobalPosition() + _padding, getDrawPosition() + _padding);
        }
    }

    void Widget::applyFamilyTrans(const sf::Vector2f& globalPos, const sf::Vector2f& drawPos)
    {
        gui::FamilyTransform::applyParentPosition(globalPos, drawPos);
        //std::cout << "x: " << globalPos.x << " y: " << globalPos.y << " x: " << drawPos.x << " y: " << drawPos.y << '\n';
        _isPosChanged = true;
    }
    
    
}