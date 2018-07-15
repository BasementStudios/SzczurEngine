#include "ListWidget.hpp"

#include <algorithm>
#include <functional>
#include <cassert>

#include "Szczur/Utility/Logger.hpp"

#include "InterfaceWidget.hpp"

#include "Widget-Scripts.hpp"



namespace rat
{
    void ListWidget::initScript(Script& script) 
    {
        auto object = script.newClass<ListWidget>("ListWidget", "GUI");

        gui::WidgetScripts::set(object);

        object.set("setBetweenPadding", &ListWidget::setBetweenPadding);
        object.set("setPropBetweenPad", &ListWidget::setPropBetweenPad);
        object.set("setAutoBetweenPadding", &ListWidget::setAutoBetweenPadding);
        object.set("makeVertical", &ListWidget::makeVertical);
        object.set("makeHorizontal", &ListWidget::makeHorizontal);
        object.set("makeFronted", &ListWidget::makeFronted);
        object.set("makeReversed", &ListWidget::makeReversed);
        object.set("popBack", &ListWidget::popBack);

        object.init();
    }
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
    void ListWidget::setBetweenPadding(float padding)
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
        auto basePos = gui::FamilyTransform::getGlobalPosition();
        basePos += getPadding();
        auto drawPos = gui::FamilyTransform::getDrawPosition();
        drawPos += getPadding();

        bool isHorizontal = _positioning == Positioning::Horizontal;

        int i = _isReversed ? int(_children.size()) - 1 : 0;
        int iEnd = _isReversed ? -1 : int(_children.size());
        int iAddon = _isReversed ? -1 : 1;

        for(; i != iEnd; i += iAddon)
        {
            auto* child = _children[i];
             
            if(child->isFullyDeactivated()) continue;
            child->applyFamilyTrans(basePos, drawPos);


            float addon = isHorizontal ? 
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

    void ListWidget::_calcPropBetweenPad()
    {
        assert(_hasPropBetweenPad);
        if(!_interface) return;

        auto size = _interface->getSizeByPropSize({_propBetweenPad, 0.f});

        setBetweenPadding(size.x);
    }

    void ListWidget::setPropBetweenPad(float propPad)
    {
        _propBetweenPad = propPad;
        _hasPropBetweenPad = true;
        if(_interface) _calcPropBetweenPad();
        else _elementsPropSizeMustBeenCalculated = true;
    }

    void ListWidget::_recalcElementsPropSize()
    {
        if(_hasPropBetweenPad) _calcPropBetweenPad();
    }

    sf::Vector2f ListWidget::_getChildrenSize()
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
            if(child->isFullyDeactivated()) continue;

            float addon = isHorizontal ? 
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
        else
        {
            _recalcChildrenPos();
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

    void ListWidget::_clear()
    {
        _isPosChanged = true;
    }
    sf::Vector2f ListWidget::_getInnerSize() const
    {
        const auto size = getSize();
        if(_positioning == Positioning::Horizontal)
        {
            return { 0.f, size.y };
        }
        else
        {
            return { size.x, 0.f };
        }
    }
}
