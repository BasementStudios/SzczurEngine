#include "ListWidget.hpp"

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void ListWidget::_addWidget(Widget* widget)
    {
        _shifts.push_back(0.f);
        _areShiftsCurrent = false;
    }
    void ListWidget::setBetweenPadding(float padding)
    {
        _betweenWidgetsPadding = padding;
        _areShiftsCurrent = false;
        _aboutToRecalculate = true;
    }
    void ListWidget::_updateShifts()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");
        for(size_t i = 0; i < _shifts.size(); ++i)
        {
            auto* child = _children[i];
            
            _shifts[i] = float(child->getSize().y) + child->getPosition().y + _betweenWidgetsPadding;
        }

        _areShiftsCurrent = true;
    }

    void ListWidget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");        
        for(size_t i = 0; i < _shifts.size(); ++i)
        {
            target.draw(*_children[i], states);
            states.transform.translate(0.f, _shifts[i]);
        }
    }

    sf::Vector2u ListWidget::_getChildrenSize()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");        
        if(!_areShiftsCurrent || _aboutToRecalculate) _updateShifts();

        auto otherDim = float(Widget::_getChildrenSize().x);
        float childrenShiftedDim = 0.f;

        for(auto& shift : _shifts)
        {
            childrenShiftedDim += shift;
        }
        if(_shifts.size() > 0) childrenShiftedDim -= _betweenWidgetsPadding;

        return {(unsigned int)otherDim, (unsigned int)childrenShiftedDim};
    }

    void ListWidget::_calculateSize()
    {
        if(!_areShiftsCurrent) _updateShifts();
    }

    void ListWidget::_inputChildren(sf::Event event)
    {
        size_t i = 0;
        for(auto child : _children) 
        {
            if(event.type == sf::Event::MouseMoved)
            {
                auto childPosition = child->getPosition();
                sf::Event tempEvent(event);

                tempEvent.mouseMove.x -= int((childPosition.x) * _winProp.x);
                tempEvent.mouseMove.y -= int(childPosition.y * _winProp.y);

                child->input(tempEvent);
                event.mouseMove.y -= int(_shifts[i] * _winProp.y);

                i++;
            }
            else child->input(event);
        }
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
