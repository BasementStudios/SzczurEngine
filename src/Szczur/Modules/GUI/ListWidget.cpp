#include "ListWidget.hpp"

#include <algorithm>
#include <functional>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void ListWidget::_addWidget(Widget* widget)
    {
        _shifts.emplace_back(sf::Vector2f{});
        _areShiftsCurrent = false;
    }
    void ListWidget::_clear()
    {
        _shifts.clear();
    }
    void ListWidget::popBack(size_t amount)
    {
        if(_children.size() < amount)
        {
            LOG_ERROR("ListWidget::popBack can't pop ", amount, " children");
            return;
        }
        
        _children.erase(_children.end() - amount, _children.end());
        _shifts.erase(_shifts.end() - amount, _shifts.end());

        _aboutToRecalculate = true;
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
    void ListWidget::makeFronted()
    {
        _isReversed = false;
    }
    void ListWidget::makeReversed()
    {
        _isReversed = true;
    }
    void ListWidget::_updateShifts()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");

        std::function<sf::Vector2f(const Widget* child)> func;
        
        func = [this](const Widget* child){
            return sf::Vector2f{ float(child->getSize().y) + child->getPosition().y - child->getPadding().y + _betweenWidgetsPadding, 0.f };
        };

        if(_positioning == Positioning::Vertical)
        {
            func = [this](const Widget* child){
                return sf::Vector2f{ 0.f, float(child->getSize().y) + child->getPosition().y - child->getPadding().y + _betweenWidgetsPadding };
            };
        }

        std::transform(_children.begin(), _children.end(), _shifts.begin(), func);

        _areShiftsCurrent = true;
    }

    sf::Vector2f ListWidget::_getChildrenShift() const
    {
        if(_shifts.size() == 0) return {};
        if(_isReversed)
        {
            auto shift = _childrenSize;
            if(_positioning == Positioning::Horizontal)
            {
                shift.x -= (_shifts.front().x - _betweenWidgetsPadding);
                shift.y = 0;
            }
            else
            {
                shift.y -= (_shifts.front().y - _betweenWidgetsPadding);
                shift.x = 0;
            }
            return static_cast<sf::Vector2f>(shift);
        }
        else return {};
    }

    void ListWidget::_drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");

        int i = _isReversed ? _shifts.size() - 1 : 0;
        int iEnd = _isReversed ? -1 : _shifts.size();
        int addon = _isReversed ? -1 : 1;
        for(; i != iEnd; i+=addon)
        {
            target.draw(*_children[i], states);
            states.transform.translate(_shifts[i]);
        }
    }

    sf::Vector2u ListWidget::_getChildrenSize()
    {
        _updateChildrenSize();
        return _childrenSize;
    }

    void ListWidget::_updateChildrenSize()
    {
        LOG_ERROR_IF(_shifts.size() != _children.size(), "Shifts and children are not synchronized...");        
        if(!_areShiftsCurrent || _aboutToRecalculate) _updateShifts();

        sf::Vector2f childrenShiftedSize;

        bool isVertical = _positioning == Positioning::Vertical;

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

        _childrenSize = static_cast<sf::Vector2u>(childrenShiftedSize);
    }

    void ListWidget::_calculateSize()
    {
        if(!_areShiftsCurrent) _updateShifts();
    }

    sf::Vector2f ListWidget::_getChildShiftByIndex(size_t index) const
    {
        if(_isReversed)
        {
            index++;
            if(index == _shifts.size()) return {};
            return -_shifts[index];
        }
        else return _shifts[index];
    }
}
