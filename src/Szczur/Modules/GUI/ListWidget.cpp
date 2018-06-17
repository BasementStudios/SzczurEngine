#include "ListWidget.hpp"

#include <algorithm>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void ListWidget::_addWidget(Widget* widget)
    {
        _shifts.emplace_back(sf::Vector2f{});
        _areShiftsCurrent = false;
    }
    void ListWidget::setBetweenPadding(float padding)
    {
        _betweenWidgetsPadding = padding;
        _areShiftsCurrent = false;
        _aboutToRecalculate = true;
    }
    void ListWidget::makeVertical()
    {
        _positioning = Positioning::Vertical;
        _aboutToRecalculate = true;
    }
    void ListWidget::makeHorizontal()
    {
        _positioning = Positioning::Horizontal;
        _aboutToRecalculate = true;
    }
    void ListWidget::_updateShifts()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");

        if(_positioning == Positioning::Vertical)
        {
            auto func = [this](const Widget* child, auto&){
                return sf::Vector2f{0.f, float(child->getSize().y) + child->getPosition().y - child->getPadding().y + _betweenWidgetsPadding};
            };

            std::transform(_children.begin(), _children.end(), _shifts.begin(), _shifts.begin(), func);
        }
        else
        {
            auto func = [this](const Widget* child, auto&){
                return sf::Vector2f{float(child->getSize().y) + child->getPosition().y - child->getPadding().y + _betweenWidgetsPadding, 0.f};
            };

            std::transform(_children.begin(), _children.end(), _shifts.begin(), _shifts.begin(), func);
        }

        _areShiftsCurrent = true;
    }

    void ListWidget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");        
        for(size_t i = 0; i < _shifts.size(); ++i)
        {
            target.draw(*_children[i], states);
            states.transform.translate(_shifts[i]);
        }
    }

    sf::Vector2u ListWidget::_getChildrenSize()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");        
        if(!_areShiftsCurrent || _aboutToRecalculate) _updateShifts();

        sf::Vector2f childrenShiftedSize;

        bool isVertical = _positioning == Positioning::Vertical;

        if(isVertical)
        {

        }

        for(auto& shift : _shifts)
        {
            childrenShiftedSize += shift;
        }
        
        if(isVertical)
        {
            childrenShiftedSize.y -= _betweenWidgetsPadding;
            childrenShiftedSize.x = Widget::_getChildrenSize().x;
        }
        else
        {
            childrenShiftedSize.x -= _betweenWidgetsPadding;
            childrenShiftedSize.y = Widget::_getChildrenSize().y;
        }
        return static_cast<sf::Vector2u>(childrenShiftedSize);
    }

    void ListWidget::_calculateSize()
    {
        if(!_areShiftsCurrent) _updateShifts();
    }

    sf::Vector2f ListWidget::_getChildShiftByIndex(size_t index) const
    {
        return _shifts[index];
    }

/*
    void ListWidget::_onMovedChildren(sf::Event event)
    {
        size_t i = 0;
        for(auto* child : _children)
        {
            auto childPos = child->getPosition();
            sf::Event tempEvent(event);
            tempEvent.mouseMove.x -= int(childPos.x * _winProp.x);
            tempEvent.mouseMove.y -= int(childPos.y * _winProp.y);

            child->_onMoved(tempEvent);

            event.mouseMove.y -= int(_shifts[i] * _winProp.y);
            i++;
        }
    }
    */
}
