#include "InterfaceWidget.hpp"

namespace rat
{
    InterfaceWidget::InterfaceWidget()
    {
        makeChildrenUnresizable();
    }
    void InterfaceWidget::setWidthToHeightProp(float prop)
    {
        _widthToHeightProp = prop;
        _hasProportion = true;

        _updateSizeProportion();
        _updateSizingSize();

        forceToUpdatePropSize();
    }

    void InterfaceWidget::setSizingWidthToHeightProportion(float prop)
    {
        if(prop < 0.f) prop = 0.f;

        _hasSizing = true;
        _sizingWidthToHeightProp = prop;

        _updateSizingSize();
        forceToUpdatePropSize();
    }

    void InterfaceWidget::_addWidget(Widget* widget)
    {
        widget->setInterface(this);
    }

    void InterfaceWidget::updateSizeByWindowSize(const sf::Vector2u& winSize)
    {
        setSize(static_cast<sf::Vector2f>(winSize));
        _updateSizeProportion();
        _updateSizingSize();

        forceToUpdatePropSize();
    }

    sf::Vector2f InterfaceWidget::getSizeByPropSize(const sf::Vector2f& propSize) const
    {
        return { float(_sizingSize.x * propSize.x), float(_sizingSize.y * propSize.y) };
    }

    void InterfaceWidget::_updateSizeProportion()
    {
        auto size = getMinimalSize();
        float wth = float(size.x) / float(size.y);

        if(_hasProportion)
        {
            if(wth > _widthToHeightProp)
            {
                size.x = (float)(float(size.y) * _widthToHeightProp);
            }
            else
            {
                size.y = (float)(float(size.x) / _widthToHeightProp);
            }
            setSize(size);
        }
        else
        {
            _widthToHeightProp = float(size.x)/float(size.y);
        }
    }

    void InterfaceWidget::_updateSizingSize()
    {
        auto innerSize = static_cast<sf::Vector2f>(getMinimalSize() - (getPadding() * 2.f));

        _sizingSize = innerSize;

        if(!_hasSizing) return;
        
        if(_widthToHeightProp > _sizingWidthToHeightProp)
        {
            _sizingSize.x = innerSize.y * _sizingWidthToHeightProp;
        }
        else
        {
            _sizingSize.y = innerSize.x / _sizingWidthToHeightProp;
        }
    }
}