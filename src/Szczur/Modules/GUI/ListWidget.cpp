#include "ListWidget.hpp"

#include <algorithm>
#include <functional>

#include "Szczur/Utility/Logger.hpp"

namespace rat
{
    void ListWidget::popBack(size_t amount)
    {
        if(_children.size() < amount)
        {
            LOG_ERROR("ListWidget::popBack can't pop ", amount, " children");
            return;
        }
        
        _children.erase(_children.end() - amount, _children.end());

        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void ListWidget::setBetweenPadding(int padding)
    {
        _betweenWidgetsPadding = padding;
        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void ListWidget::makeVertical()
    {
        _positioning = Positioning::Vertical;
        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void ListWidget::makeHorizontal()
    {
        _positioning = Positioning::Horizontal;
        _aboutToRecalculate = true;
        _isPosChanged = true;
    }
    void ListWidget::makeFronted()
    {
        _isReversed = false;
        _isPosChanged = true;
    }
    void ListWidget::makeReversed()
    {
        _isReversed = true;
        _isPosChanged = true;
    }


    void ListWidget::_recalcChildrenPos()
    {
        auto basePos = gui::FamilyTransform::getAbsolutePosition();
        basePos += getPadding();
        auto drawPos = gui::FamilyTransform::getDrawPosition();
        drawPos += getPadding();

        bool isHorizontal = _positioning == Positioning::Horizontal;

        int i = _isReversed ? _children.size() - 1 : 0;
        int iEnd = _isReversed ? -1 : _children.size();
        int iAddon = _isReversed ? -1 : 1;

        for(; i != iEnd; i += iAddon)
        {
            std::cout << i << '\n';
            auto* child = _children[i];
            child->applyFamilyTrans(basePos, drawPos);


            int addon = isHorizontal ? 
            child->getPosition().x + child->getSize().x - child->getOrigin().x + _betweenWidgetsPadding : 
            child->getPosition().y + child->getSize().y - child->getOrigin().y + _betweenWidgetsPadding;

            if(isHorizontal) 
            {
                basePos.x += addon;
                drawPos.x += addon;
            }
            else
            {
                basePos.y += addon;
                drawPos.y += addon;
            }
        }
    }



    sf::Vector2i ListWidget::_getChildrenSize()
    {
        if(_hasAutoBetweenPad) _betweenWidgetsPadding = 0;
        _updateChildrenSize();
        return _childrenSize;
    }

    void ListWidget::_updateChildrenSize()
    {
        _childrenSize = {0, 0};

        bool isHorizontal = _positioning == Positioning::Horizontal;        
        for(auto* child : _children)
        {
            int addon = isHorizontal ? 
            child->getPosition().x + child->getSize().x - child->getOrigin().x + _betweenWidgetsPadding : 
            child->getPosition().y + child->getSize().y - child->getOrigin().y + _betweenWidgetsPadding;

            if(isHorizontal) _childrenSize.x += addon;
            else _childrenSize.y += addon;
        }

        if(isHorizontal)
        {
            _childrenSize.x -= _betweenWidgetsPadding;
            _childrenSize.y = Widget::_getChildrenSize().y;
        }
        else
        {
            _childrenSize.y -= _betweenWidgetsPadding;
            _childrenSize.x = Widget::_getChildrenSize().x;
        }
    }
    void ListWidget::setAutoBetweenPadding()
    {
        _hasAutoBetweenPad = true;
        _aboutToRecalculate = true;
    }
    

    void ListWidget::_calculateSize()
    {    
        if(_hasAutoBetweenPad)
        {
            if(_children.size() < 2) return;            
            _calculateAutoBetweenPad();
        }
    }

    void ListWidget::_calculateAutoBetweenPad()
    {
        float freeSpace;
        if(_positioning == Positioning::Horizontal)
        {
            freeSpace = getSize().x - _childrenSize.x;
        }
        else
        {
            freeSpace = getSize().y - _childrenSize.y;
        }
        if(freeSpace <= 0.f) return;

        auto numOfPaddings = float(_children.size() - 1);
        _betweenWidgetsPadding = freeSpace / numOfPaddings;
    }
}
