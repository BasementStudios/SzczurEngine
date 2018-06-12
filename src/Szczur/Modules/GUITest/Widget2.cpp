#include "Widget2.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    /*
   
    void Widget2::handleEvents(const sf::Event& event);
	void Widget2::update(float deltaTime);

   */


    void Widget2::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        sf::RectangleShape shape;
        shape.setOrigin(_origin);
        shape.setSize(static_cast<sf::Vector2f>(getSize()));
        shape.setPosition(getPosition());

        shape.setFillColor(sf::Color(0,0,255,70));
        //shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(1.f);
        target.draw(shape, states);

        _drawComponents(target, states);
        _drawChildren(target, states);
    }

    Widget2* Widget2::add(Widget2* child)
    {
        _children.emplace_back(child);
        child->setParent(this);
        _recalcSize();
		return nullptr;
    }
    void Widget2::setParent(Widget2* parent)
    {
        _parent = parent;
        _recalcPos();
    }

    void Widget2::setPosition(float x, float y)
    {
        _position = {x, y};
        _recalcPos();
        if(_parent) _parent->_recalcSize();
    }


    void Widget2::setMinimalSize(int x, int y)
    {
        _isMinSizeSet = true;
        _minSize = {x, y};
        _recalcSize();
    }
    void Widget2::setOrigin(float x, float y)
    {
        _isOriginPropSet = false;
        _origin = {x, y};
        _recalcOrigin();
    }
    void Widget2::setOriginProp(float x, float y)
    {
        _isOriginPropSet = true;
        _originProp = {x, y};
        _recalcOrigin();
    }
    
    sf::Vector2i Widget2::getSize() const
    {
        return {int(_bounds.width), int(_bounds.height)};
    }
    sf::Vector2i Widget2::getMinimalSize() const
    {
        if(_isMinSizeSet) return _minSize;
        return {};
    }
    
    sf::FloatRect Widget2::getGlobalBounds() const
    {
        return _bounds;
    }
    sf::Vector2f Widget2::getPosition() const
    {
        auto pos = _position;
        if(_parent)
        {
            pos += sf::Vector2f{_parent->_bounds.left, _parent->_bounds.top};
        }
        return pos;
    }
    bool Widget2::hasMinimalSize() const
    {
        return _isMinSizeSet;
    }
    
    

    void Widget2::_recalcSize()
    {
        sf::Vector2f size = {0.f, 0.f};
        if(_isMinSizeSet)
        {
            size.x = _minSize.x;
            size.y = _minSize.y;
        }
        sf::Vector2f chSize = _getChildrenSize();
        size.x = std::max(size.x, chSize.x);
        size.y = std::max(size.y, chSize.y);

        sf::Vector2f comSize = _getComponentsSize();
        size.x = std::max(size.x, comSize.x);
        size.y = std::max(size.y, comSize.y);

        _bounds.width = size.x;
        _bounds.height = size.y;

        _recalculateComponentsSize();
        _recalcOrigin();
        if(_parent) _parent->_recalcSize();
    }

    sf::Vector2f Widget2::_getChildrenSize() const
    {
        if(_children.size() == 0) return {0.f, 0.f};
        sf::Vector2f rightCorner = {_bounds.left + _bounds.width, _bounds.top + _bounds.height};

        auto firstsBounds = _children.front()->_bounds;
        sf::Vector2f newRightCorner {firstsBounds.left + firstsBounds.width, firstsBounds.top + firstsBounds.height};

        for(auto* child : _children)
        {
            auto chBounds = child->_bounds;

            float chR = chBounds.left + chBounds.width;
            float chB = chBounds.top + chBounds.height;

            if(chR > newRightCorner.x) newRightCorner.x = chR;
            if(chB > newRightCorner.y) newRightCorner.y = chB;
        }
        return {newRightCorner.x - _bounds.left, newRightCorner.y - _bounds.top};
    }

    void Widget2::_recalcOrigin()
    {
        if(_isOriginPropSet)
        {
            _origin.x = _bounds.width * _originProp.x;
            _origin.y = _bounds.height * _originProp.y;
            _recalcPos();
        }
    }
    void Widget2::_recalcPos()
    {
        _bounds.left = _position.x - _origin.x;
        _bounds.top = _position.y - _origin.y;
        if(_parent != nullptr)
        {
            _bounds.left += _parent->_bounds.left;
            _bounds.top += _parent->_bounds.top;
        }
        _recalcChildrenPos();
        _recalculateComponentsPosition();
    }

    void Widget2::_recalcChildrenPos()
    {
        for(auto* child : _children)
        {
            child->_recalcPos();
        }
    }

    void Widget2::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for(auto* child : _children)
        {
            child->draw(target, states);
        }
    }

    
    Widget2::~Widget2()
    {
        for(auto* child : _children)
        {
            delete child;
        }
    }


}