#include "InterfaceWidget.hpp"

namespace rat
{
    void InterfaceWidget::setWidthToHeightProp(float prop)
    {
        _widthToHeightProp = prop;
        _hasProportion = true;
        _childrenSizesMustBeenRecalculated = true;

        _updateSizeProportion();
        _updateSizingSize();
    }

    void InterfaceWidget::_addWidget(Widget* widget)
    {
        widget->setInterface(this);
    }

    void InterfaceWidget::_update(float deltaTime)
    {
        if(_childrenSizesMustBeenRecalculated)
        {
            invokeToUpdatePropSize();
            invokeToUpdatePropPosition();
            _childrenSizesMustBeenRecalculated = false;
        }
    }

    void InterfaceWidget::updateSizeByWindowSize(const sf::Vector2u& winSize)
    {
        setSize(winSize);
        _updateSizeProportion();
        _updateSizingSize();
    }

    sf::Vector2i InterfaceWidget::getSizeByPropSize(const sf::Vector2f& propSize)
    {
        return { int(_sizingSize.x * propSize.x), int(_sizingSize.y * propSize.y) };
    }

    void InterfaceWidget::_updateSizeProportion()
    {
        auto size = getMinimalSize();
        float wth = float(size.x) / float(size.y);

        if(_hasProportion)
        {
            if(wth > _widthToHeightProp)
            {
                size.x = (unsigned int)(float(size.y) * _widthToHeightProp);
            }
            else
            {
                size.y = (unsigned int)(float(size.x) / _widthToHeightProp);
            }
            setSize(size);
        }
    }

    void InterfaceWidget::_updateSizingSize()
    {
        if(!_hasSizing) return;

        auto innerSize = static_cast<sf::Vector2f>(getInnerSize());

        _sizingSize = innerSize;

        if(_widthToHeightProp > _sizingWidthToHeightProp)
        {
            _sizingSize.x = innerSize.y * _widthToHeightProp;
        }
        else
        {
            _sizingSize.y = innerSize.x / _widthToHeightProp;
        }
    }
}